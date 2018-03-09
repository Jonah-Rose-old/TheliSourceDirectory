#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

//*************************************************************

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr <<  "\n";
    cerr << "  USAGE:  " << argv[0] << "file1 file2\n\n";
    cerr << "      Checks whether the data sections of two images are identical.\n";
  }
}

//*************************************************************

int main(int argc, char *argv[])
{
  long i, n1, n2, m1, m2;
  string file1, file2;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  file1 = argv[1];
  file2 = argv[2];

  checkfile(file1);
  checkfile(file2);

  myFITS image1(file1);
  myFITS image2(file2);
  n1 = image1.naxis1;
  n2 = image2.naxis1;
  m1 = image1.naxis2;
  m2 = image2.naxis2;

  if (n1 != n2 || m1 != m2) {
    cout << "Images have different sizes!\n";
    return 0;
  }
 
  for (i=0; i<n1*m1; i++) {
    if (image1.data[i] != image2.data[i]) {
      cout << "Images are NOT identical.\n";
      return 0;
    }
  }
  
  cout << "Images are identical.\n";

  return 0;
}
