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
    cerr << "          [-s pixel scale] \n\n";
    cerr << "  Returns the CD matrix elements, or calculates them from\n";
    cerr << "  CDELTi/j, CROTA, and if necessary from the pixel scale.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int i;
  double cdelt1, cdelt2, crota, crota1, crota2;
  double cd11, cd12, cd21, cd22, pixscale, phi;
  string input_image;

  pixscale = 1.0/3600.;

  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 's': pixscale = atof(argv[++i])/3600.;
            break;
      }
    }
  }

  checkfile(input_image);

  // read the FITS image
  myFITS image(input_image, false);

  cd11   = image.readKeyFailsafe("CD1_1", -999.);
  cd12   = image.readKeyFailsafe("CD1_2", -999.);
  cd21   = image.readKeyFailsafe("CD2_1", -999.);
  cd22   = image.readKeyFailsafe("CD2_2", -999.);
  cdelt1 = image.readKeyFailsafe("CDELT1", -999.);
  cdelt2 = image.readKeyFailsafe("CDELT2", -999.);
  crota1 = image.readKeyFailsafe("CROTA1", -999.);
  crota2 = image.readKeyFailsafe("CROTA2", -999.);
  crota  = image.readKeyFailsafe("CROTA", -999.);

  // if the CD matrix exists and makes sense, print it and leave
  if (fabs(cd11) < 1. && fabs(cd12) < 1. && 
      fabs(cd21) < 1. && fabs(cd22) < 1.) {
    printf("%g %g %g %g\n", cd11, cd12, cd21, cd22);
    return 0;
  }

  // if we end up here, then a valid CD matrix doesn't exist

  // obtain the rotation angle
  // crota was read above. if it doesn't exist or is invalid, then
  if (crota1 !=-999. && crota2 != -999 && crota == -999.) 
    crota = 0.5*(crota1+crota2);
  if (crota1 == -999. && crota2 == -999. && crota == -999.)
    crota = 0.;
  if (cdelt1 == -999 || fabs(cdelt1) >= 1. ) cdelt1 = -pixscale;
  if (cdelt2 == -999 || fabs(cdelt2) >= 1. ) cdelt2 = pixscale;

  phi = crota * RAD;

  cd11 =  cdelt1 * cos(phi);
  cd12 = -cdelt2 * sin(phi);
  cd21 =  cdelt1 * sin(phi);
  cd22 =  cdelt2 * cos(phi);

  printf("%g %g %g %g\n", cd11, cd12, cd21, cd22);

  return 0;
}
