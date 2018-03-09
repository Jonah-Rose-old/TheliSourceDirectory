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
    cerr << "           -o output_image \n";
    cerr << "           -m mode (Y4KCam or Tek2K)\n";
    cerr << "      Overscan correction and gain normalisation for a\n";
    cerr << "      single pasted image with four readout channel.\n\n";
    exit(1);
  }
}

void set_instrument_params(string mode);

int xmin[4], xmax[4], ymin[4], ymax[4];
int ovxmin[4], ovxmax[4], ovymin[4], ovymax[4];
int n_new, m_new;
vector<float> overscan;
vector<float> gain;

int main(int argc, char *argv[])
{
  long n, m, i, j, k, l;
  string input_image, output_image, mode;

  // print usage if no arguments were given
  if (argc==1) usage(0,argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
        case 'm': mode = argv[++i];
            break;
      }
    }
  }

  // read the FITS main header
  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  set_instrument_params(mode);

  vector<float> oscan(4,0);

  for (i=0; i<=3; i++) {
    oscan[i] = get_overscan_4quadchannels(image, ovxmin[i], ovxmax[i], ovymin[i], ovymax[i]);
  }

  vector<float> data_out(n_new*m_new,0);

  // correct the image for overscan and gain
  k = 0;
  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      // loop over the four quadrants
      for (l=0; l<=3; l++) {
	if (i >= xmin[l] && i <= xmax[l] && j >= ymin[l] && j <= ymax[l]) {
	  data_out[k++] = (image.data[i+n*j] - oscan[l]) * gain[l];
	}
      }
    }
  }

  writeImage(output_image, input_image, data_out, n_new, m_new, call);

  return 0;
}

//***************************************************************
// Set instrument specific parameters
//***************************************************************
void set_instrument_params(string mode) 
{
  // sorry, these are hard-coded
  n_new = 0;
  m_new = 0;

  if (mode.compare("Y4KCam") == 0) {
    n_new = 4064;
    m_new = 4064;
    
    xmin[0] = 0; xmin[1] = 2072; xmin[2] = 0; xmin[3] = 2072;
    xmax[0] = 2031; xmax[1] = 4103; xmax[2] = 2031; xmax[3] = 4103;
    ymin[0] = 0; ymin[1] = 0; ymin[2] = 2072; ymin[3] = 2072;
    ymax[0] = 2031; ymax[1] = 2031; ymax[2] = 4103; ymax[3] = 4103;
    ovxmin[0] = 2032; ovxmin[1] = 2052; ovxmin[2] = 2032; ovxmin[3] = 2052;
    ovxmax[0] = 2051; ovxmax[1] = 2071; ovxmax[2] = 2051; ovxmax[3] = 2071;
    ovymin[0] = 0; ovymin[1] = 0; ovymin[2] = 2072; ovymin[3] = 2072;
    ovymax[0] = 2031; ovymax[1] = 2031; ovymax[2] = 4103; ovymax[3] = 4103;
    gain[0] = 1.0000; gain[1] = 1.0123; gain[2] = 1.0574; gain[3] = 1.0637; 
    overscan.resize(21*2032,0);

    /* initializer lists for c++11 standard
       int xmin[4] = {0,2072,0,2072};
       int xmax[4] = {2031,4103,2031,4103};
       int ymin[4] = {0,0,2072,2072};
       int ymax[4] = {2031,2031,4103,4103};
       int ovxmin[4] = {2032,2052,2032,2052};
       int ovxmax[4] = {2051,2071,2051,2071};
       int ovymin[4] = {0,0,2072,2072};
       int ovymax[4] = {2031,2031,4103,4103};
       float gain[4] = {1.0000,1.0123,1.0574,1.0637};
    */
  }

  if (mode.compare("Tek2K") == 0) {
    n_new = 4064;
    m_new = 4064;
    
    xmin[0] = 21; xmin[1] = 1172; xmin[2] = 21; xmin[3] = 1172; 
    xmax[0] = 1043; xmax[1] = 2194; xmax[2] = 1043; xmax[3] = 2194; 
    ymin[0] = 0; ymin[1] = 0; ymin[2] = 1023; ymin[3] = 1023; 
    ymax[0] = 1022; ymax[1] = 1022; ymax[2] = 2045; ymax[3] = 2045; 
    ovxmin[0] = 1050; ovxmin[1] = 1110; ovxmin[2] = 1050; ovxmin[3] = 1110; 
    ovxmax[0] = 1100; ovxmax[1] = 1160; ovxmax[2] = 1100; ovxmax[3] = 1160; 
    ovymin[0] = 0; ovymin[1] = 0; ovymin[2] = 1023; ovymin[3] = 1023; 
    ovymax[0] = 1022; ovymax[1] = 1022; ovymax[2] = 2045; ovymax[3] = 2045; 
    gain[0] = 1.09249; gain[1] = 1.0000; gain[2] = 1.11877; gain[3] = 1.02534; 
    overscan.resize(51*1023,0);

    /* initializer lists for c++11 standard
       int xmin[4] = {21, 1172, 21, 1172};
       int xmax[4] = {1043, 2194, 1043, 2194};
       int ymin[4] = {0, 0, 1023, 1023};
       int ymax[4] = {1022, 1022, 2045, 2045};
       int ovxmin[4] = {1050, 1110, 1050, 1110};
       int ovxmax[4] = {1100, 1160, 1100, 1160};
       int ovymin[4] = {0, 0, 1023, 1023};
       int ovymax[4] = {1022, 1022, 2045, 2045};
       float gain[4] = {1.09249, 1.0000, 1.11877, 1.02534};
    */
  }
}
