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
    cerr << "           -p pattern (available: RGGB, GRBG, GBRG, BGGR\n";
    cerr << "           -a [average green pixels into one image] \n";
    cerr << "  PURPOSE: Splits a bayer matrix fits image into separate RGB images.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  bool flag_av = false;
  int n, m, ns, ms;
  long i, j, k1, k2, k3, k4, k;
  string input_image, pattern, tmp;
  string out1, out2, out3, out4;
  
  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  // default: no averaging of the green channel
  flag_av = 0;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'p': pattern = argv[++i];
	break;
      case 'a': flag_av = true;
	break;
      }
    }
  }
  
  if (! (pattern.compare("RGGB") == 0 || pattern.compare("GRBG") == 0 || 
         pattern.compare("GBRG") == 0 || pattern.compare("BGGR") == 0)) {
    cerr << "\nBayer pattern not recognised!\n";
    exit (1);
  }
  
  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;
  
  // chop the last row / column of pixels if the image dimensions are uneven
  if ( n % 2 != 0) n = n - 1;
  if ( m % 2 != 0) m = m - 1;
  
  ns = n / 2;
  ms = m / 2;

  // the file name prefix
  tmp = input_image;
  tmp.resize(input_image.length() - 5); // hardcoded file suffix ".fits"
  out1 = tmp;
  out2 = tmp;
  out3 = tmp;
  if (!flag_av) out4 = tmp;

  /* the indexing of the bayer matrix is as follows:
     
     channel3 channel4
     channel1 channel2
     
     if the pattern code entered is 'abcd', then:
     
     a = channel3
     b = channel4
     c = channel1
     d = channel2
     
  */

  // for the 4 different bayer patterns

  // no averaging requested
  if (!flag_av) {
    if (pattern.compare("RGGB") == 0) {
      out1 += ".G1.fits";
      out2 += ".B1.fits";
      out3 += ".R1.fits";
      out4 += ".G2.fits";
    }
    if (pattern.compare("GRBG") == 0) {
      out1 += ".B1.fits";
      out2 += ".G1.fits";
      out3 += ".G2.fits";
      out4 += ".R1.fits";
    }
    if (pattern.compare("GBRG") == 0) {
      out1 += ".R1.fits";
      out2 += ".G1.fits";
      out3 += ".G2.fits";
      out4 += ".B1.fits";
    }
    if (pattern.compare("BGGR") == 0) {
      out1 += ".G1.fits";
      out2 += ".R1.fits";
      out3 += ".B1.fits";
      out4 += ".G2.fits";
    }
  }

  // averaging requested
  if (flag_av) {
    if (pattern.compare("RGGB") == 0) {
      out1 += ".G.fits";
      out2 += ".B.fits";
      out3 += ".R.fits";
    }
    if (pattern.compare("GRBG") == 0) {
      out1 += ".B.fits";
      out2 += ".G.fits";
      out3 += ".R.fits";
    }
    if (pattern.compare("GBRG") == 0) {
      out1 += ".R.fits";
      out2 += ".G.fits";
      out3 += ".B.fits";
    }
    if (pattern.compare("BGGR") == 0) {
      out1 += ".G.fits";
      out2 += ".R.fits";
      out3 += ".B.fits";
    }
  }

  vector<float> channel1(ns*ms,0);
  vector<float> channel2(ns*ms,0);
  vector<float> channel3(ns*ms,0);
  vector<float> channel4(ns*ms,0);

  vector<float> data_out1(ns*ms,0);
  vector<float> data_out2(ns*ms,0);
  vector<float> data_out3(ns*ms,0);
  vector<float> data_out4(ns*ms,0);

  k1 = k2 = k3 = k4 = 0;

  for (j=0; j<m; j++) {
    for (i=0; i<ns; i++) {
      if ( (j % 2) == 0) {
	channel1[k1++] = image.data[2*(i+ns*j)];
	channel2[k2++] = image.data[2*(i+ns*j)+1];
      }
      else {
	channel3[k3++] = image.data[2*(i+ns*j)];
	channel4[k4++] = image.data[2*(i+ns*j)+1];
      }
    }
  }
  
  // write the split images
  for (k=0; k<ns*ms; k++) {
    // if no averaging of green channels is requested
    if (flag_av == 0) {
      data_out1[k] = channel1[k];
      data_out2[k] = channel2[k];
      data_out3[k] = channel3[k];
      data_out4[k] = channel4[k];
    }
    // if averaging of green channels is requested
    else {
      if (pattern.compare("RGGB") == 0) {
	data_out1[k] = 0.5*(channel1[k] + channel4[k]);
	data_out2[k] = channel2[k];
	data_out3[k] = channel3[k];
      }
      else if (pattern.compare("GRBG") == 0) {
	data_out1[k] = channel1[k];
	data_out2[k] = 0.5*(channel2[k]+ channel3[k]);
	data_out3[k] = channel4[k];
      }
      else if (pattern.compare("GBRG") == 0) {
	data_out1[k] = channel1[k];
	data_out2[k] = 0.5*(channel2[k] + channel3[k]);
	data_out3[k] = channel4[k];
      }
      else { 
	// BGGR
	data_out1[k] = 0.5*(channel1[k] + channel4[k]);
	data_out2[k] = channel2[k];
	data_out3[k] = channel3[k];
      }
    }
  }

  writeImage(out1, input_image, data_out1, ns, ms, call);
  writeImage(out2, input_image, data_out2, ns, ms, call);
  writeImage(out3, input_image, data_out3, ns, ms, call);
  if (!flag_av) writeImage(out4, input_image, data_out4, ns, ms, call);

  return 0;
}
