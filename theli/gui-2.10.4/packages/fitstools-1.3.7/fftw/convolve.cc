#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cstring>
#include <cmath>
#include <pwd.h>
#include <vector>
#include <cctype>
#include <fftw3.h>
#include <unistd.h>
#include "Array.h"
#include "fftw++.h"
#include "convolve.h"
#include "statistics.h"

using namespace std;
using namespace Array;
using namespace fftwpp;


//**********************************************************************************
// Add a column to the right, and/or a row on top to make image dimensions even
//**********************************************************************************
void make_image_even(vector<float> &data_even, vector<float> &data, long n, long m, long *n_new, long *m_new)
{
  long i, j, nn, mn;
  
  // check whether dimensions are even/odd, and make them even
  *n_new = n;
  *m_new = m;
  if (n % 2 != 0) *n_new = n + 1;
  if (m % 2 != 0) *m_new = m + 1;
  
  nn = *n_new;
  mn = *m_new;

  // resize the vector correspondingly
  data_even.resize(nn*mn, 0.);

  // add a column to the right, and/or a row on top if needed
  for (j=0; j<mn; j++) {
    for (i=0; i<nn; i++) {
      if (i<n && j<m) data_even[i+nn*j] = data[i+n*j];
      else {
	// repeat the last column / row to avoid artifacts from zero-valued data in the convolved image
	if (i==n) data_even[i+nn*j] = data[(i-1)+n*j];
	if (j==m) data_even[i+nn*j] = data[i+n*(j-1)];
      }
    }
  }
}


//**********************************************************************************
// Add a column to the right, and/or a row on top to make image dimensions even
//**********************************************************************************
void make_image_even2(vector<float> &data_sampled, vector<float> &data, float fwhm, long n, 
		      long m, long &n_new, long &m_new, int subsample)
{
  long i, j;
  long w, wr, wu, ns, ms;

  w = 2 * (int) fwhm;
  
  // make an even image to avoid unclear quadrant swapping
  // for odd side lengths
  if ( n % 2 != 0) 
    wr = w + 1;
  else 
    wr = w;
  if ( m % 2 != 0) 
    wu = w + 1;
  else 
    wu = w;
  
  ns = n + w + wr;
  ms = m + w + wu;
  
  // resize the vector correspondingly
  vector<float> data_large(ns*ms, 0.);

  // wrap a border around the image
  // in order to neutralize the periodic boundary condition of the FFT  
  for (j=0; j<ms; j++) {
    for (i=0; i<ns; i++) {
      // write the centre of the larger image
      if (j-w>=0 && j-w<m && i-w>=0 && i-w<n)
	data_large[i+ns*j] = data[(i-w)+n*(j-w)];
      // write the edges
      if (j<w && i>=w && i<n+w) // bottom
	data_large[i+ns*j] = data[i-w];
      if (j>=m+w && i>=w && i<n+w) // topsizeof(double));
	data_large[i+ns*j] = data[i-w+n*(m-1)];
      if (i<w && j>=w && j<m+w) // left
	data_large[i+ns*j] = data[n*(j-w)];
      if (i>=n+w && j>=w && j<m+w) // right
	data_large[i+ns*j] = data[(n-1)+n*(j-w)];
      // write the corners
      if (j<w && i<w) // ll
	data_large[i+ns*j] = data[0];
      if (j<w && i>=n+w) // lr
	data_large[i+ns*j] = data[n-1];
      if (j>=m+w && i<w) // ul
	data_large[i+ns*j] = data[n*(m-1)];
      if (j>=m+w && i>=n+w) // ur
	data_large[i+ns*j] = data[n*m-1];
    }
  }

  if (subsample == 1) {
    n_new = ns;
    m_new = ms;
    data_sampled.resize(ns*ms);
    for (i=0; i<ns*ms; i++) data_sampled[i] = data_large[i];
  }
  else if (subsample == 2) {
    n_new = ns/2;
    m_new = ms/2;
    data_sampled.resize(ns*ms/4);
    long k = 0;
    for (j=0; j<ms; j+=2) {
      for (i=0; i<ns; i+=2) {
	data_sampled[k++] = data_large[i+ns*j];
      }
    }
  }
  else {
    cerr << "ERROR: Invalid subsampling size!" << endl;
    exit (1);
  }
}


//***************************************************************
// set/get the pwd of the wisdom file to/from ${HOME}/.fftw/
//***************************************************************
void get_path_to_wisdom(char *path_to_wisdom)
{
  // the home directory
  int myuid;
  passwd *mypasswd;
  myuid = getuid();
  mypasswd = getpwuid(myuid);
 
  // path to the wisdom file
  strcpy(path_to_wisdom, mypasswd->pw_dir);
  strcat(path_to_wisdom,"/.fftw/wisdom3.txt");

  // check if the file exists
  ifstream infile(path_to_wisdom);
  if (!infile.is_open()) {
    int check = system("mkdir -p \"${HOME}/.fftw/\"");
    if (check != -1) {
      cerr << "\n   " << path_to_wisdom << " does not exist (requested by libfftw).\n\n";
      cerr << "   This file contains optimized FFT plans for your hardware for faster execution times.\n"; 
      cerr << "   It is created now for future use. This will take a few minutes.\n";
      cerr << "   If you don't want this, abort and append the \"-d\" option to the program call.\n\n";
    }
    else {
      cerr << "ERROR: Could not create " << path_to_wisdom << " (requested by libfftw)!" << endl;
      exit(EXIT_FAILURE);
    }
  }
  infile.close();
}


//***************************************************************
// Convolve a 2D array with a Gaussian
//***************************************************************
void convolve_gauss_2d(vector<float> &data, long n, long m, float fwhm, int nthreads)
{
  long i, j;
  float xcen, ycen, ssquared, s, rsquared, nfac, ext, tmp;
  
  // First of all, make the data even spaced so that we avoid issues with quadrant swapping
  // (i can't figure out how the quadrants are defined for uneven dimensions)
  vector<float> data_sampled;
  long n_new, m_new;
  int subsample = 1; // subsampling NOT IMPLEMENTED

  make_image_even2(data_sampled, data, fwhm, n, m, n_new, m_new, subsample);

  fftw::maxthreads = nthreads;

  s = fwhm / 2.35 / subsample; // sigma

  xcen = n_new/2.;
  ycen = m_new/2.;
  ssquared = s * s;
  nfac = 1.0;
  ext = 2.5; // how many sigmas wide do we want the kernel

  // Kernel normalisation
  nfac = 0.;
  for (j=0; j<m_new; j++) {
    for (i=0; i<n_new; i++) {
      rsquared = (i-xcen)*(i-xcen) + (j-ycen)*(j-ycen);
      // weight = 0 if r larger than ext*sigma:
      if (rsquared <= ext*ext*ssquared) nfac += gauss(rsquared, ssquared);
    }
  }

  int mp = m_new/2 + 1;
  size_t align = sizeof(Complex);

  array2<double> f(n_new,m_new,align);
  array2<Complex> g(n_new,mp,align);

  array2<double> kernelf(n_new,m_new,align);
  array2<Complex> kernelg(n_new,mp,align);

  rcfft2d Forward(m_new,f,g);
  crfft2d Backward(m_new,g,f);

  // create the kernel
  for (j=0; j<m_new; j++) {
    for (i=0; i<n_new; i++) {
      rsquared = (i-xcen)*(i-xcen) + (j-ycen)*(j-ycen);
      // weight = 0 if r larger than ext*sigma:
      if (rsquared <= ext*ext*ssquared) kernelf(i,j) = gauss(rsquared, ssquared) / nfac;
      else kernelf(i,j) = 0.;
    }
  }

  // write the data into an array
  for (i=0; i<n_new; i++) {
    for (j=0; j<m_new; j++) {
      f(i,j) = data_sampled[i+n_new*j];
    }
  }

  // Forward FFT
  Forward.fft(f,g);
  Forward.fft(kernelf,kernelg);

  // Convolution
  for(i=0; i<n_new; i++) {
    for(j=0; j<mp; j++) {
      g(i,j) *= kernelg(i,j);
    }
  }

  // Backward FFT
  Backward.fftNormalized(g,f);

  // swap upper half with lower half
  for (i=0; i<n_new; i++) {
    for (j=0; j<m_new/2; j++) {
      tmp = f(i,j);
      f(i,j) = f(i,j+m_new/2);
      f(i,j+m_new/2) = tmp;
    }
  }
  
  // swap left half with right half
  for (j=0; j<m_new; j++) {
    for (i=0; i<n_new/2; i++) {
      tmp = f(i,j);
      f(i,j) = f(i+n_new/2,j);
      f(i+n_new/2,j) = tmp;
    }
  }

  // restore the old image geometry
  long w = 2 * (int) fwhm;
  for (j=0; j<m_new; j++) {
    for (i=0; i<n_new; i++) {
      if (j-w>=0 && j-w<m && i-w>=0 && i-w<n) {
	data[i-w+n*(j-w)] = f(i,j);
      }
    }
  }

  // interpolate subsampled sky map
}


//***************************************************************
// Cross-correlate two images
//***************************************************************
void xcorr(vector<float> &ref, vector<float> &com, vector<float> &result, 
	   int n, int m, int nthreads)
{
  int i, j;

  fftw::maxthreads = nthreads;

  int mp = m/2 + 1;
  size_t align = sizeof(Complex);

  array2<double> fa(n,m,align), fb(n,m,align);
  array2<Complex> ga(n,mp,align), gb(n,mp,align);

  rcfft2d Forward_a(m,fa,ga);
  rcfft2d Forward_b(m,fb,gb);
  crfft2d Backward(m,ga,fa);

  // write the data into arrays
  for(i=0; i<n; i++) {
    for(j=0; j<m; j++) {
      fa(i,j) = ref[i+n*j];
      fb(i,j) = com[i+n*j];
    }
  }
  
  // Forward FFT
  Forward_a.fft(fa,ga);
  Forward_b.fft(fb,gb);
  
  Complex scale = 1.0/(n*m);
  
  // cross-correlation
  for(i=0; i<n; i++) {
    for(j=0; j<mp; j++) {
      ga(i,j) *= conj(gb(i,j)) * scale;
    }
  }
  
  // Backward FFT
  Backward.fftNormalized(ga,fa);
  
  // put things back into a vector
  for(i=0; i<n; i++) {
    for(j=0; j<m; j++) {
      result[i+n*j] = fa(i,j);
    }
  }
  
  // swap upper half with lower half
  float tmp;
  for (i=0; i<n*m/2; i++) {
    tmp = result[i];
    result[i] = result[i+n*m/2];
    result[i+n*m/2] = tmp;
  }

  // the cross-correlation image is symmetric, hence we don't need 
  // to swap the left and right halves
}
