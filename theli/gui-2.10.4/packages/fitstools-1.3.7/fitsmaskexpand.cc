#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           [-n number of iterations (default: 1)]\n";
    cerr << "           [-o output_image] \n";
    cerr << "           [-m mask_value; default: 0.0]\n";
    cerr << "      Finds pixels with value mask_value, and sets\n";
    cerr << "      all surrounding pixels to the same value.\n";
    cerr << "      If no output_image is given, the input_image\n";
    cerr << "      is overwritten.\n";
    cerr << "      The task can be iterated more than once.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int iter, niter = 1;
  long n, m, i, j, t;
  float value;
  string input_image, output_image;

  value  = 0.;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
	case 'm': value = atof(argv[++i]);
            break;
	case 'n': niter = atoi(argv[++i]);
            break;
      }
    }
  }

  if (output_image.empty()) output_image = input_image;

  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image(input_image);
  n  = image.naxis1;
  m  = image.naxis2;

  // copy the input image
  vector<float> data_out(image.data);

  // expand the mask  
  iter = 0;
  while (iter < niter) {
    for (j=0; j<m; j++) {
      for (i=0; i<n; i++) {
	if (image.data[i+n*j] == value) {
	  t = i-1+n*(j-1);
	  if (t>=0 && t<n*m) data_out[t] = value;
	  t = i+n*(j-1);
	  if (t>=0 && t<n*m) data_out[t] = value;
	  t = i+1+n*(j-1);
	  if (t>=0 && t<n*m) data_out[t] = value;
	  t = i-1+n*j;
	  if (t>=0 && t<n*m) data_out[t] = value;
	  t = i+1+n*j;
	  if (t>=0 && t<n*m) data_out[t] = value;
	  t = i-1+n*(j+1);
	  if (t>=0 && t<n*m) data_out[t] = value;
	  t = i+n*(j+1);
	  if (t>=0 && t<n*m) data_out[t] = value;
	  t = i+1+n*(j+1);
	  if (t>=0 && t<n*m) data_out[t] = value;
	}
      }
    }
    image.data = data_out;

    iter++;
  }    

  writeImage(output_image, input_image, data_out, call);
  
  return 0;
}
