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
    cerr << "           -r input_image \n";
    cerr << "           -g input_image \n";
    cerr << "           -b input_image \n";
    cerr << "           -o output_image \n";
    cerr << "  Returns the maximum value per pixel of three images\n\n";

    exit(1);
  }
}


int main(int argc, char *argv[])
{
  int n, m, n2, m2, n3, m3;
  long i, k;
  string input_image1, input_image2, input_image3, output_image;
  
  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'r': input_image1 = argv[++i];
	break;
      case 'g': input_image2 = argv[++i];
	break;
      case 'b': input_image3 = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	break;
      }
    }
  }

  // read the FITS header and the data block
  checkfile(input_image1);
  checkfile(input_image2);
  checkfile(input_image3);

  myFITS image1(input_image1);
  myFITS image2(input_image2);
  myFITS image3(input_image3);

  n = image1.naxis1;
  m = image1.naxis2;

  n2 = image2.naxis1;
  m2 = image2.naxis2;

  n3 = image3.naxis1;
  m3 = image3.naxis2;

  if (n2 != n || n3 != n || m2 != m || m3 != m) {
    cerr << "ERROR: Images don't have identical geometries!\n";
    return 1;
  }

  vector<float> data_out(n*m,0);
  vector<float> tmp(3,0);

  for (k=0; k<n*m; k++) {
    tmp[0] = image1.data[k];
    tmp[1] = image2.data[k];
    tmp[2] = image3.data[k];
    data_out[k] = max(tmp, 3);
  }

  writeImage(output_image, input_image1, data_out, "Max pixel map of RGB frame");

  return 0;
}
