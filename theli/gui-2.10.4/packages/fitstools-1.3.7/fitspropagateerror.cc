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
    cerr << "           -i image1 image2 err1 err2\n";
    cerr << "           -o output_image\n";
    cerr << "           -m [add, mult, div]\n";
    cerr << "      Calculates the propagated error image for image1 (x) image2,\n";
    cerr << "      where (x) stands for addition, multiplication, or division.\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  long n, m, i, k;
  string im1, im2, err1, err2, output_image, mode;
  float x, y, xe, ye;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': 
	  im1 = argv[++i];
	  im2 = argv[++i];
	  err1 = argv[++i];
	  err2 = argv[++i];
	  break;
        case 'o': output_image = argv[++i];
            break;
        case 'm': mode = argv[++i];
            break;
      }
    }
  }

  if (mode.empty()) {
    cerr << "ERROR: No mode specified (option -m)!\n";
    exit (1);
  }

  checkfile(im1);
  checkfile(im2);
  checkfile(err1);
  checkfile(err2);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image1(im1);
  myFITS image2(im2);
  myFITS error1(err1);
  myFITS error2(err2);
  n = image1.naxis1;
  m = image1.naxis2;
  
  if (image1.naxis1 != image2.naxis1 || image1.naxis1 != error1.naxis1 || image1.naxis1 != error2.naxis1 ||
      image1.naxis2 != image2.naxis2 || image1.naxis2 != error1.naxis2 || image1.naxis2 != error2.naxis2) {
    cerr << "ERROR: Image geometries are not identical!\n";
    cerr << im1  << " " << image1.naxis1 << " " << image1.naxis2 << endl;
    cerr << im2  << " " << image2.naxis1 << " " << image2.naxis2 << endl;
    cerr << err1 << " " << error1.naxis1 << " " << error1.naxis2 << endl;
    cerr << err2 << " " << error2.naxis1 << " " << error2.naxis2 << endl;
    return (1);
  }

  // propagate the error
  vector<float> data_out(n*m,0);

  for (k=0; k<n*m; k++) {
    x  = image1.data[k];
    y  = image2.data[k];
    xe = error1.data[k];
    ye = error2.data[k];
    if (mode.compare("add")  == 0) data_out[k] = sqrt( xe*xe + ye*ye);
    if (mode.compare("mult") == 0) data_out[k] = sqrt( pow(y*xe,2) + pow(x*ye,2));
    if (mode.compare("div")  == 0) data_out[k] = fabsf(x/y) * sqrt( pow(xe/x,2) + pow(ye/y,2));
  }

  writeImage(output_image, im1, data_out, call);

  return 0;
}
