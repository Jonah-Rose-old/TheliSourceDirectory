#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "          [-m (mean, median, sum, rms, rmsnpix, min, max, npix)]\n";
    cerr << "          [-x xmin xmax ymin ymax]\n";
    cerr << "          [-b value (ignore pixels with that value)]\n";
    cerr << "          [-q (try identify blanked pixels and ignore them)]\n";
    cerr << "          [-v (print a list with pixel values instead)]\n\n";
    cerr << "  PURPOSE: returns a specific straight statistics of an image (no funny filtering).\n";
    cerr << "           'rmsnpix' = rms * sqrt(npix).\n";

    exit(1);
  }
}


//*****************************************************************
int main(int argc, char *argv[])
{
  long i, j, n, m, k, dim, values;
  long xmin, xmax, ymin, ymax;
  float ignore, blankvalue;
  string input_image, mode, result;
  float rmsval, meanval, medval, sum, minval, maxval;
  bool region = false, flag_ignore = false, flag_blank = false;

  region = 0;
  values = 0;
  xmin = 0;
  xmax = 0;
  ymin = 0;
  ymax = 0;
  ignore = 0.;
  blankvalue = 0.;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': 
	input_image = argv[++i];
	break;
      case 'm': 
	mode = argv[++i];
	break;
      case 'v': 
	values = 1;
	break;
      case 'x': 
	xmin = atol(argv[++i]) - 1;
	xmax = atol(argv[++i]) - 1;
	ymin = atol(argv[++i]) - 1;
	ymax = atol(argv[++i]) - 1;
	region = true;
	break;
      case 'b':
	ignore = atof(argv[++i]);
	flag_ignore = true;
	break;
      case 'q':
	flag_blank = true;
	break;
      }
    }
  }

  checkfile(input_image);

  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  dim = n*m;
  if (region) dim = (xmax-xmin+1) * (ymax-ymin+1);

  if (flag_blank) blankvalue = find_blankvalue(image.data, n, m, result);
  if (result.compare("UNKNOWN") == 0) flag_blank = false;

  vector<float> data(dim,0);

  k = 0;

  if (!flag_blank) {
    if (!flag_ignore) {
      for (j=0; j<m; j++) {
	for (i=0; i<n; i++) {
	  if ( !region || (region && i>=xmin && i<=xmax && j>=ymin && j<=ymax)) {
	    data[k++] = image.data[i+n*j];
	  }
	}
      }
    }
    else {
      for (j=0; j<m; j++) {
	for (i=0; i<n; i++) {
	  if ( !region || (region && i>=xmin && i<=xmax && j>=ymin && j<=ymax)) {
	    if ( image.data[i+n*j] != ignore ) {
	      data[k++] = image.data[i+n*j];
	    }
	  }
	}
      }
    }
  }

  if (flag_blank) {
    if (!flag_ignore) {
      for (j=0; j<m; j++) {
	for (i=0; i<n; i++) {
	  if ( !region || (region && i>=xmin && i<=xmax && j>=ymin && j<=ymax)) {
	    if (image.data[i+n*j] != blankvalue && 
		!isnan(image.data[i+n*j]) && !isinf(image.data[i+n*j])) {
	      data[k++] = image.data[i+n*j];
	    }
	  }
	}
      }
    }
    else {
      for (j=0; j<m; j++) {
	for (i=0; i<n; i++) {
	  if ( !region || (region && i>=xmin && i<=xmax && j>=ymin && j<=ymax)) {
	    if ( image.data[i+n*j] != ignore && image.data[i+n*j] != blankvalue &&
		 !isnan(image.data[i+n*j]) && !isinf(image.data[i+n*j])) {
	      data[k++] = image.data[i+n*j];
	    }
	  }
	}
      }
    }
  }

  dim = k;
  if (values == 1) {
    for (i=0; i<dim; i++) {
      printf("%g\n", data[i]);
    }
    return 0;
  }

  rmsval  = rms(data, dim);
  meanval = mean(data, dim);
  medval  = median(data, dim);
  minval  = min(data, dim);
  maxval  = max(data, dim);

  sum = 0.;
  for (i=0; i<dim; i++) {
    sum += data[i];
  }

  if (mode.compare("rms") == 0)     printf("%g\n", rmsval);
  if (mode.compare("rmsnpix") == 0) printf("%g\n", rmsval*sqrt((float)dim));
  if (mode.compare("mean") == 0)    printf("%g\n", meanval);
  if (mode.compare("median") == 0)  printf("%g\n", medval);
  if (mode.compare("sum") == 0)     printf("%g\n", sum);
  if (mode.compare("min") == 0)     printf("%g\n", minval);
  if (mode.compare("max") == 0)     printf("%g\n", maxval);
  if (mode.compare("npix") == 0)    printf("%ld\n", dim);

  if (mode.empty()) {
    cout << "# mean  median  sum  rms npix rms*sqrt(npix) min max npix\n";
    printf("%g %g %g %g %ld %g %g %g %ld\n", meanval, medval, sum, rmsval, 
	   dim, rmsval*sqrt((float)dim), minval, maxval, dim);
  }

  return 0;
}
