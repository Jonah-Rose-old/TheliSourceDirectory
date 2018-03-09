/**
 ** miscellaneous service routines
 **/
 
/*
  09.04.2005:
  function error_exit:
  I added information to the error message printed
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "error.h"

void	error_exit(char *message)
{
        fprintf(stderr, "Error: ");
	fprintf(stderr, message);
	fprintf(stderr, "\nAborting program !!\n");
	exit(1);
}


