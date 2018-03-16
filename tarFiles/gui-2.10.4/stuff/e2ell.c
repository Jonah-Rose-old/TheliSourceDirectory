#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

/* compiled with
   gcc e2ell.c -lm -O3 -o e2ell
*/

void usage(int i, char *argv[])
{
  if (i == 0) {
    fprintf(stderr, "\n");
    fprintf(stderr, "          Version 1.0  (2011-09-22)\n\n");
    fprintf(stderr, "  Author: Mischa Schirmer\n\n");
    fprintf(stderr, "  USAGE:  %s \n", argv[0]);
    fprintf(stderr, "           -i file [x, y, seeing, e1, e2]\n");
    fprintf(stderr, "  Returns x, y, seeing, e_x, e_y\n\n");
    exit(1);
  }
}

//************************************************************

int main(int argc, char *argv[])
{
  
  long i, dim;
  char filename[4096], dummy[4096], outname[4096];
  
  FILE *input, *output;
  
  /* print usage if no arguments were given */
  if (argc == 1) {
    usage(0, argv);
  }
  
  /* read the arguments */
  for (i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (tolower((int)argv[i][1])) {
      case 'i': strcpy(filename, argv[++i]);
        break;
      case 'o': strcpy(outname, argv[++i]);
        break;
      }
    }
  }
  
  if ((input = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Error: Cannot open %s\n", filename);
    exit (1);
  }
  
  // count number of objects in catalog
  i = 0;
  while (fgets(dummy, 4096, input) != NULL) {
    i++;
  }
  dim = i;
  
  rewind(input);
  
  struct _data_ {
    double x;
    double y;
    double s;
    double e1;
    double e2;
    double ell;
    double theta;
    double epsx;
    double epsy;
  } data[dim];
  
  i = 0;
  while (fscanf(input, "%lf %lf %lf %lf %lf", 
		&data[i].x,
		&data[i].y,
		&data[i].s,
		&data[i].e1,
		&data[i].e2) != EOF) {
    i++;
  }
  
  fclose(input);

  if ((output = fopen(outname, "w")) == NULL) {
    fprintf(stderr, "Error: Cannot open %s\n", outname);
    exit (1);
  }

  for (i=0; i<dim; i++) {
    data[i].ell   = sqrt(data[i].e1*data[i].e1 + data[i].e2*data[i].e2);
    data[i].theta = 0.5 * atan2(data[i].e2, data[i].e1);
    data[i].epsx  = data[i].ell * cos(data[i].theta);
    data[i].epsy  = data[i].ell * sin(data[i].theta);
    //    data[i].theta = 0.5 * atan2(data[i].e2 / data[i].e1);
    // if (data[i].e1 < 0.0) data[i].theta += pi/2.;

    //    fprintf(output, "%lf %lf %lf %lf %lf\n", data[i].x, data[i].y, data[i].s, data[i].ell, data[i].theta*180./pi);
    fprintf(output, "%lf %lf %lf %lf %lf\n", data[i].x, data[i].y, data[i].s, data[i].epsx, data[i].epsy);
  }
  fclose(output);

  return (0);
}
