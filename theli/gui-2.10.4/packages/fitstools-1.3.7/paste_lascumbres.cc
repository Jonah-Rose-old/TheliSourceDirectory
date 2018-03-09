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
    cerr << "           -i input_image root name\n";
    cerr << "           -o output_image \n";
    cerr << "           -n instrumentname (FL03_LCOGT@CTIO or FL04_LCOGT@CTIO)\n";
    cerr << "      Overscan, gain and cross-talk correction, and pasting into a single image\n";
    cerr << "      for the Las Cumbres network with four readout channels.\n\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  long n, m, i, j, k;
  string input_image, output_image, instname;

  // print usage if no arguments were given
  if (argc==1) usage(0,argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
        case 'n': instname = argv[++i];
            break;
      }
    }
  }

  vector<float> oscan(4,0.);
  vector<float> gain(4,0.);
  vector<int> xmin(4,0.);
  vector<int> xmax(4,0.);

  // read the FITS main header
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image0(input_image+"_sl01.fits");
  myFITS image1(input_image+"_sl02.fits");
  myFITS image2(input_image+"_sl03.fits");
  myFITS image3(input_image+"_sl04.fits");
  n = image1.naxis1;
  m = image1.naxis2;

  // Overscans
  int ovxmin = 2060;
  int ovxmax = 2075;
  int ovymin = 1;
  int ovymax = 2048;
  oscan[0] = get_overscan_4quadchannels(image0, ovxmin, ovxmax, ovymin, ovymax);
  oscan[1] = get_overscan_4quadchannels(image1, ovxmin, ovxmax, ovymin, ovymax);
  oscan[2] = get_overscan_4quadchannels(image2, ovxmin, ovxmax, ovymin, ovymax);
  oscan[3] = get_overscan_4quadchannels(image3, ovxmin, ovxmax, ovymin, ovymax);

  // Gains
  if (instname.compare("FL03_LCOGT@CTIO")==0) {
    gain[0] = 2.1000/2.1000;
    gain[1] = 2.0895/2.1000;
    gain[2] = 2.1683/2.1000;
    gain[3] = 2.1517/2.1000;
  } else if (instname.compare("FL04_LCOGT@CTIO")==0) {
    gain[0] = 2.1000/2.1000/1.01675; 
    gain[1] = 2.1252/2.1000/1.01675; 
    gain[2] = 2.1819/2.1000; 
    gain[3] = 2.1584/2.1000;          
  } else {
    cerr << "Unknown instrument: " << instname << endl;
    cerr << "Must be either FL03_LCOGT@CTIO or FL04_LCOGT@CTIO" << endl;
    exit (1);
  }

  // correct the image for overscan and gain
  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      image0.data[i+n*j] = (image0.data[i+n*j] - oscan[0]) * gain[0];
      image1.data[i+n*j] = (image1.data[i+n*j] - oscan[1]) * gain[1];
      image2.data[i+n*j] = (image2.data[i+n*j] - oscan[2]) * gain[2];
      image3.data[i+n*j] = (image3.data[i+n*j] - oscan[3]) * gain[3];
    }
  }
  
  // Trim the overscan sections
  vector<float> d0;
  vector<float> d1;
  vector<float> d2;
  vector<float> d3;

  long nn = 2048;
  long mm = 2048;
  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      if (i<nn && j<mm) {
	d0.push_back(image0.data[i+n*j]);
	d1.push_back(image1.data[i+n*j]);
	d2.push_back(image2.data[i+n*j]);
	d3.push_back(image3.data[i+n*j]);
      }
    }
  }

  // Xtalk coefficients
  vector<float>xtalk_0(4,0.0);
  vector<float>xtalk_1(4,0.0);
  vector<float>xtalk_2(4,0.0);
  vector<float>xtalk_3(4,0.0);

  if (instname.compare("FL03_LCOGT@CTIO")==0) {
    xtalk_0[0] = 0.0;
    xtalk_0[1] = 0.00076;
    xtalk_0[2] = 0.00079;
    xtalk_0[3] = 0.00115;
    xtalk_1[0] = 0.00073;
    xtalk_1[1] = 0.0;
    xtalk_1[2] = 0.00117;
    xtalk_1[3] = 0.00084;
    xtalk_2[0] = 0.00074;
    xtalk_2[1] = 0.00113;
    xtalk_2[2] = 0.0;
    xtalk_2[3] = 0.00084;
    xtalk_3[0] = 0.00105;
    xtalk_3[1] = 0.00075;
    xtalk_3[2] = 0.00080;
    xtalk_3[3] = 0.0;
  } else if (instname.compare("FL04_LCOGT@CTIO")==0) {
    xtalk_0[0] = 0.0;
    xtalk_0[1] = 0.00088;
    xtalk_0[2] = 0.00096;
    xtalk_0[3] = 0.00131;
    xtalk_1[0] = 0.00087;
    xtalk_1[1] = 0.0;
    xtalk_1[2] = 0.00132;
    xtalk_1[3] = 0.00099;
    xtalk_2[0] = 0.00087;
    xtalk_2[1] = 0.00127;
    xtalk_2[2] = 0.0;
    xtalk_2[3] = 0.00103;
    xtalk_3[0] = 0.00123;
    xtalk_3[1] = 0.00089;
    xtalk_3[2] = 0.00094;
    xtalk_3[3] = 0.0;
  }

  // Xtalk correction
  vector<float> tmp_0;
  vector<float> tmp_1;
  vector<float> tmp_2;
  vector<float> tmp_3;

  k = 0;
  for (j=0; j<mm; j++) {
    for (i=0; i<nn; i++) {
      tmp_0.push_back(d0[k] - xtalk_0[0]*d0[k] - xtalk_1[0]*d1[k] - xtalk_2[0]*d2[k] - xtalk_3[0]*d3[k]);
      tmp_1.push_back(d1[k] - xtalk_0[1]*d0[k] - xtalk_1[1]*d1[k] - xtalk_2[1]*d2[k] - xtalk_3[1]*d3[k]);
      tmp_2.push_back(d2[k] - xtalk_0[2]*d0[k] - xtalk_1[2]*d1[k] - xtalk_2[2]*d2[k] - xtalk_3[2]*d3[k]);
      tmp_3.push_back(d3[k] - xtalk_0[3]*d0[k] - xtalk_1[3]*d1[k] - xtalk_2[3]*d2[k] - xtalk_3[3]*d3[k]);
      k++;
    }
  }

  // Fix a particularly crummy way of storing the data
  k = 0;
  for (j=0; j<mm; j++) {
    for (i=0; i<nn; i++) {
      // chip 1: rotate by 180 degrees
      d0[k] = tmp_0[nn-i-1+nn*(mm-j-1)];
      // chip 2: flip vertically
      d1[k] = tmp_1[i + nn*(mm-1-j)];
      // chip 3: do nothing
      d2[k] = tmp_2[i + nn*j];
      // chip 4: flip horizontally
      d3[k] = tmp_3[nn-1-i + nn*j];
      k++;
    }
  }

  // Paste the quadrants together
  k = 0;

  // Instrument parameters
  int n_new = 2*nn;
  int m_new = 2*mm;  

  int ii, jj;
  vector<float> data_out(n_new*m_new, 0.);
  for (j=0; j<m_new; j++) {
    for (i=0; i<n_new; i++) {
      // loop over the four quadrants
      if (i >= 0 && i < nn && j >= mm && j < m_new) {
	ii = i;
	jj = j-mm;
	data_out[i+n_new*j] = d1[ii+nn*jj];
      }
      if (i >= nn && i < n_new && j >= mm && j < m_new) {
	ii = i-nn;
	jj = j-mm;
	data_out[i+n_new*j] = d0[ii+nn*jj];
      }
      if (i >= nn && i < n_new && j >= 0 && j < mm) {
	ii = i-nn;
	jj = j;
	data_out[i+n_new*j] = d3[ii+nn*jj];
      }
      if (i >= 0 && i < nn && j >= 0 && j < mm) {
	ii = i;
	jj = j;
	data_out[i+n_new*j] = d2[ii+nn*jj];
      }
    }
  }

  // Rotate the entire darn thing by 180 degrees
  k = 0;
  vector<float> data_out1(n_new*m_new, 0.);
  for (j=0; j<m_new; j++) {
    for (i=0; i<n_new; i++) {
      data_out1[k] = data_out[n_new-i-1+n_new*(m_new-j-1)];
      k++;
    }
  }

  writeImage(output_image, input_image+".fits", data_out1, n_new, m_new, call);
  // writeImage(output_image, input_image+"_sl01.fits", data_out1, n_new, m_new, call);

  return 0;
}
