#ifndef PI
#define PI 3.1415926535
#endif

#ifndef __CONVOLVE_H
#define __CONVOLVE_H

#include <typeinfo>
#include <cstring>
#include <limits>

using namespace std;

//**********************************************************************************
// Add a column to the right, and/or a row on top to make image dimensions even
//**********************************************************************************
void make_image_even(vector<float> &data_even, vector<float> &data, long n, long m, long *n_new, long *m_new);

//**********************************************************************************
// Padd the image to avoid aliasing
//**********************************************************************************
void make_image_even2(vector<float> &data_even, vector<float> &data, float fwhm, long n, long m, 
		      long &n_new, long &m_new, int subsample=1);

//***************************************************************
// set/get the pwd of the wisdom file to/from ${HOME}/.fftw/
//***************************************************************
void get_path_to_wisdom(char *path_to_wisdom);

//***************************************************************
// Convolve a 2D array with a Gaussian
//***************************************************************
void convolve_gauss_2d(vector<float> &data, long n, long m, float fwhm, int nthreads);

//***************************************************************
// Cross-correlate two images
//***************************************************************
void xcorr(vector<float> &ref, vector<float> &com, vector<float> &result, 
	   int n, int m, int nthreads);

#endif
