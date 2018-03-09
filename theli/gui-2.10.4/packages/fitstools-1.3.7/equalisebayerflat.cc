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
    cerr << "           -o output_image \n";
    cerr << "  Note: Equalises the fluxes of the RGB channels in a Bayer Flat field.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  int n, m;
  long i, j;
  string input_image, output_image;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
      }
    }
  }

  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  vector<float> data_out(n*m,0);

  // calculate the average 2x2 pixel
  float ll, lr, ul, ur;

  ll = lr = ul = ur = 0.;

  for (j=0; j<m-1; j+=2) {
    for (i=0; i<n-1; i+=2) {
      ll += image.data[i+n*j];
      lr += image.data[i+1+n*j];
      ul += image.data[i+n*(j+1)];
      ur += image.data[i+1+n*(j+1)];
    }
  }
  
  ll = ll / ( n*m/4. );
  lr = lr / ( n*m/4. );
  ul = ul / ( n*m/4. );
  ur = ur / ( n*m/4. );

  // "normequalise" the image:
  for (j=0; j<m-1; j+=2) {
    for (i=0; i<n-1; i+=2) {
      data_out[i+n*j]       = image.data[i+n*j] / ll;
      data_out[i+1+n*j]     = image.data[i+1+n*j] / lr;
      data_out[i+n*(j+1)]   = image.data[i+n*(j+1)] / ul;
      data_out[i+1+n*(j+1)] = image.data[i+1+n*(j+1)] / ur;
    }
  }
  
  writeImage(output_image, input_image, data_out, call);

  return 0;
}
