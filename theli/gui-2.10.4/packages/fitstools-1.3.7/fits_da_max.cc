#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "  PURPOSE: Returns a typical peak value of the image (filtered for outliers)\n\n";
    exit(1);
  }
}

//**************************************************************
int main(int argc, char *argv[])
{
  int n, m, n_ext, extension;
  long i;
  float maxval;
  string input_image, naxis1, naxis2;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      }
    }
  }

  checkfile(input_image);

  // read the FITS image
  myFITS image(input_image);
  n_ext = image.numExt();

  // if there is no fits extension
  if (n_ext == 0) {
    n = image.naxis1;
    m = image.naxis2;
    maxval = get_max_median(image.data, n, m);
    cout << maxval << endl;
    return 0;
  }

  // if there is one or more extensions
  else {
    vector<float> maxarr(n_ext,0);

    for (extension=0; extension<n_ext; extension++) {
      // extensions are numbered starting from 1, not from 0!
      n = image.readExtKey("NAXIS1", extension+1);
      m = image.readExtKey("NAXIS2", extension+1);
      image.readExtData(extension+1);

      vector<float> tmp(image.data);      
      maxarr[extension] = get_max_median(tmp, n, m);
    }

    maxval = max(maxarr, extension);

    cout << maxval << endl;
  }

  return 0;
}
