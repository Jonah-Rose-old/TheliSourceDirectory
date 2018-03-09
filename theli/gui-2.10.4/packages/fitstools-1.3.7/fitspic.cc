#include <CCfits>
#include <fitsio.h>
#include <fstream>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

//*****************************************************************

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input catalog \n";
    cerr << "           -a input image \n";
    cerr << "           -o output catalog \n";
    cerr << "           [ -f filter size (default: 200.0)]\n";
    cerr << "           [ -l minimum number data points for statistics (default: 5)]\n";
    cerr << "           [ -m If only a global statistics should be obtained]\n";
    cerr << "  PURPOSE: Calculates the mean magnitude zeropoint and dumps\n";
    cerr << "           a file with 2D information about (future) illumination correction.\n\n";
    exit(1);
  }
}


//*****************************************************************

int main(int argc, char *argv[])
{
  long i, j, k, nobj, nummin, galcount, tmpcount, naxis1, naxis2;
  int  mosaic;
  float filtersize_tmp, rmsval;
  float filtersize, medianoffset;
  float xmax, ymax, sizemax;
  char output_catalog[FILEMAX];
  string input_image, input_file;

  nummin = 5;
  filtersize = 200;
  mosaic = 0;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  // read the arguments
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_file = argv[++i];
	break;
      case 'a': input_image = argv[++i];
	break;
      case 'o': strcpy(output_catalog,argv[++i]);
	break;
      case 'f': filtersize = atof(argv[++i]);
	break;
      case 'l': nummin = atol(argv[++i]);
	break;
      case 'm': mosaic = 1;
      }
    }
  }
  
  checkfile(input_image);
  string call = programcall(argv, argc);

  // Get the geometry, do not read the data
  myFITS image_in(input_image, false);
  naxis1 = image_in.naxis1;
  naxis2 = image_in.naxis2;

  if (naxis1 <= 0 || naxis2 <= 0) {
    printf("ERROR: Invalid image dimensions!\n");
    exit (1);
  }

  vector<float> x;
  vector<float> y;
  vector<float> mag_obj;
  vector<float> mag_ref;

  myFITS fitstable(input_file, false);
  fitstable.readTableColumn("PSSC", "Xpos", x);
  fitstable.readTableColumn("PSSC", "Ypos", y);
  fitstable.readTableColumn("PSSC", "Mag_OBJ", mag_obj);
  fitstable.readTableColumn("PSSC", "Mag_REF", mag_ref);
  nobj = fitstable.nrows;

  vector<float> mag_diff(nobj, 0.);

  for (i=0; i<nobj; i++) {
    mag_diff[i] = mag_obj[i] - mag_ref[i];
  }

  xmax = max(x, nobj);
  ymax = max(y, nobj);
  sizemax = sqrt(xmax*xmax + ymax*ymax);

  for (i=0; i<nobj; i++) {
    mag_diff[i] = mag_obj[i] - mag_ref[i];
  }

  medianoffset = median(mag_diff, nobj);
  rmsval = rms(mag_diff, nobj);
  float medianoffset_err = rmsval / sqrt((float) nobj);

  if (mosaic == 1) {
    cout << -1.0 * medianoffset << " " << medianoffset_err << " " << rmsval << endl;
    return 0;
  }

  // remove a constant zero point offset
  // calculate flux correction factors for the image
  vector<float> fluxcorr(nobj, 0.);
  for (i=0; i<nobj; i++) {
    fluxcorr[i] = pow(10, 0.4 * (mag_obj[i] - mag_ref[i] - medianoffset));
  }

  // median filtering
  // we want at least nummin objects in the statistics, and a minimum
  // filter size of filtersize

  ofstream outfile(output_catalog);
  if (!outfile.is_open()) {
      cerr << "ERROR: Could not open file " << output_catalog << "!\n";
    exit (1);
  }

  vector<float> dist(nobj, 0.);
  vector<float> fluxcorrmedian(nobj, 0.);
  vector<float> tmparr;
  for (i=0; i<nobj; i++) {
    filtersize_tmp = filtersize;
    galcount = 0;
    for (j=0; j<nobj; j++) {
      dist[j] = sqrt((x[j] - x[i]) * (x[j] - x[i]) +
		     (y[j] - y[i]) * (y[j] - y[i]));
    }
    while (galcount < nummin && filtersize_tmp < sizemax) {
      tmpcount = 0;
      for (j=0; j<nobj; j++) {
	if (dist[j] < filtersize_tmp) tmpcount++;
      }
      filtersize_tmp *= 1.3;
      galcount = tmpcount;
      tmpcount = 0;
    }
    filtersize_tmp /= 1.3;

    tmparr.resize(galcount,0.);
    k = 0;
    for (j=0; j<nobj; j++) {
      if (dist[j] < filtersize_tmp) {
	tmparr[k] = fluxcorr[j];
	k++;
      }
    }
    fluxcorrmedian[i] = median(tmparr, k);

    outfile << x[i] << " " << y[i] << " " << fluxcorrmedian[i] << endl;
  }

  rmsval = rms(fluxcorr, nobj);

  cout << -1.0 * medianoffset<< " " << medianoffset_err << " " << rmsval << endl;

  outfile.close();

  return (0);
}
