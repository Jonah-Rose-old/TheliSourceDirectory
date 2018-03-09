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
    cerr << "           -i input_image (main prefix, i.e. without the \"c[1-4].fits\")\n";
    cerr << "      Overscan correction and gain normalisation for the Swope camera (LCO)\n";
    cerr << "      Output will be written to \"prefix\".fits\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  long n, m, i, j, k;
  string input_prefix, input_tmp;

  // print usage if no arguments were given
  if (argc==1) usage(0,argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_prefix = argv[++i];
            break;
      }
    }
  }

  // sorry, lots of hardcoding in here and not-so-nice-code-as-i-would-like;
  // only so much time to do this;

  string in1 = input_prefix+"c1.fits";
  string in2 = input_prefix+"c2.fits";
  string in3 = input_prefix+"c3.fits";
  string in4 = input_prefix+"c4.fits";

  myFITS image1(in1);
  myFITS image2(in2);
  myFITS image3(in3);
  myFITS image4(in4);

  // Image geometries are the same for all four FITS
  n = image1.naxis1;
  m = image1.naxis2;

  vector<float> gain;
  gain.push_back(image1.readKeyFailsafe("EGAIN", 1.0));
  gain.push_back(image2.readKeyFailsafe("EGAIN", 1.0));
  gain.push_back(image3.readKeyFailsafe("EGAIN", 1.0));
  gain.push_back(image4.readKeyFailsafe("EGAIN", 1.0));

  string call = programcall(argv, argc);

  // measure the mean bias / overscan level in the upper right corner
  vector<float> bias1, bias2, bias3, bias4;
  vector<float> bias;
  for (j=2060; j<m; j++) {
    for (i=2060; i<n; i++) {
      bias1.push_back(image1.data[i+n*j]);
      bias2.push_back(image2.data[i+n*j]);
      bias3.push_back(image3.data[i+n*j]);
      bias4.push_back(image4.data[i+n*j]);
    }
  }
  bias.push_back( mean(bias1, bias1.size()));
  bias.push_back( mean(bias2, bias2.size()));
  bias.push_back( mean(bias3, bias3.size()));
  bias.push_back( mean(bias4, bias4.size()));

  // Apply bias / gain correction
  for (k=0; k<n*m; k++) {
    image1.data[k] = (image1.data[k] - bias[0]) * gain[0]; 
    image2.data[k] = (image2.data[k] - bias[1]) * gain[1]; 
    image3.data[k] = (image3.data[k] - bias[2]) * gain[2]; 
    image4.data[k] = (image4.data[k] - bias[3]) * gain[3]; 
  }

  // Trim the overscan sections
  vector<float> data_trim_1;
  vector<float> data_trim_2;
  vector<float> data_trim_3;
  vector<float> data_trim_4;

  long nn = 2048;
  long mm = 2056;
  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      if (i<nn && j<mm) {
	data_trim_1.push_back(image1.data[i+n*j]);
	data_trim_2.push_back(image2.data[i+n*j]);
	data_trim_3.push_back(image3.data[i+n*j]);
	data_trim_4.push_back(image4.data[i+n*j]);
      }
    }
  }

  // Fix a particularly crummy way of storing the data
  vector<float> data_corr_1;
  vector<float> data_corr_2;
  vector<float> data_corr_3;
  vector<float> data_corr_4;
  for (j=0; j<mm; j++) {
    for (i=0; i<nn; i++) {
      // chip 1: rotate by 180 degrees
      data_corr_1.push_back(data_trim_1[nn-i-1+nn*(mm-j-1)]);
      // chip 2: flip vertically
      data_corr_2.push_back(data_trim_2[i + nn*(mm-1-j)]);
      // chip 3: do nothing
      data_corr_3.push_back(data_trim_3[i + nn*j]);
      // chip 4: flip horizontally
      data_corr_4.push_back(data_trim_4[nn-1-i + nn*j]);
    }
  }

  int n_new = 2*nn;
  int m_new = 2*mm;

  vector<float> data_out(n_new*m_new,0);

  // paste the stuff back together
  long ii, jj;
  k = 0;
  for (j=0; j<m_new; j++) {
    for (i=0; i<n_new; i++) {
      // loop over the four quadrants
      if (i >= 0 && i < nn && j >= mm && j < m_new) {
	ii = i;
	jj = j-mm;
	data_out[k++] = data_corr_2[ii+nn*jj];
      }
      if (i >= nn && i < n_new && j >= mm && j < m_new) {
	ii = i-nn;
	jj = j-mm;
	data_out[k++] = data_corr_1[ii+nn*jj];
      }
      if (i >= nn && i < n_new && j >= 0 && j < mm) {
	ii = i-nn;
	jj = j;
	data_out[k++] = data_corr_4[ii+nn*jj];
      }
      if (i >= 0 && i < nn && j >= 0 && j < mm) {
	ii = i;
	jj = j;
	data_out[k++] = data_corr_3[ii+nn*jj];
      }
    }
  }

  writeImage(input_prefix+".fits", input_prefix+"c1.fits", data_out, n_new, m_new, call);

  return 0;
}
