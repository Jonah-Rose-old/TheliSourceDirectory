#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"

using namespace std;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\nLIRIS@WHT pixel mapping descrambler\n";
    cerr << "  USAGE:  " << argv[0] << "input_image output_image\n";
    exit(1);
  }
}

void descramble_liris(vector< vector<float> > &image2D, vector< vector<float> > &image2Dmod);

int main(int argc, char *argv[])
{
  int i, j, n, m;
  string input_image, output_image;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  input_image = argv[1];
  output_image = argv[2];

  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS image
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  vector< vector<float> > image2D(n);
  vector< vector<float> > image2Dmod(n);
  for(i=0; i<n; i++) {
    image2D[i].resize(m,0);
    image2Dmod[i].resize(m,0);
  }
   
  // Put the image data into the 2D array
  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      image2D[i][j] = image.data[i+n*j];
    }
  }
   
  // correct the pixel mapping problem
  descramble_liris(image2D, image2Dmod);

  // Write the result
  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      image.data[i+n*j] = image2Dmod[i][j];
    }
  }
  writeImage(output_image, input_image, image.data, call);

  return 0;
}

//********************************************************************
// Descramble the image
//********************************************************************
void descramble_liris(vector< vector<float> > &image2D, vector< vector<float> > &image2Dmod)
{
  long i, j;

  // correct first quadrant
  // shift lower left corner to the upper right
  image2Dmod[510][511] = image2D[0][0];
  image2Dmod[511][511] = image2D[1][0];
  // shift main block two columns to the left
  for (i=0; i<=509; i++)  {
    for (j=0; j<=511; j++)  {
      image2Dmod[i][j] = image2D[i+2][j];
    }
  }
  // shift left two columns to the right and one row down
  for (j=1; j<=511; j++)  {
    image2Dmod[510][j-1] = image2D[0][j];
    image2Dmod[511][j-1] = image2D[1][j];
  }
   
   
  // correct second quadrant
  // shift lower left corner to the upper right
  image2Dmod[1023][511] = image2D[512][0];
  // shift main block one column to the left
  for (i=512; i<=1022; i++)  {
    for (j=0; j<=511; j++)  {
      image2Dmod[i][j] = image2D[i+1][j];
    }
  }
  // shift left column to the right and one row down
  for (j=1; j<=511; j++)  {
    image2Dmod[1023][j-1] = image2D[512][j];
  }
   
   
  // correct third quadrant
  // shift lower left corner to the upper right
  image2Dmod[511][1023] = image2D[0][512];
  // shift main block one column to the left
  for (i=0; i<=510; i++)  {
    for (j=512; j<=1023; j++)  {
      image2Dmod[i][j] = image2D[i+1][j];
    }
  }
  // shift left column to the right and one row down
  for (j=513; j<=1023; j++)  {
    image2Dmod[511][j-1] = image2D[0][j];
  }
   
   
  // correct fourth quadrant
  // shift lower left corner to the upper right
  image2Dmod[1023][1023] = image2D[512][512];
  // shift main block one column to the left
  for (i=512; i<=1022; i++)  {
    for (j=513; j<=1023; j++)  {
      image2Dmod[i][j] = image2D[i+1][j];
    }
  }
  // shift left column to the right and one row down
  for (j=513; j<=1023; j++)  {
    image2Dmod[1023][j-1] = image2D[512][j];
  }
}
