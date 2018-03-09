#include <CCfits>
#include <fitsio.h>
#include "fitstools.h"
#include "statistics.h"

using namespace std;

double get_value(vector<float> &data, float radius);

void usage(int i, char *argv[])
{
  if (i == 0) {
    cerr << "\n";
    cerr << "  USAGE: " << argv[0] << endl;
    cerr << "           -i input_image \n";
    cerr << "           -o output_image \n";
    cerr << "           -x x_max y_max (PSF peak position)\n";
    cerr << "           -w radius of the PSF [pixels] \n";
    cerr << "           [-k kappa-sigma threshold (default: 2.0)]\n";
    cerr << "           [-n number of iterations (default: 3)]\n";
    cerr << "           [-m value (do not use pixels with this value)]\n\n";
    cerr << "  PURPOSE: Calculates an azimuthally averaged PSF\n";
    cerr << "           around the specified coordinates.\n";
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  bool masked = false;
  int n, m, niter;
  int left, right, bottom, top;
  long i, j, k, nelem, s, wtot, width;
  float xpsf, ypsf, xi, yi, dist, norm, maskval, kappa;
  string input_image, output_image;

  // print usage if no arguments were given
  if (argc==1) usage(0, argv);

  kappa = 2.0;
  niter = 3;
  maskval = 0.;

  xpsf = 0.;
  ypsf = 0.;
  width = 0.;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
        case 'i': input_image = argv[++i];
            break;
        case 'o': output_image = argv[++i];
            break;
        case 'x': xpsf = atof(argv[++i]);
            ypsf  = atof(argv[++i]);
            break;
        case 'w': width = atol(argv[++i]);
            break;
        case 'k': kappa = atof(argv[++i]);
            break;
	case 'n': niter = atoi(argv[++i]);
            break;
	case 'm': maskval = atof(argv[++i]);
	    masked = true;
            break;
      }
    }
  }
  
  checkfile(input_image);
  
  // read the FITS header and the data block
  myFITS image(input_image);
  n = image.naxis1;
  m = image.naxis2;

  wtot = (2*width+1)*(2*width+1);
  vector<float> rad(width+1,0);
  vector<float> subimage(wtot,0);
  
  // get the image section around the specified PSF centre
  k = 0;
  
  left   = xpsf - width - 1;
  right  = xpsf + width - 1;
  bottom = ypsf - width - 1;
  top    = ypsf + width - 1;
  
  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      if (i>=left && i<=right && j>=bottom && j<=top) {
	subimage[k++] = image.data[i+n*j];
      }
    }
  }

  // divide the PSF in rings
  for (k=0; k<=width; k++) {
    nelem = 0;
    // count how many elements we have in each ring
    for (i=0; i<=2*width; i++) {
      xi = i-width;
      for (j=0; j<=2*width; j++) {
	yi = j-width;
	dist = sqrt(xi*xi+yi*yi);
	 if (!masked) {
	   if (dist<k+1. && dist>=k) nelem++;
	 }
	 else {
	   if (dist<k+1. && dist>=k && subimage[i+(2*width+1)*j] != maskval) nelem++;
	 }
      }
    }	   
    
    if (nelem > 0) {
      vector<float> radtmp(nelem,0);
      // get a sigma-clipped average value for each ring
      s = 0;
      for (i=0; i<=2*width; i++) {
	xi = i-width;
	for (j=0; j<=2*width; j++) {
	  yi = j-width;
	  dist = sqrt(xi*xi+yi*yi);
	  if (!masked) {
	    if (dist<k+1. && dist>=k) {
	      radtmp[s++] = subimage[i+(2*width+1)*j];
	    }
	  }
	  else {
	    if (dist<k+1. && dist>=k && subimage[i+(2*width+1)*j] != maskval) {
	      radtmp[s++] = subimage[i+(2*width+1)*j];
	    }
	  }	   
	}
      }
      
      // write the result
      rad[k] = mean_iter(radtmp,nelem,kappa,niter);
    }
    else rad[k] = 0.;
    //    cout << rad[k] << endl;
  }
  
  vector<float> data_out((2*width+1)*(2*width+1),0);
  vector<double> data((2*width+1)*(2*width+1),0);
  
  // write the PSF image
  for (i=0; i<=2*width; i++) {
    xi = i - width;
    for (j=0; j<=2*width; j++) {
      yi = j - width;
      dist = sqrt(xi*xi+yi*yi);
      k = i + (2 * width + 1) * j;
      data[k] = get_value(rad,dist);
    }
  }
   
  // get the normalisation factor
  norm = 0.;
  for (k=0; k<=2*2*width*width; k++) {
    norm += data[k];
  }
  
  //   cout << "NORM: " << norm << endl;
  
  // normalise the psf
  k = 0;
  for (k=0; k<=2*2*width*width; k++) {
    data_out[k] = data[k] / norm;
  }
  
  writeImage(output_image, input_image, data_out, 2*width+1, 2*width+1);
  
  return 0;
}

//*****************************************************************
double get_value(vector<float> &data, float radius)
{
  int i;
  long dim = data.size();
  float value = 0.;
  
  for (i = 0; i <= dim; i++) {
    if (i >= floor(radius) && i < ceil(radius)) {
      value = data[i];
    }
    // ceilf() does not return the next highest integer
    // if the float tested is equal to an integer value
    if (i == floor(radius) && i == ceil(radius)) {
      value = data[i];
    }
  }
  
  return (value);
}
