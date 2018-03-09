#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -o output_image \n";
    cerr << "           -d xdim ydim \n";
    cerr << "           -s width (in pixels, default: 1.0)\n";
    cerr << "          [-d xcen ycen \n]";
    cerr << "          [-n (normalised image)]\n\n";
    cerr << "      Makes an image containing a 2D Gaussian.\n";
    cerr << "      If no center coordinates are specified, the gaussian will be centred in the image.\n\n";
    exit(1);
  }
}


int main(int argc, char *argv[])
{
  bool norm = false, centroid = false;
  long n, m, i, j;
  float x, y, s, rsquared, ssquared, xcen, ycen, nfac;
  string output_image;
  
  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  s = 1.0;
  n = 0;
  m = 0;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'o': output_image = argv[++i];
            break;
        case 'd': n = atoi(argv[++i]);
	    m = atoi(argv[++i]);
            break;
        case 'x': xcen = atof(argv[++i]);
	    ycen = atof(argv[++i]);
	    centroid = true;
            break;
	case 's': s = atof(argv[++i]);
            break;
	case 'n': norm = true;
            break;
      }
    }
  }

  if (n <= 0 || m <= 0) {
    cerr << "ERROR: Image dimension can't be smaller than 1!\n";
    exit (1);
  }

  if (!centroid) {
    xcen = (float) (n-1)/2.;
    ycen = (float) (m-1)/2.;
  }
  else {
    xcen -= 1.;
    ycen -= 1.;
  }

  ssquared = s * s;

  vector<float> data_out(n*m,0);

  nfac = 1.0;

  // get normalisation factor
  if (norm) {
    nfac = 0.;
    for (j=0; j<m; j++) {
      y = (float) j;
      for (i=0; i<n; i++) {
	x = (float) i;
	rsquared = (x-xcen)*(x-xcen) + (y-ycen)*(y-ycen);
	nfac += gauss(rsquared, ssquared);
      }
    }
  }

  // write image
  for (j=0; j<m; j++) {
    y = (float) j;
    for (i=0; i<n; i++) {
      x = (float) i;
      rsquared = (x-xcen)*(x-xcen) + (y-ycen)*(y-ycen);
      data_out[i+n*j] = gauss(rsquared, ssquared) / nfac;
    }
  }

  writeImage(output_image, data_out, n, m, -32);
  
  return 0;
}

