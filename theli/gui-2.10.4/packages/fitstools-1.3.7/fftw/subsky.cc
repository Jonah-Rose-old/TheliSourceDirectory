#include <CCfits>
#include <thread>
#include <fitsio.h>
#include <fftw3.h>
#include <unistd.h>
#include "fftw++.h"
#include "convolve.h"
#include "fitstools.h"
#include "statistics.h"

using namespace std;

vector<long> objects_size;
vector<long> objects_xext;
vector<long> objects_yext;

long nobj, nobjincrement = 1;

void globalstats(const vector<float>&, const vector<long>&, const long, const long, float&, float&);
void get_objectdimensions(const vector<long>&, const long, const long, const bool);
void floodfill(vector<float>&, const vector<long>&, const float, const long, 
	       const long, const long,const  long, const long);
void fill_objects(const vector<float>&, const vector<long>&, vector<float>&, 
		  const long, const long, const long, const int);
void fill_objects_thread(const long, const long, const long, const long, const long, const vector<float>&, 
			 const vector<long>&, vector<float>&);
void update_segimage(const vector<float>&, vector<long>&, const float, const float, 
		     const int, const float, const long, const long);


//************************************************************
void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           -s segmentation_image \n";
    cerr << "           -n number of pixels used for estimation (default: 100)\n";
    cerr << "           -f FWHM of the smoothing kernel in pixels\n";
    cerr << "           -c number of CPUs to use\n";
    cerr << "           -w weight_image\n";
    cerr << "           [-a input mask catalog]\n";
    cerr << "           [-b ellipse scaling parameter (suggest: 3.0 or higher)]\n";
    cerr << "           [-o output_image]\n";
    cerr << "           [-m model_image]\n";
    cerr << "           [-p ds9-mask]\n";
    cerr << "           [-t minthresh maxthresh (manual thresholding)]\n";
    cerr << "           [-k sigma clip for the threshold (default: 5)]\n\n";
    cerr << "           [-d (do not use FFTW wisdom)]\n";
    cerr << "    Replaces objects (defined in a segmentation image)\n";
    cerr << "    with a local background estimate. The latter is obtained\n";
    cerr << "    from the -n nearest background pixels (automatically adjusted).\n\n";
    cerr << "    Optionally, a ds9 region file can be specified. Pixels in polygons\n";
    cerr << "    and circles therein will be kept or masked, depending on the 'sense'\n";
    cerr << "    of the ds9 region file. The latter is defined through this comment line:\n";
    cerr << "    # Sense: in|out\n";
    cerr << "    If a Sextractor catalog is given (-a), the object ellipses will be scaled\n";
    cerr << "    (-b) and pixels inside masked as well (e.g. to catch faint halos).\n\n";
    exit(1);
  }
}


//************************************************************
int main(int argc, char *argv[])
{
  int n, m, flag_mt, flag_ds9, flag_cat, flag_rad, ncpu, nthreads, nthreads_user;
  long i, npix;
  float lowthresh, highthresh, fwhm, clip, r;
  string input_image, seg_image, weight_image, output_image, model_image;
  char ds9region[FILEMAX];
  char input_catalog[FILEMAX];

  // print usage if no arguments were given
  flag_mt  = 0;
  flag_ds9 = 0;
  flag_cat = 0;
  flag_rad = 0;
  clip     = 10.;
  npix     = 100;
  fwhm     = 0.;
  lowthresh  = 0.;
  highthresh = 0.;
  nthreads_user = 0;
  r        = 3.0;

  fftwpp::fftw::effort = FFTW_MEASURE;

  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 's': seg_image = argv[++i];
	break;
      case 'm': model_image = argv[++i];
	break;
      case 'w': weight_image = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	break;
      case 'n': npix = atol(argv[++i]);
	break;
      case 'd': fftwpp::fftw::effort = FFTW_ESTIMATE;
	break;
      case 't': 
	lowthresh  = atof(argv[++i]);
	highthresh = atof(argv[++i]);
	flag_mt = 1;
	break;
      case 'k': clip = atof(argv[++i]);
	break;
      case 'f': fwhm = atof(argv[++i]);
	break;
      case 'c': nthreads_user = atoi(argv[++i]);
	break;
      case 'p': strcpy(ds9region,argv[++i]);
	flag_ds9 = 1;
	break;
      case 'a': strcpy(input_catalog, argv[++i]);
	flag_cat = 1;
	break;
      case 'b': r = atof(argv[++i]);
	flag_rad = 1;
	break;
      }
    } 
  }

  if (fwhm < 2.) {
    cerr <<  "ERROR: The FWHM of the smoothing kernel was not specified\n";
    cerr <<  "       or is smaller than 2. Exiting.\n";
    exit (1);
  }

  // multi-threading
  ncpu = sysconf(_SC_NPROCESSORS_ONLN);
  nthreads = ncpu;
  if (nthreads_user > 0) {
    if (nthreads_user > ncpu) nthreads = ncpu;
    else nthreads = nthreads_user;
  }

  // Read the FITS images
  checkfile(input_image);
  string call = programcall(argv, argc);

  myFITS image_in(input_image);
  myFITS image_seg(seg_image);
  myFITS image_w(weight_image);
  n = image_in.naxis1;
  m = image_in.naxis2;

  // Read the segmentation image
  vector<long> seg(n*m,0);
  for (i=0; i<n*m; i++) 
    seg[i] = long(image_seg.data[i]);

  image_seg.destroy();

  // how many objects are in the segmentation image
  nobj = max(seg, n*m);

  // Filter the segmentation image (expand objects, and masking of statistical outliers)
  update_segimage(image_in.data, seg, lowthresh, highthresh, flag_mt, clip, n, m);

  // Expand object ellipses if requested
  if (flag_cat == 1 && flag_rad == 1) {

    // Read the object catalog
    vector<float> x;
    vector<float> y;
    vector<float> cxx;
    vector<float> cyy;
    vector<float> cxy;
    vector<float> aimg;
    float dx, dy;
    float xtmp, ytmp, cxxtmp, cyytmp, cxytmp, atmp;
    ifstream input(input_catalog);
    if (!input.is_open()) {
      cerr <<  "ERROR: Could not open " << input_catalog << "!\n";
      exit (1);
    }
    else {
      while ( input.good() )
	{
	  input >> xtmp >> ytmp >> cxxtmp >> cyytmp >> cxytmp >> atmp;
	  // only include objects if they are not "just" outside the image
	  // (excluding spurious sextractor detections that appear for very low thresholds)
	  if (image_w.data [ floor(xtmp) + n*(floor(ytmp))] != 0) {
	    x.push_back(xtmp);
	    y.push_back(ytmp);
	    cxx.push_back(cxxtmp);
	    cyy.push_back(cyytmp);
	    cxy.push_back(cxytmp);
	    aimg.push_back(atmp);
	  }
	}
      input.close();
    }

    long nobj = x.size();

    // Expand the segmentation image
    long imin, imax, jmin, jmax, j, k;
    for (k=0; k<nobj; k++) {
      // the max area containing the object
      imin = (x[k] - aimg[k]*r > 0) ? int(x[k] - aimg[k]*r) : 0;
      imax = (x[k] + aimg[k]*r < n) ? int(x[k] + aimg[k]*r) : n-1;
      jmin = (y[k] - aimg[k]*r > 0) ? int(y[k] - aimg[k]*r) : 0;
      jmax = (y[k] + aimg[k]*r < m) ? int(y[k] + aimg[k]*r) : m-1;
      for (j=jmin; j<=jmax; j++) {
	for (i=imin; i<=imax; i++) {
	  dx = float(i) - x[k];
	  dy = float(j) - y[k];
	  if (cxx[k] * dx * dx +
	      cyy[k] * dy * dy + 
	      cxy[k] * dx * dy <= r*r)
	    seg[i+n*j] = k+1;
	}
      }
    }
  }

  // a mask
  vector<int> mask(n*m,0);

  // apply a ds9 region mask file to the segmentation image, if requested
  if (flag_ds9 == 1) {
    // the empty strings mean we use keywords in the ds9 region file
    make_ds9_maskimage(mask, ds9region, n, m, "", "");
    bool testflag = false;
    // complement the segmentation image with the polygon mask;
    // we assume the mask is zero for bad and 1 for good pixels
    // (it will be multiplied with the sky-subtracted image at the end)
    for (i=0; i<n*m; i++) {
      if (mask[i] == 0 && seg[i] == 0) {
	seg[i] = nobj + nobjincrement;
	testflag = true;
      }
    }
    if (testflag) nobjincrement++;
  }

  // Add additional pixels to the segmentation map if they are zero in the weight
  // Only do that if that pixel isn't already included in the segmentation map
  bool testflag = false;
  for (i=0; i<n*m; i++) {
    if (image_w.data[i] == 0. && seg[i] == 0) {
      seg[i] = nobj + nobjincrement;
      testflag = true;
    }
  }
  if (testflag) nobjincrement++;    

  // a copy of the input image, containing "filled" objects
  vector<float> image(image_in.data);

  // The object dimensions
  nobj += nobjincrement - 1; // update the number of pixel clusters (statistical outliers, ds9 mask, weight map)
  objects_size.resize(nobj);
  get_objectdimensions(seg, n, m, false);

  // the image with objects replaced by a dynamic background estimate
  vector<float> image_back(n*m,0);
  // in principle, there should be no difference between the following two, but there is a slight one
  //  fill_objects(image_in.data, seg, image_back, n, m, npix, nthreads);
  fill_objects(image, seg, image_back, n, m, npix, nthreads);

  // convolve the image with a gaussian
  convolve_gauss_2d(image_back, n, m, fwhm, nthreads);
  
  // write the result
  vector<float> data_out(n*m,0);
  if (flag_ds9 == 0) {
    for (i=0; i<n*m; i++) 
      data_out[i] = image_in.data[i] - image_back[i];
  }
  if (flag_ds9 == 1) {
    for (i=0; i<n*m; i++) 
      data_out[i] = (image_in.data[i] - image_back[i]) * mask[i];
  }

  writeImage(output_image, input_image, data_out, call);
  
  if (!model_image.empty()) {
    for (i=0; i<n*m; i++) data_out[i] = image_back[i];
    writeImage(model_image, input_image, data_out, call);
  }

  return 0;
}

//************************************************************
void globalstats(const vector<float> &data, const vector<long> &seg, const long n, const long m, 
		 float &globmedian, float &globrms)
{
  int l, t, j;
  long dim_small, dim, dim90, i;
  
  // define some odd number
  // then select every l-th pixel in the array
  // that is always smaller than n for n > 9
  // the sqrt ensures that the array is probed in a quasi-random pattern
  
  l = 2/3*n + sqrt(n); 
  
  // the number of elements we test
  dim = n * m;
  dim_small = dim / l;
  
  vector<float> sub(dim_small,0);
  
  // select the small array
  t = 0;
  j = 0;
  for (i=0; i<dim; i++) {
    if (t==0 && j < dim_small && seg[i] == 0) {
      sub[j] = data[i];
      j++;
    }
    if (t<l) t++;
    if (t == l) t = 0;
  }
  
  // sort the array
  sort(sub.begin(), sub.begin()+j-1);
  
  // get the rms from the middle 90%    
  dim90 = 0;
  for (i=0.05*j; i<0.95*j; i++) {
    dim90++;
  }
  vector<float> arr90(dim90,0);
  dim90 = 0;
  for (i=0.05*j; i<0.95*j; i++) {
    arr90[dim90++] = sub[i];
  }
  
  //globmedian = median(arr90, dim90);
  globmedian = arr90[dim90/2];
  globrms  = rms(arr90, dim90);
}


//***************************************************************
// count how large the individual objects are
//***************************************************************
void get_objectdimensions(const vector<long> &data, const long n, const long m, const bool getextensions)
{
  long i, j, id;

  // the number of pixels in each object
  for (i=0; i<n*m; i++) {
    id = data[i] - 1;
    if (data[i] != 0) objects_size[id]++;
  }

  // we need this only in the first call to this function, 
  // the second one is simply to update the object sizes after autoexpansion
  // This part of the code is INACTIVE
  if (getextensions) {
    vector<long> objects_xmin(nobj, n);
    vector<long> objects_xmax(nobj, 0);
    vector<long> objects_ymin(nobj, m);
    vector<long> objects_ymax(nobj, 0);

    for (j=0; j<m; j++) {
      for (i=0; i<n; i++) {
	id = data[i+n*j] - 1;
	if (data[i+n*j] != 0) {
	  if (i<objects_xmin[id]) objects_xmin[id] = i;
	  if (i>objects_xmax[id]) objects_xmax[id] = i;
	  if (j<objects_ymin[id]) objects_ymin[id] = j;
	  if (j>objects_ymax[id]) objects_ymax[id] = j;
	}
      }
    }
    
    for (id=0; id<nobj; id++) {
      objects_xext[id] = objects_xmax[id] - objects_xmin[id] + 1;
      objects_yext[id] = objects_ymax[id] - objects_ymin[id] + 1;
    }
  }
}


//***************************************************************
// This function replaces neighbouring pixels around a given pixel
// with the same value assigned to this pixel.
// Like that big objects can be filled rapidly.
// The larger the object, the more pixels we fill in
//***************************************************************

void floodfill(vector<float>&data, const vector<long>&seg, const float value, const long n, 
	       const long m, const long i, const long j, const long size)
{
  long imin, imax, jmin, jmax;
  long range;
  long i1, j1;

  range = sqrt(size) / 3;
  if (range < 4) range = 4;

  imin = i-range;
  imax = i+range;
  jmin = j-range;
  jmax = j+range;

  // respect image boundaries
  if (imin  < 0) imin = 0;
  if (imax >= n) imax = n-1;
  if (jmin  < 0) jmin = 0;
  if (jmax >= m) jmax = m-1;

  for (j1=jmin; j1<=jmax; j1++) {
    for (i1=imin; i1<=imax; i1++) {
      if (seg[i1+n*j1] != 0) data[i1+n*j1] = value;
    }
  }
}

//********************************************************************
// This function interpolates pixels in the image which are non-zero 
// in the segmentation image, by a local background estimate.
// The interpolated pixel value within an object will vary, meaning
// this yields good values also for large objects with varying 
// background 
//********************************************************************

void fill_objects(const vector<float> &data, const vector<long> &seg, vector<float> &image_back, 
		  const long n, const long m, const long npix, const int NUM_THREADS)
{
  long line_start, line_end;
  vector<thread> mythreads;
  int t;

  // create arguments and launch threads
  for(t=0; t<NUM_THREADS; t++) {
    line_start = t * (m / NUM_THREADS);
    if (t <  NUM_THREADS-1) line_end = (t + 1) * (m / NUM_THREADS) - 1;
    if (t == NUM_THREADS-1) line_end = m - 1;
    mythreads.push_back( thread( fill_objects_thread, 
				 n, m, npix, line_start, line_end, ref(data), 
				 ref(seg), ref(image_back))); 
  }
  
  // wait for threads to finish
  for (t=0; t<NUM_THREADS; t++)
    mythreads[t].join();
}


//*******************************************************************
// fill objects with a background estimate;
// this will be called from multiple threads
//*******************************************************************

void fill_objects_thread(const long n, const long m, const long npix, const long line_start, 
			 const long line_end, const vector<float> &data, const vector<long> &seg, 
			 vector<float> &image_back)
{
  long count, i, j, npixlocal, id;
  long imin, imax, jmin, jmax, i1, j1;
  long radius, delta_rad;
  float value;

  vector<float> tmpnpix(npix,0);

  // fill objects
  count = 0;
  for (j=line_start; j<=line_end; j++) {
    for (i=0; i<n; i++) {
      id = seg[i+n*j] - 1;
      if (seg[i+n*j] > 0 && image_back[i+n*j] == 0.) {
	// dynamic estimate for the number of good pixels
	npixlocal = 3 * objects_size[id];
	if (npixlocal > npix) npixlocal = npix;
	// npix is the minimum number of pixels from which to obtain a sky estimate for that object;
	// we add 5 to the radius to make sure it is larger than 0 
	// (and to quickly jump over the small objects)

	radius = sqrt( (float) npixlocal / PI) + 5;
	while (count < npixlocal) {
	  imin = i - radius;
	  imax = i + radius;
	  jmin = j - radius;
	  jmax = j + radius;
	  // respect image boundaries
	  if (imin  < 0) imin = 0;
	  if (imax >= n) imax = n;
	  if (jmin  < 0) jmin = 0;
	  if (jmax >= m) jmax = m;
	  for (j1=jmin; j1<jmax; j1++) {
	    for (i1=imin; i1<imax; i1++) {
	      if (seg[i1+n*j1] == 0 && count < npixlocal) {
		tmpnpix[count] = data[i1+n*j1];
		count++;
	      }
	    }
	  }
	  
	  if (count<npixlocal) {
	    // estimate by how much we have to increase the radius to get npix good pixels
	    delta_rad = (npixlocal - count + radius * radius * PI) / PI;
	    if (delta_rad < 0) radius *= 1.1;
	    else {
	      delta_rad = sqrt( (float) delta_rad ) - radius + 5;
	      radius += delta_rad;
	    }
	  }
	}
	value = mean(tmpnpix, (long) (0.7*npixlocal));
	// The threads can overwrite data in the floodfill routine, hence this is
	// not threadsafe. However, the values are guesswork anyway so it doesn't matter
	// from which thread the estimate comes.
	floodfill(image_back, seg, value, n, m, i, j, objects_size[id]);
      } 
      if (seg[i+n*j] == 0 && image_back[i+n*j] == 0.)
	image_back[i+n*j] = data[i+n*j];
      count = 0;
    }
  }
}


//***************************************************************************************************
// spatially convolve a copy of the segmentation map to expand any objects
//***************************************************************************************************
void update_segimage(const vector<float> &image, vector<long> &seg, const float lthresh, 
		     const float hthresh, const int flag, const float clip, const long n, const long m)
{
  long i, j, k, l;
  float global_median, global_rms, lthresh_out, hthresh_out;

  // automatic thresholding, if no manual thresholds are provided
  globalstats(image, seg, n, m, global_median, global_rms);

  // A (default) 10 sigma threshold to clip extreme outliers
  lthresh_out = global_median - clip * global_rms;
  hthresh_out = global_median + clip * global_rms;

  // add non-zero pixels to the segmentation image if the corresponding pixels in the input image 
  // are outside automatic and manual thresholds (only if the segmentation image is zero, i.e. not already recognized)
  bool testflag = false;
  for (i=0; i<n*m; i++) {
    if ( seg[i] == 0 && 
	 (( image[i] < lthresh_out || image[i] > hthresh_out) ||  
	  ((image[i] < lthresh || image[i] > hthresh) && flag == 1))) {
      seg[i] = nobj+nobjincrement; // special value to distinguish them from sextractor masks
      testflag = true;
    }
  }
  if (testflag) nobjincrement++;

  vector<long> copy(n*m,0);
  vector<float> copycopy(n*m,0);
  long sample = 0, ik, jl;

  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      // spatial convolution with a 5x5 box filter to catch neighbouring (possibly bad) pixels as well
      sample = 0;
      for (k=-2; k<=2; k++) {
	ik = i+k;
	// respect image boundaries
	if (ik < 0) ik = 0;
	if (ik >= n) ik = n-1;
	for (l=-2; l<=2; l++) {
	  jl = j+l;
	  // respect image boundaries
	  if (jl < 0) jl = 0;
	  if (jl >= m) jl = m-1;
	  if (seg[ik+n*jl] > 0) sample = seg[ik+n*jl];
	  copy[i+n*j] += seg[ik+n*jl];
	}
      }
      // the extended region should have the same value as the original source;
      // This is not a very stringent requirement but nice to have
      copy[i+n*j] = copy[i+n*j] > 0 ? sample : 0;  
      copycopy[i+n*j] = float(copy[i+n*j]);
    }
  }

  for (i=0; i<n*m; i++) 
    seg[i] = copy[i];
}