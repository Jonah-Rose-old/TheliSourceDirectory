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
  size_t nx, ny;
  double* z; // pointer to values of 2D function
  double* sigma; // dito for sigmas
};


//**********************************************************
int gauss_f(const gsl_vector* x, void* params, gsl_vector* f)
{
  size_t nx = ((struct data *) params)->nx;
  size_t ny = ((struct data *) params)->ny;
  double *z = ((struct data *) params)->z;
  double *sigma = ((struct data *) params)->sigma;
  size_t xi, yi;
  int indx;
  double Fxy;

  // extract parameters from x
  double b  = gsl_vector_get(x,0);
  double x0 = gsl_vector_get(x,1);
  double y0 = gsl_vector_get(x,2);
  double s  = gsl_vector_get(x,3);

  for(yi=0; yi<ny; yi++) {
    for(xi=0; xi<nx; xi++) {
      indx = xi + nx*yi; 
      // calculate Fxy based on parameters
      Fxy = b * exp(-0.5*( (xi-x0)*(xi-x0) + (yi-y0)*(yi-y0)) / (s*s));
      gsl_vector_set(f, indx, (Fxy - z[indx])/sigma[indx]);
    }
  }
  return GSL_SUCCESS;
}


//**********************************************************
int gauss_df (const gsl_vector *x, void *params, gsl_matrix *J)
{
  size_t nx = ((struct data *)params)->nx;
  size_t ny = ((struct data *)params)->ny;
  double *sigma = ((struct data *)params)->sigma;

  size_t xi, yi;
  int indx;
  double f0;
  
  double x0 = gsl_vector_get(x,1);
  double y0 = gsl_vector_get(x,2);
  double s  = gsl_vector_get(x,3);
  
  for(yi=0; yi<ny; yi++) {
    for(xi=0; xi<nx; xi++) {
      indx = xi + nx*yi; 
      f0 = exp(-0.5*( (xi-x0)*(xi-x0) + (yi-y0)*(yi-y0)) / (s*s));
      // The Jacobian matrix
      gsl_matrix_set (J, indx, 0, 1./sigma[indx] * f0); 
      gsl_matrix_set (J, indx, 1, 1./(s*s*sigma[indx]) * (xi - x0) * f0); 
      gsl_matrix_set (J, indx, 2, 1./(s*s*sigma[indx]) * (yi - y0) * f0);
      gsl_matrix_set (J, indx, 3, 1./(s*s*s*sigma[indx]) * (pow(xi-x0,2) + pow(yi-y0,2)) * f0);
    }
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
