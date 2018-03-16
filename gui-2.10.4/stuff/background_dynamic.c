#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "stuff.h"

int compare_double(const void *a, const void *b);

void usage(int i)
{
  if (i == 0) {
    fprintf(stderr,"\n");
    fprintf(stderr,"Date: 2013-04-30, Author: Mischa Schirmer\n\n");
    fprintf(stderr,"Usage: background_dynamic \n");
    fprintf(stderr,"       -i reference MJD\n");
    fprintf(stderr,"       -f file with sky (or science) images (containing two columns with name and the MJD)\n");
    fprintf(stderr,"       -w window size (from how many exposures do we create the background model?)\n");
    fprintf(stderr,"      [-g maximum gap size in hours (default: 1.0)]\n\n");
    fprintf(stderr,"Purpose:\n");
    fprintf(stderr,"      Option -f is a file containing a list of (masked) images from which a background\n");
    fprintf(stderr,"      model can be calculated. In addition to the file names the MJDs are tabulated as well.\n");
    fprintf(stderr,"      Option -i lists the reference MJD of an image that is to be corrected.\n");
    fprintf(stderr,"      The program will then select the n closest images (option -w) in time from the list\n");
    fprintf(stderr,"      of masked images, excluding the corrected image should that be contained therein.\n");
    fprintf(stderr,"      Optionally, a gap size can be specified. If there is a gap larger than this value\n");
    fprintf(stderr,"      in the series of masked images, then a warning is printed if the to be corrected exposure\n");
    fprintf(stderr,"      is too far away, or an error is printed if the window covers the gap.\n\n");
  }

  exit (0);
}


//********************************************************  
int main(int argc, char *argv[])
{
  FILE *sky_file;
  long win = 4;
  double ref_mjd = -1.0, gap = 1.0 / 24.;
  long i, j, n_sky;
  char sky_filename[1000], dummy[1000];

  typedef struct {
    char name[1000];
    double mjd;
    double index;
    double diff;
    double absdiff;
    int blockid;
  } data;

  // print usage if no arguments were given

  if (argc==1) usage(0);

  //  Read command line arguments
  
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'i': ref_mjd = atof(argv[++i]);
	break;
      case 'f': strcpy(sky_filename,argv[++i]);
	break;
      case 'w': win = atol(argv[++i]);
	break;
      case 'g': gap = atof(argv[++i]) / 24.;
	break;
      }
    }
  }

  if (ref_mjd < 0.) {
    fprintf(stderr, "ERROR: Reference MJD was not defined.\n");
    exit (1);
  }

  if (win <= 2) {
    fprintf(stdout, "ERROR: %ld images used for dynamic background modelling. Must be at least 3!\n", win);
    exit (1);
  }

  // open the files
  if ((sky_file = fopen(sky_filename, "r")) == NULL) {
    fprintf(stderr, "ERROR: Cannot open %s\n", sky_filename);
    exit (1);
  }
  // how many elements are there
  i = 0;
  while(fgets(dummy,1000,sky_file) != NULL) {
    i++;
  }
  n_sky = i;
  rewind(sky_file);
  data skydata[n_sky];
  
  int test1 = 0;

  j = 0;
  double mjd_tmp;
  int testcomp1;
  for (i=0; i<n_sky; i++) {
    if (fscanf(sky_file, "%s %lf", &dummy[0], &mjd_tmp) == 0) {
      fprintf(stderr, "ERROR: Could not read from %s\n", sky_filename);
      exit (1);
    }
    // delete the entry if the reference MJD is equal to the sky MJD
    // Can happen maximally once (i.e. remove the to-be-corrected exposure from the list)
    testcomp1 = test1;
    if (ref_mjd == mjd_tmp) test1++;
    if (testcomp1 == test1) {
      strcpy(skydata[j].name,dummy);
      skydata[j].mjd = mjd_tmp;
      j++;
    }
  }

  n_sky = j;

  if (test1 > 1) {
    fprintf(stderr, "ERROR: Identical MJD entries found!\n");
    exit (1);
  }

  //****************************************************************************
  // DYNAMIC MODE: Print the names of the 'win' closest images
  //****************************************************************************

  // How many blocks do we have, based on the maximum allowed gap size?
  // Only the sky data are relevant here (which may contain the science data, though)
  float diff;
  int blockcount = 1, n_blocks;
  for (i=1; i<n_sky; i++) {
    diff = skydata[i].mjd - skydata[i-1].mjd;
    if (diff > gap) blockcount ++;
  }
  n_blocks = blockcount;
  
  // Loop over the list again to determine the block membership of an individual sky exposure
  blockcount = 0;
  skydata[0].blockid = 0;
  for (i=1; i<n_sky; i++) {
    diff = skydata[i].mjd - skydata[i-1].mjd;
    if (diff > gap) blockcount++;
    skydata[i].blockid = blockcount;
  }

  
  // for (i=0; i<n_sky; i++) 
  //  printf("%s %lf %d\n", skydata[i].name, skydata[i].mjd, skydata[i].blockid);


  // Add the time difference between the sky exposures and the reference MJD
  double *mjd_diff_sort;
  double *mjd_absdiff_sort;
  mjd_diff_sort = (double*) calloc(n_sky, sizeof(double));
  mjd_absdiff_sort = (double*) calloc(n_sky, sizeof(double));
  
  for (i=0; i<n_sky; i++) {
    skydata[i].diff     = skydata[i].mjd - ref_mjd;
    skydata[i].absdiff  = fabs(skydata[i].mjd - ref_mjd);
    mjd_diff_sort[i]    = skydata[i].diff;
    mjd_absdiff_sort[i] = skydata[i].absdiff;
  }
  
  // Sort the (absolute) MJD diffs in ascending order to identify the closest exposures
  // By using the absolute values, we do not know where in the sorted list the exposures are,
  // because they are always at the beginning so we only need to count from [0; win-1]
  qsort(mjd_diff_sort, n_sky, sizeof(double), compare_double);
  qsort(mjd_absdiff_sort, n_sky, sizeof(double), compare_double);
  
  // 's' and 'abs_s' are indices of indices, sorted
  long *s, *abs_s;
  s     = (long*) calloc(n_sky, sizeof(long));
  abs_s = (long*) calloc(n_sky, sizeof(long));
  for (i=0; i<n_sky; i++) {
    for (j=0; j<n_sky; j++) {
      if (skydata[i].absdiff == mjd_absdiff_sort[j]) abs_s[j] = i;
      if (skydata[i].diff == mjd_diff_sort[j]) s[j] = i;
    }
  }

  /*
    print sorted diff names
  for (i=0; i<n_sky; i++) {
    printf("%s\n", skydata[s[i]].name);
  }

  print sorted abs diff names
  printf("\n");
  for (i=0; i<n_sky; i++) {
    printf("%s\n", skydata[abs_s[i]].name);
  }
  */

  // Print the matching file names

  // If all exposures are taken after the reference MJD, 
  // with at least "gap" between MJD and the first exposure
  if (ref_mjd < skydata[0].mjd && fabs(ref_mjd - skydata[0].mjd) > gap) {
    printf("NO_MATCHING_SKY_FOUND\n");
    exit (1);
  }

  // If all exposures are taken before the reference MJD, 
  // with at least "gap" between MJD and the last exposure
  if (ref_mjd > skydata[n_sky-1].mjd && fabs(ref_mjd - skydata[n_sky-1].mjd) > gap) {
    printf("NO_MATCHING_SKY_FOUND\n");
    exit (1);
  }

  // Which is the first exposure index in the list of sorted MJDs (non-absolute)
  long unabs_start_index = 0;
  double *match_mjds; // an array containing the mjds of the win closest exposures
  match_mjds = (double*) calloc(win, sizeof(double));
  for (j=0; j<win; j++) {
    match_mjds[j] = skydata[abs_s[j]].mjd;
  }
  double min_mjd = min_array_dbl(match_mjds,win);
  // printf("min MJD: %lf\n", min_mjd);
  for (i=0; i<n_sky; i++) {
    if (skydata[s[i]].mjd == min_mjd) unabs_start_index = s[i];
  }

  int test_gap = 0;
  double found_gap = 0;
  int index_gap1 = 0, index_gap2 = 0;
  // test if there is a gap larger than the max gap size in the window
  for (j=unabs_start_index; j<unabs_start_index+win-1; j++) {
    //    printf("comp: %s %s\n", skydata[s[j]].name, skydata[s[j+1]].name);
    if (fabs(skydata[s[j]].mjd - skydata[s[j+1]].mjd) > gap) {
      test_gap  = 1;
      found_gap = fabs(skydata[s[j]].mjd - skydata[s[j+1]].mjd);
      index_gap1 = s[j];
      index_gap2 = s[j+1];
    }
  }

  if (test_gap == 1) {
    fprintf(stderr, "WARNING: The background window covers a gap of %lf hours between these exposures:\n", found_gap*24.);
    fprintf(stderr, "         %s\n", skydata[index_gap1].name);
    fprintf(stderr, "         %s\n", skydata[index_gap2].name);
    fprintf(stderr, "         This is more than the maximum allowed gap size of %lf hours.\n", gap*24.);
    //    exit (1);
  }

  // OK, no gap larger than the gap size!

  // Issue a warning if the window is larger than the gap size
  // This is not a problem as such, but it may mean that the number of images
  // is too large to sample the sky variations quickly enough
  //  for (j=unabs_start_index; j<unabs_start_index+win-1; j++) {
  double win_extent = fabs(skydata[unabs_start_index+s[win-1]].mjd - skydata[unabs_start_index+s[0]].mjd);
  if (win_extent > gap) {
    fprintf(stderr, "WARNING: The background window length (%f hours)\n", win_extent*24);
    fprintf(stderr, "         is larger than the maximum allowed gap size (%f hours).\n", gap*24);
    fprintf(stderr, "         Sky variations might be sampled too \"slowly\".\n");
  }

  // print images, beginning with the closest image
  /*
  for (j=0; j<win; j++) {
    printf("%s\n", skydata[abs_s[j]].name);
  }
  */

  // print images in window, sorted with respect to MJD
  for (j=0; j<win; j++) {
    printf("%s\n", skydata[unabs_start_index+s[j]].name);
  }

  free(mjd_diff_sort);
  free(mjd_absdiff_sort);
  free(s);
  free(abs_s);

  fclose(sky_file);

  exit (0);
  
}


//*****************************************************************
int compare_double(const void *a, const void *b)
{
  const double *da = (const double *) a;
  const double *db = (const double *) b;
  
  return (*da > *db) - (*da < *db);
}
