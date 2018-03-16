#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// compiled with

// gcc get_rotimsize.c -o get_rotimsize -L. -lm


float min(float *, long);
float max(float *, long);

void usage(int i)
{
  if (i == 0) {
      fprintf(stderr,"\n");
      fprintf(stderr,"Get the image size for a rotated image\n");
      fprintf(stderr,"Usage: get_rotimsize \n");
      fprintf(stderr,"       -g NAXIS1 NAXIS2\n");
      fprintf(stderr,"       -s final skyposition angle\n");
      fprintf(stderr,"      [-i initial skyposition angle (default: 0)\n\n");
  }

  exit (0);
}
  
int main(int argc, char *argv[])
{
  long i, ymax, ymin, xmax, xmin, sizex, sizey;
  float r, pa_fin, pa_ini, *yarr, *xarr, phi, naxis1, naxis2;
  float rad = 3.14159265 / 180.;
  
  naxis1 = 0;
  naxis2 = 0;

  // print usage if no arguments were given
  
  if (argc==1) usage(0);
  
  //  Read command line arguments
  xarr = (float*) calloc(4, sizeof(float));
  yarr = (float*) calloc(4, sizeof(float));

  pa_ini = 0;
  pa_fin = 0;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'g': 
	naxis1 = atof(argv[++i]);
	naxis2 = atof(argv[++i]);
	break;
      case 's': pa_fin = atof(argv[++i]);
	break;
      case 'i': pa_ini = atof(argv[++i]);
	break;
      }
    }
  }
  
  if (naxis1 == 0 || naxis2 == 0) {
    fprintf(stderr, "ERROR: Invalid NAXIS1/2 (zero!)\n");
    exit (1);
  }

  r = 0.5 * sqrt(naxis1 * naxis1 + naxis2 * naxis2) + 1;
  
  phi = atan(naxis2/naxis1) / rad;
  
  xarr[0] = r * cos(rad * (pa_fin - pa_ini + phi));
  xarr[1] = r * cos(rad * (pa_fin - pa_ini - phi + 180));
  xarr[2] = r * cos(rad * (pa_fin - pa_ini + phi + 180));
  xarr[3] = r * cos(rad * (pa_fin - pa_ini - phi + 360));
  
  yarr[0] = r * sin(rad * (pa_fin - pa_ini + phi));
  yarr[1] = r * sin(rad * (pa_fin - pa_ini - phi + 180));
  yarr[2] = r * sin(rad * (pa_fin - pa_ini + phi + 180));
  yarr[3] = r * sin(rad * (pa_fin - pa_ini - phi + 360));

  xmin = (long) min(xarr,4);
  xmax = (long) max(xarr,4);
  ymin = (long) min(yarr,4);
  ymax = (long) max(yarr,4);

/*
  for(i=0;i<4;i++) {
      printf("%f %f\n", xarr[i], yarr[i]);
  }
*/

  sizex = xmax - xmin;
  sizey = ymax - ymin;

  printf("%ld %ld\n", sizex, sizey);

  exit (0);

}

//*******************************************************

float max(float *data, long dim)
{
  long i;
  float max;
  
  max = data[0];
  
  for (i = 0; i < dim; i++)  {
    if (data[i] > max) max = data[i];
  }
  
  return (max);
}

//*******************************************************
float min(float *data, long dim)
{
  long i;
  float min;
  
  min = data[0];
  
  for (i = 0; i < dim; i++)  {
    if (data[i] < min) min = data[i];
  }
  
  return (min);
}
