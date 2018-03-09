#include <iostream>
#include <cstdlib> 
#include <string>  
#include <cstring> 
#include <cstdio>  
#include <cmath>   
#include <sstream> 
#include <fstream> 
#include <vector>
#include <algorithm>
#include "statistics.h"

using namespace std;


//***************************************************************
// Collapse a 2D array; calculate median row/columns
//***************************************************************
void collapse(vector<float> const &data, vector<float> &result, string direction, 
	      long n, long m, string mode, string norm)
{
  long i, j, u;
  vector<float> row(n,0);
  vector<float> col(m,0);

  // collapse along x-axis
  if (direction.compare("x") == 0) {
    // extract rows, median combination
    for (j=0; j<m; j++)  {
      u = 0;
      for (i=0; i<n; i++) row[u++] = data[i+n*j];
      if (mode.compare("median") == 0) col[j] = median(row, n);
      else col[j] = mean(row, n);
    }
    // write collapsed FITS array
    for (i=0; i<n; i++)  {
      for (j=0; j<m; j++)  {
	result[i+n*j] = col[j];
      }
    } 
  }
  
  // collapse along y-axis
  if (direction.compare("y") == 0) {
    // extract columns, median combination
    for (i=0; i<n; i++)  {
      u = 0;
      for (j=0; j<m; j++) col[u++] = data[i+n*j];
      if (mode.compare("median") == 0) row[i] = median(col, m);
      else row[i] = mean(col, m);
    }
    // write collapsed FITS array
    for (j=0; j<m; j++)  {
      for (i=0; i<n; i++)  {
	result[i+n*j] = row[i];
      }
    }
  }

  // normalise the collapsed array
  if (norm.compare("normalise") == 0) {
    float maxval = max(result, n*m);
    float minval = min(result, n*m);
    float diff = maxval - minval;
    for (j=0; j<m; j++)  {
      for (i=0; i<n; i++)  {
	result[i+n*j] = (result[i+n*j] - minval) / diff + 1.;
      }
    }
  }
}


//***************************************************************
// Length of quasi-random index list (subsample of an array)
//***************************************************************
long get_array_subsample_size(long dim)
{
  long l;

  // define some odd number
  // then select every l-th pixel in the array
  // that is always smaller than n for n > 9
  // the sqrt ensures that the array is probed in a quasi-random pattern
  l = (long) (2/3*dim + sqrt(dim));

  return dim/l - 1;  // In rare occasions (to be verified) we hit the end of the array, hence the -1
}


//***************************************************************
// ds9-style z-scale
//***************************************************************
// ZZZ has to be verified on image!
void get_zscale(vector<float> &data, int n, int m, vector<float> &zscale, 
		float shrink, float contrast)
{
  long dim_small, dim = n*m, i;
  vector<float> fit(2,0);
  float min, max;
  
  dim_small = get_array_subsample_size(dim);
  vector<float> sample(dim_small,0);
  get_array_subsample(data, sample, dim, dim_small);

  // sort the array
  sort(sample.begin(), sample.end());
  
  min = sample[0];
  max = sample[dim_small-1];
  
  // the linear fit
  linfit(sample, dim_small, fit);
  // replace the data by the fit
  for (i=0; i<dim_small; i++) {
    sample[i] = fit[1] + fit[0] * ((float) (i - dim_small/2));
  }
  
  zscale[0] = sample[dim_small/2] + fit[0] / contrast * (float) (1 - dim_small/2);
  zscale[1] = sample[dim_small/2] + fit[0] / contrast * (float) (dim_small/2);
  
  // don't let the range get larger / smaller than the actual data extremes
  if (zscale[0] < min) zscale[0] = min;
  if (zscale[1] > max) zscale[1] = max;
  
  // shrink the dynamic range if requested (increase the lower threshold)
  if (shrink != 0.) {
    zscale[0] = zscale[0] + shrink * (zscale[1] - zscale[0]);
  }
}

//************************************************************
// Statistics to assigns some significance to the occurrence 
// of a particular numeric value in a FITS image
//************************************************************
double logbinomial(double n, double k, double p)
{
  double r, xi, num, denom;
  long i;

  num = 0.;
  denom = 0.;

  for (i=0; i<=k-1; i++) {
    xi = (double) i;
    num += log(n-xi);
  }
  num += k * log(p) + (n-k) * log(1.-p);

  for (i=0; i<k; i++) {
    xi = (double) i;
    denom += log(k-xi);
  }
  
  r = num - denom;

  return (r);
}


//************************************************************
// Create some global statistics (based on sparse sampling)
// trying to identify a blank value (same float number occurring 
// multiple times)
//************************************************************
float find_blankvalue(vector<float> &data, long n, long m, string &result)
{
  long dim_small, dim, i, j, l, indexmax;
  float maxvalue;
  double p;
  
  // define some odd number
  // then select every l-th pixel in the array
  // that is always smaller than n for n > 9
  // the sqrt ensures that the array is probed in a quasi-random pattern
  // Otherwise this will run forever
  l = 3*n/2 + sqrt(n);

  // the number of elements we test
  dim = n * m;
  dim_small = dim / l;
  
  vector<float> sample(dim_small,0);
  vector<float> common(dim_small,0);
  
  // select the sample array
  get_array_subsample(data, sample, dim, dim_small);
  
  // find the most common value
  for (i=0; i<dim_small; i++) {
    for (j=0; j<dim_small; j++) {
      if (sample[j] == sample[i]) common[i]++;
    }
  }
  
  maxvalue = max(common, dim_small);
  indexmax = maxindex(common, dim_small);

  // We consider the result safe if it is a 5 sigma statistical detection
  // Number of pixels in the array: dim_small
  // Number of digits in a floating-point number: 6
  // Probability that two numbers are the same: 1.e-6
  // Limit of 1 - ((n-1)/n)^n for n->infty = 1-exp(-1) = 0.63
  
  // Probability of having maxvalue identical values 
  // in a sample of dim_small floating point numbers:
  // we don't need to consider any higher terms as the first one is 
  // by far dominating

  p = logbinomial((double) dim_small, (double) maxvalue, 1.e-6);

  // log(5 sigma propability) = log(5.7e-7) = -14.4

  if (p < -14.4) {
    result = "OK";
    return sample[indexmax];
  }
  else {
    result = "UNKNOWN";
    return -1.e12;
  }
}
