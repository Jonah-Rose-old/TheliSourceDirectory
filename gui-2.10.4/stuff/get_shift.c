#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "stuff.h"

/*
 compiled with
 gcc get_shift.c -lm -O3 -o get_shift
*/

// This program determines the shift vector between
// two astronomical catalogues

void usage(int i, char *argv[])
{
  if (i == 0) {
    fprintf(stderr,"\n");
    fprintf(stderr,"          Version 1.2  (2009-06-25)\n\n");
    fprintf(stderr,"  Author: Mischa Schirmer\n\n");
    fprintf(stderr,"  USAGE:  %s \n", argv[0]);
    fprintf(stderr,"             -i catalog_list \n");
    fprintf(stderr,"             -s step size [default: 100] \n");
    fprintf(stderr,"             -f reduction factor [default: 10] \n");
    fprintf(stderr,"             -m minimum box size [default: 2.] \n");
    fprintf(stderr,"             -o overlap [default: 0] \n");
    fprintf(stderr,"  PURPOSE: Determines shift between astronomical\n");
    fprintf(stderr,"           catalogues in the format 'x y', using a\n");
    fprintf(stderr,"           recursive algorithm.\n");
    fprintf(stderr,"           The list of catalogs must have the \n");
    fprintf(stderr,"           number of catalogs in the first line. \n");
    fprintf(stderr,"           Each catalog must have the number of\n");
    fprintf(stderr,"           objects in the first line.\n\n");
    exit(1);
  }
}


int main(int argc, char *argv[])
{
  FILE *input;
  int diffdim, printed;
  long ncat, nobjref, nobj, max, nmax, mmax;
  long i, j, k, l, n, m, s, t, smax;
  long **diff;
  float xstart, ystart, xstartold, ystartold, xstart0, ystart0;
  float f, xmean, ymean, zmean, xrms, yrms, zrms, meanphotzp;
  float step, stepini, minbox, rx, ry, ov;
  float *xref, *yref, *zref, *x, *y, *z, **xs;
  float **ys, *fullxs, *fullys, *fullzp, *photzp;
  char input_file[FILEMAX];

// print usage if no arguments were given

  if (argc==1) usage(0, argv);

  stepini = 100.;
  f = 10.;
  minbox = 2.;
  ov = 0.;
  printed = 0;

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': strcpy(input_file,argv[++i]);
	break;
      case 's': stepini = atof(argv[++i]);
	break;
      case 'f': f = atof(argv[++i]);
	break;
      case 'm': minbox = atof(argv[++i]);
	break;
      case 'o': ov = atof(argv[++i]);
	break;
      }
    }
  }

  minbox = minbox / 2.;  // we take it as the radius

// get the number of catalogues
  input = fopen(input_file, "r");
  if (fscanf(input, "%ld", &ncat) == 0) {
    printf("Error: Could not read from %s, or file is empty!\n", input_file);
    exit (1);
  }

  struct catalog {
    char name[FILEMAX];
    float xs;
    float ys;
    float zp;
    float xrms;
    float yrms;
    float zrms;
    long  smax;
    float minbox;
  } cat[ncat];

  // get the catalog names
  i = 0;
  for (i=0; i<ncat; i++) {
    if(fscanf(input, "%s", &(cat[i].name)[0]) == 0) {;
      printf("Error: Could not read catalog name from %s\n", input_file);
      exit (1);
    }
  }
  fclose(input);

  // read the reference catalogue

  input = fopen(cat[0].name, "r");
  if(fscanf(input, "%s", &(cat[i].name)[0]) == 0) {;
      printf("Error: Could not read from reference catalog %s, or file is empty!\n", &(cat[0].name)[0]);
      exit (1);
    }
  if (fscanf(input, "%ld", &nobjref) == 0) printf("Read error.\n");
  
  xref = (float*) calloc(nobjref, sizeof(float));
  yref = (float*) calloc(nobjref, sizeof(float));
  zref = (float*) calloc(nobjref, sizeof(float));
  fullxs = (float*) calloc(nobjref, sizeof(float));
  fullys = (float*) calloc(nobjref, sizeof(float));
  fullzp = (float*) calloc(nobjref, sizeof(float));

  i = 0;
  for (i=0; i<nobjref; i++) {
    if( fscanf(input, "%f %f %f", &xref[i], &yref[i], &zref[i]) == 0) 
      printf("Read error.\n");
  }
  fclose(input);

  diffdim = 10;

  xs   = (float**) calloc(diffdim*2, sizeof(float*));
  ys   = (float**) calloc(diffdim*2, sizeof(float*));
  for(i=0; i<diffdim*2; i++) {
    xs[i]   = (float*) calloc(diffdim*2, sizeof(float));
    ys[i]   = (float*) calloc(diffdim*2, sizeof(float));
  }

  // get the offsets for the catalogs

  i = 0;
  for (i=0; i<ncat; i++) {
    input = fopen(cat[i].name, "r");
    if (fscanf(input, "%ld", &nobj) == 0) printf("Read error.\n");
    
    x = (float*) calloc(nobj, sizeof(float));
    y = (float*) calloc(nobj, sizeof(float));
    z = (float*) calloc(nobj, sizeof(float));
      
    // read the catalog
    j = 0;
    for (j=0; j<nobj; j++) {
      if( fscanf(input, "%f %f %f", &x[j], &y[j], &z[j]) == 0) 
	printf("Read error.\n");
    }
    fclose(input);
    
    step = stepini;
    
    // count the number of offset vectors in bins
    diff = (long**)  calloc(diffdim, sizeof(long*));
    for(t=0; t<diffdim; t++) {
      diff[t] = (long*)  calloc(diffdim, sizeof(long));
    }
    
    for (n=0; n<diffdim; n++) {
      for (m=0; m<diffdim; m++) {
	diff[n][m] = 0;
      }
    }
    
    for (n=0; n<diffdim*2; n++) {
      for (m=0; m<diffdim*2; m++) {
	xs[n][m] = 0.;
	ys[n][m] = 0.;
      }
    }
    
    xstart0 = diffdim / 2. * step;
    ystart0 = diffdim / 2. * step;
    
    // put a 10x10 scan grid, determine the starting value
    for (k=0; k<nobjref; k++) {
      for (l=0; l<nobj; l++) {
	rx = xref[k] - x[l] + xstart0;
	ry = yref[k] - y[l] + ystart0;
	m = 0;
	n = 0;
	for (n=0; n<diffdim; n++) {
	  for (m=0; m<diffdim; m++) {
	    if (rx <  (float) (n+1) * step && 
		rx >= (float) (n)   * step &&
		ry <  (float) (m+1) * step && 
		ry >= (float) (m)   * step) {
	      diff[n][m]++;
	    }
	  }
	}
      }
    }
    
    // get the starting index
    
    max = diff[0][0];
    nmax = 0;
    mmax = 0;
    for (n=0; n<diffdim; n++) {
      for (m=0; m<diffdim; m++) {
	if (diff[n][m] > max) {
	  max = diff[n][m];
	  nmax = n;
	  mmax = m;
	}
      }
    }
    
    xstart = (nmax - 1) * step;
    ystart = (mmax - 1) * step;
    
    for(t=0; t<diffdim; t++) {
      free(diff[t]);
    }
    free(diff);
    
    diff = (long**)  calloc(diffdim*2, sizeof(long*));
    for(t=0; t<diffdim*2; t++) {
      diff[t] = (long*)  calloc(diffdim*2, sizeof(long));
    }
    
    // now start the loop
    while (step > minbox) {
      k = 0;
      l = 0;
      s = 0;
      for (k=0; k<nobjref; k++) {
	fullxs[k] = 0.;
	fullys[k] = 0.;
	fullzp[k] = 0.;
      }
      
      step = step / f;
      
      for (k=0; k<nobjref; k++) {
	for (l=0; l<nobj; l++) {
	  rx = xref[k] - x[l] + xstart0;
	  ry = yref[k] - y[l] + ystart0;
	  m = 0;
	  n = 0;
	  for (n=0; n<diffdim*2; n++) {
	    for (m=0; m<diffdim*2; m++) {
	      if (rx <  (float) (n+1) * step + xstart && 
		  rx >= (float) (n)   * step + xstart &&
		  ry <  (float) (m+1) * step + ystart && 
		  ry >= (float) (m)   * step + ystart) {
		diff[n][m]++;
		xs[n][m] = xs[n][m] + xref[k] - x[l];
		ys[n][m] = ys[n][m] + yref[k] - y[l];
	      }
	    }
	  }
	}
      }
      
      // get the index that corresponds to the maximum of "diff"
      
      max = diff[0][0];
      nmax = 0;
      mmax = 0;
      for (n=0; n<diffdim*2; n++) {
	for (m=0; m<diffdim*2; m++) {
	  if (diff[n][m] > max) {
	    max = diff[n][m];
	    nmax = n;
	    mmax = m;
	  }
	  xs[n][m] = xs[n][m] / (float) max;
	  ys[n][m] = ys[n][m] / (float) max;
	}
      }
      
      xstartold = xstart;
      ystartold = ystart;
      
      // calculate new start coordinates:
      xstart = xstart + (nmax-1) * step;
      ystart = ystart + (mmax-1) * step;
      
      if (step < minbox) {
	for (k=0; k<nobjref; k++) {
	  for (l=0; l<nobj; l++) {
	    rx = xref[k] - x[l] + xstart0;
	    ry = yref[k] - y[l] + ystart0;
	    if (rx <  (float) (nmax+ov+1) * step + xstartold && 
		rx >= (float) (nmax-ov)   * step + xstartold &&
		ry <  (float) (mmax+ov+1) * step + ystartold && 
		ry >= (float) (mmax-ov)   * step + ystartold) {
	      fullxs[s] = xref[k] - x[l];
	      fullys[s] = yref[k] - y[l];
	      fullzp[s] = zref[k] - z[l];
	      s++;
	    }
	  }
	}
	smax = s;
	xmean = mean_of_array(fullxs, smax);
	ymean = mean_of_array(fullys, smax);
	zmean = mean_of_array(fullzp, smax);
	xrms  = rms_of_array(fullxs, smax);
	yrms  = rms_of_array(fullys, smax);
	zrms  = rms_of_array(fullzp, smax);
	
	// fill in the result
	cat[i].xs = xmean;
	cat[i].ys = ymean;
	cat[i].zp = zmean;
	cat[i].xrms = xrms;
	cat[i].yrms = yrms;
	cat[i].zrms = zrms;
	cat[i].smax = smax;
	cat[i].minbox = 2.*(step+ov);
	//	      printf("%s %#10.2f %#10.2f %#10.4f %#10.2f %#10.2f %#8.4f %6ld %6.1f\n", 
	//		     cat[i].name, xmean, ymean, zmean, xrms, yrms, zrms, smax, 
	//		     2.*(step+ov));
	printed = 1;
      }
      
      for (n=0; n<diffdim*2; n++) {
	for (m=0; m<diffdim*2; m++) {
	  diff[n][m] = 0;
	  xs[n][m] = 0.;
	  ys[n][m] = 0.;
	}
      }
      
    }
    
    free(x);
    free(y);
    free(z);
    
    for(t=0; t<diffdim*2; t++) {
      free(diff[t]);
    }
    free(diff);
  }
  
  // rescale the relative zeropoints such that their mean is zero

  photzp = (float*) calloc(ncat, sizeof(float));
  for (i=0; i<ncat; i++) {
    photzp[i] = cat[i].zp;
  }
  
  meanphotzp = mean_of_array(photzp, ncat);
  
  for (i=0; i<ncat; i++) {
    printf("%s %#10.2f %#10.2f %#10.6f %#10.2f %#10.2f %#8.4f %6ld %6.1f\n", 
	   cat[i].name, cat[i].xs, cat[i].ys, cat[i].zp-meanphotzp, 
	   cat[i].xrms, cat[i].yrms, cat[i].zrms, cat[i].smax, 
	   cat[i].minbox);
  }
  
  if (printed == 0) {
    printf("Error. No match found. Increase either the minimum box\n"); 
    printf("size with -m or decrease the step size with -f\n");
  }
  
  free(xref);
  free(yref);
  free(zref);
  free(fullxs);
  free(fullys);
  free(fullzp);
  free(photzp);

  for(i=0; i<diffdim*2; i++) {
      free(xs[i]);
      free(ys[i]);
  }
  free(xs);
  free(ys); 

  exit (0);
}
