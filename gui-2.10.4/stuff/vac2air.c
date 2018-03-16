#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

double vac2air(double, double);

void usage(int i)
{
  if (i == 0) {
      fprintf(stderr,"\n");
      fprintf(stderr,"Converts vacuum to air wavelengths\n");
      fprintf(stderr,"2013-02-13, Mischa Schirmer\n\n");
      fprintf(stderr,"Usage: vac2air \n");
      fprintf(stderr,"       -f input catalog\n");
      fprintf(stderr,"       -o output catalog\n");
      fprintf(stderr,"       -i input wavelength\n");
      fprintf(stderr,"       -p pressure (mbar)\n\n");
      fprintf(stderr,"Either an input and an output file must be specified,\n");
      fprintf(stderr,"or an individual vacuum wavelength given with '-i'.\n");
      fprintf(stderr,"       \n");
  }

  exit (0);
}


//********************************************************  
int main(int argc, char *argv[])
{
  FILE *input_file, *output_file;
  long i, n;
  double lvac, lair, p = 1020.;
  char dummy[100], infile[100], outfile[100];

  // print usage if no arguments were given

  if (argc==1) usage(0);

  //  Read command line arguments
  
  int filemode = 0;
  
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'f': strcpy(infile,argv[++i]);
	filemode = 1;
	break;
      case 'o': strcpy(outfile,argv[++i]);
	break;
      case 'i': lvac = atof(argv[++i]);
	filemode = 0;
	break;
      case 'p': p = atof(argv[++i]);
	break;
      }
    }
  }

  // open the files
  if (filemode) {
    if ((input_file = fopen(infile, "r")) == NULL) {
      fprintf(stderr, "Cannot open %s\n", infile);
      exit (1);
    }
    if ((output_file = fopen(outfile, "w")) == NULL) {
      fprintf(stderr, "Cannot open %s\n", outfile);
      exit (1);
    }
    
    // how many elements are there
    i = 0;
    while(fgets(dummy,100,input_file) != NULL) {
      i++;
    }
    n = i;

    rewind(input_file);
    
    // do the conversion
    for (i=0; i<n; i++) {
      if (fscanf(input_file, "%lf", &lvac) == 0) {
	fprintf(stderr, "ERROR: Could not read from %s\n", infile);
	exit (1);
      }
      lair = vac2air(lvac, p);
      fprintf(output_file, "%4.2lf\n", lair);
    }
  
    fclose(input_file);
    fclose(output_file);
  }
  else {
    printf("%4.2lf\n", vac2air(lvac, p));
  }

  exit (0);
}
  
//******************************************************
// conversion
//******************************************************
  
double vac2air(double l0, double p)
{
  double lair, diff;
  double b1 = 2.94981e-2;
  double b2 = 2.5540e-4;
  double c1 = 146.0;
  double c2 = 41.0;

  // Ciddor 1996, Prieto 2011

  lair = (l0 - (c1 + c2) * pow(l0,3) + c1*c2*pow(l0,5)) /
    (1. - (b1 + b2 + c1 + c2)*pow(l0,2) + (b2*c1 + (b1 + c1)*c2)*pow(l0,4));

  diff = l0 - lair;

  // rough pressure correction;
  lair = l0 - diff * p/1020.;

  return (lair);
}
