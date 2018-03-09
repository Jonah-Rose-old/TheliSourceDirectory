#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <typeinfo>
#include <sstream> 
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
    cerr << "           [-l low threshold] \n";
    cerr << "           [-h high threshold] \n";
    cerr << "           [-e value to be masked (can be 'inf' or 'nan' as well)] \n";    
    cerr << "           [-c comparison_image] \n";
    cerr << "           [-o output_image] \n";
    cerr << "           [-n (set all other pixels to 1)] \n";
    cerr << "           [-v value; default: 0.0]\n\n";
    cerr << "      Sets pixels in the input_image to the value\n";
    cerr << "      given with the -v option, if they are outside\n";
    cerr << "      the thresholds [in the comparison image].\n";
    cerr << "      If no output_image is given, the input_image\n";
    cerr << "      is overwritten.\n";
    cerr << "      An exact value (and 'nan' and 'inf') may also be specified using\n";
    cerr << "      the -e option.\n";
    cerr << "      If the -n option is given, all other pixels are\n";
    cerr << "      set to 1.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  bool flag_e, flag_c, flag_h, flag_l, flag_n;
  long n, m, nc, mc, i, k;
  float value, low, high;
  float exactval_float = 0.;
  string input_image, output_image, comparison_image, exactval;

  value = 0.;
  low   = 0.;
  high  = 0.;

  flag_c = false;
  flag_h = false;
  flag_l = false;
  flag_n = false;
  flag_e = false;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
        case 'c': comparison_image = argv[++i];
	    flag_c = true;
            break;
        case 'e': exactval = argv[++i];
	    flag_e = true;
            break;
        case 'l': low = atof(argv[++i]);
	    flag_l = true;
            break;
        case 'h': high = atof(argv[++i]);
	    flag_h = true;
            break;
	case 'v': value = atof(argv[++i]);
            break;
        case 'n': flag_n = true;
            break;
      }
    }
  }

  checkfile(input_image);
  string call = programcall(argv, argc);

  if (output_image.empty()) output_image = input_image;

  if (!flag_h && !flag_l && !flag_e) {
    cout << "ERROR: No thresholds or mask values specified.\n";
    exit (1);
  }

  // read the FITS header and the data block
  myFITS image_in(input_image);
  myFITS image_comp(comparison_image, false); // don't read yet, we just have do declare the pointer now;
  // the pointer will be a NULL pointer if the image doesn't exist (i.e. wasn't specified)
  n  = image_in.naxis1;
  m  = image_in.naxis2;

  if (flag_c == 1) {
    checkfile(comparison_image);
    image_comp.readData();
    nc = image_comp.naxis1;
    mc = image_comp.naxis2;
    if (nc != n || mc != m) {
      cerr << "ERROR: Input image and comparison image do not have the same size!\n";
      return 1;
    }
  }

  // convert string to float only once instead of every time in the for loop
  if (flag_e) {
    if (exactval.compare("nan") != 0 && exactval.compare("inf") != 0) {
      exactval_float = atof(exactval.c_str());
    }
  }

  // set the according pixels to the value given
  vector<float> data_out(n*m,0);
  vector<float> data_ptr; // actually, I want this to be a pointer at some point so that it doesn't copy the data

  if (!flag_c)   
    data_ptr = image_in.data;
  else 
    data_ptr = image_comp.data;

  for (k=0; k<n*m; k++) {
    // first, copy the image data, or set everything to 1
    if (!flag_n) data_out[k] = data_ptr[k];
    else data_out[k] = 1.0;

    // high threshold in comparison image
    if (flag_h && data_ptr[k] >= high) data_out[k] = value;

    // low threshold in comparison image
    if (flag_l && data_ptr[k] <= low) data_out[k] = value;

    // mask an exact value
    if (flag_e) {
      // is NAN?
      if (exactval.compare("nan") == 0) {
	if (isnan(data_ptr[k])) data_out[k] = value;
      }
      // is INF?
      else if (exactval.compare("inf") == 0) {
	if (isinf(data_ptr[k])) data_out[k] = value;
      }
      // is exact numeric value?
      else {
	if (data_ptr[k] == exactval_float) data_out[k] = value;
      }
    }
  }
  
  writeImage(output_image, input_image, data_out, call);

  // Free the memory

  return 0;
}
