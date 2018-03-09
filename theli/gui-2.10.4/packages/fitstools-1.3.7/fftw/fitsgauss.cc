#include <CCfits>
#include <fitsio.h>
#include <fftw3.h>
#include <unistd.h>
#include "fftw++.h"
#include "convolve.h"
#include "fitstools.h"
#include "statistics.h"

using namespace std;
using namespace fftwpp;

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           -o output_image \n";
    cerr << "           -f FWHM (the width of the kernel in pixels, may be a float number)\n";
    cerr << "          [-m MODE (convolution / classic); defaulted to 'convolution']\n";
    cerr << "          [-e extent (width of the gaussian window, in units of fwhm, default: 2.0; for MODE=convolution only)\n";
    cerr << "          [-n number of CPUs to use (automatic, if not specified); for MODE=convolution only]\n";
    cerr << "          [-v mask value (for MODE=classic only)]\n";
    cerr << "          [-d (dumb mode, use no FFTW wisdom, results in slower execution)]\n";
    cerr << "    Convolves an image with a Gaussian.\n";
    cerr << "    The kernel is set to zero for pixels at radii\n";
    cerr << "    -- MODE=convolution: FFTW transforms and backtransforms are used to convolve the image.\n";
    cerr << "    -- MODE=classic: The image is convolved in real space; use small kernels of a few pixels only,\n";
    cerr << "                     but uses min/max outlier rejection.\n";
    cerr << "                     Options -e and -m will be ignored.\n";
    cerr << "    The kernel is set to zero for pixels at radii\n";
    cerr << "    larger than 'extent' times the FWHM.\n\n";
    exit(1);
  }
}


int main(int argc, char *argv[])
{
  int n, m, nthreads_user, ncpu, nthreads;
  long i;
  double fwhm, ext;
  float maskvalue = -1e12;
  string input_image, output_image, mode;
  bool use_wisdom = true, mask = false;
  char path_to_wisdom[1000];

  // print usage if no arguments were given  
  if (argc==1) usage(0, argv);
  
  fwhm = 0.;
  ext = 2.0;
  nthreads_user = 0;
  mode = "convolution";

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': input_image = argv[++i];
	break;
      case 'o': output_image = argv[++i];
	break;
      case 'f': fwhm = atof(argv[++i]);
	break;
      case 'm': mode = argv[++i];
	break;
      case 'e': ext = atof(argv[++i]);
	break;
      case 'n': nthreads_user = atoi(argv[++i]);
	break;
      case 'd': use_wisdom = false;
	break;
      case 'v': mask = true;
	maskvalue = atof(argv[++i]);
	break;
      }
    }
  }
  
  // multi-threading
  ncpu = sysconf(_SC_NPROCESSORS_ONLN);
  nthreads = ncpu;
  if (nthreads_user > 0) {
    if (nthreads_user > ncpu) nthreads = ncpu;
    else nthreads = nthreads_user;
  }

  // read the FITS header and the data block
  checkfile(input_image);
  string call = programcall(argv, argc);
  
  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  // using FFTW for the convolution
  if (mode.compare("convolution") == 0) {
      
    // if the kernel is too small, the convolution will not yield a convolved image
    i = 0;
    while (fwhm*ext < 1.0 && i < 100) {
      ext *= 1.2;
      i++;
    }

    // do we use wisdom?
    if (!use_wisdom) fftw::effort=FFTW_ESTIMATE;
    else {
      fftw::effort=FFTW_MEASURE;
      get_path_to_wisdom(path_to_wisdom);
      fftw_import_wisdom_from_filename(path_to_wisdom);
    }

    convolve_gauss_2d(image.data, n, m, fwhm, nthreads);

    writeImage(output_image, input_image, image.data, call);

    // export wisdom
    //    if (use_wisdom) fftw_export_wisdom_to_filename(path_to_wisdom);

    writeImage(output_image, input_image, image.data, call);
  }

  // doing it the old fashioned way with min/max rejection and optional masking
  else {
    int errorscale_flag = 0;
    long it, jt, i, j;
    double rsq, min, minw, max, maxw, sum, sumw, gaussval, imgdata;
    vector<float> data_out(n*m,0);

    fwhm *= 0.5;  // used as a "radius"

    for (j=0; j<m; j++) {
      for (i=0; i<n; i++) {
	min  = image.data[i+n*j];
	max  = image.data[i+n*j];
	sum  = 0.;
	sumw = 0.;
	minw = 0.;
	maxw = 0.;
	for (jt=floor(j-fwhm);jt<=ceil(j+fwhm);jt++) {
	  for (it=floor(i-fwhm);it<=ceil(i+fwhm);it++) {
	    if (it>=0 && jt>=0 && it<n && jt<m) {
	      rsq = (i-it)*(i-it) + (j-jt)*(j-jt);
	      if (mask) {
		if (rsq <= fwhm*fwhm && image.data[it+n*jt] != maskvalue) {
		  gaussval = exp(-rsq/8.); // normalization in 'sumw'; using sigma = 2
		  imgdata = image.data[it+n*jt];
		  if (imgdata < min) {
		    min = imgdata;
		    minw = gaussval;
		  }
		  if (imgdata > max) {
		    max = imgdata;
		    maxw = gaussval;
		  }
		  sum += imgdata * gaussval;
		  sumw += gaussval;
		}
	      }
	      else { // no masking
		if (rsq <= fwhm*fwhm) {
		  gaussval = exp(-rsq/8.); // normalization in 'sumw'; using sigma = 2
		  imgdata = image.data[it+n*jt];
		  if (imgdata < min) {
		    min = imgdata;
		    minw = gaussval;
		  }
		  if (imgdata > max) {
		    max = imgdata;
		    maxw = gaussval;
		  }
		  sum += imgdata * gaussval;
		  sumw += gaussval;
		}
	      }
	    }
	  }
	}
	sum  = sum - min*minw - max*maxw;
	sumw = sumw - minw - maxw;
	
	// print the correction factor for the error map
	if (i == n/2 && j == m/2 && errorscale_flag == 0) {
	  cout << "Correction factor for error map: " << sqrt(sumw) << endl;
	  errorscale_flag = 1;
	}
	
	data_out[i+n*j] = sum / sumw;
      }
    }
    
    writeImage(output_image, input_image, data_out, call);
  }

  return 0;
}
