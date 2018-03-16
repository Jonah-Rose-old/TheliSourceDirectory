#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "eclipse.h"
#include "fitstools.h"

// compiled with
// gcc rotate_cdmatrix.c -L. -leclipse -lqfits -lm -o rotate_cdmatrix

void rotate_cdmatrix(double*, double*, double*, double*, double);

void usage(int i)
{
  if (i == 0) {
      fprintf(stderr,"\n");
      fprintf(stderr,"VERSION: 2011-10-04, Author: Mischa Schirmer\n\n");
      fprintf(stderr,"PURPOSE: Rotates a CD matrix to a new position angle.\n\n");
      fprintf(stderr,"USAGE: rotate_cdmatrix \n");
      fprintf(stderr,"          -p new position angle for the output CD matrix (counting North over East)\n");
      fprintf(stderr,"         [-i global scamp-style .ahead file (CRPIX, CD-matrix per chip, separated by END keyword)]\n");
      fprintf(stderr,"         [-o output name for the new .ahead file]\n");
      fprintf(stderr,"         [-c cd11 cd12 cd21 cd22]\n");
      fprintf(stderr,"         [-l (print cd11 cd12 cd21 cd22 in a single line; only with option -c)]\n\n");
      fprintf(stderr,"         [-f FITS file with CD matrix in the header]\n\n");
      fprintf(stderr,"      Either an input file, or a list of CD matrix elements, or an image must be given.\n\n");
  }

  exit (0);
}

//**************************************************************

int main(int argc, char *argv[])
{
  FILE *_file_, *_fileout_;
  int FILEMAX=4000, fileflag, cdflag, fitsflag, globflag, lineflag;
  long i, numlines, numheads;
  double phi_new;
  double value, crpix1, crpix2, cd11, cd12, cd21, cd22;
  char file[FILEMAX], dummy[FILEMAX], newstring[FILEMAX], fileout[FILEMAX];
  char keyword[9], delim[2], fitsfile[FILEMAX];

  fileflag = 0;
  fitsflag = 0;
  cdflag = 0;
  lineflag = 0;
  phi_new = 0.;
  globflag = 0;
  crpix1 = 0.;
  crpix2 = 0.;

  // print usage if no arguments were given

  if (argc==1) usage(0);

  //  Read command line arguments

  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'i': strcpy(file,argv[++i]);
	fileflag = 1;
	globflag++;
	break;
      case 'o': strcpy(fileout,argv[++i]);
	break;
      case 'l': lineflag = 1;
	break;
      case 'c': 
	cd11 = atof(argv[++i]);
	cd12 = atof(argv[++i]);
	cd21 = atof(argv[++i]);
	cd22 = atof(argv[++i]);
	cdflag = 1;
	globflag++;
	break;
      case 'p': phi_new = atof(argv[++i])*RAD;
	break;
      case 'f': strcpy(fitsfile,argv[++i]);
	fitsflag = 1;
	globflag++;
	break;
      }
    }
  }
  
  if (globflag > 1) {
    fprintf(stderr, "ERROR: Either a header file, or a CD matrix, or a FITS image must be given.\n");
    exit (1);
  }

  if (fileflag == 0 && cdflag == 0 && fitsflag == 0) {
    fprintf(stderr, "ERROR: No input specified.\n");
    exit (1);
  }

  // *************************************************************
  // a CD matrix was given
  // *************************************************************

  if (cdflag == 1 && lineflag == 0) {
    rotate_cdmatrix(&cd11, &cd12, &cd21, &cd22, phi_new);
    printf("CD1_1   = %3.10g\n", cd11);
    printf("CD1_2   = %3.10g\n", cd12);
    printf("CD2_1   = %3.10g\n", cd21);
    printf("CD2_2   = %3.10g\n", cd22);
    return (0);
  }

  if (cdflag == 1 && lineflag == 1) {
    rotate_cdmatrix(&cd11, &cd12, &cd21, &cd22, phi_new);
    printf("%3.10g %3.10g %3.10g %3.10g\n", cd11, cd12, cd21, cd22);
    return (0);
  }

  // *************************************************************
  // a FITS file was given
  // *************************************************************

  if (fitsflag == 1) {
    qfits_header *header;
    header = qfits_header_read(fitsfile);
    if ( qfits_query_hdr( fitsfile, "CD1_1") == NULL ||
	 qfits_query_hdr( fitsfile, "CD1_2") == NULL ||
	 qfits_query_hdr( fitsfile, "CD2_1") == NULL ||
	 qfits_query_hdr( fitsfile, "CD2_2") == NULL) {
      fprintf(stderr, "ERROR: CD matrix elements missing in the FITS header!\n");
      qfits_header_destroy(header);
      exit (1);
    }
    else {
      cd11 = atof( qfits_query_hdr( fitsfile, "CD1_1"));
      cd12 = atof( qfits_query_hdr( fitsfile, "CD1_2"));
      cd21 = atof( qfits_query_hdr( fitsfile, "CD2_1"));
      cd22 = atof( qfits_query_hdr( fitsfile, "CD2_2"));
      qfits_header_destroy(header);
      rotate_cdmatrix(&cd11, &cd12, &cd21, &cd22, phi_new);
      printf("CD1_1   = %3.10g\n", cd11);
      printf("CD1_2   = %3.10g\n", cd12);
      printf("CD2_1   = %3.10g\n", cd21);
      printf("CD2_2   = %3.10g\n", cd22);
      return (0);
    }
  }

  // *************************************************************
  // a scamp-style global_header.ahead file was specified as input
  // *************************************************************

  // how many images are in the file
  numlines = 0;
  numheads = 0;
  if ( (_file_ = fopen(file, "r")) == NULL) {
    printf("\tError: Could not read from %s!\n", file);
    exit (1);
  }

  while ( fgets(dummy, FILEMAX, _file_) != NULL) {
    strcpy(newstring, trimwhitespace(dummy));
    if (strcmp(newstring, "END") == 0) numheads++;
    numlines++;
  }

  rewind(_file_);
  
  struct _datastring_ {
    char line[FILEMAX];
  } datastring[numlines];

  i = 0;
  while ( fgets(dummy, FILEMAX, _file_) != NULL) {
    strcpy(datastring[i].line, trimwhitespace(dummy));
    i++;
  }

  fclose(_file_);

  // read and transform the CD matrix

  if ( (_fileout_ = fopen(fileout, "w")) == NULL) {
    fprintf(stderr, "ERROR: Cannot open %s for writing!\n", fileout);
    exit (1);
  }

  for (i=0; i<numlines; i++) {
    if (strcmp(datastring[i].line,"END") != 0) {
      sscanf(datastring[i].line, "%s%s%lf", keyword, delim, &value);
      if (strcmp(keyword, "CRPIX1") == 0) crpix1 = value;
      else if (strcmp(keyword, "CRPIX2") == 0) crpix2 = value;
      else if (strcmp(keyword, "CD1_1") == 0)  cd11   = value;
      else if (strcmp(keyword, "CD1_2") == 0)  cd12   = value;
      else if (strcmp(keyword, "CD2_1") == 0)  cd21   = value;
      else if (strcmp(keyword, "CD2_2") == 0)  cd22   = value;
      else fprintf(_fileout_, "%s\n", datastring[i].line);
    }
    else {
      // one chip has been read in; now transform it:
      rotate_cdmatrix(&cd11, &cd12, &cd21, &cd22, phi_new);
      // write the result
      fprintf(_fileout_, "CRPIX1  = %3.9g\n", crpix1);
      fprintf(_fileout_, "CRPIX2  = %3.9g\n", crpix2);
      fprintf(_fileout_, "CD1_1   = %3.9g\n", cd11);
      fprintf(_fileout_, "CD1_2   = %3.9g\n", cd12);
      fprintf(_fileout_, "CD2_1   = %3.9g\n", cd21);
      fprintf(_fileout_, "CD2_2   = %3.9g\n", cd22);
      fprintf(_fileout_, "END\n");
    }
  }

  fclose(_fileout_);

  return (0);
}


//************************************************
void rotate_cdmatrix(double *cd11_ptr, double *cd12_ptr, 
		     double *cd21_ptr, double *cd22_ptr, 
		     double pn)
{
  double po, det;
  double cd11, cd12, cd21, cd22;
  double f11, f12, f21, f22;

  cd11 = *cd11_ptr;
  cd12 = *cd12_ptr;
  cd21 = *cd21_ptr;
  cd22 = *cd22_ptr;

  // the current position angle of the CD matrix
  po = get_posangle(cd11, cd12, cd21, cd22);

  // is the matrix flipped (det(CD) > 0 if flipped)
  det = cd11*cd22 - cd12*cd21;
  if (det < 0) 
    f11 = 1.;
  else 
    f11 = -1.;

  f12 = 0.;
  f21 = 0.;
  f22 = 1.;

  // unflip the matrix
  matrix_mult(f11, f12, f21, f22, &cd11, &cd12, &cd21, &cd22);
  
  // rotate the matrix to the new position angle
  matrix_mult(cos(pn-po), -sin(pn-po), sin(pn-po), cos(pn-po),
	      &cd11, &cd12, &cd21, &cd22);

  // flip the matrix
  matrix_mult(f11, f12, f21, f22, &cd11, &cd12, &cd21, &cd22);

  *cd11_ptr = cd11;
  *cd12_ptr = cd12;
  *cd21_ptr = cd21;
  *cd22_ptr = cd22;
}
