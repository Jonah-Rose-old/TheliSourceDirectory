#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

void usage(int i)
{
  if (i == 0) {
      fprintf(stderr,"\n");
      fprintf(stderr,"Prints the word \"ERROR\" if the first argument is zero.\n\n");
      fprintf(stderr,"Date:  2011-03-15, Author: Mischa Schirmer\n\n");
      fprintf(stderr,"Usage: errtest <number>\n");
  }

  exit (0);
}

//************************************************************

int main(int argc, char *argv[])
{
  // print usage if no arguments were given
  if (argc==1) usage(0);

  //  Read command line argument
  if (atoi(argv[1]) == 0)
    printf("ERROR: \n");

  exit(0);
}
