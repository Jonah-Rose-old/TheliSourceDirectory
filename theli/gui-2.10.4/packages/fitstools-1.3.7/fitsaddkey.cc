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
    cerr << "           -k key name (8 char max)\n";
    cerr << "           -v key value \n";
    cerr << "          [-c key comment] \n\n";
    cerr << "  Adds a key to the FITS header.\n";
    cerr << "  If blanks are contained in the value or the\n";
    cerr << "  comment, it must be encompassed by single quotes.\n\n";
    exit (1);
  }
}

int main(int argc, char *argv[])
{
  long i;
  string input_image, keyname, keyvalue, keycomment;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'k': keyname = argv[++i];
            break;
        case 'v': keyvalue = argv[++i];
            break;
        case 'c': keycomment = argv[++i];
            break;
      }
    }
  }

  checkfile(input_image);

  // sanity check
  if (keyname.empty() || keyvalue.empty()) {
    cerr << "ERROR: No keyname or keyvalue given. Nothing will be done.\n";
    return 0;
  }
  
  // let's stick to the FITS standard
  if (keyname.length() > 8) {
    cerr << "Warning: keyname is too long. It will be truncated to 8 chars.\n";
    keyname[8]='\0';
  }
  if (keyvalue.length() > 30) {
    cerr << "Warning: keyvalue is too long. It will be truncated to 30 chars.\n";
    keyvalue[30]='\0';
  }
  if (keycomment.length() > 40) {
    cerr << "Warning: keycomment is too long. It will be truncated to 40 chars.\n";
    keycomment[40]='\0';
  }

  myFITS image(input_image, false, "Write");
  image.addKey(keyname, keyvalue, keycomment);

  return 0;
}
