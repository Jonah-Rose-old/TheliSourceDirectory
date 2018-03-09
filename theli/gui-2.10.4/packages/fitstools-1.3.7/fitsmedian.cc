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
    cerr << "          [-s filter size (default: 1)\n";
    cerr << "          [-m mask value]\n";
    cerr << "          [-d mode (normal | fast; default: normal)]\n";
    cerr << "          [-e (0: write error map only)]\n";
    cerr << "          [-e (1: write error as well)]\n\n";
    cerr << "  PURPOSE: Calculates a median-filtered image.\n";
    cerr << "          Pixels with values -m are ignored.\n";
    cerr << "          The output error map will have '.err' inserted before the '.fits' suffix\n\n";

    exit(1);
  }
}

int main(int argc, char *argv[])
{
  bool maskflag = false, errorflag = false;
  int n, m, filtersize, it, jt, s, error = -1;
  long i, j, k, t;
  float maskval;
  string mode = "normal";
  string input_image, output_image;

  filtersize = 1;
  maskval = 0.;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	break;
      case 's': filtersize = atoi(argv[++i]);
	break;
      case 'm': maskval = atof(argv[++i]);
	maskflag = true;
	break;
      case 'e': error = atoi(argv[++i]);
	errorflag = true;
	break;
      case 'd': mode = argv[++i];
	break;
      }
    }
  }

  if (mode.compare("normal") != 0 && mode.compare("fast") != 0) {
    cerr << "ERROR: Invalid mode!\n";
    exit (1);
  }

  checkfile(input_image);
  string call = programcall(argv, argc);

  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  s = 2*filtersize+1;
  s = s*s;
  vector<float> chunk(s,0);
  vector<float> data_out(n*m,0);
  vector<float> error_out(n*m,0);

  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      k = 0;
      for (jt=j-filtersize;jt<=j+filtersize;jt++) {
	for (it=i-filtersize;it<=i+filtersize;it++) {
	  if (!maskflag) {
	    if (it>=0 && jt>=0 && it<n && jt<m) {
	      chunk[k++] = image.data[it+n*jt];
	    }
	  }
	  else {
	    if (it>=0 && jt>=0 && it<n && jt<m && image.data[it+n*jt] != maskval) {
	      chunk[k++] = image.data[it+n*jt];
	    }
	  }
	}
      }

      // the median-filtered image
      if (error != 0) {
	if (k>1)
	  data_out[i+n*j] = median(chunk, k, mode);
	else data_out[i+n*j] = maskval;
      }
      
      // the error map of the median image
      if (errorflag) {
	if (k>0) {
	  for (t=0; t<k; t++) {
	    error_out[i+n*j] += rms(chunk, k);
	  }
	  // sqrt(PI/2) = 1.25331 is the rescaling from a mean to a median error 
	  // (assuming gaussian parent distribution)
	  error_out[i+n*j] = error_out[i+n*j] * 1.25331 / (float) k;
	}
	else error_out[i+n*j] = 0.;
      }
    }
  }
  
  writeImage(output_image, input_image, data_out, call);

  if (errorflag) {
    string errout = output_image;
    errout.resize(output_image.length() - 5);
    errout += ".err.fits";
    writeImage(errout, input_image, error_out, call);
  }

  return 0;
}
