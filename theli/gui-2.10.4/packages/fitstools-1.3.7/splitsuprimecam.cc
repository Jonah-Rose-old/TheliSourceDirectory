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
    cerr << "      Cuts a SuprimeCam (2008 new config) into the four readout sections\n\n";
    exit(1);
  }
}

void get_overscan_overall(vector<float>&data, int n, int m, int w, int xmin, float &overscan);

float overscan1, overscan2, overscan3, overscan4;


//***************************************************************
int main(int argc, char *argv[])
{
  long n_new, m_new, i, j, k, t, n, m;
  float w;
  string input_image, output_image;
  float S_GAIN1, S_GAIN2, S_GAIN3, S_GAIN4, gainmax;
  int S_OSMN11, S_OSMX21, S_OSMN31, S_OSMX41;
  int S_EFMN11, S_EFMX11, S_EFMN21, S_EFMX21, S_EFMN31, S_EFMX31, S_EFMN41, S_EFMX41;

  // print usage if no arguments were given
  if (argc==1) usage(0,argv);

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

  // read the FITS header and the data block
  checkfile(input_image);
  string call = programcall(argv, argc);

  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  // get the gains
  S_GAIN1 = image.readKeyFailsafe("S_GAIN1", 1.0);
  S_GAIN2 = image.readKeyFailsafe("S_GAIN2", 1.0);
  S_GAIN3 = image.readKeyFailsafe("S_GAIN3", 1.0);
  S_GAIN4 = image.readKeyFailsafe("S_GAIN4", 1.0);

  vector<float> gains(4,0);
  gains[0] = S_GAIN1;
  gains[1] = S_GAIN2;
  gains[2] = S_GAIN3;
  gains[3] = S_GAIN4;
  gainmax = max(gains, 4);

  // get the overscans
  S_OSMN11 = image.readKeyFailsafe("S_OSMN11", 1.0) - 1;
  S_OSMN31 = image.readKeyFailsafe("S_OSMN31", 1.0) - 1;
  S_OSMX21 = image.readKeyFailsafe("S_OSMX21", 1.0) - 1;
  S_OSMX41 = image.readKeyFailsafe("S_OSMX41", 1.0) - 1;

  // get the image regions
  S_EFMN11 = image.readKeyFailsafe("S_EFMN11", 1.0) - 1;
  S_EFMN21 = image.readKeyFailsafe("S_EFMN21", 1.0) - 1;
  S_EFMN31 = image.readKeyFailsafe("S_EFMN31", 1.0) - 1;
  S_EFMN41 = image.readKeyFailsafe("S_EFMN41", 1.0) - 1;
  S_EFMX11 = image.readKeyFailsafe("S_EFMX11", 1.0) - 1;
  S_EFMX21 = image.readKeyFailsafe("S_EFMX21", 1.0) - 1;
  S_EFMX31 = image.readKeyFailsafe("S_EFMX31", 1.0) - 1;
  S_EFMX41 = image.readKeyFailsafe("S_EFMX41", 1.0) - 1;

  // the overscan width
  w = 25; 

  // get the median overscan for each section
  get_overscan_overall(image.data, n, m, w, S_OSMN11, overscan1);
  get_overscan_overall(image.data, n, m, w, S_OSMX21-w+1, overscan2);
  get_overscan_overall(image.data, n, m, w, S_OSMN31, overscan3);
  get_overscan_overall(image.data, n, m, w, S_OSMX41-w+1, overscan4);

  n_new = S_EFMX11-S_EFMN11 + S_EFMX21-S_EFMN21 + S_EFMX31-S_EFMN31 + S_EFMX41-S_EFMN41 + 4;
  m_new = m;

  vector<float> dataout(n_new*m_new,0);

  // correct the image regions for overscan and gain
  t = 0;
  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      k = i+n*j;
      // process sections 1-4
      if (i >= S_EFMN11 && i <= S_EFMX11) dataout[t++] = (image.data[k] - overscan1) * S_GAIN1 / gainmax;
      if (i >= S_EFMN21 && i <= S_EFMX21) dataout[t++] = (image.data[k] - overscan2) * S_GAIN2 / gainmax;
      if (i >= S_EFMN31 && i <= S_EFMX31) dataout[t++] = (image.data[k] - overscan3) * S_GAIN3 / gainmax;
      if (i >= S_EFMN41 && i <= S_EFMX41) dataout[t++] = (image.data[k] - overscan4) * S_GAIN4 / gainmax;
    }
  }

  writeImage(output_image, input_image, dataout, n_new, m_new, call);

  return 0;
}


//***************************************************************
void get_overscan_overall(vector<float>&data, int n, int m, int w, int xmin, float &overscan)
{
  long i, j, k, t;

  vector<float> oscan(w*m,0);

  t = 0;
  for (j=0; j<m; j++) {
    for (k=0; k<w; k++) {
      i = xmin + k;
      oscan[t++] = data[i+n*j];
    }
  }
  overscan = median(oscan, w*m);
}  
