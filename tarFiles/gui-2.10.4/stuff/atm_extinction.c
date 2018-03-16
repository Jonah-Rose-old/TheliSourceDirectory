#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
 
/*
 compiled with
 gcc atm_extinction.c -L. -lgsl -lm -O3 -o atm_extinction
*/

void usage(int i)
{
  if (i == 0) {
    fprintf(stderr,"\n");
    fprintf(stderr,"        atm_extinction  Version 1.0  (2012-02-26)\n\n");
    fprintf(stderr,"  Author: Mischa Schirmer\n\n");
    fprintf(stderr,"  USAGE:  atm_extinction \n");
    fprintf(stderr,"           -i input wavelength\n");
    fprintf(stderr,"           -a airmass\n");
    fprintf(stderr,"          [-z redshift (optionally redshift the wavelength)\n");
    fprintf(stderr,"  PURPOSE: Returns a correction factor for atmospheric extinction at the given wavelength.\n");
    fprintf(stderr,"           The redshift is just for convenience so that unredshifted\n");
    fprintf(stderr,"           wavelengths can be given without conversion.\n");

    exit(1);
  }
}


//*******************************************************************
int main(int argc, char *argv[])
{

  int dim = 77, i;
  double wavelength = 0., z = 0., airmass = 1.0;
  double ext_value;

  // the wavelengths at which we have atmospheric extinction
  double lambda[77] = {3325.0, 3375.0, 3425.0, 3475.0, 3525.0, 3575.0, 3625.0, 3675.0, 3725.0, 
		       3775.0, 3825.0, 3875.0, 3925.0, 3975.0, 4025.0, 4075.0, 4125.0, 4175.0, 
		       4225.0, 4275.0, 4325.0, 4375.0, 4425.0, 4475.0, 4525.0, 4575.0, 4625.0, 
		       4675.0, 4725.0, 4775.0, 4825.0, 4875.0, 4925.0, 4975.0, 5025.0, 5075.0, 
		       5125.0, 5175.0, 5225.0, 5275.0, 5325.0, 5375.0, 5425.0, 5475.0, 5525.0, 
		       5575.0, 5625.0, 5675.0, 5725.0, 5775.0, 5825.0, 5875.0, 5925.0, 5975.0, 
		       6025.0, 6075.0, 6125.0, 6175.0, 6225.0, 6275.0, 6325.0, 6375.0, 6425.0, 
		       6475.0, 6525.0, 6575.0, 6625.0, 6675.0, 6725.0, 6775.0, 7060.0, 7450.0, 
		       7940.0, 8500.0, 8675.0, 8850.0, 10000.0};
  
  // mean Paranal extinction at airmass 1.0
  double ext[77] = {0.686, 0.606, 0.581, 0.552, 0.526, 0.504, 0.478, 0.456, 0.430, 0.409, 
		    0.386, 0.378, 0.363, 0.345, 0.330, 0.316, 0.298, 0.285, 0.274, 0.265, 
		    0.253, 0.241, 0.229, 0.221, 0.212, 0.204, 0.198, 0.190, 0.185, 0.182, 
		    0.176, 0.169, 0.162, 0.157, 0.156, 0.153, 0.146, 0.143, 0.141, 0.139, 
		    0.139, 0.134, 0.133, 0.131, 0.129, 0.127, 0.128, 0.130, 0.134, 0.132, 
		    0.124, 0.122, 0.125, 0.122, 0.117, 0.115, 0.108, 0.104, 0.102, 0.099, 
		    0.095, 0.092, 0.085, 0.086, 0.083, 0.081, 0.076, 0.072, 0.068, 0.064, 
		    0.064, 0.048, 0.042, 0.032, 0.030, 0.029, 0.022};

  // print usage if no arguments were given

  if (argc==1) usage(0);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': wavelength = atof(argv[++i]);
	break;
      case 'z': z = atof(argv[++i]);
	break;
      case 'a': airmass = atof(argv[++i]);
	break;
      }
    }
  }

  // create interpolating function for extinction 
  gsl_interp_accel *acc = gsl_interp_accel_alloc();
  gsl_spline *spline = gsl_spline_alloc(gsl_interp_linear, dim);
  gsl_spline_init(spline, lambda, ext, dim);

  // extinction at airmass 1
  ext_value = gsl_spline_eval(spline, wavelength*(1.+z), acc);

  // extinction at airmass != 1
  ext_value = 1. - exp( log( 1. - ext_value) * airmass);

  // print flux correction factor
  printf("%f\n", 1. / (1. - ext_value));

  gsl_spline_free(spline);
  gsl_interp_accel_free(acc);

  exit (0);
}
