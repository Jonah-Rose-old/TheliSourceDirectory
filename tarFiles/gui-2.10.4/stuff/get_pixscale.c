#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// compiled with

// gcc get_pixscale.c -o get_pixscale -L. -lm


float min(float *, long);
float max(float *, long);

void usage(int i)
{
  if (i == 0) {
      fprintf(stderr,"\n");
      fprintf(stderr,"Get the pixelscale (in arcsec) from the CD matrix\n");
      fprintf(stderr,"Usage:\n\n"); 
      fprintf(stderr, "      get_pixscale -c cd11 cd21\n\n");
  }

  exit (0);
}
  
int main(int argc, char *argv[])
{
  long i;
  float cd11, cd21;
  float pscale;

  cd11 = 0.;
  cd21 = 0.;

  // print usage if no arguments were given

  if (argc==1) usage(0);

  //  Read command line arguments
  
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'c': 
	cd11 = atof(argv[++i]);
	cd21 = atof(argv[++i]);
	break;
      }
    }
  }

  pscale = sqrt(cd11 * cd11 + cd21 * cd21);

  printf("%lf\n", pscale * 3600.);

  return (0);
}
