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
    cerr << "           -x left right bottom top\n";
    cerr << "      Extracts a sub-area from the image.\n\n";
    exit(1);
  }
}

  
int main(int argc, char *argv[])
{
  long xmin, xmax, ymin, ymax;
  long n, m, nsub, msub, i;
  float crpix1, crpix2;
  string input_image, output_image;

  xmin = xmax = ymin = ymax = 1;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);
  
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	break;
      case 'x': 
	xmin = atoi(argv[++i]);
	xmax = atoi(argv[++i]);
	ymin = atoi(argv[++i]);
	ymax = atoi(argv[++i]);
	break;
      }
    }
  }
  
  checkfile(input_image);
  string call = programcall(argv, argc);

  // some health check
  if (xmin <= 0 || xmax <= 0 || ymin <= 0 || ymax <= 0) {
    cerr << "ERROR: The region defined with -x must be entirely contained in the image!\n";
    exit (1);
  }
  
  // read the FITS image and the rectangular data subsection
  myFITS image(input_image, false);
  image.readDataSection(xmin, xmax, ymin, ymax);
  n = image.naxis1;
  m = image.naxis2;

  nsub = xmax - xmin + 1;
  msub = ymax - ymin + 1;
  
  // write the result
  writeImage(output_image, input_image, image.data, nsub, msub, call);

  // Read the CRPIX keywords (return a default value if not present), and adjust it
  crpix1 = image.readKeyFailsafe("CRPIX1", float(n/2), "Reference pixel of first axis");
  crpix2 = image.readKeyFailsafe("CRPIX2", float(m/2), "Reference pixel of second axis");
  crpix1 = crpix1 - xmin + 1;
  crpix2 = crpix2 - ymin + 1;

  // update CRPIX
  myFITS imageout(output_image, false, "Write");
  imageout.addKey("CRPIX1", crpix1, "Reference pixel of first axis");
  imageout.addKey("CRPIX2", crpix2, "Reference pixel of second axis");

  return 0;
}
