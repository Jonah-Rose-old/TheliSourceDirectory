#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>

// compile with
// gcc imalyzer.c -o imalyzer -L. -lm -O3

// some functions

int smooth_field(float *xgal, float *ygal, 
		 float *e1, float *e2, float *fwhm, float xdim, float ydim,
		 long n, long m, long ngal, char *out, float meandist);
float calc_mean(float *data, long start, long end);
float maxarr(float *data, long dim);
float minarr(float *data, long dim);
float mean_filtered(float *data, long, float, float);
float rms_filtered(float *, long, float, float);
int compare(const void *a, const void *b);

// Usage of the program

void usage(int i)
{
  if (i == 0) {
    fprintf(stderr,"\n");
    fprintf(stderr,"     Version 1.0 2010-09-16 (Mischa Schirmer)\n\n");
    fprintf(stderr,"     Smooths a FWHM and ellipticity field\n");
    fprintf(stderr,"     USAGE: imalyzer \n");
    fprintf(stderr,"                 -i input catalog\n");
    fprintf(stderr,"                 -o output file\n");
    fprintf(stderr,"                 -x xdim ydim (image dimensions)]\n\n");
    fprintf(stderr,"     Returns: xgrid ygrid fwhm epsx epsy\n\n");
    exit(1);
  }
}


//*********************************************************************

int main(int argc, char *argv[])
{
  FILE *input_file;
  int FILEMAX=4096;
  long i, n, m, nstar, xdim, ydim, nmax, mmax;
  float *xgal, *ygal, *fwhm, *e1, *e2;
  float meandist;
  char dummy[FILEMAX], catname[FILEMAX], outputfile[FILEMAX];

// Print usage if no arguments were given

  if (argc==1) usage(0);

// Read command line arguments

  xdim = 0;
  ydim = 0;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': strcpy(catname,argv[++i]);
	break;
      case 'o': strcpy(outputfile,argv[++i]);
	break;
      case 'x': xdim = atol(argv[++i]);
	ydim = atol(argv[++i]);
	break;
      }
    }
  }
  
  // Determine how many objects are in the catalog
  if ((input_file = fopen(catname, "r")) == NULL) {
      fprintf(stderr, "Cannot open %s\n", catname);
      exit (1);
  }
  i = 0;    
  while(fgets(dummy,100,input_file) != NULL) {
      i++;
  }
  nstar = i;
  
  // Initialise data arrays
  xgal   = (float*) calloc(nstar, sizeof(float));
  ygal   = (float*) calloc(nstar, sizeof(float));
  fwhm   = (float*) calloc(nstar, sizeof(float));
  e1     = (float*) calloc(nstar, sizeof(float));
  e2     = (float*) calloc(nstar, sizeof(float));

  // Read the catalog
  rewind(input_file);
  i = 0;
  while (fscanf(input_file, "%f%f%f%f%f", &xgal[i], 
		&ygal[i], &fwhm[i], &e1[i], &e2[i]) != EOF)
    i++;
  fclose(input_file);
  
  // average distance of objects
  meandist = sqrt(xdim*ydim/((float)nstar));

  // number of grid points along x and y
  n = (int) xdim / meandist;
  m = (int) ydim / meandist;

  // make sure we don't use more than 30 grid points

  /*
  nmax = (int) xdim / 100;
  mmax = (int) ydim / 100;
  if (n > nmax) n = nmax;
  if (m > mmax) m = mmax;
  */

  nmax = 30;
  mmax = nmax * (float) ydim / (float) xdim;

  if (mmax > nmax && mmax > 30) {
    float ratio = (float) nmax / (float) mmax;
    mmax = (int) ((float) mmax * ratio);
    nmax = (int) ((float) nmax * ratio);
  }

  if (n > nmax) n = nmax;
  if (m > mmax) m = mmax;

  smooth_field( xgal, ygal, e1, e2, fwhm, xdim, ydim, n, m, 
		nstar, outputfile, 1.5*meandist);

// Free the memory
  free(xgal);
  free(ygal);
  free(e1);
  free(e2);

  return (0);
}


//***************************************************************
// Smooth the data field
//***************************************************************

int smooth_field(float *xgal, float *ygal, float *e1, float *e2, 
		 float *fwhm, float xdim, float ydim, long n, long m, long nstar,
		 char *outputfile, float rad)
{

  FILE *output;
  
  int loopmax;
  long i, j, k, l, nobj, redo, t, nmin, count, count_thresh;
  float dsq, xgrid, ygrid, rad_ini, dx, dy;
  //  float g1min, g1max, g2min, g2max, fwmin, fwmax;
  float nkept, pi = 3.14159, theta, g1, g2, fw;
  float *g1tmp, *g2tmp, *fwtmp;
  float ellp, epsx, epsy;
  //  float rms_fw, rms_g1, rms_g2;
  float mean_fw, mean_g1, mean_g2;

  float r_g1, r_g2, r_fw;
  float diff_fw, diff_g1, diff_g2;

  // size of grid cell in pixels
  dx = (float) xdim / (float) n;
  dy = (float) ydim / (float) m;

  // search radius (we expect 3.141 objects in the aperture, as
  // search radius = linear size of grid cell, and we expect
  // one object per grid cell on average
  rad_ini = rad;
  
  // minimum number of objects required per aperture
  nmin = 6;
  
  // the global rms in FWHM:
  //  rms_fw = rms_filtered(fwhm, nstar, 0.0, 0.2);
  //  rms_g1 = rms_filtered(e1, nstar, 0.0, 0.2);
  //  rms_g2 = rms_filtered(e2, nstar, 0.0, 0.2);

  output = fopen(outputfile, "w");

  for (j=0; j<m; j++) {
    for (i=0; i<n; i++) {
      xgrid = dx * (float) i + 0.5 * dx;
      ygrid = dy * (float) j + 0.5 * dy;
      rad   = rad_ini;
      nobj  = 0;
      redo  = 0;
      // count objects
      while (redo == 0) {
	for (k=0; k<nstar; k++) {
	  dsq = (xgal[k] - xgrid) * (xgal[k] - xgrid) + 
	        (ygal[k] - ygrid) * (ygal[k] - ygrid);
	  if (dsq < rad*rad) nobj++;
	}
	if (nobj >= nmin) redo = 1; 
	
	// enlarge the radius if less than the expected  
	// number of objects are found in the window
	if (nobj < nmin) {
	  rad  *= 1.3;
	  nobj = 0;
	}
      }

      // we have nobj objects in the aperture
      g1tmp = (float*) calloc(nobj,sizeof(float));
      g2tmp = (float*) calloc(nobj,sizeof(float));
      fwtmp = (float*) calloc(nobj,sizeof(float));

      l = 0;
      for (k=0; k<nstar; k++) {
	dsq = (xgal[k] - xgrid) * (xgal[k] - xgrid) + 
	      (ygal[k] - ygrid) * (ygal[k] - ygrid);
	if (dsq < rad*rad) {
	  g1tmp[l] = e1[k];
	  g2tmp[l] = e2[k];
	  fwtmp[l] = fwhm[k];
	  l++;
	}
      }

      // 1. remove the lowest and highest values from the stack
      //    and calculate the mean of the rest
      // 2. if data points deviate by more than the global rms, reject them

      // the three calls to mean_filtered sort the data

      // we need the data in sorted order

      qsort(fwtmp, nobj, sizeof(float), compare);
      qsort(g1tmp, nobj, sizeof(float), compare);
      qsort(g2tmp, nobj, sizeof(float), compare);

      // sorting must be done BEFORE these (we don't use all data points)
      mean_fw = mean_filtered(fwtmp, 0.5*nobj, 0., 0.);
      mean_g1 = mean_filtered(g1tmp, 0.5*nobj, 0., 0.);
      mean_g2 = mean_filtered(g2tmp, 0.5*nobj, 0., 0.);

      // calculate the rms of the lower half of data points
      r_fw = rms_filtered(fwtmp, 0.5*nobj, 0., 0.);
      r_g1 = rms_filtered(g1tmp, 0.5*nobj, 0., 0.);
      r_g2 = rms_filtered(g2tmp, 0.5*nobj, 0., 0.);

      g1 = 0.;
      g2 = 0.;
      fw = 0.;
      nkept = 0.;
      count = 0;
      count_thresh = 1;

      // if the difference between two adjacent points is larger than 3 sigma,
      // then stop. if nothing is left, do it again, but require that at least
      // two measurements (countthresh) must deviate by more than 3 sigma
      // before a data point is rejected.
      // if then still all points are rejected, we can't measure anything here

      loopmax = 0;
      while (nkept == 0. && loopmax < 2) {
	for (t=0; t<nobj-1; t++) {

	  diff_g1 = g1tmp[t+1] - g1tmp[t];
	  diff_g2 = g2tmp[t+1] - g2tmp[t];
	  diff_fw = fwtmp[t+1] - fwtmp[t];
	  // ignore the lowest data point if the 2nd is more than 3.0 sigma away,
	  // and between the 2nd and the 3rd is less than 2.0 sigma
	  if (t==0 && diff_g1 >= 3.*r_g1 && g1tmp[t+2] - g1tmp[t+1] <= 2.0*r_g1) continue;
	  if (t==0 && diff_g2 >= 3.*r_g2 && g2tmp[t+2] - g2tmp[t+1] <= 2.0*r_g2) continue;
	  if (t==0 && diff_fw >= 3.*r_fw && fwtmp[t+2] - fwtmp[t+1] <= 2.0*r_fw) continue;

	  if (diff_g1 >= 3.*r_g1) count ++;
	  if (diff_g2 >= 3.*r_g2) count ++;
	  if (diff_fw >= 3.*r_fw) count ++;

	  // no outlier ...
	  if (count < count_thresh &&
	      // more filtering: 2.5 sigma rejection
	      fabs(mean_g1 - g1tmp[t]) <= 2.5*r_g1 &&
	      fabs(mean_g2 - g2tmp[t]) <= 2.5*r_g2 &&
	      fabs(mean_fw - fwtmp[t]) <= 2.5*r_fw) {
	    g1 += g1tmp[t];
	    g2 += g2tmp[t];
	    fw += fwtmp[t];
	    nkept++;
	  }
	  // outlier
	  else break;

	  // get the last object into the statistics, too
	  if (t == nobj-2 && count < count_thresh &&
	      diff_g1 <= 2.0*r_g1 && diff_g2 <= 2.0*r_g2 && diff_fw <= 2.0*r_fw) {
	    g1 += g1tmp[t+1];
	    g2 += g2tmp[t+1];
	    fw += fwtmp[t+1];
	    nkept++;
	  }
	}
	if (nkept < 1.) count_thresh = 2;
	loopmax++;
      }
      
      if (nkept > 0.) {
	g1 /= nkept;
	g2 /= nkept;
	fw /= nkept;
	theta = 0.5 * atan2( g2, g1);
	
	ellp = sqrt(g1*g1 + g2*g2);
	epsx = ellp * cos(theta);
	epsy = ellp * sin(theta);
	
	fprintf(output, "%f %f %f %f %f\n", xgrid, ygrid, fw, epsx, epsy);
      }

      free(g1tmp);
      free(g2tmp);
      free(fwtmp);
    }
  }
  rad = rad_ini;

  fclose(output);

  return (0);
}


//************************************************************
float mean_filtered(float *data, long n, float lowfrac, float highfrac)
{
  long i, start, end;
  float mean, l, *datatmp;
  
  datatmp = (float*) calloc(n, sizeof(float));
  for (i=0; i<n; i++) datatmp[i] = data[i];

  qsort(datatmp, n, sizeof(float), compare);
  
  start = lowfrac*n;
  end = (1.-highfrac)*n;

  // make sure we reject something if rejection is requested
  if (lowfrac  > 0. && start == 0) start = 1;
  if (highfrac > 0. && end   == n) end = n-1;

  mean = 0.;
  l = 0.;

  for (i=start; i<end; i++) {
    mean += datatmp[i];
    l++;
  }
  if (l>1)
    mean = mean / l;
  else mean = 0.;

  return (mean);

}


//*****************************************************************
float maxarr(float *data, long dim)
{
    long i;
    float max;

    max = data[0];

    for (i = 0; i < dim; i++)  {
        if (data[i] > max) max = data[i];
    }

    return (max);
}


//*****************************************************************
float minarr(float *data, long dim)
{
    long i;
    float min;

    min = data[0];

    for (i = 0; i < dim; i++)  {
        if (data[i] < min) min = data[i];
    }

    return (min);
}

//*****************************************************************
int compare(const void *a, const void *b)
{
  float a1 = *((float*)a);
  float b1 = *((float*)b);

  if (a1<b1) 
      return -1;
  else if (a1>b1) 
      return 1;
  else 
      return 0;    
}

//************************************************************
float rms_filtered(float *data, long n, float lowfrac, float highfrac)
{
  long i, start, end;
  float sdev, mean, l, *datatmp, *step, minrms;
  
  // reject the lowest and highest 10% values before calculating 
  // the rms

  datatmp = (float*) calloc(n, sizeof(float));
  for (i=0; i<n; i++) datatmp[i] = data[i];
  qsort(datatmp, n, sizeof(float), compare);

  // derive a minimum useful rms (to work around pathologic cases where two data points
  // lie extremely close together
  step = (float*) calloc(n-1, sizeof(float));
  for (i=0; i<n-1; i++) {
    step[i] = fabs(datatmp[i+1] - datatmp[i]);
  }
  qsort(step,n-1, sizeof(float), compare);
  if (n % 2 == 0) minrms = 0.5 * (step[n/2-1] + step[n/2]);
  else minrms = step[n/2];
  
  start = lowfrac*n;
  end = (1.-highfrac)*n;

  // make sure we reject something if rejection is requested
  if (lowfrac  > 0. && start == 0) start = 1;
  if (highfrac > 0. && end   == n) end = n-1;

  mean = 0.;
  l = 0.;
  for (i=start; i<end; i++) {
    mean += datatmp[i];
    l++;
  }
  if (l>1)
    mean = mean / l;
  else mean = 0.;

  l = 0;
  sdev = 0.;
  for (i=start; i<end; i++)  {
    sdev += (mean - datatmp[i]) * (mean - datatmp[i]);
    l++;
  }
  
  if (l>1) {
    sdev = sqrt(sdev / (l - 1.));
    // a lower cut-off to the rms, in case some data points lie very close together
    if (sdev < minrms) sdev = minrms;
    return (sdev);
  }
  else return (0.);
}
