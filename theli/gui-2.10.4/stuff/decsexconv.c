#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

void usage(int i)
{
  if (i == 0) {
      fprintf(stderr,"\n");
      fprintf(stderr,"Decimal to HHMMSS converter\n");
      fprintf(stderr,"Date: July 2008, Author: Mischa Schirmer\n\n");
      fprintf(stderr,"Usage: decsexconv \n");
      fprintf(stderr,"       -i input catalog\n");
      fprintf(stderr,"       -o output catalog\n\n");
  }

  exit (0);
}

void dectohms( double, int*, int*, float*);
void dectodms( double, int*, int*, float*);

//********************************************************  
int main(int argc, char *argv[])
{
  FILE *input_file, *output_file;
  long i, n;
  int hh, dd, mm1, mm2;
  float mag, ss1, ss2;
  double ra, dec;
  char dummy[100], infile[100], outfile[100];
  char r1[2], r2[2], r3[2], d1[3], d2[2], d3[2], sgn[2];

  // print usage if no arguments were given

  if (argc==1) usage(0);

  //  Read command line arguments
  
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'i': strcpy(infile,argv[++i]);
	break;
      case 'o': strcpy(outfile,argv[++i]);
	break;
      }
    }
  }

  // open the files
  if ((input_file = fopen(infile, "r")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", infile);
  }
  if ((output_file = fopen(outfile, "w")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", outfile);
  }
  
  // how many elements are there
  i = 0;
  while(fgets(dummy,100,input_file) != NULL) {
    i++;
  }
  n = i;
  
  rewind(input_file);
  
  hh = 0;
  dd = 0;
  mm1 = 0;
  mm2 = 0;
  ss1 = 0.0;
  ss2 = 0.0;

  // do the conversion
  i = 0;
  for (i=0; i<n; i++) {
    if (fscanf(input_file, "%lf %lf %f", &ra, &dec, &mag) == 0) {
      fprintf(stderr, "ERROR: Could not read from %s\n", infile);
      exit (1);
    }
    dectohms(ra,  &hh, &mm1, &ss1);
    dectodms(dec, &dd, &mm2, &ss2);
    
    strcpy(r1, "");
    strcpy(r2, "");
    strcpy(r3, "");
    strcpy(d1, "");
    strcpy(d2, "");
    strcpy(d3, "");
    
    if (hh<10) strcpy(r1,"0");
    if (mm1<10) strcpy(r2,"0");
    if (ss1<10) strcpy(r3,"0");
    if (dd<0) strcpy(sgn,"-"); else strcpy(sgn,"+");
    if (dd<10 && dd > -10) strcpy(d1,"0");
    if (mm2<10) strcpy(d2,"0");
    if (ss2<10) strcpy(d3,"0");
    
    fprintf(output_file, "%s%d:%s%d:%s%2.4f %s%s%d:%s%d:%s%2.4f %f\n", r1, hh, r2, mm1, r3, ss1, 
	    sgn, d1, abs(dd), d2, mm2, d3, ss2, mag);
  }
  
  fclose(input_file);
  fclose(output_file);
  
  exit (0);
  
}

//******************************************************
// dectohms
//******************************************************

void dectohms( double ra, int *hh, int *mm, float *ss)
{
  double tmp;
  
  ra  = ra / 15.;
  *hh = floor(ra);
  tmp = (ra - *hh) * 60.;
  *mm = floor(tmp);
  *ss = (tmp - *mm) * 60.;
}

//******************************************************
// dectodms
//******************************************************

void dectodms( double dec, int *dd, int *mm, float *ss)
{
  double tmp;
  float sign;
  
  if (dec < 0) sign = -1.;
  else sign = 1.;
  
  *dd  = floor(fabs(dec));
  tmp  = (fabs(dec) - *dd) * 60.;
  *dd  = sign * *dd;
  *mm  = floor(tmp);
  *ss  = (tmp - *mm) * 60.;
}
