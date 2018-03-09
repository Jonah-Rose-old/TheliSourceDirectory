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
    cerr << "           -r radius (pixel) \n";
    cerr << "           -x xpos ypos\n\n";
    cerr << "   Adds all values in the circle around the position specified.\n";
    cerr << "   Pixels are divided into 10x10 subpixels for higher accuracy.\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{

//******************************************************
// Coadds all pixel values around a given position
//******************************************************

  int n, m;
  long i, j, ii, jj;
  float xpos, ypos, r, dist, sum;
  string input_image;
  
  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  xpos = ypos = 0.;
  r = 1.;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
      case 'r': r = atof(argv[++i]);
	    break;
      case 'x': xpos = atof(argv[++i]);
	        ypos = atof(argv[++i]);
	    break;
      }
    }
  }

  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image_in(input_image);
  n = image_in.naxis1;
  m = image_in.naxis2;

  // sum up the FITS values within the circle around the given position
  sum = 0.;

  // sub-pixelisation
  r *= 10.;
  xpos *= 10.;
  ypos *= 10.;

  long imin = xpos - r;
  long imax = xpos + r;
  long jmin = ypos - r;
  long jmax = ypos + r;
  if (imin<0) imin = 0;
  if (jmin<0) jmin = 0;
  if (imax>=n*10) imax = n*10;
  if (jmax>=m*10) jmax = m*10;

  for (i=imin; i<imax; i++) {
    ii = i / 10;
    for (j=jmin; j<jmax; j++) {
      jj = j / 10;
      dist = sqrt( ((float) i - xpos) * ((float) i - xpos) + 
		   ((float) j - ypos) * ((float) j - ypos));
      if (dist <= r) sum += 0.01 * image_in.data[ii+n*jj];
    }
  }

  cout << sum << endl;

  return 0;
}
