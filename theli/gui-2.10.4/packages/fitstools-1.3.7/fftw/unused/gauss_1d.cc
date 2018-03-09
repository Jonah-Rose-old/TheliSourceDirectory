#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <typeinfo>
#include <sstream> 
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>

struct data {
  size_t n;
  double* y; // pointer to values of 1D function
  double* sigma; // dito for sigmas
};


//**********************************************************
int gauss_f(const gsl_vector* x, void* params, gsl_vector* f)
{
  size_t n = ((struct data *) params)->n;
  double *y = ((struct data *) params)->y;
  double *sigma = ((struct data *) params)->sigma;
  size_t xi;
  int indx;
  double Fx;

  // extract parameters from x
  double b  = gsl_vector_get(x,0);
  double x0 = gsl_vector_get(x,1);
  double s  = gsl_vector_get(x,2);
  
  for(xi=0; xi<n; xi++) {
    indx = xi;
    // calculate Fxy based on parameters
    // A Gaussian with PEDESTIAL of 1.0!!!
    Fx = 1. + b * exp(-0.5*( (xi-x0)*(xi-x0)) / (s*s));
    gsl_vector_set(f, indx, (Fx - y[indx])/sigma[indx]);
  }
  return GSL_SUCCESS;
}


//**********************************************************
int gauss_df (const gsl_vector *x, void *params, gsl_matrix *J)
{
  size_t n = ((struct data *)params)->n;
  double *sigma = ((struct data *)params)->sigma;

  size_t xi;
  double f0;

  double b  = gsl_vector_get(x,0);
  double x0 = gsl_vector_get(x,1);
  double s  = gsl_vector_get(x,2);
  
  for(xi=0; xi<n; xi++) {
    f0 = exp(-0.5*( (xi-x0)*(xi-x0)) / (s*s));
    // The Jacobian matrix
    gsl_matrix_set (J, xi, 0, 1./sigma[xi] * f0); 
    gsl_matrix_set (J, xi, 1, b*1./(s*s*sigma[xi]) * (xi - x0) * f0); 
    gsl_matrix_set (J, xi, 2, b*1./(s*s*s*sigma[xi]) * pow(xi-x0,2) * f0);
  }
  return GSL_SUCCESS;
}

//**********************************************************
int gauss_fdf (const gsl_vector *x, void *params,
	       gsl_vector *f, gsl_matrix *J)
{
  gauss_f  (x, params, f);
  gauss_df (x, params, J);
  
  return GSL_SUCCESS;
}
