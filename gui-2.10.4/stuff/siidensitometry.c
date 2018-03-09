#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
 
/*
 compiled with
 gcc siidensitometry.c -L. -lgsl -lgslcblas -lm -O3 -o siidensitometry
*/

void usage(int i)
{
  if (i == 0) {
    fprintf(stderr,"\n");
    fprintf(stderr,"        siidensitometry  Version 1.0  (2012-02-14)\n\n");
    fprintf(stderr,"  Author: Mischa Schirmer\n\n");
    fprintf(stderr,"  USAGE:  fitssiidensitometry \n");
    fprintf(stderr,"           -i line ratio\n");
    fprintf(stderr,"          [-t electron temperature (default: 1e4 K)]\n");
    fprintf(stderr,"\n    Calculates electron density\n\n");
    exit(1);
  }
}


int main(int argc, char *argv[])
{
  long i, dim;
  float temperature, ratio = 0.;

  double model_ratio[] = {0.44, 0.47, 0.5, 0.57, 0.62, 0.7, 0.84, 1., 
			  1.14, 1.22, 1.31, 1.34, 1.4, 1.41, 1.42, 1.423, 
			  1.425, 1.427, 1.429, 1.431, 1.433, 1.435};

  double model_dens[] = {100000., 20000., 10000., 5000., 3000., 2000., 
			 1000., 500., 300., 200., 100., 70., 30., 20., 
			 10., 7., 6., 5., 4., 3., 2., 1.};

  dim = 22; // number of data points

  // print usage if no arguments were given
  if (argc==1) usage(0);

  temperature = 1.e4;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': ratio = atof(argv[++i]);
	break;
      case 't': temperature = atof(argv[++i]);
	break;
      }
    }
  }

  // rescale the density with the temperature dependence: sqrt(1e4/T)
  for (i=0; i<dim; i++) {
    model_dens[i] *= sqrt(1.e4/temperature);
  }

  // create interpolating function
  gsl_interp_accel *acc = gsl_interp_accel_alloc();
  gsl_spline *spline = gsl_spline_alloc(gsl_interp_cspline, dim);
  gsl_spline_init(spline, model_ratio, model_dens, dim);

  // calculate density image
  // we have to keep data points within the interpolated range
  if (ratio > 1.43) ratio = 1.43;
  if (ratio < 0.45 && ratio != 0.) ratio = 0.45;
  if (ratio > 0.)
    printf("%f\n", gsl_spline_eval(spline, ratio, acc));

  gsl_spline_free (spline);
  gsl_interp_accel_free (acc);
  
  exit (0);
}
