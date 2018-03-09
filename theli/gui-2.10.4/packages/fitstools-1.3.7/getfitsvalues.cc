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
    cerr << "           -x xpix1 xpix2 ypix\n";
    cerr << "          [-a (print ALL pixel values, do nothing else)]\n";
    cerr << "          [-v verbose]\n";
    cerr << "          [-s suppress summary line]\n\n";
    cerr << "      Returns the sum, the mean and the rms of the pixel values within a row section.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int n, m, k;
  long i, xpos1, xpos2, ypos;
  string input_image;
  bool suppress = false, flag_all = false, verbose = false;

  // print usage if no arguments were given
  if (argc==1) usage(0,argv);

  //  Read command line arguments
  xpos1 = 0;
  xpos2 = 0;
  ypos  = 0;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'x': 
	xpos1 = atol(argv[++i]) - 1;
	xpos2 = atol(argv[++i]) - 1;
	ypos = atol(argv[++i]) - 1;
	break;
      case 'a': flag_all = true;
	break;
      case 'v': verbose = true;
	break;
      case 's': suppress = true;
	break;
      }
    }
  }

  checkfile(input_image);

  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  if (flag_all) {
    for (i=0; i<n*m; i++) {
      cout << image.data[i] << endl;
    }
    return 0;
  }

  int dim = xpos2 - xpos1 + 1;

  vector<float> data(dim,0);

  // print the value
  if (xpos1 >= 0 && xpos1 < n && 
      xpos2 >= 0 && xpos2 < n &&
      ypos  >= 0 && ypos  < m) {
    k = 0;
    for (i=xpos1; i<=xpos2; i++) {
      data[k++] = image.data[i+n*ypos];
    }
  }
  else {
    cerr << "The specified pixel lies outside the data array.\n";
    return 1;
  }

  float sum = 0.;
  for (i=0; i<dim; i++) {
    sum += data[i];
    if (verbose) printf("%g\n", data[i]);
  }
  
  if (!suppress) printf("%g %g %g\n", sum, mean(data, dim), rms(data, dim));

  return 0;
}
