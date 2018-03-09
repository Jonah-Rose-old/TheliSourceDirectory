#include <cstring>
#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << " image fitskey ['clean' or 'cleancolon']\n";
    cerr << "          If the 'clean' ('cleancolon') keyword is given, leading and trailing\n";
    cerr << "          whitespace is removed, whitespace inside replaced by _ (:),\n";
    cerr << "          and meta-characters are deleted without replacement.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  string input_image, keyname, newkey, final, s, slast;
  int i, clean = 0;
  char remove[] = " ()[]{}/;\"*#^$%üä&ö\t\n";   // cannot include a single ' in the pattern

  if (argc==1) usage(0, argv);

  input_image = argv[1];
  keyname = argv[2];
  
  if (argc==4) {
    if (strcmp(argv[3],"clean") == 0) clean = 1;
    if (strcmp(argv[3],"cleancolon") == 0) clean = 2;
  }

  checkfile(input_image);

  // open the file
  myFITS image(input_image);

  string keyvalue = image.readKey(keyname);
  if (keyvalue.empty()) return 0;

  if (clean == 0) {
    cout << keyvalue << endl;
    return 0;
  }

  // Replace single quotes with blanks
  int lstr = keyvalue.length();
  for (i=0; i<lstr; i++) {
    if (keyvalue[i] == 39) keyvalue[i] = ' ';
  }

  // trim leading and trailing whitespace
  //  trimwhitespace(keyvalue);
  trim(keyvalue," ");
  newkey = keyvalue;

  // Replace embedded blanks with underscores
  if (clean > 0) {
    lstr = newkey.length();
    for (i=0; i<lstr; i++) {
      if (keyvalue[i] == ' ' && clean == 1) keyvalue[i] = '_';
      if (keyvalue[i] == ' ' && clean == 2) keyvalue[i] = ':';
    }
  }

  if (clean == 0) cout << newkey << endl;
  else {
    // remove metacharacters from keyword if 'clean' is requested
    final = newkey;
    final = RemoveChars(newkey, remove);
    cout << final << endl;
  }

  return 0;
}
