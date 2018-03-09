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
    cerr << "    Looks for the most common pixel value (i.e. blanked pixels with identical values.\n\n";

    exit(1);
  }
}

int main(int argc, char *argv[])
{
  long i, n, m;
  string input_image;
  
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

  // read the FITS header and the data block
  myFITS image_in(input_image);
  n = image_in.naxis1;
  m = image_in.naxis2;
  
  string result;
  float blankvalue = find_blankvalue(image_in.data, n, m, result);
  if (result.compare("UNKNOWN") == 0) cout << result << endl;
  else cout << blankvalue << endl;

  return 0;
}
