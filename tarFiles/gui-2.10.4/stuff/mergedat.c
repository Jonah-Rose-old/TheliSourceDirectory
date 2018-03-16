#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "stuff.h"

void usage(int i)
{
  if (i == 0) {
      fprintf(stderr,"\n");
      fprintf(stderr,"Merge two data tables (v1.0 2012-0-07)\n");
      fprintf(stderr,"Author: Mischa Schirmer\n\n");
      fprintf(stderr,"Usage: mergedat \n");
      fprintf(stderr,"       -i input catalog 1\n");
      fprintf(stderr,"       -j input catalog 2\n");
      fprintf(stderr,"      [-k input catalog 3]\n");
      fprintf(stderr,"      [-l input catalog 4]\n");
      fprintf(stderr,"       -o output catalog\n");
  }

  exit (0);
}

//********************************************************  
int main(int argc, char *argv[])
{
  FILE *input_file1, *input_file2, *input_file3, *input_file4, *output_file;
  long i, n1, n2, n3, n4, buf=1000;
  int flag_3 = 0, flag_4 = 0;
  char dummy1[buf], dummy2[buf], dummy3[buf], dummy4[buf];
  char infile1[100], infile2[100], infile3[100], infile4[100], outfile[100];

  // print usage if no arguments were given

  if (argc==1) usage(0);

  //  Read command line arguments
  
  for (i=1; i<argc; i++) {
    if (argv[i][0] == '-') {
      switch((int)argv[i][1]) {
      case 'i': strcpy(infile1,argv[++i]);
	break;
      case 'j': strcpy(infile2,argv[++i]);
	break;
      case 'k': strcpy(infile3,argv[++i]);
	flag_3 = 1;
	break;
      case 'l': strcpy(infile4,argv[++i]);
	flag_4 = 1;
	break;
      case 'o': strcpy(outfile,argv[++i]);
	break;
      }
    }
  }

  // open the files
  if ((input_file1 = fopen(infile1, "r")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", infile1);
  }
  if ((input_file2 = fopen(infile2, "r")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", infile2);
  }
  if (flag_3 && (input_file3 = fopen(infile3, "r")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", infile3);
  }
  if (flag_4 && (input_file4 = fopen(infile4, "r")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", infile4);
  }
  if ((output_file = fopen(outfile, "w")) == NULL) {
    fprintf(stderr, "Cannot open %s\n", outfile);
  }
  
  // how many elements are there
  i = 0;
  while(fgets(dummy1,buf,input_file1) != NULL) {
    i++;
  }
  n1 = i;

  i = 0;
  while(fgets(dummy2,buf,input_file2) != NULL) {
    i++;
  }
  n2 = i;
  
  if (flag_3) {
    i = 0;
    while(fgets(dummy3,buf,input_file3) != NULL) {
      i++;
    }
    n3 = i;
  }

  if (flag_4) {
    i = 0;
    while(fgets(dummy4,buf,input_file4) != NULL) {
      i++;
    }
    n4 = i;
  }  

  if (n1 != n2) {
    fprintf(stderr, "ERROR: Input files don't have same number of rows (%ld, %ld)!\n", n1, n2);
    exit (1);
  }

  if (flag_3 && n1 != n2 && n1 != n3) {
    fprintf(stderr, "ERROR: Input files don't have same number of rows (%ld, %ld, %ld)!\n", n1, n2, n3);
    exit (1);
  }

  if (flag_3 && flag_4 && n1 != n2 && n1 != n3 && n1 != n4) {
    fprintf(stderr, "ERROR: Input files don't have same number of rows (%ld, %ld, %ld, %ld)!\n", n1, n2, n3, n4);
    exit (1);
  }

  rewind(input_file1);
  rewind(input_file2);
  if (flag_3) rewind(input_file3);
  if (flag_4) rewind(input_file4);

  // do the merging
  i = 0;
  for (i=0; i<n1; i++) {
    if (fgets(dummy1,buf,input_file1) == NULL) {
      fprintf(stderr, "ERROR: Cannot read from %s!\n", infile1);
      exit (1);
    }
    if (fgets(dummy2,buf,input_file2) == NULL) {
      fprintf(stderr, "ERROR: Cannot read from %s!\n", infile2);
      exit (1);
    }
    if (flag_3) {
      if (fgets(dummy3,buf,input_file3) == NULL) {
	fprintf(stderr, "ERROR: Cannot read from %s!\n", infile3);
	exit (1);
      }
    }
    if (flag_4) {
      if (fgets(dummy4,buf,input_file4) == NULL) {
	fprintf(stderr, "ERROR: Cannot read from %s!\n", infile4);
	exit (1);
      }
    }

    if (!flag_3 && !flag_4) fprintf(output_file, "%s %s\n", trimwhitespace(dummy1), trimwhitespace(dummy2));
    if (flag_3 && !flag_4) fprintf(output_file, "%s %s %s\n", trimwhitespace(dummy1), trimwhitespace(dummy2), trimwhitespace(dummy3));
    if (flag_3 && flag_4) fprintf(output_file, "%s %s %s %s\n", trimwhitespace(dummy1), trimwhitespace(dummy2), 
				  trimwhitespace(dummy3), trimwhitespace(dummy4));
  }
  
  fclose(input_file1);
  fclose(input_file2);
  if (flag_3) fclose(input_file3);
  if (flag_4) fclose(input_file4);
  fclose(output_file);
  
  exit (0);  
}
