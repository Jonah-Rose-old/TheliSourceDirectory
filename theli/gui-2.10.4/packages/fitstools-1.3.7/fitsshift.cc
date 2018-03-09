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
    cerr << "           -o output_image \n";
    cerr << "           -x x y\n";
    cerr << "          [-m mask value (default: 0.0)]\n";
    cerr << "          [-s scale factor (default: 1.0)]\n";
    cerr << "      Shifts those parts of an image with values other than -m by x and y pixels,\n";
    cerr << "      and rescales them (if requested through -s).\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  long xshift = 0, yshift = 0, is, js;
  long n, m, i, j;
  float maskval = 0., scale = 1.0;
  string input_image, output_image;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);
  
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	break;
      case 'x': xshift = atoi(argv[++i]);
	yshift  = atoi(argv[++i]);
	break;
      case 'm': maskval = atof(argv[++i]);
	break;
      case 's': scale = atof(argv[++i]);
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
  
  // initialise the output image with the masking value
  vector<float> data_out(n*m,maskval);

  for (j=0; j<m; j++) {
    js = j + yshift;
    for (i=0; i<n; i++) {
      is = i + xshift;
      if (js >= 0 && js < m && is >= 0 && is < n && image.data[i+n*j] != maskval) {
	data_out[is+n*js] = image.data[i+n*j] * scale;
      }
    }
  }
  
  writeImage(output_image, input_image, data_out, call);
   
  return 0;
}
