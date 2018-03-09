#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE:  " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           -c comparison_image \n";
    cerr << "           -t thresholds [low, high]\n";
    cerr << "          [-o output_image] \n";
    cerr << "      Replaces pixels in the input image with that from the\n";
    cerr << "      comparison image if they are brighter/fainter than the\n";
    cerr << "      thresholds given.\n";
    cerr << "      Overwrites the input image if no output image is given.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  long n, m, nc, mc, i, k;
  float hthreshold, lthreshold;
  string input_image, output_image, comparison_image;
  
  hthreshold = 0.;
  lthreshold = 0.;
  
  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
        case 'c': comparison_image = argv[++i];
            break;
        case 't': lthreshold = atof(argv[++i]);
	    hthreshold = atof(argv[++i]);
            break;
      }
    }
  }

  if (output_image.empty()) output_image = input_image;

  checkfile(input_image);
  checkfile(comparison_image);
  string call = programcall(argv, argc);

  // read the FITS image
  myFITS image_in(input_image);
  myFITS image_comp(comparison_image);
  n = image_in.naxis1;
  m = image_in.naxis2;
  nc = image_comp.naxis1;
  mc = image_comp.naxis2;
  
  if (nc != n || mc != m) {
    cerr << "ERROR: Images don't have the same geometry.\n";
    return 1;
  }

  // set the according pixels to the value given
  for (k=0; k<n*m; k++) {
    if (image_in.data[k] >= hthreshold || image_in.data[k] <= lthreshold) 
      image_in.data[k] = image_comp.data[k];
  }

  writeImage(output_image, input_image, image_in.data, call);

  return 0;
}
