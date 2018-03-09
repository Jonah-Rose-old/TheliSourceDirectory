#include <cstring> // for strcpy()
#include <fstream>
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
    cerr << "           -f coefficients (file with nonlinearity correction coefficients) \n";
    cerr << "           -c chip number\n";
    cerr << "          [-o output_image]\n\n";
    cerr << "  Format:  The file with the nonlinearity coefficients must contain one\n";
    cerr << "           line per chip, each line containing four coefficients a0, a1, a2, a3\n";
    cerr << "           for the correction polynomial a0 + a1 x^1 + a2 x^2 + a3 x^3.\n";
    cerr << "           If e.g. only a lower order polynomial is known, then the higher\n";
    cerr << "           coefficients must be set explicitly to zero.\n";
    cerr << "           The chip number must correspond to the line number in the\n";
    cerr << "           coefficients file.\n\n";
    cerr << "           The input image is overwritten if no output image is given.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  const int FILEMAX = 4096;
  int chip = 0;
  long n, m, i, k;
  double a0, a1, a2, a3;
  double x;
  string input_image, output_image;
  char coefficients[FILEMAX];
  
  // print usage if no arguments were given  
  if (argc==1) usage(0, argv);
  
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	break;
      case 'f': strcpy(coefficients,argv[++i]);
	break;
      case 'c': chip = atoi(argv[++i]);
	break;
      }
    }
  }
  
  if (chip == 0) {
    cerr << "ERROR: No chip was specified.\n";
    exit (1);
  }
  
  string test(coefficients);

  ifstream file(coefficients);
  if (!file.is_open()) {
    cerr << "ERROR: Could not read from " << coefficients << "!\n";
    return 1;
  }
  
  a0 = 0.;
  a1 = 1.;
  a2 = 0.;
  a3 = 0.;

  i = 1;
  while (file) {
    file >> a0 >> a1 >> a2 >> a3;
    if (i==chip) break;
    else i++;
  }
  file.close();

  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  // correct for nonlinearity
  for (k=0; k<n*m; k++) {
    x = image.data[k];
    image.data[k] = a0 + a1*x + a2*x*x + a3*x*x*x;
  }

  if (output_image.empty()) output_image = input_image;

  writeImage(output_image, input_image, image.data, call);

  return 0;
}
