#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

float gauss(float rad, float sigma);

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           -o output_image \n";
    cerr << "           [-w 1-sigma width (default: 1.0)]\n";
    cerr << "           [-e extent (sigma) (default: 3.0)]\n";
    cerr << "           [-m mask value]\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int n, m, mask, imin, imax, jmin, jmax, iu, ju;
  long i, j, k;
  float width, totalwidth, ext, dist, maskvalue;
  string input_image, output_image;
  
  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  mask = 0;
  maskvalue = -1.e9;
  width = 1.0;
  ext = 3.0;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
        case 'w': width = atof(argv[++i]);
            break;
        case 'e': ext = atof(argv[++i]);
            break;
	case 'm': mask = 1;
	    maskvalue = atof(argv[++i]);
            break;
      }
    }
  }

  checkfile(input_image);
  string call = programcall(argv, argc);
  
  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;
  
  vector<double> weight(n*m,0);
  vector<float> imfitsout(n*m,0);
  vector<double> imfits(n*m,0);

  // convolve with the gaussian
  totalwidth = width * ext;
  
  for (i=0; i<n; i++) {
    imin = (i - totalwidth > 0) ? (i-totalwidth) : 0;
    imax = (i + totalwidth < n) ? (i+totalwidth) : n-1;
    for (j=0; j<m; j++) {
      k = i + n * j;
      jmin = (j - totalwidth > 0) ? (j-totalwidth) : 0;
      jmax = (j + totalwidth < m)  ? (j+totalwidth) : m-1;
      for (iu = imin; iu <= imax; iu++) {
	for (ju = jmin; ju <= jmax; ju++) {
	  dist = (i-iu) * (i-iu) + (j-ju) * (j-ju);
	  if (mask == 0 && dist <= totalwidth * totalwidth) {
	    imfitsout[k] += image.data[iu+n*ju] * gauss(dist, width);
	    weight[k] += gauss(dist, width);
	  }
	  if (mask == 1 && dist <= totalwidth * totalwidth &&
	      image.data[iu+n*ju] != maskvalue && image.data[k] != maskvalue ) {
	    imfitsout[k] += image.data[iu+n*ju] * gauss(dist, width);
	    weight[k] += gauss(dist, width);
	  }
	}
      }
    }
  }

  // normalise
  for (k=0; k<n*m; k++) {
    if (weight[k] != 0.) imfitsout[k] /= weight[k];
    else imfitsout[k] = 0.;
  }

  writeImage(output_image, input_image, imfitsout, call);
  
  return 0;
}

// *****************************************************
float gauss(float rad, float sigma)
{
  float value;
  
  value = 0.398942 / sigma * exp(-rad/(2.*sigma*sigma));
  
  return (value);
}
