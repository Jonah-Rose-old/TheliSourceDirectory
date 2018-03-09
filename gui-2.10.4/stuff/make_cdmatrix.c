#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// compiled with
// gcc make_cdmatrix.c -L. -lm -o make_cdmatrix

void matrix_mult(double, double, double, double, double*, double*, double*, double*);

void usage(int i)
{
  if (i == 0) {
      fprintf(stderr,"\n");
      fprintf(stderr,"VERSION: 2012-02-29, Author: Mischa Schirmer\n\n");
      fprintf(stderr,"PURPOSE: Makes a CD matrix\n\n");
      fprintf(stderr,"USAGE: make_cdmatrix \n");
      fprintf(stderr,"          -s pixel scale [in arcsec]\n");
      fprintf(stderr,"          -p position angle\n");
      fprintf(stderr,"         [-f (optional; provide if matrix should be flipped)\n");
      fprintf(stderr,"      Outputs a CD matrix.\n\n");
  }

  exit (0);
}

//**************************************************************

int main(int argc, char *argv[])
{
  int i;
  double cd11, cd12, cd21, cd22;
  double f11, f12, f21, f22;
  double pixscale, pa;

  // flip matrix elements
  f11 = 1.;
  f12 = 0.;
  f21 = 0.;
  f22 = 1.;
  pa  = 0.;
  pixscale = 0.;

  // print usage if no arguments were given
  if (argc==1) usage(0);

  //  Read command line arguments
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 's': pixscale = atof(argv[++i]) / 3600.;
	break;
      case 'p': pa = atof(argv[++i]) * 3.14159265 / 180.;
	break;
      case 'f': f11 = -1.;
	break;
      }
    }
  }

  // the unrotated CD matrix, North up East left:
  cd11 = -pixscale;
  cd12 = 0.0;
  cd21 = 0.0;
  cd22 = pixscale;

  // rotate it to the required PA
  matrix_mult(cos(pa), -sin(pa), sin(pa), cos(pa), &cd11, &cd12, &cd21, &cd22);

  // flip the matrix (no effect if option -f is not given)
  matrix_mult(f11, f12, f21, f22, &cd11, &cd12, &cd21, &cd22);

  printf("CD1_1   = %3.10g\n", cd11);
  printf("CD1_2   = %3.10g\n", cd12);
  printf("CD2_1   = %3.10g\n", cd21);
  printf("CD2_2   = %3.10g\n", cd22);

  return (0);
}


//********************************************************
void matrix_mult(double a11, double a12, double a21, double a22, 
		 double *b11_ptr, double *b12_ptr, double *b21_ptr, 
		 double *b22_ptr)
{
  double c11, c12, c21, c22;

  c11 = a11 * *b11_ptr + a12 * *b21_ptr;
  c12 = a11 * *b12_ptr + a12 * *b22_ptr;
  c21 = a21 * *b11_ptr + a22 * *b21_ptr;
  c22 = a21 * *b12_ptr + a22 * *b22_ptr;

  *b11_ptr = c11;
  *b12_ptr = c12;
  *b21_ptr = c21;
  *b22_ptr = c22;
}
