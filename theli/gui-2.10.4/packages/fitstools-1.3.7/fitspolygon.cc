#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

float sqrtsumsq(vector<float> const &, long);
void printstat(vector<float> &, long, char*);
void measure_stat(myFITS &image_in, vector<int>&, string, string, long, long, float);

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "          -i input_image\n";
    cerr << "         [-p ds9-type region file]\n";
    cerr << "         [-o output_image]\n";
    cerr << "         [-c scan direction]\n";
    cerr << "         [-l low threshold]\n";
    cerr << "         [-a (mask pixels only if fabs(pixel) lower than low_threshold)]\n";
    cerr << "         [-t comparison image]\n";
    cerr << "         [-u comparison image lower threshold]\n";
    cerr << "         [-w comparison image mask value (inverted)]\n";
    cerr << "         [-n (if set, no statistics will be measured and printed)]\n";
    cerr << "         [-k and|or (combination type if several masks in the ds9 region file; default: or)]\n";
    cerr << "         [-r min max (min and max column/row in the polygon to stack)]\n";
    cerr << "         [-s in|out (keep pixels inside|outside polygon; default: in)]\n";
    cerr << "         [-v masking value (default: 0.0)]\n";
    cerr << "         [-m mean|median|sum|rms|rmsnpix|sqrtsumsq (return one of these)]\n\n";
    cerr << "  PURPOSE: Does various things with a FITS image and a ds9 region file.\n\n";
    cerr << "      If a scan direction is specified, the corresponding statistical value\n";
    cerr << "      will be printed for each line or row, instead of for the entire polygon\n";
    cerr << "      area. If an output image is specified, then a masked copy of the input\n";
    cerr << "      image will be written to it.\n";
    cerr << "      Option -l defines a lower threshold. Pixels less than this value will\n";
    cerr << "      be set to the mask value defined with option -v.\n";
    cerr << "      Option -t provides a comparison image. Pixels with the mask value\n";
    cerr << "      (option -u) will be set to the masking value (-v). Alternatively,\n";
    cerr << "      pixels OTHER THAN the mask value (option -w) will be set to the masking value (-v).\n";
    cerr << "      If the ds9 region file contains the header line\n\n";
    cerr << "      # Sense: xxx\n\n";
    cerr << "      where 'xxx' is either 'in' or 'out', then only pixels inside or outside\n";
    cerr << "      the polygon or circle will be kept. If no such header line is present,\n";
    cerr << "      then 'in' is assumed. If the sense is specified with option -s on the\n";
    cerr << "      command line, then the (or any) header line in the region file will be\n";
    cerr << "      ignored. Likewise, if the ds9 region file contains the header line\n\n";
    cerr << "      # Combine: xxx\n\n";
    cerr << "      where 'xxx' is either 'and' or 'or', then only pixels common to both mask,\n";
    cerr << "      respectively in either mask, will be kept. If no such header line is present,\n";
    cerr << "      then we assume 'or'. The value in the mask can be overridden with option -k.\n\n";
  }
  
  exit (0);
}


//**************************************************************
int main(int argc, char *argv[])
{
  long i, n, m, nc, mc, min, max;
  string input_image, output_image, comparison_image, mode, scandir;
  string combine="", area=""; // must be initialised to empty strings
  char ds9region[FILEMAX];
  float maskvalue, lowthresh, compthresh;
  bool flag_out = false, flag_low = false, flag_comp = false, flag_poly = false;
  bool flag_mode = false, flag_abs = false, flag_u = false, flag_w = false, flag_stat = false;

  maskvalue = 0.0;
  lowthresh = 0.;
  compthresh = 0.;
  min = -1;
  max = -1;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  //  Read command line arguments

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'i': input_image = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	flag_out = true;
	break;
      case 'p': strcpy(ds9region,argv[++i]);
	flag_poly = true;
	break;
      case 'k': combine = argv[++i];
	break;
      case 's': area = argv[++i];
	break;
      case 'c': scandir = argv[++i];
	break;
      case 'm': mode = argv[++i];
	flag_mode = true;
	break;
      case 't': comparison_image = argv[++i];
	flag_comp = true;
	break;
	// only one of the following two can be specified
      case 'u': compthresh = atof(argv[++i]);
	flag_u = true;
	break;
      case 'w': compthresh = atof(argv[++i]);
	flag_w = true;
	break;
      case 'r': 
	min = atof(argv[++i]) - 1;
	max = atof(argv[++i]) - 1;
	break;
      case 'v': maskvalue = atof(argv[++i]);
	break;
      case 'n': flag_stat = true;
	break;
      case 'l': lowthresh = atof(argv[++i]);
	flag_low = true;
	break;
      case 'a': flag_abs = true;
	break;
      }
    }
  }
  
  if (area.compare("in") != 0 && area.compare("out") != 0 && area.compare("") != 0) {
    cerr <<  "ERROR: Invalid area specification.\n";
    cerr <<  "       Must be 'in' or 'out' if specified with option -s.\n";
    exit (1);
  }

  if (flag_u == 1 && flag_w == 1) {
    cerr <<  "ERROR: Only one of the options -u and -w can be specified at a time!\n";
    exit (1);
  }

  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the image
  myFITS image_in(input_image);
  myFITS image_comp(comparison_image, false);
  n = image_in.naxis1;
  m = image_in.naxis2;

  // the mask image
  vector<int> mask(n*m,0);

  // if a mask was (not) provided
  if (flag_poly) make_ds9_maskimage(mask, ds9region, n, m, area, combine);
  else {
    for (i=0; i<n*m; i++) mask[i] = 1;
  }

  // load the comparison image if requested
  if (flag_comp) {
    image_comp.readData();
    nc = image_comp.naxis1;
    mc = image_comp.naxis2;
    if (n != nc || m != mc) {
      cerr << "ERROR: Images do not have the same dimensions!\n";
      return 1;
    }
  }

  // mask the input image
  for (i=0; i<n*m; i++) {
    if (!flag_abs) {
      if (flag_u) {
	if ( (mask[i] == 0) || 
	     (flag_low  && image_in.data[i] <= lowthresh) ||
	     (flag_comp && image_comp.data[i] == compthresh)) {
	  image_in.data[i] = maskvalue;
	}
      }
      if (flag_w) {
	if ( (mask[i] == 0) || 
	     (flag_low  && image_in.data[i] <= lowthresh) ||
	     (flag_comp && image_comp.data[i] != compthresh)) {
	  image_in.data[i] = maskvalue;
	}
      }
      if (!flag_u && !flag_w) {
	if ( (mask[i] == 0) || 
	     (flag_low && image_in.data[i] <= lowthresh)) {
	  image_in.data[i] = maskvalue;
	}
      }
    }
    else {
      if (flag_u) {
	if ( (mask[i] == 0) || 
	     (flag_low  && fabs(image_in.data[i]) <= lowthresh) ||
	     (flag_comp && fabs(image_comp.data[i]) == compthresh)) {
	  image_in.data[i] = maskvalue;
	}
      }
      if (flag_w) {
	if ( (mask[i] == 0) || 
	     (flag_low  && fabs(image_in.data[i]) <= lowthresh) ||
	     (flag_comp && fabs(image_comp.data[i]) != compthresh)) {
	  image_in.data[i] = maskvalue;
	}
      }
      if (!flag_u && !flag_w) {
	if ( (mask[i] == 0) || 
	     (flag_low && fabs(image_in.data[i]) <= lowthresh)) {
	  image_in.data[i] = maskvalue;
	}
      }
    }
  }

  // save the masked image if requested
  if (flag_out) writeImage(output_image, input_image, image_in.data, call);

  // print statistics if not suppressed
  if (!flag_stat && flag_mode) {
    measure_stat(image_in, mask, mode, scandir, min, max, maskvalue);
  }

  return 0;
}


//****************************************************
float sqrtsumsq(vector<float> const &data, long dim)
{
  float sum;
  long i;

  sum = 0.;
  for (i=0; i<dim; i++) {
    sum += data[i]*data[i];
  }
  return sqrt(sum);
}


//***********************************************************************
void printstat(vector<float> &data, long dim, string mode)
{
  if (mode.compare("mean") == 0)      printf("%f\n", mean(data, dim));
  if (mode.compare("median") == 0)    printf("%g\n", median(data, dim, "conserve"));
  if (mode.compare("sum") == 0)       printf("%g\n", mean(data, dim)*(float) dim);
  if (mode.compare("rms") == 0)       printf("%g\n", rms(data, dim));
  if (mode.compare("rmsnpix") == 0)   printf("%g\n", rms(data, dim)*sqrt((float) dim));
  if (mode.compare("sqrtsumsq") == 0) printf("%g\n", sqrtsumsq(data, dim));
}


//***********************************************************************
void measure_stat(myFITS &image_in, vector<int> &mask, string mode, 
		   string scandir, long min, long max, float maskvalue)
{
  long n, m, k, i, j;

  n = image_in.naxis1;
  m = image_in.naxis2;

  vector<float> data(n*m,0);
  vector<float> coord(n*m,0);

  // obtain global statistics
  if (scandir.empty()) {
    k = 0;
    for (i=0; i<n*m; i++) {
      if (image_in.data[i] != maskvalue && mask[i] != 0) {
	data[k++] = image_in.data[i];
      }
    }
    printstat(data, k, mode);
  }

  // obtain statistics along x-direction in polygon
  if (scandir.compare("x") == 0 && min < 0 && max < 0) {
    for (j=0; j<m; j++) {
      k = 0;
      for (i=0; i<n; i++) {
	if (image_in.data[i+n*j] != maskvalue && mask[i+n*j] != 0) {
	  data[k++] = image_in.data[i+n*j];
	}
      }
      if (k>0) {
	cout << j+1 << " ";
	printstat(data, k, mode);
      }
    }
  }

  // obtain statistics along y-direction in polygon
  if (scandir.compare("y") == 0 && min < 0 && max < 0) {
    for (i=0; i<n; i++) {
      k = 0;
      for (j=0; j<m; j++) {
	if (image_in.data[i+n*j] != maskvalue && mask[i+n*j] != 0) {
	  data[k++] = image_in.data[i+n*j];
	}
      }
      if (k>0) {
	cout << i+1 << " ";
	printstat(data, k, mode);
      }
    }
  }

  // obtain statistics along x-direction in polygon, within some rows
  if (scandir.compare("x") == 0 && min >= 0 && max >= 0) {
    k = 0;
    for (j=0; j<m; j++) {
      for (i=0; i<n; i++) {
	if (image_in.data[i+n*j] != maskvalue && j >= min && j <= max && mask[i+n*j] != 0) {
	  data[k] = image_in.data[i+n*j];
	  coord[k] = (float) j;
	  k++;
	}
      }
    }
    if (k>0) {
      cout << mean(coord,k)+1. << " " << rms(coord,k) << " ";
      printstat(data, k, mode);
    }
  }
  
  // obtain statistics along y-direction in polygon, within some rows
  if (scandir.compare("y") == 0 && min >= 0 && max >= 0) {
    k = 0;
    for (j=0; j<m; j++) {
      for (i=0; i<n; i++) {
	if (image_in.data[i+n*j] != maskvalue && i >= min && i <= max && mask[i+n*j] != 0) {
	  data[k] = image_in.data[i+n*j];
	  coord[k] = (float) i;
	  k++;
	}
      }
    }
    if (k>0) {
      cout << mean(coord,k)+1. << " " << rms(coord,k) << " ";
      printstat(data, k, mode);
    }
  }
}
