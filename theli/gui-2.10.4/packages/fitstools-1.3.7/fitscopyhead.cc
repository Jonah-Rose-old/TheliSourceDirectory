#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE:  " << argv[0] << endl;
    cerr << "           -i data_image \n";
    cerr << "           -h header_image \n";
    cerr << "           -o output_image \n";
    cerr << "          [-a correct for mismatches in image geometries]\n\n";
    cerr << "  Replaces the header in data_image with that from header_image.\n\n";
    cerr << "  WARNING: Only checks if the geometry of the new\n";
    cerr << "           header is consistent with that of the old header. If a\n";
    cerr << "           mismatch is found, the programme stops. In this case you\n";
    cerr << "           can add the -a flag to automatically adjust the gometries.\n";
    cerr << "           NO OTHER CONSISTENCY CHECKS ARE MADE.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  bool happy = false;
  int i, n, m, nh, mh;
  string input_image, output_image, header_image;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'h': header_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
        case 'a': happy = true;
            break;
      }
    }
  }

  checkfile(input_image);
  checkfile(header_image);
  string call = programcall(argv, argc);

  myFITS image1(input_image);
  myFITS image2(header_image);
  n = image1.naxis1;
  m = image1.naxis2;
  nh = image2.naxis1;
  mh = image2.naxis2;

  if (!happy) {
    if (n != nh || m != mh) {
      cerr << "ERROR: NAXIS keys do not match!\n";
      cerr << input_image << ": " << n << " " << m << endl;
      cerr << header_image << ": " << nh << " " << mh << endl;
      cerr << "Add the -a argument if you want the new FITS header to reflect the\n";
      cerr << "image geometries of the old header. However, NO CHECKS are made if\n";
      cerr << "this corrupts the output image or not!\n";
      return 1;
    }
  }
  
  // write the result
  writeImage(output_image, header_image, image1.data, n, m, call);

  // here we are "happy", thus correct the header
  if (n != nh || m != mh) {
    myFITS image_out(output_image, "Write");
    image_out.addKey("NAXIS1", n, "Reference pixel of first axis");
    image_out.addKey("NAXIS2", m, "Reference pixel of second axis");
  }

  return 0;
}
