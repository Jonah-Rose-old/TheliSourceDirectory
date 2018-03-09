#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

double coll_x(vector<float>&, vector<double>&, long, long, vector<int>&, bool, string); 
double coll_y(vector<float>&, vector<double>&, long, long, vector<int>&, bool, string);
double coll_cross(vector<float>&, vector<double>&, long, long, vector<int>&, bool, string, string); 
double coll_quad(vector<float>&, vector<double>&, long, long, vector<int>&, bool, string, string); 
double mean_sigma(vector<double>&, string);
void write_image(vector<float>&, vector<double>&, vector<int>&, string, string, double, bool, bool);
void report_resetanomaly(vector<double>&, long, long);
void create_mask(vector<float>&, vector<int>&, long, long, long, long, long, long, int, int, string, char*);

double threshold, kappa, maskval = -1.e9;
uint niter, masked;
bool report;
string targetimage_in, targetimage_out, maskimage_out, call;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           -o output_image \n";
    cerr << "           -c (x, y, xy, xyyx, yxxy, xxxx or yyyy collapse direction(s))\n";
    cerr << "           [-m maskimage_in]\n";
    cerr << "           [-q maskimage_out]\n";
    cerr << "           [-d ds9 region file]\n";
    cerr << "           [-p targetimage_in targetimage_out]\n";
    cerr << "           [-s (subtract the collapsed image)]\n";
    cerr << "           [-t lower threshold (-60000.)]\n";
    cerr << "           [-k kappa-sigma-threshold (2.5)]\n";
    cerr << "           [-x xmin xmax ymin ymax (region that is ignored)]\n";
    cerr << "           [-j (invert the ignored region)]\n";
    cerr << "           [-n number of iterations (5)]\n";
    cerr << "           [-r report reset anomaly amplitude]\n";
    cerr << "           [-v calculate rms instead of mean column]\n";
    cerr << "           [-a add columns/rows instead of calculating mean]\n\n";
    cerr << "  PURPOSE: Collapses images along x/y-directions.\n\n";
    cerr << "  NOMENCLATURE: \n";
    cerr << "           input_image:   image that is to be corrected.\n";
    cerr << "           output_image:  name of the final corrected image.\n";
    cerr << "           maskimage_in:  image that is zero for good, and non-zero for bad pixels.\n";
    cerr << "           maskimage_out: input_image, with maskimage_in superimposed.\n";
    cerr << "           targetimage_in: another image, that should have the mask imprinted on it.\n";
    cerr << "           targetimage_out: name of the output target image.\n\n";
    cerr << "     If the -s option is given, then the output is the input image\n";
    cerr << "     minus the collapsed image. Otherwise the collapsed image is returned.\n";
    cerr << "     Pixel values smaller than -t are ignored.\n\n";
    cerr << "     An iterative loop is run for outlier rejection. Options -n and -k\n";
    cerr << "     specify the number of iterations and the sigma threshold.\n";
    cerr << "     Option -x specifies a box that is excluded from the measurement.\n";
    cerr << "     With -r the amplitude of the reset anomaly is returned.\n\n";
    cerr << "  COLLAPSE directions:\n\n";
    cerr << "     x:    Image collapsed horizontally (to remove vertical gradients)\n";
    cerr << "     y:    Image collapsed vertically (to remove horizontal gradients)\n";
    cerr << "     xy:   Image collapsed horizontally and vertically\n";
    cerr << "     xyyx: Lower left, lower right, upper left, upper right quadrant\n";
    cerr << "           collapsed along x, y, y and x, respectively\n";
    cerr << "     yxxy: Lower left, lower right, upper left, upper right quadrant\n";
    cerr << "           collapsed along y, x, x and y, respectively\n";
    cerr << "     xxxx: All four quadrants are collapsed horizontally, but independently from each other." << endl;
    cerr << "     yyyy: All four quadrants are collapsed vertically, but independently from each other." << endl;
    exit(1);
  }
}

//**********************************************************************
int main(int argc, char *argv[])
{
  long n, m, i, k, xmin, xmax, ymin, ymax;
  int invert;
  double sky;
  char ds9region[4096];
  string input_image, output_image, maskimage_in, direction, addstring;
  bool rmsval, add, subtract;

  subtract = false;
  niter = 5;
  kappa = 2.5;
  threshold = -60000.;
  masked = 0;
  invert = 0;

  xmin = xmax = ymin = ymax = 0;
  report = false;
  rmsval = false;
  add = false;
  sky = 0.0;

  strcpy(ds9region,"");

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	break;
      case 'd': strcpy(ds9region,argv[++i]);
	break;
      case 'p': targetimage_in = argv[++i];
             	targetimage_out = argv[++i];
	break;
      case 'm': maskimage_in = argv[++i];
	masked = 1;
	break;
      case 'q': maskimage_out = argv[++i];
	break;
      case 'c': direction = argv[++i];
	break;
      case 's': subtract = true;
	break;
      case 't': threshold = atof(argv[++i]);
	break;
      case 'k': kappa = atof(argv[++i]);
	break;
      case 'n': niter = atol(argv[++i]);
	break;
      case 'j': invert = 1;
	break;
      case 'x': 
	xmin = atol(argv[++i]) - 1;
	xmax = atol(argv[++i]) - 1;
	ymin = atol(argv[++i]) - 1;
	ymax = atol(argv[++i]) - 1;
	break;
      case 'r': report = true;
	break;
      case 'v': rmsval = true;
	break;
      case 'a': add = true;
	break;
      }
    }
  }

  if (add) addstring="add";
  else addstring = "noadd";

  if (direction.empty()) {
    cerr <<  "ERROR: No collapse direction pattern (option -c) was specified!\n";
    exit (1);
  }

  checkfile(input_image);
  call = programcall(argv, argc);

  // read the FITS data block
  myFITS image_in(input_image);
  n = image_in.naxis1;
  m = image_in.naxis2;

  vector<double> collap(n*m,0);
  vector<int> mask(n*m,0);

  // create the mask image
  create_mask(image_in.data, mask, n, m, xmin, xmax, ymin, ymax, masked, invert, maskimage_in, ds9region);
    
  // collapse direction: x, y, yxxy, xyyx
  if (direction.compare("x") == 0) 
    sky = coll_x(image_in.data, collap, n, m, mask, rmsval, addstring);
  if (direction.compare("y") == 0) 
    sky = coll_y(image_in.data, collap, n, m, mask, rmsval, addstring);
  if (direction.compare("xyyx") == 0 || direction.compare("yxxy") == 0 )
    sky = coll_cross(image_in.data, collap, n, m, mask, rmsval, addstring, direction);
  if (direction.compare("xxxx") == 0 || direction.compare("yyyy") == 0 )
    sky = coll_quad(image_in.data, collap, n, m, mask, rmsval, addstring, direction);

  if (report && direction.compare("xy") != 0) {
    report_resetanomaly(collap, n, m);
    return 0;
  }

  // write the result
  if (direction.compare("xy") != 0)
    write_image(image_in.data, collap, mask, input_image, output_image, sky, subtract, rmsval);

  // collapse direction: xy
  if (direction.compare("xy") == 0) {
    vector<double> collap_all(n*m,0);
    // collapse along x
    sky = coll_x(image_in.data, collap, n, m, mask, rmsval, addstring);
    vector<float> tmp(n*m,0);
    for (k=0; k<n*m; k++) {
      collap_all[k] = collap[k];
      tmp[k] = image_in.data[k] - collap[k];
      collap[k] = 0.;
    }
    // collapse along y
    coll_y(tmp, collap, n, m, mask, rmsval, addstring);
    for (k=0; k<n*m; k++) collap_all[k] += collap[k];
    write_image(image_in.data, collap_all, mask, input_image, output_image, sky, subtract, rmsval);
  }

  return 0;
}


//*****************************************************************
// make a mask image
void create_mask(vector<float> &data_in, vector<int> &mask, long n, long m, long xmin, long xmax, 
		 long ymin, long ymax, int masked, int invert, 
		 string maskimage_in, char *ds9region)
{
  long i, j;

  // The mask image is zero for all good pixels, and 1 for all bad pixels.
  // (i.e. objects detected by sextractor, and a rectangular regioned 
  // pointed out by the user, and a ds9 region file)
  
  // we start out with mask[i] = 0 everywhere

  // RECTANGLE (user provided)
  if (xmin >= 0 && xmax >= 0 && ymin >= 0 && ymax >= 0) {
    // if the mask is uninverted
    if (invert == 0) {
      for (j=0; j<m; j++) {
	for (i=0; i<n; i++) {
	  if (i>=xmin && i<=xmax && j>=ymin && j<=ymax) mask[i+n*j] = 1; 
	}
      }
    }
    // if the mask is inverted
    if (invert == 1) {
      for (j=0; j<m; j++) {
	for (i=0; i<n; i++) {
	  if (!(i>=xmin && i<=xmax && j>=ymin && j<=ymax)) mask[i+n*j] = 1; 
	}
      }
    }
  }
  
  // SEXTRACTOR OBJECT image (zero for good pixels)
  if (masked == 1) {
    myFITS imagemask(maskimage_in);
    for (i=0; i<n*m; i++)
      if (imagemask.data[i] != 0.) mask[i] = 1;
  }

  // DS9 REGION FILE
  if (strcmp(ds9region,"") != 0) {
    vector<int> ds9mask(n*m,0);

    // the empty strings means we use keywords in the ds9 region file
    // that is, good pixels will be one and bad are zero, which we then have to invert
    make_ds9_maskimage(ds9mask, ds9region, n, m, "", "");
    
    // complement the mask image with the ds9 region;
    // we assume the mask is zero for good and 1 for bad pixels
    for (i=0; i<n*m; i++)
      if (ds9mask[i] == 0) mask[i] = 1;
  }
  
  // THRESHOLDING
  for (i=0; i<n*m; i++)
    if (data_in[i] < threshold) mask[i] = 1;
}


//*****************************************************************
// the kappa-sigma average for an array
double mean_sigma(vector<double> &data, string add)
{
  ulong i, j, k, dim2;
  ulong dim = data.size();
  double avg, sdev, avgfinal;
  
  vector<double> tmp1(data);
  vector<double> tmp2(dim,0);

  avgfinal = 0.;
  
  dim2 = dim;
  avg  = mean(tmp1, dim2, maskval);
  sdev = rms(tmp1, dim2, maskval);
  
  for (k=0; k<niter; k++) {
    j = 0;
    for (i=0; i<dim2; i++)  {
      if ( tmp1[i] < avg + kappa * sdev && 
	   tmp1[i] > avg - kappa * sdev) {
	tmp2[j++] = tmp1[i];
      }
    }
    avg  = mean(tmp2, j, maskval);
    sdev = rms(tmp2, j, maskval);

    avgfinal = mean(tmp2, j, maskval, add);
    
    for (i=0; i<j; i++) tmp1[i] = tmp2[i];
    dim2 = j;
  }

  if (sdev == 0.) avgfinal = mean(data, dim, maskval, add);
  
  return (avgfinal);
}


//*****************************************************************
// collapse along x

double coll_x(vector<float> &data, vector<double> &collap, long n, long m, vector<int> &mask, bool rmsval, string add) 
{
  long i, j;
  double sky;

  // average columns / rows
  vector<double> row(n,0);
  vector<double> col(m,0);

  // extract rows, average with sigma-rejection
  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      if (mask[i+n*j] == 0) row[i] = data[i+n*j];
      else row[i] = maskval;
    }
    if (!rmsval) col[j] = mean_sigma(row, add);
    else col[j] = rms(row, n, maskval);
  }
  // write collapsed FITS array
  for (i=0; i<n; i++)  {
    for (j=0; j<m; j++)  {
      collap[i+n*j] = col[j];
    }
  }
  // the sky background estimate
  sky = median(col, m);

  return (sky);
}


//*****************************************************************
// collapse along y

double coll_y(vector<float> &data, vector<double> &collap, long n, long m, vector<int> &mask, bool rmsval, string add) 
{
  long i, j;
  double sky;

  // average columns / rows
  vector<double> row(n,0);
  vector<double> col(m,0);

  // extract columns, average with sigma-rejection
  for (i=0; i<n; i++) {
    for (j=0; j<m; j++) {
      if (mask[i+n*j] == 0) col[j] = data[i+n*j];
      else col[j] = maskval;
    }
    if (!rmsval) row[i] = mean_sigma(col, add);
    else row[i] = rms(col, m, maskval);
  }
  // write collapsed FITS array
  for (j=0; j<m; j++)  {
    for (i=0; i<n; i++)  {
      collap[i+n*j] = row[i];
    }
  }

  // add the sky value if it doesn't exist already
  sky = median(row, n);

  return (sky);
}


//*****************************************************************
// collapse along vhhv (vertical, horizontal, horizontal, vertical readout quadrants)
// or hvvh directions
// (for certain HAWAII-2 arrays, such as FLAMINGOS2@GEMINI or MOIRCS@SUBARU)

double coll_cross(vector<float> &data, vector<double> &collap, long n, long m, 
		  vector<int> &mask, bool rmsval, string add, string mode) 
{
  long i, j, nh, mh, loop, n1, m1;
  double sky;
  
  n1 = 0;
  m1 = 0;
  sky = 0.;

  if (n % 2 != 0 || m % 2 != 0) {
    cerr << "ERROR: image must have even numbered dimensions for this operation!\n";
    cerr << "       Dimensions: " << n << "x" << m << endl;
    exit (1);
  }

  nh = n/2;
  mh = m/2;
  
  vector<float> quadrant(nh*mh,0);
  vector<double> collquad(nh*mh,0);
  vector<int> maskquad(nh*mh,0);

  // do the four quadrants
  loop = 0;

  while (loop <= 3) {
    // the four quadrants
    // lower left
    if (loop == 0) {
      n1 = 0;
      m1 = 0;
    }
    // lower right
    if (loop == 1) {
      n1 = nh;
      m1 = 0;
    }
    // upper left
    if (loop == 2) {
      n1 = 0;
      m1 = mh;
    }
    // upper right
    if (loop == 3) {
      n1 = nh;
      m1 = mh;
    }

    // copy the quadrant and the mask
    for (j=0; j<mh; j++) {
      for (i=0; i<nh; i++) {
	quadrant[i+nh*j] = data[i+n1+n*(j+m1)];
	maskquad[i+nh*j] = mask[i+n1+n*(j+m1)];
      }
    }
    
    // collapse the quadrant
    if (mode.compare("yxxy") == 0) {
      if (loop == 0) sky  = coll_y(quadrant, collquad, nh, mh, maskquad, rmsval, add);
      if (loop == 1) sky += coll_x(quadrant, collquad, nh, mh, maskquad, rmsval, add);
      if (loop == 2) sky += coll_x(quadrant, collquad, nh, mh, maskquad, rmsval, add);
      if (loop == 3) sky += coll_y(quadrant, collquad, nh, mh, maskquad, rmsval, add);
    }
    if (mode.compare("xyyx") == 0) {
      if (loop == 0) sky  = coll_x(quadrant, collquad, nh, mh, maskquad, rmsval, add);
      if (loop == 1) sky += coll_y(quadrant, collquad, nh, mh, maskquad, rmsval, add);
      if (loop == 2) sky += coll_y(quadrant, collquad, nh, mh, maskquad, rmsval, add);
      if (loop == 3) sky += coll_x(quadrant, collquad, nh, mh, maskquad, rmsval, add);
    }

    // reconstruct the full (n x m) sized image
    for (j=0; j<mh; j++) {
      for (i=0; i<nh; i++) {
	collap[i+n1+n*(j+m1)] = collquad[i+nh*j];
      }
    }
    loop++;
  }

  return (sky/4.);
}


//***********************************************************************
// collapse quadrants independently from each other in the same direction
// (Greetings from Las Cumbres Global Telescope at CTIO...

double coll_quad(vector<float> &data, vector<double> &collap, long n, long m, 
		  vector<int> &mask, bool rmsval, string add, string mode) 
{
  long i, j, nh, mh, loop, n1, m1;
  double sky;
  
  n1 = 0;
  m1 = 0;
  sky = 0.;

  if (n % 2 != 0 || m % 2 != 0) {
    cerr << "ERROR: image must have even numbered dimensions for this operation!\n";
    cerr << "       Dimensions: " << n << "x" << m << endl;
    exit (1);
  }

  nh = n/2;
  mh = m/2;
  
  vector<float> quadrant(nh*mh,0);
  vector<double> collquad(nh*mh,0);
  vector<int> maskquad(nh*mh,0);

  // do the four quadrants
  loop = 0;

  while (loop <= 3) {
    // the four quadrants
    // lower left
    if (loop == 0) {
      n1 = 0;
      m1 = 0;
    }
    // lower right
    if (loop == 1) {
      n1 = nh;
      m1 = 0;
    }
    // upper left
    if (loop == 2) {
      n1 = 0;
      m1 = mh;
    }
    // upper right
    if (loop == 3) {
      n1 = nh;
      m1 = mh;
    }

    // copy the quadrant and the mask
    for (j=0; j<mh; j++) {
      for (i=0; i<nh; i++) {
	quadrant[i+nh*j] = data[i+n1+n*(j+m1)];
	maskquad[i+nh*j] = mask[i+n1+n*(j+m1)];
      }
    }
    
    // collapse the quadrant
    if (mode.compare("xxxx") == 0) {
      if (loop == 0) sky  = coll_x(quadrant, collquad, nh, mh, maskquad, rmsval, add);
      if (loop == 1) sky += coll_x(quadrant, collquad, nh, mh, maskquad, rmsval, add);
      if (loop == 2) sky += coll_x(quadrant, collquad, nh, mh, maskquad, rmsval, add);
      if (loop == 3) sky += coll_x(quadrant, collquad, nh, mh, maskquad, rmsval, add);
    }
    if (mode.compare("yyyy") == 0) {
      if (loop == 0) sky  = coll_y(quadrant, collquad, nh, mh, maskquad, rmsval, add);
      if (loop == 1) sky += coll_y(quadrant, collquad, nh, mh, maskquad, rmsval, add);
      if (loop == 2) sky += coll_y(quadrant, collquad, nh, mh, maskquad, rmsval, add);
      if (loop == 3) sky += coll_y(quadrant, collquad, nh, mh, maskquad, rmsval, add);
    }

    // reconstruct the full (n x m) sized image
    for (j=0; j<mh; j++) {
      for (i=0; i<nh; i++) {
	collap[i+n1+n*(j+m1)] = collquad[i+nh*j];
      }
    }
    loop++;
  }

  return (sky/4.);
}


//********************************************************************
void write_image(vector<float> &data_in, vector<double> &collap, vector<int> &mask, string input_image, 
		 string output_image, double sky, bool subtract, bool rmsval)
{
  ulong k, dim;

  dim = data_in.size();

  vector<float> data_out(dim,0);

  if (!subtract || rmsval) {
    for (k=0; k<dim; k++) data_out[k] = collap[k];
  }
  if (subtract && !rmsval) {
    for (k=0; k<dim; k++) data_out[k] = data_in[k] - collap[k];
  }

  writeImage(output_image, input_image, data_out, call);
  myFITS imageout(output_image, false, "Write");
  imageout.addKey("SKYVALUE", sky, "Sky background");

  // if a mask image exists
  if (masked == 1) {
    vector<float> data_mask_out(dim,0);
    
    // if the input image should be masked
    if (maskimage_out.compare("") != 0) {
      myFITS masked_output(maskimage_out,false,"Write");
      for (k=0; k<dim; k++) {
	if (mask[k] != 1) data_mask_out[k] = data_in[k] - collap[k];
	else data_mask_out[k] = -1e09;
      }
      writeImage(maskimage_out, input_image, data_mask_out);
    }
    
    // if an additonal image ("target_image") should be masked
    if (targetimage_in.compare("")  != 0 &&
	targetimage_out.compare("") != 0) {
      checkfile(targetimage_in);
      myFITS image_in_target(targetimage_in);
      for (k=0; k<dim; k++) {
	if (mask[k] != 1) data_mask_out[k] = image_in_target.data[k];
	else data_mask_out[k] = -1e09;
      }
      writeImage(targetimage_out, targetimage_in, data_mask_out);
    }
  }
}


//********************************************************************
void report_resetanomaly(vector<double> &collap, long n, long m)
{
  // this function will exit main() when called!
  long nred, mred, i, j, k, n1, n2, m1, m2;
  float maxamp;

  nred = 0.1 * n;
  mred = 0.1 * m;
  n1 = 0.4 * n;
  n2 = 0.5 * n;
  m1 = 0.4 * m;
  m2 = 0.5 * m;
  k = 0;
  maxamp = -1.0e-9;
  for (j=m1; j<m2; j++) {
    for (i=n1; i<n2; i++) {
      if (k<nred*mred) {
	if (collap[i+n*j] > maxamp) maxamp = collap[i+n*j];
	k++;
      }
    }
  }
  cout << maxamp << endl;
}
