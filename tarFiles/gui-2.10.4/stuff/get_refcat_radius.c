#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "stuff.h"

void usage(int i, char *argv[])
{
  if (i == 0) {
    fprintf(stderr,"\n");
    fprintf(stderr,"          Version 1.0  (2012-11-15)\n\n");
    fprintf(stderr,"  Author: Mischa Schirmer\n\n");
    fprintf(stderr,"  USAGE:  %s \n", argv[0]);
    fprintf(stderr,"             -i geometries NAXIS1 NAXIS2 CRPIX1 CRPIX2 CD1_1 CD1_2 CD2_1 CD2_2\n");
    fprintf(stderr,"            [-l positions]\n");
    fprintf(stderr,"  PURPOSE: Determines optimal search radius for the astrometric reference catalog.\n");
    fprintf(stderr,"           The file 'geometries' contains the following for all chips in an exposure:\n");
    fprintf(stderr,"           NAXIS1 NAXIS2 CRPIX1 CRPIX2 CD1_1 CD1_2 CD2_1 CD2_2\n");
    fprintf(stderr,"           The file 'positions' (optional) contains for all exposures\n");
    fprintf(stderr,"           CRVAL1 CRVAL2\n\n");
    exit(1);
  }
}


int main(int argc, char *argv[])
{
  FILE *input;
  int nchips, npos, flag_pos = 0;
  long i;
  float *naxis1, *naxis2, *crpix1, *crpix2, *cd11, *cd12, *cd21, *cd22;
  float ramin, ramax, decmin, decmax, fieldradius;
  float ratmp[4], dectmp[4];
  float ramintmp, ramaxtmp, decmintmp, decmaxtmp;
  float raoff, decoff, decmiddle;
  
  char input_geometry[FILEMAX], input_crval[FILEMAX], dummy[FILEMAX];

  // print usage if no arguments were given

  if (argc==1) usage(0, argv);

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch(tolower((int)argv[i][1])) {
      case 'i': strcpy(input_geometry,argv[++i]);
	break;
      case 'l': strcpy(input_crval,argv[++i]);
	flag_pos = 1;
	break;
      }
    }
  }

  nchips = 0;
  input = fopen(input_geometry, "r");
  if (input == NULL) {
    fprintf(stderr, "Error: File does not exist or could not be read!\n");
    exit (1);
  } else {
    while ( fgets(dummy, FILEMAX, input) != NULL) nchips++;
  }

  rewind(input);

  naxis1 = (float*) calloc(nchips, sizeof(float));
  naxis2 = (float*) calloc(nchips, sizeof(float));
  crpix1 = (float*) calloc(nchips, sizeof(float));
  crpix2 = (float*) calloc(nchips, sizeof(float));
  cd11   = (float*) calloc(nchips, sizeof(float));
  cd12   = (float*) calloc(nchips, sizeof(float));
  cd21   = (float*) calloc(nchips, sizeof(float));
  cd22   = (float*) calloc(nchips, sizeof(float));

  // get the geometries
  i = 0;
  while ( !feof(input)) {
    if (fscanf(input, "%f %f %f %f %f %f %f %f", &naxis1[i], &naxis2[i], &crpix1[i], 
	       &crpix2[i], &cd11[i], &cd12[i], &cd21[i], &cd22[i]) == 0) {
      fprintf(stderr, "Error: Could not read from %s\n", input_geometry);
      exit (1);
    }
    i++;
  }

  fclose(input);

  if (flag_pos == 1) {
  }

  ramin  = 360.;
  ramax  = 0.;
  decmin = 90.;
  decmax = -90.;

  for (i=0; i<nchips; i++) {
    // RA and DEC for the four corners of chip i at RA = 180 DEC = 0
    // to avoid the RA 360-0 boundary
    ratmp[0]  = 180. - cd11[i] * (crpix1[i] -        1.) - cd12[i] * (crpix2[i] -        1.) ;
    ratmp[1]  = 180. - cd11[i] * (crpix1[i] - naxis1[i]) - cd12[i] * (crpix2[i] -        1.) ;
    ratmp[2]  = 180. - cd11[i] * (crpix1[i] - naxis1[i]) - cd12[i] * (crpix2[i] - naxis2[i]) ;
    ratmp[3]  = 180. - cd11[i] * (crpix1[i] -        1.) - cd12[i] * (crpix2[i] - naxis2[i]) ;
    
    dectmp[0] = - cd22[i] * (crpix2[i] -        1.) - cd21[i] * (crpix1[i] -        1.);
    dectmp[1] = - cd22[i] * (crpix2[i] -        1.) - cd21[i] * (crpix1[i] - naxis1[i]);
    dectmp[2] = - cd22[i] * (crpix2[i] - naxis2[i]) - cd21[i] * (crpix1[i] - naxis1[i]);
    dectmp[3] = - cd22[i] * (crpix2[i] - naxis2[i]) - cd21[i] * (crpix1[i] -        1.);
    
    ramintmp  = min_array(ratmp, 4);
    ramaxtmp  = max_array(ratmp, 4);
    decmintmp = min_array(dectmp, 4);
    decmaxtmp = max_array(dectmp, 4);
    
    if (ramintmp < ramin) ramin = ramintmp;
    if (ramaxtmp > ramax) ramax = ramaxtmp;
    if (decmintmp < decmin) decmin = decmintmp;
    if (decmaxtmp > decmax) decmax = decmaxtmp;
  }

  // the radius of the detector array, in degrees
  fieldradius = 0.5 * sqrt( pow(ramin-ramax, 2) + pow(decmin-decmax, 2));

  if (flag_pos == 1) {
    float *crval1, *crval2;
    // read the positions
    npos = 0;
    input = fopen(input_crval, "r");
    if (input == NULL) {
      fprintf(stderr, "Error: File does not exist or could not be read!\n");
      exit (1);
    } else {
      while ( fgets(dummy, FILEMAX, input) != NULL) npos++;
    }
    
    rewind(input);
    
    crval1 = (float*) calloc(npos, sizeof(float));
    crval2 = (float*) calloc(npos, sizeof(float));
    
    // get the positions
    // we add 1000 to RA to avoid problems if exposures are scattered across the 360-0 boundary
    i = 0;
    while ( !feof(input)) {
      if (fscanf(input, "%f %f", &crval1[i], &crval2[i]) == 0) {
	fprintf(stderr, "Error: Could not read from %s\n", input_crval);
	exit (1);
      }
      i++;
    }
    
    for (i=0; i<npos; i++) {
      crval1[i] += 1000.;
    }
    
    fclose(input);
    // get the max dither offset between pointings
    decmiddle = 0.5 * (max_array(crval2, npos) + min_array(crval2, npos));
    
    raoff = 0.5 * (max_array(crval1, npos) - min_array(crval1, npos)) * cos(decmiddle*3.14159/180.);
    decoff = 0.5 * (max_array(crval2, npos) - min_array(crval2, npos));
    
    fieldradius += sqrt(raoff*raoff + decoff*decoff);
    free(crval1);
    free(crval2);
  }

  printf("%ld\n", (long) ceil(fieldradius*60.));

  free(naxis1);
  free(naxis2);
  free(crpix1);
  free(crpix2);
  free(cd11);
  free(cd12);
  free(cd21);
  free(cd22);

  exit (0);
}
