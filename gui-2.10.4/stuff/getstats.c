#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

 
int liststats(float*, int, float*, float*, float*, int); 
int findmode(float *, int, float, float, float *, float*); 

void usage(int i)
{
  if (i == 0) {
      fprintf(stderr,"\n");
      fprintf(stderr,"Calculates some basic statistics of an ascii file (1 column only)\n\n");
      fprintf(stderr,"Date:  2009-05-21, Author: Mischa Schirmer\n\n");
      fprintf(stderr,"Usage: getstats -i file\n");
      fprintf(stderr,"               [-m <all | mode | median | mean | lquart | uquart | sigma | min | max>]\n");
      fprintf(stderr,"               [-b (suppress header line; only for \"-m all\") ]\n");
      fprintf(stderr,"               [-f (force floating point output) ]\n");
      fprintf(stderr,"               [-n (do not calculate the average for the median if length=even) ]\n\n");
  }

  exit (0);
}

//************************************************************

int main(int argc, char *argv[])
{
  FILE *pipe;
  int noavg = 0, forcefloat = 0;
  long	i, b;
  long	samplesize;
  float	mean, mode, median, lquart, uquart, sigma, *sample, flquart;
  float min, max;
  char input_file[100], dummy[100], stats[100];

  b = 0;
  // print usage if no arguments were given

  if (argc==1) usage(0);
  //  Read command line arguments

  strcpy(stats, "all");

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'i': strcpy(input_file,argv[++i]);
	break;
      case 'm': strcpy(stats,argv[++i]);
	break;
      case 'b': b = 1;
	break;
      case 'f': forcefloat = 1;
	break;
      case 'n': noavg = 1;
      }
    }
  }

  // how many data points?
  samplesize = 0;
  pipe = fopen(input_file, "r");
  if (pipe == NULL) {
    fprintf(stderr, "Error: File does not exist or could not be read!\n");
    exit (1);
  } else {
    while ( !feof(pipe )) {
      if (fscanf(pipe, "%s", dummy) == 0) {
	fprintf(stderr, "Error: Could not read from %s\n", input_file);
	exit (1);
      }
      samplesize++;
    }
  }

  samplesize-=1;

  sample = (float*) calloc(samplesize, sizeof(float));

  rewind(pipe);

  i = 0;
  while ( !feof(pipe )) {
    if (fscanf(pipe, "%f", &sample[i]) == 0) {
      fprintf(stderr, "Error: Could not read from %s\n", input_file);
      exit (1);
    }
    i++;
  }

  fclose(pipe);

  if (liststats(sample, samplesize, &median, &lquart, &uquart, noavg))
    fprintf(stderr, "fdo_stats: warning: problem with stats from sample\n");

  findmode(sample, samplesize, lquart, uquart, &mode, &flquart);

  mean = 0.;
  for (i=0; i<samplesize; i++) mean += sample[i];
  mean /= samplesize;

  sigma = 0.;
  for (i=0; i<samplesize; i++) {
      sigma += (sample[i] - mean) * (sample[i] - mean);
  }
  if (samplesize > 0.) sigma = sqrt(sigma / ((float) samplesize-1.));
  else sigma = 0.;

  min = sample[0];
  max = sample[0];
  for (i=0; i<samplesize; i++) {
    if (sample[i] < min) min = sample[i];
    if (sample[i] > max) max = sample[i];
  }

  free(sample);

  if (strcmp(stats,"all") == 0) {
    if (b == 0) printf("# NUM MODE MEDIAN MEAN LQUARTILE UQUARTILE SIGMA MIN MAX\n");
    if (forcefloat == 0) 
      printf("%ld %g %g %g %g %g %g %g %g\n", samplesize, mode, median, 
	     mean, lquart, uquart, sigma, min, max);
    if (forcefloat == 1) 
      printf("%ld %f %f %f %f %f %f %f %f\n", samplesize, mode, median, 
	     mean, lquart, uquart, sigma, min, max);
  }

  if (forcefloat == 0) {
    if (strcmp(stats,"mode") == 0)   printf("%g\n", mode);
    if (strcmp(stats,"median") == 0) printf("%g\n", median);
    if (strcmp(stats,"mean") == 0)   printf("%g\n", mean);
    if (strcmp(stats,"lquart") == 0) printf("%g\n", lquart);
    if (strcmp(stats,"uquart") == 0) printf("%g\n", uquart);
    if (strcmp(stats,"sigma") == 0)  printf("%g\n", sigma);
    if (strcmp(stats,"min") == 0)    printf("%g\n", min);
    if (strcmp(stats,"max") == 0)    printf("%g\n", max);
  }
  if (forcefloat == 1) {
    if (strcmp(stats,"mode") == 0)   printf("%f\n", mode);
    if (strcmp(stats,"median") == 0) printf("%f\n", median);
    if (strcmp(stats,"mean") == 0)   printf("%f\n", mean);
    if (strcmp(stats,"lquart") == 0) printf("%f\n", lquart);
    if (strcmp(stats,"uquart") == 0) printf("%f\n", uquart);
    if (strcmp(stats,"sigma") == 0)  printf("%f\n", sigma);
    if (strcmp(stats,"min") == 0)    printf("%f\n", min);
    if (strcmp(stats,"max") == 0)    printf("%f\n", max);
  }

  exit(0);
}


//***********************************************************

int liststats(float *fsample, int samplesize, 
	      float *medianptr, 
	      float *lquartptr, 
	      float *uquartptr,
	      int noavg)
{
  int 	floatcmp();
  long	uppi, lowi;
  float	lquart, uquart, median;
  int	returnvalue = 0;
  
  lowi = floor(0.5 + 0.25 * samplesize);
  uppi = floor(0.5 + 0.75 * samplesize);
  qsort(fsample, samplesize, sizeof(float), floatcmp);
  *lquartptr = lquart = fsample[lowi];
  if (noavg == 0) {
    if (samplesize % 2) {
      *medianptr = median = fsample[samplesize / 2];
    } else {
      *medianptr = median = 0.5 * (fsample[samplesize / 2] + 
				   fsample[samplesize / 2 - 1]);
    }
  }
  else {
    // do not calculate the average of the middle two data points
    *medianptr = median = fsample[samplesize / 2];
  }

  *uquartptr = uquart = fsample[uppi];
  
  /* crude estimate of sigma from quartiles */
  return (returnvalue);
}


//***********************************************************

int floatcmp(float *f1, float *f2)
{
  return (*f1 > *f2 ? 1 : (*f1 == *f2 ? 0 : -1));	/* ascending order */
}


/*
 * - estimate the mode given sorted array of values f with median, quartiles.
 * - works by making a histogram of counts over range BIG_FACTOR * central
 *	50 percential range. 
 *	Smooths with gaussian width sigma (where sigma is
 *	estimated from the quartiles.  
 *
 * we also return 'flquart' which is the value such that equally many
 * pixels lie in f < flquart and flquart < f < mode, which is used to 
 * give a better sigma..
 *
 */
 
int findmode(float *f, int nf, float lquart, float uquart, 
	     float *mode, float *flquart)
{
  float fmin, fmax;
  long BINS = nf/10, BIG_FACTOR = 4;
  long	count[BINS]; 
  long	index, i, j, k, b, db, bmax, dbmax, submodecount, tempcount;
  long   steps_left, steps_right;
  float	smoothcount[BINS], smoothcountmax, df, sigma;
  float	*expon, slope_left, slope_right, diff, sigmaold;
  int	returnvalue;
  
  if (uquart <= lquart) {
      *mode = 0.5 * (uquart + lquart);
      *flquart = *mode;
      return (1);
  }


  // we need to guess the proper smoothing scale iteratively

  sigma = (uquart - lquart) / (2. * 0.67);
  sigmaold = sigma;

  fmin = 0.5 * (uquart + lquart) - BIG_FACTOR * sigma;
  fmax = 0.5 * (uquart + lquart) + BIG_FACTOR * sigma;
  df = (fmax - fmin) / BINS;  // bin size
  
  for (j=0;j<5; j++) {

    dbmax = sigma / df;   // smoothing window 2.0 sigma wide

    expon = (float *) calloc(2 * dbmax + 1, sizeof(float)) + dbmax;
    for (db = - dbmax ; db <= dbmax; db++) {
      expon[db] = exp (-0.5 * df * df * db * db / (sigma * sigma));
    }	
    
    // initialise arrays
    for (b = 0; b < BINS; b++)
      smoothcount[b] = count[b] = 0;
    
    // make counts
    for (i = 0; i < nf; i++) {
      index = floor(0.5 + (f[i] - fmin) / df);
      if (index >= 0 && index < BINS) {
	  count[index]++;
      }
    }
    
    // smooth counts
    for (b = 0; b < BINS; b++) {
      for (db = - dbmax ; db <= dbmax; db++) {
	if (b + db >= 0 && b + db < BINS)
	  smoothcount[b] += count[b + db] * expon[db];
      }	
    }
    
    // find peak of smoothed counts
    smoothcountmax = bmax = 0;
    for (b = 0; b < BINS; b++) {
      if (smoothcount[b] > smoothcountmax) {
	smoothcountmax = smoothcount[b];
	bmax = b;
      }
    }

    // the peak position is roughly known, now refine it:

    steps_left  = 0;
    steps_right = 0;
    slope_left  = 0.;
    slope_right = 0.;
    
    // min and max slope around the peak
    k = 0;
    for (i=bmax; i>0; i--) {
      diff = smoothcount[i] - smoothcount[i-1];
      if (diff > slope_left) {
	slope_left = diff;
	k++;
      }
    }
    steps_left = k;

    k = 0;
    for (i=bmax; i<BINS-1; i++) {
      diff = smoothcount[i+1] - smoothcount[i];
      //      printf("%f\n", diff);
      if (diff < slope_right) {
	slope_right = diff;
	k++;
      }
    }
    steps_right = k;
    
    // the new "width" of the peak is:
    sigma = (steps_right + steps_left) / 2.0 * df;

    free(expon - dbmax);

    if (sigma > sigmaold) break;

  }

  submodecount = 0;
  tempcount = 0;
  
  if (bmax > 0 && bmax < BINS - 1) {
    *mode = fmin + df * bmax;
    /* compute flquart */
    for (b = 0; b < bmax; b++) {
      submodecount += count[b];
    }
    submodecount += count[bmax] / 2;
    for (b = 0; b < bmax; b++) {
      tempcount += count[b];
      if (tempcount > 0.5 * submodecount)
	break;
    }
    *flquart = fmin + df * b;
    returnvalue = 0;
  } else {
    *mode = 0.5 * (uquart + lquart);
    *flquart = lquart;
    returnvalue = 1;
  }

  return (returnvalue);
}
