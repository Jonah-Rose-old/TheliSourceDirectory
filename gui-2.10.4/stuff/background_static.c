#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


void usage(int i)
{
  if (i == 0) {
      fprintf(stderr,"\n");
      fprintf(stderr,"Date: 2014-09-30, Author: Mischa Schirmer\n\n");
      fprintf(stderr,"Usage: background_static \n");
      fprintf(stderr,"       -i reference MJD\n");
      fprintf(stderr,"       -f file with sky images (containing two columns with name and the MJD)\n");
      fprintf(stderr,"       -s file with science images (may be the same as the one with sky images)\n");
      fprintf(stderr,"       -o output file with science images and assigned block ids\n");
      fprintf(stderr,"      [-g maximum gap size in hours (default: 1.0)]\n\n");
  }

  exit (0);
}


//********************************************************  
int main(int argc, char *argv[])
{
  FILE *sci_file, *sky_file, *sciout_file;
  int flag_refmjd = 0, withinblock;
  double ref_mjd = -1.0, gap = 1.0 / 24.;
  long i, j, n_sky, n_sci;
  char sky_filename[1000], dummy[1000], sci_filename[1000], sciout_filename[1000];

  typedef struct {
    char name[1000];
    double mjd;
    double index;
    double diff;
    int blockid;
  } data;

  // print usage if no arguments were given

  if (argc==1) usage(0);

  //  Read command line arguments
  
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'i': ref_mjd = atof(argv[++i]);
	flag_refmjd = 1;
	break;
      case 'f': strcpy(sky_filename,argv[++i]);
	break;
      case 's': strcpy(sci_filename,argv[++i]);
	break;
      case 'o': strcpy(sciout_filename,argv[++i]);
	break;
      case 'g': gap = atof(argv[++i]) / 24.;
	break;
      }
    }
  }

  // open the files
  if ((sky_file = fopen(sky_filename, "r")) == NULL) {
    fprintf(stderr, "ERROR: Cannot open %s\n", sky_filename);
    exit (1);
  }

  if ((sci_file = fopen(sci_filename, "r")) == NULL) {
    fprintf(stderr, "ERROR: Cannot open %s\n", sci_filename);
    exit (1);
  }

  // how many elements are there
  i = 0;
  while(fgets(dummy,1000,sky_file) != NULL) i++;
  n_sky = i;

  i = 0;
  while(fgets(dummy,1000,sci_file) != NULL) i++;
  n_sci = i;

  rewind(sky_file);
  rewind(sci_file);
  data skydata[n_sky];
  data scidata[n_sci];
  
  // read the science data
  for (i=0; i<n_sci; i++) {
    if (fscanf(sci_file, "%s %lf", &scidata[i].name[0], &scidata[i].mjd) == 0) {
      fprintf(stderr, "ERROR: Could not read from %s\n", sci_filename);
      exit (1);
    }
    // initialise the rest so the compiler doesn't complain
    scidata[i].index = 0.;
    scidata[i].diff = 0.;
    scidata[i].blockid = -1;
  }

  int test1 = 0;
  j = 0;
  for (i=0; i<n_sky; i++) {
    if (fscanf(sky_file, "%s %lf", &skydata[i].name[0], &skydata[i].mjd) == 0) {
      fprintf(stderr, "ERROR: Could not read from %s\n", sky_filename);
      exit (1);
    }
    // Check for duplicate MJDs
    if (ref_mjd == skydata[i].mjd) test1++;
  }

  if (test1 > 1) {
    fprintf(stderr, "ERROR: Identical MJD entries found!\n");
    exit (1);
  }

  if (n_sky <= 2) {
    printf("NO_MATCHING_SKY_FOUND\n");
    exit (1);
  }


  //****************************************************************************
  // STATIC MODE:
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

  // ********************************************
  // To which block does the reference MJD belong, or does it sit between blocks (for sky observations)?
  // ********************************************
  if (flag_refmjd) {
    int ref_blockid = 0;
    if (ref_mjd < skydata[0].mjd) {
      ref_blockid = 0;
      if (fabs(ref_mjd - skydata[0].mjd) > gap) {
  	printf("NO_MATCHING_SKY_FOUND\n");
	exit(0);
      }
    }
    if (ref_mjd > skydata[n_sky-1].mjd) {
      ref_blockid = n_blocks - 1;
      if (fabs(ref_mjd - skydata[n_sky-1].mjd) > gap) {
  	printf("NO_MATCHING_SKY_FOUND\n");
	exit(0);
      }
    }
    
    withinblock = 0;
    for (i=0; i<n_sky-1; i++) {
      // The reference MJD is within a block
      if (ref_mjd >= skydata[i].mjd && ref_mjd <= skydata[i+1].mjd && skydata[i].blockid == skydata[i+1].blockid) {
  	ref_blockid = skydata[i].blockid;
  	withinblock = 1;
      }
    }
    
    if (!withinblock) {
      for (i=0; i<n_sky-1; i++) {
  	// The reference MJD is between blocks
  	if (ref_mjd > skydata[i].mjd && ref_mjd < skydata[i+1].mjd && skydata[i].blockid != skydata[i+1].blockid) {
  	  // The reference MJD is too far away from neighbouring sky blocks, and therefore cannot be treated
  	  if (fabs(skydata[i].mjd - ref_mjd) > gap && fabs(skydata[i+1].mjd - ref_mjd) > gap) {
  	    printf("NO_MATCHING_SKY_FOUND\n");
  	    exit(0);
  	  }
  	  // Assign the closer block id
  	  if (fabs(skydata[i].mjd - ref_mjd) <= fabs(skydata[i+1].mjd - ref_mjd))
  	    ref_blockid = skydata[i].blockid;
  	  else
  	    ref_blockid = skydata[i+1].blockid;
  	}
      }
    }

    // Now that the block ID is known, we print all associated sky images
    for (i=0; i<n_sky; i++) {
      if (skydata[i].blockid == ref_blockid) printf("%s\n", skydata[i].name);
      //printf("%s %d %d\n", skydata[i].name, skydata[i].blockid, ref_blockid);
    }
  }

  // *************************************************
  // To which blocks do the science exposures belong
  // *************************************************

  if ((sciout_file = fopen(sciout_filename, "w")) == NULL) {
    fprintf(stderr, "ERROR: Cannot open %s\n", sciout_filename);
    exit (1);
  }

  for (j=0; j<n_sci; j++) {
    if (scidata[j].mjd < skydata[0].mjd) {
      scidata[j].blockid = 0;
      if (fabs(scidata[j].mjd - skydata[0].mjd) > gap) 
	scidata[j].blockid = -1; // negative blockid means that it does not belong to a sky block
      continue;
    }

    if (scidata[j].mjd > skydata[n_sky-1].mjd) {
      scidata[j].blockid = n_blocks - 1;
      if (fabs(scidata[j].mjd - skydata[n_sky-1].mjd) > gap)
	scidata[j].blockid = -1;
      continue;
    }

    withinblock = 0;
    // The reference MJD now must be within a block
    for (i=0; i<n_sky; i++) {
      // First, test if a science exposure is the same as one of the sky exposures 
      // (i.e. sky exposures = sci exposures)
      if (scidata[j].mjd == skydata[i].mjd) {
	scidata[j].blockid = skydata[i].blockid;
	withinblock = 1;
	break;
      }
      
      if (i<n_sky-1 &&
	  scidata[j].mjd > skydata[i].mjd && 
	  scidata[j].mjd < skydata[i+1].mjd && 
	  skydata[i].blockid == skydata[i+1].blockid) {
  	scidata[j].blockid = skydata[i].blockid;
  	withinblock = 1;
	break;
      }
    }

    if (withinblock) continue;

    // Image is between blocks, find out which is the closest block or whether the gap size is too large
    else {
      for (i=0; i<n_sky-1; i++) {
	// The reference MJD is between blocks
	if (scidata[j].mjd > skydata[i].mjd && 
	    scidata[j].mjd < skydata[i+1].mjd && 
	    skydata[i].blockid != skydata[i+1].blockid) {
	  // The reference MJD is too far away from neighbouring sky blocks, and therefore cannot be treated
	  if (fabs(skydata[i].mjd   - scidata[j].mjd) > gap && 
	      fabs(skydata[i+1].mjd - scidata[j].mjd) > gap) {
	    scidata[j].blockid = -1;
	    break;
	  }
	  // Assign the closer block id
	  if (fabs(skydata[i].mjd - scidata[j].mjd) <= fabs(skydata[i+1].mjd - scidata[j].mjd))
	    scidata[j].blockid = skydata[i].blockid;
	  else
	    scidata[j].blockid = skydata[i+1].blockid;
	}
      }
    }
  }

  // Now that the block ID is known, we print the block assignments
  for (j=0; j<n_sci; j++) {
    fprintf(sciout_file, "%s %d\n", scidata[j].name, scidata[j].blockid);
  }


  fclose(sky_file);
  fclose(sci_file);
  fclose(sciout_file);

  exit (0);
}
