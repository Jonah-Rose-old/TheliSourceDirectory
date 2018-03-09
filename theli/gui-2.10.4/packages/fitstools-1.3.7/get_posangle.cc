#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr <<  "          [-c cd11 cd12 cd21 cd22]\n";
    cerr <<  "          [-f fitsfile]\n\n";
    cerr <<  "      Either the CD matrix elements, or a FITS file must be given.\n";
    cerr << "       Gets the sky position angle from the CD matrix\n";
  }

  exit (0);
}

//*************************************************************

int main(int argc, char *argv[])
{
  long i;
  bool fitsflag = false, cdflag = false;
  double cd11, cd12, cd21, cd22;
  double pa;
  string input_image;

  cd11 = 0.;
  cd12 = 0.;
  cd21 = 0.;
  cd22 = 0.;

  // print usage if no arguments were given
  if (argc==1) usage(0,argv);

  //  Read command line arguments
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'c': 
	cd11 = atof(argv[++i]);
	cd12 = atof(argv[++i]);
	cd21 = atof(argv[++i]);
	cd22 = atof(argv[++i]);
	cdflag = true;
	break;
      case 'f': input_image = argv[++i];
	fitsflag = true;
	break;
      }
    }
  }

  if (fitsflag && cdflag) {
    cerr <<  "ERROR: Either CD matrix elements, or a FITS file must be given. You can't do both.\n";
    exit (1);
  }
  
  if (fitsflag) {
    checkfile(input_image);
    // read the FITS image
    myFITS image(input_image);
    image.readmyKey("CD1_1", cd11);
    image.readmyKey("CD1_2", cd12);
    image.readmyKey("CD2_1", cd21);
    image.readmyKey("CD2_2", cd22);
  }
  
  pa = get_posangle(cd11, cd12, cd21, cd22);
  
  cout << pa*180./PI << endl;
  
  return 0;
}
