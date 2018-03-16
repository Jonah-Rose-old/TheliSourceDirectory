#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
 
/*
 compiled with
 gcc spec_filter_convolve.c -L. -leclipse -lqfits -lgsl -lm -O3 -o spec_filter_convolve
*/

void usage(int i)
{
  if (i == 0) {
    fprintf(stderr,"\n");
    fprintf(stderr,"        balmerdecrement  Version 1.0  (2011-10-29)\n\n");
    fprintf(stderr,"  Author: Mischa Schirmer\n\n");
    fprintf(stderr,"  USAGE:  balmerdecrement \n");
    fprintf(stderr,"           -i input wavelength\n\n");
    fprintf(stderr,"          [-z redshift (optionally redshift the wavelength)\n\n");
    fprintf(stderr,"          [-e E(B-V)\n\n");
    fprintf(stderr,"  PURPOSE: Returns the extinction curve f(lambda) at the given wavelength.\n");
    fprintf(stderr,"           If E(B-V) is given as well, it will return the direct flux\n");
    fprintf(stderr,"           correction factor for that (redshifted) wavelength.\n");
    exit(1);
  }
}


//*******************************************************************
int main(int argc, char *argv[])
{

  int dim = 32, i, ebv_flag = 0;
  double wavelength = 0., z = 0., ebv = 0., fl;

  // the wavelengths at which we have extinction
  double lambda[32] = {1216, 1250, 1429, 1538, 1549, 1667, 1818, 1909, 
		       2083, 2273, 2381, 2631, 2780, 2941, 3333, 3571, 
		       3846, 4167, 4340, 4545, 4860, 5000, 5555, 6250, 
		       6565, 7143, 8333, 10000, 12500, 18700, 20000, 33333};
  
  // extinction curve f(lambda) (Osterbrock 2005, Table 7.1, for R=3.1)
  double f_lambda[32] = {3.503, 3.337, 2.780, 2.617, 2.606, 2.522, 2.529, 2.630, 
			 3.091, 2.953, 2.601, 2.113, 1.949, 1.851, 1.643, 1.575, 
			 1.509, 1.409, 1.346, 1.271, 1.164, 1.122, 0.987, 0.866, 
			 0.818, 0.728, 0.552, 0.404, 0.282, 0.147, 0.132, 0.058};
  
  // print usage if no arguments were given

  if (argc==1) usage(0);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': wavelength = atof(argv[++i]);
	  break;
      case 'z': z = atof(argv[++i]);
	  break;
      case 'e': ebv = atof(argv[++i]);
	ebv_flag = 1;
	  break;
      }
    }
  }

  // create interpolating function for extinction 
  gsl_interp_accel *acc = gsl_interp_accel_alloc();
  gsl_spline *spline = gsl_spline_alloc(gsl_interp_linear, dim);
  gsl_spline_init(spline, lambda, f_lambda, dim);

  fl = gsl_spline_eval(spline, wavelength*(1.+z), acc);

  if (ebv_flag == 0) printf("%f\n", fl);
  if (ebv_flag == 1) printf("%f\n", pow(10., 1.2987 * ebv * fl)); //Osterbrock 2005, chapter 7.2

  gsl_spline_free(spline);
  gsl_interp_accel_free(acc);

  exit (0);
}
