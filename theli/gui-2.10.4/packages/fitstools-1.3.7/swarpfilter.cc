#include <fstream> 
#include <thread>
#include <CCfits>
#include <fitsio.h>
#include <semaphore.h>
#include "fitstools.h"
#include "statistics.h"
#include <sys/time.h>
#include <ctime>
#include <unistd.h>

using namespace std;

typedef struct {
  string image;
  string weight;
  long  naxis1;
  long  naxis2;
  float crpix1;
  float crpix2;
  float fluxscale;
  float sky;
  long  xoffset;
  long  yoffset;
  myFITS image_fits;
  myFITS weight_fits;
} image_type;

typedef unsigned long timestamp_t;

static timestamp_t
get_timestamp ()
{
  struct timeval now;
  gettimeofday (&now, NULL);
  return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

sem_t semaphor; // control the number of threads writing simultaneously
int numwritethreads = 1;

long nthreads;
vector< vector<long> > badpixindex;
vector<long> maxbadpix;
vector<float> gooddata;
vector<long> gooddataind;

bool get_coaddblock(image_type &, const long, vector < vector<float> >&, 
		   vector < vector<char> >&, const long, const long, const long);
void identify_bad_pixels(const vector<float> &, 
			 const float&, const long&, 
			 vector<long>&, const long&);
void stackfilter_rejectmax(const vector<float>&, const vector<long>&, const long&, float&, float&);
void stackfilter_rejectminmax(const vector<float>&, const vector<long>&, const long&, float&, float&);
void writeWeight(image_type&, vector<char>&);
void writeimages_thread(image_type *, long, long, long, long, long);

//******************************************************************

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr <<  "\n";
    cerr <<  "  USAGE:  %s \n" << argv[0] << endl;
    cerr <<  "           -i input_list \n";
    cerr <<  "           [ -k kappa-sigma-threshold (4.0)\n";
    cerr <<  "           [ -m minimum number of bad pixels in a cluster (max: 9)]\n";
    cerr <<  "           [ -e border width]\n";
    cerr <<  "           [ -n number of lines read in one chunk (default: 300, or less depending on memory)\n";
    cerr <<  "           [ -c number of CPUs to use (if not specified, all available CPUs will be used)]\n";
    cerr <<  "           [ -p number of writing threads (defaulted to 1, CAN lead to segfaults otherwise)]\n";
    cerr <<  "  PURPOSE: Detects outliers in a set of resampled \n";
    cerr <<  "     images. Corresponding pixels in the weight maps \n";
    cerr <<  "     are set to zero.\n";
    cerr <<  "     If the -m option is given, then bad pixels are only masked\n";
    cerr <<  "     if AT LEAST this many bad pixels are grouped together.\n";
    cerr <<  "     If -e 1, 3x3 pixels centred on the bad pixel will be masked.\n";
    cerr <<  "     If -e 2, 5x5 pixels centred on the bad pixel will be masked, etc ...\n";
    cerr <<  "     The program determines its optimal memory usage automatically.\n";
    cerr <<  "     This can be overridden by specifying the number of lines that\n";
    cerr <<  "     should be processed in one go.\n\n";
    exit(1);
  }
}

//******************************************************************
int main(int argc, char *argv[])
{
  int            pagesize, numcpu;
  long           i, l, currentpixel, num_images, clustersize, maskwidth;
  long           blocksize, block, nblocks, chunksize, coadd_naxis1, coadd_naxis2;
  long           pages, npresentimages, ngoodweight;
  float          kappa, blockfr, coadd_crpix1, coadd_crpix2;
  char           input_file[FILEMAX];

  kappa       = 4.0;
  blocksize   = -1;
  clustersize = 1;
  maskwidth   = 0;

  // get some system info (number of online CPUs and available memory)
  nthreads = sysconf(_SC_NPROCESSORS_ONLN);
  pages    = sysconf(_SC_PHYS_PAGES);
  pagesize = sysconf(_SC_PAGESIZE);
  numcpu   = nthreads;

  // print usage if no arguments were given
  if (argc == 1) usage(0, argv);

  // read the arguments
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch (tolower((int)argv[i][1])) {
      case 'i': strcpy(input_file, argv[++i]);
        break;
      case 'k': kappa = atof(argv[++i]);
        break;
      case 'n': blocksize = atol(argv[++i]);
        break;
      case 'c': numcpu = atol(argv[++i]);
        break;
      case 'm': clustersize = atol(argv[++i]);
        break;
      case 'e': maskwidth = atol(argv[++i]);
        break;
      case 'p': numwritethreads = atol(argv[++i]);
        break;
      }
    }
  }

  // upper limit for the cluster size
  if (clustersize > 9) clustersize = 9;

  // read the file list
  ifstream input(input_file);
  if (!input.is_open()) {
    cerr <<  "ERROR: Could not open " << input_file << "!\n";
    exit (1);
  }
  input >> num_images >> coadd_naxis1 >> coadd_naxis2 >> coadd_crpix1 >> coadd_crpix2;

  nthreads = 1;  // no multithreading implemented (at least not for the badpix ID part)
  if (blocksize == -1) {
    // maxmimum memory used: 50% of the available RAM (1.25: 1 float array, and one char array)
    // In reality, it's topping off at more like 20% of the available RAM
    blocksize = 0.5 * pages * pagesize /  coadd_naxis1 / num_images / sizeof(float) / 1.25 / nthreads;
    blockfr   = 0.5 * pages * pagesize / (coadd_naxis1 * coadd_naxis2 * sizeof(float) * 1.25 * 12); 
    blocksize = (long)(blockfr * coadd_naxis2);
    
    if (blocksize < 1) {
      cerr << "ERROR: NOT ENOUGH MEMORY AVAILABLE FOR SWARPFILTER!\n";
      exit(1);
    }
    else if (blocksize < 3) {
      cerr << "WARNING: You seem to run low on memory! Swarpfilter will run fine,\n";
      cerr << "but next time consider stopping other memory consuming applications.\n";
    }
    else if (blocksize > 300) 
      blocksize = 300;

    cout << "Best memory usage: BLOCKSIZE = " << blocksize << endl;
  } 
  else 
    cout << "Manually provided blocksize: " << blocksize << endl;

  // number of pixels to be analysed in one step
  chunksize = coadd_naxis1 * blocksize;

  image_type *data = new image_type[num_images];

  // read the file names and image geometries etc
  i = 0;
  for (i=0; i<num_images; i++) {
    input >> data[i].image >> data[i].weight >> data[i].naxis1 >> data[i].naxis2 >>
      data[i].crpix1 >> data[i].crpix2 >> data[i].fluxscale >> data[i].sky;
    // We assume that CRPIX1/2 is INTEGER (which is the case if stacked with SWarp)
    data[i].xoffset = long (coadd_crpix1 - data[i].crpix1);
    data[i].yoffset = long (coadd_crpix2 - data[i].crpix2);
  }
  input.close();
  
  // memory
  vector < vector<float> > block_img_comp(num_images);
  vector < vector<char> > block_wgt_comp(num_images);
  vector <float> sky(num_images, 0);
  vector <long> presentimages(num_images, 0);
  vector <long> running_bpi(num_images, 0);
  maxbadpix.resize(num_images, 10000);
  badpixindex.resize(num_images);
  gooddata.resize(num_images, 0);
  gooddataind.resize(num_images, 0);
  for (i=0; i<num_images; i++) {
    block_img_comp[i].resize(chunksize,0);
    block_wgt_comp[i].resize(chunksize,0);
    badpixindex[i].resize(maxbadpix[i],0);
    sky[i] = data[i].sky * data[i].fluxscale;
  }

  // timing
  timestamp_t t0 = get_timestamp();
  timestamp_t tmpstamp = 0, tmpstamp_old;

  // Open all file handles in one go to save some time
  // UPDATE: does not work with CCfits as it has a lower file handle limit

  /*
  cout << "Opening " << num_images << " files\n";
  for (i=0; i<num_images; i++) {
    data[i].image_fits.open(data[i].image, false);
    data[i].weight_fits.open(data[i].weight, false);
  }
  */

  nblocks = (long) (coadd_naxis2 / blocksize + 1.) - 1;

  // load the images and identify bad pixels
  for (block=0; block<(long)(coadd_naxis2 / blocksize + 1.); block++) {
    npresentimages = 0;

    // read a chunk of data from the images
    for (i=0; i<num_images; i++) {
      if (get_coaddblock(data[i], blocksize, block_img_comp, 
			 block_wgt_comp, coadd_naxis1, block, i)) { 
	presentimages[npresentimages++] = i;
      }
    }

    // if no data were obtained
    if (npresentimages == 0) continue;

    // find the bad pixels
    tmpstamp_old = get_timestamp();
    cout << "Read " << npresentimages << " images in block " << block << " of " << nblocks << endl;

    for (l=0; l<chunksize; l++) {
      // Get the number of non-zero-weight pixels in the stack;
      // The gooddata vector may contain data values from a previous pass as we do not clear it
      // every time to save time. Instead, we remember the currently relevant index values;
      ngoodweight = 0;
      for (i=0; i<npresentimages; i++) {
	if (block_wgt_comp[presentimages[i]][l] > 0) {
	  gooddata[presentimages[i]] = block_img_comp[presentimages[i]][l];
	  gooddataind[ngoodweight] = presentimages[i];
	  ngoodweight++;
	}
      }
      currentpixel = l + block*chunksize;
      if (ngoodweight >= 2) identify_bad_pixels(sky, kappa, currentpixel, running_bpi, ngoodweight);
    }

    tmpstamp += (get_timestamp() - tmpstamp_old);

    // reset the weight array; the others don't need to be zeroed, the weight is sufficient for full control
    for (i=0; i<npresentimages; i++) {
      fill(block_wgt_comp[presentimages[i]].begin(), block_wgt_comp[presentimages[i]].begin()+chunksize, 0);
    }
  }

  for (i=0; i<num_images; i++) {
    block_img_comp[i].clear();
    block_wgt_comp[i].clear();
  }
  block_img_comp.clear();
  block_wgt_comp.clear();
  sky.clear();
  running_bpi.clear();

  // Time stamping
  timestamp_t t1 = get_timestamp();
  double secs = (t1 - t0) / 1000000.0L;
  double secs2 = tmpstamp  / 1000000.0L;
  cout << "Data read time: " << secs-secs2 << endl;
  cout << "Badpix ID time: " << secs2 << endl;
  t0 = get_timestamp();



  // Writing results

  // PREPARE THREADS
  int NUM_THREADS_WRITE = numcpu;
  int th, step, th_max;
  long num_images_min, num_images_max, modcount, mod;

  vector<thread> mythreads;

  th_max = NUM_THREADS_WRITE;
  num_images_min = num_images_max = modcount = 0;
  step = num_images / NUM_THREADS_WRITE;  // may be zero
  mod = num_images % NUM_THREADS_WRITE;

  // A POSIX semaphor that controls the number of threads writing simultaneously (default max threads writing = 1)
  // I used to have three, but this lead to segfaults occasionally and I don't understand yet why. As far as I can see,
  // everything is entirely threadsafe, but maybe not within CCfits?
  sem_init(&semaphor, 0, numwritethreads);

  // create arguments and launch threads
  for(th=0; th<NUM_THREADS_WRITE; th++) {

    // The following code assigns images to the threads in this manner:
    // example: 10 images, 4 threads
    // thread 0: images 0 1 2
    // thread 1: images 3 4 5
    // thread 2: images 6 7
    // thread 3: images 8 9

    if (modcount < mod) num_images_max = num_images_min + step;
    else num_images_max = num_images_min + step - 1;
    modcount++;

    // cout << "THREAD " << th << " working on images: " << num_images_min << " to " <<num_images_max << endl;

    mythreads.push_back( thread( writeimages_thread, ref(data), maskwidth, clustersize, 
				 coadd_naxis1, num_images_min, num_images_max));
    
    num_images_min = num_images_max + 1;
    if (num_images_min >= num_images) {
      th_max = th;
      break;
    }
  }

  // wait for th_max threads to finish
  for (th=0; th<=th_max; th++) {
    mythreads[th].join();
  }

  maxbadpix.clear();
  delete [] data;

  t1 = get_timestamp();
  secs = (t1 - t0) / 1000000.0L;
  cout << "Writing time:   " << secs << endl;

  return 0;
}


//******************************************************************
bool get_coaddblock(image_type &data, const long blocksize, 
		    vector < vector<float> >&block_img_comp, 
		    vector < vector<char> >&block_wgt_comp, 
		    const long coadd_naxis1, const long block, 
		    const long currentimage)
{
  long   i, j, k, l, xi, yi;
  long   firstline2read, lastline2read; // we start counting at 0!
  long   bbs0, bbs1;

  bbs0 = block * blocksize;
  bbs1 = bbs0 + blocksize;

  /*
    data.xoffset: number of pixels between the left border of 
    coadd.fits and the image
    data.yoffset: number of pixels between the lower border of 
    coadd.fits and the image
    0 <= data.xoffset <= coadd_naxis1 - naxis1
    0 <= data.yoffset <= coadd_naxis2 - naxis2 
  */

  // if the image is entirely below or above the
  //   current coadd block, then there is nothing to do: 
  if (data.yoffset + data.naxis2 <= bbs0 || data.yoffset >= bbs1) {
    return false;
  }

  // The image overlaps with the current coadd block

  // assume that the image covers the block entirely:
  firstline2read = bbs0 - data.yoffset;
  lastline2read  = firstline2read + blocksize - 1;

  // if the image does not cover the lower part of the block
  // but the upper part entirely 
  if (firstline2read < 0 && data.yoffset + data.naxis2 >= bbs1) {
    lastline2read  = bbs1 - data.yoffset - 1;
    firstline2read = 0;
  }

  // if the image does not cover the upper part of the block
  // but the lower part entirely 
  if (data.yoffset + data.naxis2 < bbs1 && data.yoffset <= bbs0) {
    lastline2read  = data.naxis2 - 1;
    firstline2read = bbs0 - data.yoffset;
  }

  // if the image is entirely contained in the block
  // without touching its borders 
  if (data.yoffset > bbs0 && data.yoffset + data.naxis2 < bbs1) {
    firstline2read = 0;
    lastline2read  = data.naxis2 - 1;
  }

  valarray<float> data_in, weight_in;

  // Load the data sections;
  // map the image block onto the coadded image (block_img_coadd)

  data.image_fits.open(data.image, false);
  data.weight_fits.open(data.weight, false);

  data.image_fits.readDataSectionEff(1, data.naxis1, firstline2read + 1, lastline2read + 1, data_in);
  data.weight_fits.readDataSectionEff(1, data.naxis1, firstline2read + 1, lastline2read + 1, weight_in);
  l = 0;  // the running index of the image
  k = 0;  // the running index of the coadd

  for (j=0; j<blocksize; j++) {
    for (i=0; i<coadd_naxis1; i++) {
      xi = i - data.xoffset;
      yi = bbs0 + j - data.yoffset;
      // if the image overlaps with the coadd block
      // (if not then these arrays are initialised to zero anyway)
      if (xi >= 0 && yi >= 0 && xi < data.naxis1 && yi < data.naxis2) {
        block_img_comp[currentimage][k] = data_in[l] * data.fluxscale;
	block_wgt_comp[currentimage][k] = (weight_in[l] > 0.) ? 1 : 0;
        l++;
      }
      k++;
    }
  }

  // release the file handles
  data.image_fits.destroy();
  data.weight_fits.destroy();

  return true;
}


//***************************************************************************************
// Identify the bad pixels in a stack
//***************************************************************************************
void identify_bad_pixels(const vector<float> &sky, 
			 const float &kappa, const long &currentpixel,
			 vector<long> &running_bpi, const long &ngoodweight)
  
{
  long  i, gi;
  float mean, rms, thresh, sum;

  // 2 pixels in the stack: use poisson rms
  if (ngoodweight == 2) {
    sum  = gooddata[gooddataind[0]] + gooddata[gooddataind[1]];
    mean = 0.5 * sum;
    rms  = sum + (sky[gooddataind[0]] + sky[gooddataind[1]]);
    if (rms > 0.) rms = sqrt(0.5 * rms);
    else return;
    thresh = rms;
  }

  // 3-4 pixels in the stack: reject max pixel
  else if (ngoodweight <= 4) {
    mean = rms = 0.;
    stackfilter_rejectmax(gooddata, gooddataind, ngoodweight, mean, rms);
    if (rms > 0.) {
      mean /= (float)(ngoodweight - 1);
      rms = sqrt(((ngoodweight - 1) / (ngoodweight - 2)) * 
                 (rms / (ngoodweight - 1) - mean * mean));
    }
    else return; 
    thresh = 6. * kappa / ngoodweight * rms;
  } 

  // 5 or more pixels in the stack: reject min and max pixel
  else {
    mean = rms = 0.;
    stackfilter_rejectminmax(gooddata, gooddataind, ngoodweight, mean, rms);
    if (rms > 0.) {
      mean /= (float)(ngoodweight - 2);
      rms = sqrt(((ngoodweight - 2) / (ngoodweight - 3)) * 
                 (rms / (ngoodweight - 2) - mean * mean));      
    } 
    else return; 
    // adaptive rms threshold
    if (ngoodweight < 6) thresh = 6. * kappa / ngoodweight * rms;
    else thresh = kappa * rms; 
  }

  // running_bpi = "running_badpixindex"

  if (ngoodweight > 2) {
    for (i=0; i<ngoodweight; i++) {
      gi = gooddataind[i];
      if (fabs(gooddata[gi] - mean) > thresh) {
	badpixindex[gi][running_bpi[gi]] = currentpixel;
	running_bpi[gi]++;
      }
      // dynamic memory reallocation
      if (running_bpi[gi] >= maxbadpix[gi]) {
	maxbadpix[gi] *= 2;
	badpixindex[gi].resize(maxbadpix[gi],0);
      }
    }
  }
  else {
    for (i=0; i<ngoodweight; i++) {
      gi = gooddataind[i];
      if (fabs(gooddata[gi] - mean) > thresh) {
	// reject pixel only if it is the brighter one
	if ((i == 0 && gooddata[gooddataind[0]] > gooddata[gooddataind[1]]) ||  
	    (i == 1 && gooddata[gooddataind[1]] > gooddata[gooddataind[0]])) {
	  badpixindex[gi][running_bpi[gi]] = currentpixel;
	  running_bpi[gi]++;
	}
	// dynamic memory reallocation
	if (running_bpi[gi] >= maxbadpix[gi]) {
	  maxbadpix[gi] *= 2;
	  badpixindex[gi].resize(maxbadpix[gi],0);
	}
      }
    }
  }
}



//***************************************************************************************
// ID the max value in the stack, reject it, and calculate meansq and rmssq
//***************************************************************************************
void stackfilter_rejectmax(const vector<float>&data, const vector<long>&index, 
			   const long &dim, float &meanval, float &rmsval)
{
  // Initialisation
  float maxval = 0., di;
  long i;
  
  maxval = data[index[0]];
  meanval = rmsval = 0.;

  for (i=0; i<dim; i++) {
    di = data[index[i]];
    if (di > maxval) maxval = di;
    meanval += di;
    rmsval += di*di;
  }

  meanval -= maxval;
  rmsval -= (maxval*maxval);
}


//***************************************************************************************
// ID the min and max values in the stack, reject them, and calculate meansq and rmssq
//***************************************************************************************
void stackfilter_rejectminmax(const vector<float>&data, const vector<long>&index, 
			      const long &dim, float &meanval, float &rmsval)
{
  // Initialisation
  float minval = 0.;
  float maxval = 0.;
  float di;
  long i;
  
  minval = maxval = data[index[0]];
  meanval = rmsval = 0.;

  for (i=0; i<dim; i++) {
    di = data[index[i]];
    if (di < minval) minval = di;
    else if (di > maxval) maxval = di;
    meanval += di;
    rmsval += di*di;
  }

  meanval -= (minval + maxval);
  rmsval -= (minval*minval + maxval*maxval);
}


//***************************************************************************************
// Write the weight image. We use the valarray directly to avoid unnecessary type 
// conversion. Otherwise I rather avoid them.
//***************************************************************************************
void writeWeight(image_type &data, vector<char> &weight_out)
{
  ulong i;
  myFITS weightout(data.weight, false);
  valarray<float> data_out;
  weightout.readData(data_out);
  for (i=0; i<data_out.size(); i++) 
    data_out[i] *= weight_out[i];
  weight_out.clear();
  writeImage(data.weight, data_out, data.naxis1, data.naxis2, -32);
}


//**************************************************************
void writeimages_thread(image_type *data, long maskwidth, long clustersize, long coadd_naxis1, 
			long num_images_min, long num_images_max)
{
  long i, j, k, l, n, m, s, t, o;
  long masklinemin, masklinemax;
  long maskcolmin, maskcolmax;
  long clustercount;
  vector<char> weight_out, weight_cpy;

  // cout << "This is thread " << args->th << endl;

  for (i=num_images_min; i<=num_images_max; i++) {
    s = 0;
    t = 0;
    n = data[i].naxis1;
    m = data[i].naxis2;
    weight_out.resize(n*m,0);
    weight_cpy.resize(n*m,0);

    // mask all bad pixels found
    for (j=data[i].yoffset; j<(data[i].yoffset + m); j++) {
      for (l=data[i].xoffset; l<(data[i].xoffset + n); l++) {
        k = l + coadd_naxis1 * j;
        weight_out[s] = 1;
        weight_cpy[s] = 1;
        if (k == badpixindex[i][t]) {
          weight_out[s] = 0;
          if (clustersize < 2) weight_cpy[s] = 0;
          t++;
        }
        s++;
      }
    }

    // mask only clusters consisting of at least 'clustersize' pixels
    if (clustersize > 1) {
      // we ignore the 1 pixel wide border of the image
      for (j=1; j<m-1; j++) {
        for (k=1; k<n-1; k++) {
          if (weight_out[k+n*j] == 0) {
            clustercount = 1;
            for (l=j-1; l<=j+1; l++) {
              for (o=k-1; o<=k+1; o++) {
                clustercount += (weight_out[o+(l*n)] ^ 1);
              }
            }
            if (clustercount >= clustersize) {
              for (l=j-1; l<=j+1; l++) {
                for (o=k-1; o<=k+1; o++) {
                  weight_cpy[o+(l*n)] = weight_out[o+(l*n)];
                }
              }              
            }
          }
        }
      }
      // copy the vectors
      weight_out = weight_cpy;
    }

    // if a border of width 'width' pixels should be masked around a 
    // bad pixel, too 
    if (maskwidth > 0) {
      for (j=0; j<m; j++) {
        for (k=0; k<n; k++) {
          if (weight_cpy[k+n*j] == 0) {
            masklinemin = (j - maskwidth) < 0 ? 0 : (j - maskwidth);
            maskcolmin  = (k - maskwidth) < 0 ? 0 : (k - maskwidth);
            masklinemax = (j + maskwidth) > (m - 1) ? (m - 1) : (j + maskwidth);
            maskcolmax  = (k + maskwidth) > (n - 1) ? (n - 1) : (k + maskwidth);   
            for (l=masklinemin; l <= masklinemax; l++) {
              for (o=maskcolmin; o <= maskcolmax; o++) {
                weight_out[o+(l*n)] = 0;
              }
            }
          }
        }
      }
    }

    weight_cpy.clear();

    // write the new weight image, max 3 threads allowed

    // reduce the semaphor by 1
    sem_wait(&semaphor);

    /*
    int sval;
    sem_getvalue(&semaphor, &sval);
    cout << "Active threads: " << 3-sval << endl;
    */
    writeWeight(data[i], weight_out);
    sem_post(&semaphor);

  }
}
