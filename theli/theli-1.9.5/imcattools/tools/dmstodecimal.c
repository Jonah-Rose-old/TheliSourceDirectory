/*-------------------------------------------------------------------------*/
/**
  @file     dmstodecimal.c
  @author   Nick Kaiser, Thomas Erben
  @date     2002
  @version  $Revision: 1.3 $
  @brief    converts an angle from sexagesimal (d:m:s) to decimal notation

  The programs main intention is to convert the declination from
  astronomical coordinates into decimal notation.
*/
/*--------------------------------------------------------------------------*/

/*
	$Id: dmstodecimal.c,v 1.3 2006/11/06 19:46:40 thomas Exp $
	$Author: thomas $
	$Date: 2006/11/06 19:46:40 $
	$Revision: 1.3 $
*/

/*----------------------------------------------------------------------------
                                History Coments
 ---------------------------------------------------------------------------*/

/*
  14.10.2006:
  I extended the program to also accept an input file with
  coordinates. 

  30.10.2006:
  We can give stdin as input file ('-' as filename)
*/

/*---------------------------------------------------------------------------
   								Includes
 ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#include "error.h"
#include "radecio.h"

/*---------------------------------------------------------------------------
   								Defines
 ---------------------------------------------------------------------------*/

#ifndef MAXSTRINGLENGTH
#define MAXSTRINGLENGTH 256
#endif

/** usage information */
#define usage "\n\
NAME\n\
	dmstodecimal --- convert an angle from d:m:s to decimal degrees format\n\
\n\
SYNOPSIS\n\
	dmstodecimal angle or\n\
        dmstodecimal -f inputfile\n\
\n\
DESCRIPTION\n\
	dmstodecimal takes a string as argument; decipers it\n\
	using \"%%d:%%d:%%lf\" format specification to d, m, s;\n\
	checks that m, s lie in range 0-60 and writes the \n\
	angle\n\
		theta = (sign(d)) * (fabs(d) + m / 60.0 + s / 3600.0)\n\
	to standard output.\n\
        As an alternative an input file having one coordinate per line\n\
        can be given as input. '-' as filename means stdin.\n\
\n\
AUTHOR\n\
	Nick Kaiser	kaiser@hawaii.edu\n\
\n\n"


/*-----------------------------------------------------------------------------
                                Main
 -----------------------------------------------------------------------------*/
int main (int argc, char *argv[])
{
	double	 angle;
	FILE    *inputfile = stdin;
	char     tmpstring[MAXSTRINGLENGTH-1];

	if (argc < 2) 
        {
	  error_exit(usage);
	}
	if (argv[1][0] == '-' && argv[1][1] == 'u') 
        {
	  error_exit(usage);
	}

	/* only one coordinate given on the command line */
	if(argc == 2)
	{
	  if (!xmstodecimal(argv[1], &angle)) 
          {
	    error_exit(usage);
	  }
	  fprintf(stdout, "%13.8f\n", angle);
	}
	else /* a file with coordinates was given */
	{
	  if(strcmp(argv[1], "-f") != 0)
	  {
	    error_exit(usage);
	  }
	  else
	  {
	    if(strcmp(argv[2], "-") != 0)
	    {
	      if((inputfile = fopen(argv[2], "r")) == NULL)
	      {
	        error_exit("error opening input file!\n");
	      }
	    }
	    while(fgets((char *)tmpstring, MAXSTRINGLENGTH-1, inputfile))
	    {
	      if (!xmstodecimal(tmpstring, &angle)) 
              {
	        error_exit("error while processing input file");
	      }
	      fprintf(stdout, "%13.8f\n", angle);		
	    }
	    if(strcmp(argv[2], "-") != 0)
	    {
	      fclose(inputfile);
	    }
	  }
	}

	return 0;
}
