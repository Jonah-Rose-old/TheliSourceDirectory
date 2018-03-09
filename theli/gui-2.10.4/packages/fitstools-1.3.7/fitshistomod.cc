#include <cstring> // for strcpy()
#include <fstream> // for std::ofstream file
#include <iomanip>
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
    cerr << "           -o output_file \n";
    cerr << "          [-x min max ADU range to probe; full range if omitted]\n";
    cerr << "          [-s step size (default: 1)]\n";
    cerr << "          [-t truncate fraction (e.g. 0.99 to keep the central 99% of the data)]\n\n";
    cerr << "  PURPOSE: Creates a histogram of the FITS image within a fixed dynamic range.\n\n";

    exit(1);
  }
}


int main(int argc, char *argv[])
{
  bool flag_range = false, flag_trunc = false;
  int i, k, n, m;
  long t;
  float minval = 0., maxval = 0., step = 1., truncate = 1.0;
  string input_image;
  char output_file[4096];
  
  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'o': strcpy(output_file,argv[++i]);
	break;
      case 'x': 
	minval = atof(argv[++i]);
	maxval = atof(argv[++i]);
	flag_range = true;
	break;
      case 's': step = atof(argv[++i]);
	break;
      case 't': 
	truncate = atof(argv[++i]);
	flag_trunc = true;
	break;
      }
    }
  }

  checkfile(input_image);

  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  if (!flag_range) {
    minval = min(image.data, image.data.size());
    maxval = max(image.data, image.data.size());
  }

  long dim = (maxval - minval) / step;

  //  vector<float> histo_x(dim,0);
  vector<long> histo_x(dim,0);
  vector<float> histo_y(dim,0);

  // the middle of the bins
  for (i=0; i<dim; i++) {
    histo_x[i] = (minval + 0.5*step) + i*step;
    //    histo_x[i] = (minval + 0.5*step) + i*step - 0.5*step;
  }

  // fill in the histogram
  for (k=0; k<n*m; k++) {
    // the index can be calculated directly from the data values
    t = (long) (image.data[k] - minval) / step;
    // acknowledge boundaries
    if (t>=0 && t<dim) {
      histo_y[t]++;
    }
  }

  // normalize histogram
  float norm = 0.;
  for (k=0; k<dim; k++) norm += histo_y[k];
  for (k=0; k<dim; k++) histo_y[k] /= norm;

  // optionally truncate the histogram
  long ind_min = 0;
  long ind_max = dim;
  float cumul;
  if (flag_trunc) {
    cumul = 0.;
    for (k=0; k<dim; k++) {
      cumul += histo_y[k];
      if (cumul >= 0.5 * (1. - truncate)) {
	ind_min = k;
	break;
      }
    }
    cumul = 0.;
    for (k=0; k<dim; k++) {
      cumul += histo_y[k];
      if (cumul >= 0.5 * (1. + truncate)) {
	ind_max = k;
	break;
      }
    }
  }

  // write the histogram
  ofstream file(output_file);
  if (!file.is_open()) {
    cerr << "ERROR: Could not open " << output_file << endl;
    return 1;
  }

  for (k=ind_min; k<ind_max; k++) {
    file << histo_x[k] << " " << histo_y[k] << endl;
  }
  file.close();

  return 0;
}
