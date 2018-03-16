#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "sky2xy_helper.h"
#include "stuff.h"

double min(double, double);
double max(double, double);

void usage(int i)
{
  if (i == 0) {
    fprintf(stderr,"\n");
    fprintf(stderr,"Identifies one image in a series of images\n");
    fprintf(stderr,"containing a certain RA DEC region.\n");
    fprintf(stderr,"The x/y coordinates are returned.\n");
    fprintf(stderr,"Date:  2009-01-21, Author: Mischa Schirmer\n\n");
    fprintf(stderr,"Usage: sky2xy_helper \n");
    fprintf(stderr,"       -i image list\n");
    fprintf(stderr,"       -c ra1 dec1 ra2 dec2\n");
    fprintf(stderr,"      [-t (output in theli syntax)]\n\n");
  }

  exit (0);
}

//*****************************************************************
int main(int argc, char *argv[])
{
  FILE *input_file, *pipe;
  int FILEMAX=4000;
  long i, xmin, xmax, ymin, ymax, area, areamax;
  long xxmin, xxmax, yymin, yymax;
  int num, n_images, indmax, theli;
  int check1, check2, check3;
  char infile[FILEMAX], image[FILEMAX];
  char ra1[FILEMAX], dec1[FILEMAX], offt1[FILEMAX];
  char ra2[FILEMAX], dec2[FILEMAX], offt2[FILEMAX];
  char command[FILEMAX];

  // print usage if no arguments were given

  if (argc==1) usage(0);
  
  //  Read command line arguments
  
  theli = 0;
  
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'i': strcpy(infile,argv[++i]);
	break;
      case 'c': 
	strcpy(ra1,argv[++i]);
	strcpy(dec1,argv[++i]);
	strcpy(ra2,argv[++i]);
	strcpy(dec2,argv[++i]);
	break;
      case 't': theli = 1;
      }
    }
  }
  
  // how many images are in the list
  num = 0;
  input_file = fopen(infile, "r");
  while ( !feof(input_file )) {
    if (fscanf(input_file, "%s", image) == 0) {
      printf("\tError: Could not read from stdin!\n");
    }
    num++;
  }
  
  n_images = num - 1;
  
  struct image {
    char name[FILEMAX];
    long naxis1;
    long naxis2;
    double x1;
    double y1;
    double x2;
    double y2;
    int off1;
    int off2;
    int offtotal;
  } data[n_images];
  
  rewind(input_file);
  
  // do the stuff
  for (i=0; i<n_images; i++) {
    if (fscanf(input_file, "%s", &data[i].name[0]) == 0) {
      printf("\tError: Could not read from stdin!\n");
    }
    
    strcpy(offt1,"");
    strcpy(offt2,"");
    
    // get the pixel coordinates
    strcpy(command, bindir);
    strcat(command, "/sky2xy -j ");
    strcat(command, data[i].name);
    strcat(command, " ");
    strcat(command, ra1);
    strcat(command, " ");
    strcat(command, dec1);
    strcat(command, " | gawk '{print $5, $6, $7}'");

    pipe = popen(command, "r");
    if (fscanf(pipe, "%lf %lf %s", &data[i].x1, &data[i].y1, offt1) == 0) {
      printf("\tError: Could not read from stdin!\n");
    }
    if (strcmp(offt1,"") == 0) data[i].off1 = 0;
    else data[i].off1 = 1;
    pclose(pipe);

    // get the pixel coordinates
    strcpy(command, bindir);
    strcat(command, "/sky2xy -j ");
    strcat(command, data[i].name);
    strcat(command, " ");
    strcat(command, ra2);
    strcat(command, " ");
    strcat(command, dec2);
    strcat(command, " | gawk '{print $5, $6, $7}'");
    pipe = popen(command, "r");
    if (fscanf(pipe, "%lf %lf %s", &data[i].x2, &data[i].y2, offt2) == 0) {
      printf("\tError: Could not read from stdin!\n");
    }
    if (strcmp(offt2,"") == 0) data[i].off2 = 0;
    else data[i].off2 = 1;
    pclose(pipe);
    
    data[i].offtotal = 0;
    if (data[i].off1 + data[i].off2 > 0) data[i].offtotal = 1;

    // get the image size
    strcpy(command, bindir);
    strcat(command, "/dfits ");
    strcat(command, image);
    strcat(command, " | ");
    strcat(command, bindir);
    strcat(command, "/fitsort -d NAXIS1 NAXIS2 | gawk '{print $2, $3}'");
    pipe = popen(command, "r");
    if (fscanf(pipe, "%ld %ld", &data[i].naxis1, &data[i].naxis2) == 0) {
      printf("\tError: Could not read from stdin!\n");
    }
    pclose(pipe);
  }
  
  fclose(input_file);
  
  check1 = 1;
  check2 = 1;
  check3 = 1;
  
  for(i=0; i<n_images; i++) {
    check1 *= data[i].off1; 
    check2 *= data[i].off2; 
    check3 *= data[i].offtotal;
  }
  
  // CASE 1: region is not contained in any image
  
  if (check1 == 1 && check2 == 1) {
    printf("ERROR: Region could not be found in any image!\n");
    exit (1);
  }
  
  // CASE 2: region is fully contained in one image
  
  if (check3 == 0) {
    for(i=0;i<n_images;i++) {
      if ((data[i].off1 + data[i].off2) == 0) {
	// determine xmin, xmax, ymin, ymax
	xmin = (long) min(data[i].x1, data[i].x2);
	xmax = (long) max(data[i].x1, data[i].x2);
	ymin = (long) min(data[i].y1, data[i].y2);
	ymax = (long) max(data[i].y1, data[i].y2);
	if (theli == 0)
	  printf("%s %ld %ld %ld %ld\n", data[i].name, xmin, xmax, ymin, ymax);
	if (theli == 1)
	  printf("%s -s %ld %ld %ld %ld\n", data[i].name, xmin, xmax, ymin, ymax);
	break;
      }
    }   
  }
  
  // CASE 3: region is only partially contained in one or more images
  
  if (check3 != 0 && (check1 == 0 || check2 == 0)) {
    indmax = 0;
    areamax = 0;
    xxmin = 0;
    xxmax = 0;
    yymin = 0;
    yymax = 0;

    for(i=0;i<n_images;i++) {
      if (data[i].off1 * data[i].off2 == 0) {
	// determine xmin, xmax, ymin, ymax
	xmin = (long) min(data[i].x1, data[i].x2);
	xmax = (long) max(data[i].x1, data[i].x2);
	ymin = (long) min(data[i].y1, data[i].y2);
	ymax = (long) max(data[i].y1, data[i].y2);
	
	if (xmin<1) xmin = 1;
	if (xmax>data[i].naxis1) xmax = data[i].naxis1;
	if (ymin<1) ymin = 1;
	if (ymax>data[i].naxis2) ymax = data[i].naxis2;
	
	area = (xmax-xmin+1)*(ymax-ymin+1);
	if (area > areamax) {
	  areamax = area;
	  indmax = i;
	  xxmin = xmin;
	  xxmax = xmax;
	  yymin = ymin;
	  yymax = ymax;
	}
      }
    }
    if (xxmin == 0 && xxmax == 0 && yymin == 0 && yymax == 0) {
      fprintf(stderr, "ERROR: Could not identify area!\n");
      exit (1);
    }
    if (theli == 0)
      printf("%s %ld %ld %ld %ld\n", data[indmax].name, xxmin, xxmax, yymin, yymax);
    if (theli == 1)
      printf("%s -s %ld %ld %ld %ld\n", data[indmax].name, xxmin, xxmax, yymin, yymax);
  }
  
  return (0);
}

//***********************************************

double min(double v1, double v2)
{
  if (v1 <= v2) return(v1);
  else return (v2);
}

double max(double v1, double v2)
{
  if (v1 >= v2) return(v1);
  else return (v2);
}
