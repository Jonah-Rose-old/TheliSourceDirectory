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
    cerr << "           [-w left right bottom top width of the border; default: 100]\n";
    cerr << "           [-v border value; default: 0.0]\n\n";
    cerr << "      Adds a border of the specified widths\n";
    cerr << "      around the image. If one or more values given\n";
    cerr << "      with the -w option are negative, then these\n";
    cerr << "      numbers of rows or lines are cut from the\n";
    cerr << "      corresponding sides of the image.\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int left, right, bottom, top;
  long n, m, n_new, m_new, i, j;
  float value, crpix1, crpix2;
  string input_image, output_image;
  
  value  = 0.;
  left   = 100;
  right  = 100;
  bottom = 100;
  top    = 100;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
        case 'w': 
	    left   = atoi(argv[++i]);
	    right  = atoi(argv[++i]);
	    bottom = atoi(argv[++i]);
	    top    = atoi(argv[++i]);
            break;
	case 'v': value = atof(argv[++i]);
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

  n_new = n + left + right;
  m_new = m + bottom + top;

  // wrap the border
  vector<float> data_out(n_new*m_new,0);

  for (j=0; j<m_new; j++) {
    for (i=0; i<n_new; i++) {
      if (i>=left && i<n_new-right && j>=bottom && j<m_new-top) 
	data_out[i+n_new*j] = image.data[i-left + n*(j-bottom)];
      else data_out[i+n_new*j] = value;
    }
  }

  // write the result
  writeImage(output_image, input_image, data_out, n_new, m_new, call);

  // Read the CRPIX keywords (return a default value if not present), and adjust it
  crpix1 = image.readKeyFailsafe("CRPIX1", float(n/2), "Reference pixel of first axis");
  crpix2 = image.readKeyFailsafe("CRPIX2", float(m/2), "Reference pixel of second axis");
  crpix1 += left;
  crpix2 += bottom;

  // update CRPIX
  myFITS imageout(output_image, false, "Write");
  imageout.addKey("CRPIX1", crpix1, "Reference pixel of first axis");
  imageout.addKey("CRPIX2", crpix2, "Reference pixel of second axis");

  return 0;
}
