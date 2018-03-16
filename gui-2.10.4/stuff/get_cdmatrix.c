#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "stuff.h"

// compiled with

// gcc get_cdmatrix.c -o get_cdmatrix -L. -lm

void usage(int i)
{
  if (i == 0) {
      fprintf(stderr,"\n");
      fprintf(stderr,"Get the CD matrix from the pixel scale, position angle and flipping\n");
      fprintf(stderr,"Usage:\n\n"); 
      fprintf(stderr, "      get_cdmatrix \n");
      fprintf(stderr, "          -s scale [arcsec]\n");
      fprintf(stderr, "         [-p posangle (degrees, counting North over East; defaulted to: 0)]\n");
      fprintf(stderr, "         [-f (flipped)]\n");
      fprintf(stderr, "         [-n (print CD11, CD12, CD21, CD22 in one line without comments)]\n\n");
  }

  exit (0);
}

//****************************************************************

int main(int argc, char *argv[])
{
  int style;
  long i;
  double cd11, cd12, cd21, cd22, flip;
  double pscale, pa;
  double rad = PI / 180.;
  double f11, f12, f21, f22;

  // print usage if no arguments were given

  if (argc==1) usage(0);

  //  Read command line arguments

  flip = 0;
  style = 0;
  pscale = 0.;
  pa = 0.;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 's': pscale = atof(argv[++i]) / 3600.;
	break;
      case 'p': pa = atof(argv[++i]) * rad;
	break;
      case 'f': flip = 1;
	break;
      case 'n': style = 1;
	break;
      }
    }
  }

  // CD_NEW = FLIP x ROT x CD_OLD

  if (pscale == 0.) {
    fprintf(stderr, "ERROR: Pixel scale not defined or zero!\n");
    exit (1);
  }

  // start with posangle = 0, no flip
  cd11 = -pscale;
  cd12 = 0.;
  cd21 = 0.;
  cd22 = pscale;

  // rotate
  matrix_mult(cos(pa), -sin(pa), sin(pa), cos(pa), &cd11, &cd12, &cd21, &cd22);

  // flip, if necessary
  if (flip == 1) {
    f11 = -1.;
    f12 = 0.;
    f21 = 0.;
    f22 = 1.;
    matrix_mult(f11, f12, f21, f22, &cd11, &cd12, &cd21, &cd22);
  }

  if (style == 0) {
    printf("CD1_1   = %3.10g\n", cd11);
    printf("CD1_2   = %3.10g\n", cd12);
    printf("CD2_1   = %3.10g\n", cd21);
    printf("CD2_2   = %3.10g\n", cd22);
  }
  else 
    printf("%3.10g %3.10g %3.10g %3.10g\n", cd11, cd12, cd21, cd22);

  exit(0);
}
