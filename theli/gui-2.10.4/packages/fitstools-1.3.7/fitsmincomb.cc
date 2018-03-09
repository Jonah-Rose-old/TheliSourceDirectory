#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i image 1 \n";
    cerr << "           -j image 2 \n";
    cerr << "           -o output_image \n";
    cerr << "  PURPOSE: Does a minimum stack of two images (keeps the smaller value).\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int n, m, nc, mc;
  long i, k;
  string input_image1, input_image2, output_image;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image1 = argv[++i];
            break;
        case 'j': input_image2 = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
      }
    }
  }

  checkfile(input_image1);
  checkfile(input_image2);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image1(input_image1);
  myFITS image2(input_image2);
  n  = image1.naxis1;
  m  = image1.naxis2;
  nc = image2.naxis1;
  mc = image2.naxis2;

  if (n != nc || m != mc) {
    cerr << "ERROR: Images do not have the same dimensions!\n";
    return 1;
  }

  for (k=0; k<n*m; k++) {
    if (image1.data[k] > image2.data[k]) 
      image1.data[k] = image2.data[k];
  }

  writeImage(output_image, input_image1, image1.data, call);

  return 0;
}
