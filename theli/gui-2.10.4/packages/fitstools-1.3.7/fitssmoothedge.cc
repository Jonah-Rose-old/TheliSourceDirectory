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
    cerr << "          [-r (round edge)]\n";
    cerr << "          [-n edge length (default: 100)]\n";
    cerr << "      Smoothes the edge of an image to drop to zero.\n\n";
    exit(1);
  }
}


int main(int argc, char *argv[])
{
  bool flag = false;
  long n, m, i, j;
  float dxmin, dymin;
  float edge, d;
  string input_image, output_image;
  
  edge = 100.;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
        case 'n': edge = atof(argv[++i]);
            break;
        case 'r': flag = true;
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

  // smooth the edges
  for (j=0; j<m; j++) {
    dymin = (j <= m-j) ? j : m-j;
    for (i=0; i<n; i++) {
      dxmin = (i <= n-i) ? i : n-i;
      d = (dxmin <= dymin) ? dxmin : dymin;
      // round edge
      if (flag) {
	if (dymin <= edge && dxmin <= edge) {
	  d = edge - sqrt((edge-dxmin)*(edge-dxmin)+(edge-dymin)*(edge-dymin));
	  if (d < 0.) d = 0.; 
	}
      }

      if (d<=edge)
	image.data[i+n*j] = 0.5*(-cos(d/edge*PI)+1.)*image.data[i+n*j];
    }
  }

  writeImage(output_image, input_image, image.data, call);

  return 0;
}
