#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE:  " << argv[0] << endl;
    cerr <<  "       -i fitsfile\n";
    cerr <<  "      [-m mode (PA, PIXSCALE, FLIP)]\n\n";
    cerr <<  "       Analyses the CD matrix of an image and reports\n";
    cerr <<  "       pixel scale, position angle, and flip.\n";
    cerr <<  "       If -m is specified, only that particular value is returned.\n";
  }

  exit (1);
}

//*************************************************************
int main(int argc, char *argv[])
{
  long i;
  double cd11, cd12, cd21, cd22;
  double pa, det, pscale;
  string input_image, mode, flip;

  cd11 = 0.;
  cd12 = 0.;
  cd21 = 0.;
  cd22 = 0.;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  //  Read command line arguments
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'i': input_image = argv[++i];
	break;
      case 'm': mode = argv[++i];
	break;
      }
    }
  }

  checkfile(input_image);

  if (mode.compare("") != 0 && 
      mode.compare("PA") != 0 && 
      mode.compare("PIXSCALE") != 0 && 
      mode.compare("FLIP") != 0) {
    cerr <<  "ERROR: Invalid mode " << mode << ". Must be PA, PIXSCALE, FLIP, or left unspecified!\n";
    exit(1);
  }

  // read the FITS image
  myFITS image(input_image, false);
  
  image.readmyKey("CD1_1", cd11);
  image.readmyKey("CD1_2", cd12);
  image.readmyKey("CD2_1", cd21);
  image.readmyKey("CD2_2", cd22);

  pa     = get_posangle(cd11, cd12, cd21, cd22) * 180./3.14159265;
  det    = cd11*cd22 - cd12*cd21;
  pscale = sqrt(fabs(det));

  if (mode.empty()) {
    if (det < 0) flip = "No";
    else flip = "Yes";
    cout << "Pixel scale [\"/pix]  : " << pscale*3600. << endl;
    cout << "Position angle [deg] : " << pa << endl;
    cout << "Is the image flipped : " << flip << endl;
  }

  if (mode.compare("PA") == 0) cout << pa << endl;
  if (mode.compare("PIXSCALE") == 0) cout << pscale << endl;
  if (mode.compare("FLIP") == 0) {
    if (det < 0) cout << "Unflipped\n";
    else cout << "Flipped\n";
  }

  return 0;
}
