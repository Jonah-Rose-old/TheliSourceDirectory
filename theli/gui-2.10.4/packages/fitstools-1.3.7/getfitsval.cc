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
    cerr << "           -x xpix ypix\n\n";
    cerr << "  Returns the value of a certain pixel.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int n, m;
  long i, xpos, ypos;
  string input_image;

  // print usage if no arguments were given
  if (argc==1) usage(0,argv);

  xpos = 0;
  ypos = 0;
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'x': 
	xpos = atol(argv[++i]) - 1;
	ypos = atol(argv[++i]) - 1;
	break;
      }
    }
  }

  checkfile(input_image);

  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  // print the value
  if (xpos >= 0 && xpos < n && ypos >= 0 && ypos < m) { 
    printf("%g\n", image.data[xpos+n*ypos]);
  }
  else cout << "Pixel " << xpos << " " << ypos << " lies outside the data array.\n";
  
  return 0;
}
