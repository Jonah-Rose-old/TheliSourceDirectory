#include <iostream>
#include <cstdlib>
#include <string>
#include <cstring>
#include <cstdio>
#include <vector>
#include <cmath>
#include <typeinfo>
#include <sstream> 
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multifit_nlin.h>


using namespace std;

//*******************************************
// this code fits a 1D gaussian to some data:
// f(x) = b exp(-0.5(x-x0)^2/s^2)
//*******************************************


struct fitdata {
  size_t n;
  double* x;
  double* y;
  double* error;
};


//*******************************************************************
int gauss1d_f(const gsl_vector *x, void *params, gsl_vector *f)
{  
  // fit parameters
  const double b     = gsl_vector_get(x, 0);
  const double x0    = gsl_vector_get(x, 1);
  const double sigma = gsl_vector_get(x, 2);

  // fit data
  size_t n = ((struct fitdata *)params)->n;
  double *xdata = ((struct fitdata *)params)->x;
  double *ydata = ((struct fitdata *)params)->y;
  double *error = ((struct fitdata *)params)->error;
  
  for (size_t i=0; i<n; i++) {
    double fx = b * exp(-0.5*( pow(xdata[i]-x0, 2)) / (sigma*sigma));
    gsl_vector_set(f, i, (ydata[i] - fx) / error[i]);
  }
  return GSL_SUCCESS;
}


//*****************************************************************
int gauss1d_df (const gsl_vector *x, void *params, gsl_matrix *J)
{
  // fit parameters
  double b     = gsl_vector_get (x, 0);
  double x0    = gsl_vector_get (x, 1);
  double sigma = gsl_vector_get (x, 2);

  // fit data
  size_t n = ((struct fitdata *)params)->n;
  double *xdata = ((struct fitdata *)params)->x;
  double *error = ((struct fitdata *)params)->error;

  for (size_t i=0; i<n; i++) {
    double f0 = exp(-0.5*pow(xdata[i]-x0, 2) / (sigma*sigma));
    double df_db  = f0;
    double df_dx0 = b * f0 * pow(xdata[i]-x0,1) / pow(sigma, 2);
    double df_ds  = b * f0 * pow(xdata[i]-x0,2) / pow(sigma, 3);
    gsl_matrix_set (J, i, 0, df_db  / error[i]);
    gsl_matrix_set (J, i, 1, df_dx0 / error[i]);
    gsl_matrix_set (J, i, 2, df_ds  / error[i]);
  }
  return GSL_SUCCESS;
}


//**********************************************************
int gauss1d_fdf (const gsl_vector *x, void *params,
		 gsl_vector *f, gsl_matrix *J)
{
  gauss1d_f  (x, params, f);
  gauss1d_df (x, params, J);
  
  return GSL_SUCCESS;
}


//**********************************************************
// This program uses a Levenberg-Markquardt solver
//**********************************************************
void do_gauss1d_fit(const vector<float> &datax, const vector<float> &datay,
		    const float p1guess, const float p2guess, const float p3guess,
		    vector<float> &fitresult, const bool verbose)
{
  const gsl_multifit_fdfsolver_type *T = gsl_multifit_fdfsolver_lmsder;
  gsl_multifit_fdfsolver *s;
  int status, info;
  size_t i;
  const size_t n = datax.size();                  // Number of data points
  const size_t p = 3;                             // Number of fit parameters

  gsl_matrix *J = gsl_matrix_alloc(n, p);         // Jacobian matrix
  gsl_matrix *covar = gsl_matrix_alloc (p, p);    // covariance matrix

  // The arrays holding the data (GSL doesn't know anything about vectors)
  double *x = new double[n];
  double *y = new double[n];
  double *error = new double[n];
  double *weights = new double[n];

  // Copy the data
  for (i=0; i<n; i++) {
    x[i] = datax[i];
    y[i] = datay[i];
    error[i] = 1.;  // same weight for all data points
    weights[i] = 1./pow(error[i],2);
  }

  struct fitdata d = { n, x, y, weights };
  gsl_multifit_function_fdf f;
  double x_init[3] = {p1guess, p2guess, p3guess};  // Initial parameter starting values
  gsl_vector_view xvec = gsl_vector_view_array (x_init, p);
  gsl_vector_view wvec = gsl_vector_view_array(weights, n);
  const gsl_rng_type * type;
  gsl_rng * r;
  gsl_vector *res_f;
  double chi, chi0;

  const double xtol = 1e-8;
  const double gtol = 1e-8;
  const double ftol = 0.0;

  gsl_rng_env_setup();

  type = gsl_rng_default;
  r = gsl_rng_alloc (type);

  f.f = &gauss1d_f;
  f.df = &gauss1d_df;
  f.n = n;
  f.p = p;
  f.params = &d;

  s = gsl_multifit_fdfsolver_alloc (T, n, p);

  // initialize solver with starting point and weights
  gsl_multifit_fdfsolver_wset (s, &f, &xvec.vector, &wvec.vector);

  // compute initial residual norm
  res_f = gsl_multifit_fdfsolver_residual(s);
  chi0 = gsl_blas_dnrm2(res_f);

  // solve the system with a maximum of 20 iterations
  status = gsl_multifit_fdfsolver_driver(s, 20, xtol, gtol, ftol, &info);

  gsl_multifit_fdfsolver_jac(s, J);
  gsl_multifit_covar (J, 0.0, covar);

  // compute final residual norm
  chi = gsl_blas_dnrm2(res_f);

  #define FIT(i) gsl_vector_get(s->x, i)
  #define ERR(i) sqrt(gsl_matrix_get(covar,i,i))

  if (verbose) { 
    cerr << "Number of iterations: " << gsl_multifit_fdfsolver_niter(s) << endl;
    string reason = (info == 1) ? "small step size" : "small gradient";
    cerr << "reason for stopping: " << reason << endl;
    cerr << "initial |f(x)| = " << chi0 << endl;
    cerr << "final   |f(x)| = " << chi << endl;

    double dof = n - p;
    double c = GSL_MAX_DBL(1, chi / sqrt(dof)); 
    //    cerr << "chisq/dof = " << pow(chi, 2.0) / dof << endl;
    cerr << "b      = " << FIT(0) <<  "+/-" << c*ERR(0) << endl;
    cerr << "x0     = " << FIT(1) <<  "+/-" << c*ERR(1) << endl;
    cerr << "sigma  = " << FIT(2) <<  "+/-" << c*ERR(2) << endl;
    cerr << "Status = " << gsl_strerror (status) << endl;
  }

  fitresult[0] = FIT(0);
  fitresult[1] = FIT(1);
  fitresult[2] = FIT(2);

  gsl_multifit_fdfsolver_free (s);
  gsl_matrix_free (covar);
  gsl_matrix_free (J);
  gsl_rng_free (r);

  delete [] x;
  delete [] y;
  delete [] weights;
  delete [] error;
}
