#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
 
/*
 compiled with
 gcc extinction.c -L. -leclipse -lqfits -lgsl -lm -O3 -o extinction
*/

void usage(int i)
{
  if (i == 0) {
    fprintf(stderr,"\n");
    fprintf(stderr,"        extinction  Version 1.1  (2011-11-07)\n\n");
    fprintf(stderr,"  Author: Mischa Schirmer\n\n");
    fprintf(stderr,"  USAGE:  extinction \n");
    fprintf(stderr,"           -i input wavelength\n");
    fprintf(stderr,"           -r (3.1 or 5.5 or SMC extinction curve)\n");
    fprintf(stderr,"          [-z redshift (optionally redshift the wavelength)\n");
    fprintf(stderr,"          [-e E(B-V)\n");
    fprintf(stderr,"          [-s I_1_err I_2_err\n");
    fprintf(stderr,"          [-f I_1 I_2 I_01/I_02 lambda_1 lambda_2 (return E(B-V))\n\n");
    fprintf(stderr,"  PURPOSE: Returns the extinction curve f(lambda) at the given wavelength.\n");
    fprintf(stderr,"           If E(B-V) is given as well, it will return the direct flux\n");
    fprintf(stderr,"           correction factor for that (redshifted) wavelength.\n");
    fprintf(stderr,"           The redshift is just for convenience so that unredshifted\n");
    fprintf(stderr,"           wavelengths can be given without conversion.\n");
    fprintf(stderr,"           If option -f is given (together with observed wavelength\n");
    fprintf(stderr,"           intensities I_1 and I_2, theoretical intensity ratio, and the two wavelengths),\n");
    fprintf(stderr,"           then E(B-V) is returned (a dummy wavelength has to be given with -i).\n");
    fprintf(stderr,"           Options -e and -f cannot be specified at the same time.\n\n");

    exit(1);
  }
}


//*******************************************************************
int main(int argc, char *argv[])
{

  int dim = 32, i, ebv_flag = 0, balmerdec = 0;
  double wavelength = 0., z = 0., ebv = 0., fl, *f_lambda;
  double extcorrfac, fb, fv;
  double i_l1, i_l2, iratio, l1, l2, fl1, fl2, i_l1_err, i_l2_err;
  char R[4096];

  // the wavelengths at which we have extinction
  double lambda[32] = {1216, 1250, 1429, 1538, 1549, 1667, 1818, 1909, 
		       2083, 2273, 2381, 2631, 2780, 2941, 3333, 3571, 
		       3846, 4167, 4340, 4545, 4860, 5000, 5555, 6250, 
		       6565, 7143, 8333, 10000, 12500, 18700, 20000, 33333};
  
  // extinction curves f(lambda) (Osterbrock 2005, Table 7.1, for R=3.1, R=5.5, SMC)
  double f_lambda_31[32]  = {3.503, 3.337, 2.780, 2.617, 2.606, 2.522, 2.529, 2.630, 
			     3.091, 2.953, 2.601, 2.113, 1.949, 1.851, 1.643, 1.575, 
			     1.509, 1.409, 1.346, 1.271, 1.164, 1.122, 0.987, 0.866, 
			     0.818, 0.728, 0.552, 0.404, 0.282, 0.147, 0.132, 0.058};

  double f_lambda_55[32]  = {1.448, 1.413, 1.329, 1.323, 1.323, 1.337, 1.388, 1.456,
			     1.718, 1.713, 1.571, 1.390, 1.341, 1.316, 1.304, 1.301, 
			     1.276, 1.226, 1.194, 1.156, 1.099, 1.076, 0.991, 0.898, 
			     0.858, 0.784, 0.639, 0.478, 0.334, 0.174, 0.157, 0.069};

  double f_lambda_SMC[32] = {6.857, 6.586, 5.376, 4.822, 4.775, 4.299, 3.806, 3.559, 
			     3.161, 2.816, 2.649, 2.327, 2.152, 2.020, 1.726, 1.584, 
			     1.446, 1.310, 1.246, 1.178, 1.085, 1.049, 0.923, 0.799, 
			     0.753, 0.679, 0.561, 0.446, 0.334, 0.188, 0.170, 0.064};
  
  strcpy(R,"");

  i_l1   = 0.;
  i_l2   = 0.;
  iratio = 0.;
  l1     = 0.;
  l2     = 0.;
  i_l1_err = 0.;
  i_l2_err = 0.;

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
      case 'r': strcpy(R,argv[++i]);
	break;
      case 'f': 
	i_l1   = atof(argv[++i]);
	i_l2   = atof(argv[++i]);
	iratio = atof(argv[++i]);
	l1     = atof(argv[++i]);
	l2     = atof(argv[++i]);
	balmerdec = 1;
	break;
      case 's': 
	i_l1_err = atof(argv[++i]);
	i_l2_err = atof(argv[++i]);
	break;
      }
    }
  }

  if      (strcmp(R,"3.1") == 0) f_lambda = f_lambda_31;
  else if (strcmp(R,"5.5") == 0) f_lambda = f_lambda_55;
  else if (strcmp(R,"SMC") == 0) f_lambda = f_lambda_SMC;
  else if (strcmp(R,"") == 0) {
    fprintf(stderr, "ERROR: You must select an extinction model (option -r)!\n");
    exit (1);
  }
  else {
    fprintf(stderr, "ERROR: Invalid extinction model (option -r)!\n");
    fprintf(stderr, "       Must be 3.1, 5.5 or SMC!\n");
    exit (1);
  }

  if (balmerdec == 1 && ebv_flag == 1) {
    fprintf(stderr, "ERROR: You cannot specify both options 'e' and 'f' at the same time!\n");
    exit (1);
  }

  // create interpolating function for extinction 
  gsl_interp_accel *acc = gsl_interp_accel_alloc();
  gsl_spline *spline = gsl_spline_alloc(gsl_interp_linear, dim);
  gsl_spline_init(spline, lambda, f_lambda, dim);

  /*
    extinction correction factor:
    exp[ E(B-V) ln(10) / 2.5 f(lambda) / ( f(B) - f(V) ) ] 
    derived from Osterbrock 2005, chapter 7.2, correcting a sign error:
    A(l) = -2.5 log(I/I0)
  */

  fl = gsl_spline_eval(spline, wavelength*(1.+z), acc);
  fb = gsl_spline_eval(spline, 4440., acc);
  fv = gsl_spline_eval(spline, 5500., acc);
  extcorrfac = exp(ebv * log(10.) / 2.5 * fl / (fb - fv));

  if (ebv_flag == 0 && balmerdec == 0) printf("%lf\n", fl);
  if (ebv_flag == 1) printf("%lf\n", extcorrfac); 

  if (balmerdec == 1) {
    float ebv, ebv_err;
    fl1 = gsl_spline_eval(spline, l1, acc);
    fl2 = gsl_spline_eval(spline, l2, acc);
    ebv = -2.5 / log(10.) * log( (i_l1 / i_l2) / iratio ) * (fb-fv) / (fl1-fl2);
    ebv_err = 2.5 / log(10.) * fabs((fb-fv) / (fl1-fl2)) * 
      sqrt( pow(i_l1_err/i_l1, 2) + pow(i_l2_err/i_l2, 2));
    printf("%lf %lf\n", ebv, ebv_err);
  }

  gsl_spline_free(spline);
  gsl_interp_accel_free(acc);

  exit (0);
}
