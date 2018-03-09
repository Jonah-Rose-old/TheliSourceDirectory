
/*----------------------------------------------------------------------------
   
   File name    :   mjd.c
   Author       :   Thomas Erben
   Created on   :   30.05.2004
   Description  :   transform civil date to Julian and Modified Julian Date 

 ---------------------------------------------------------------------------*/

/*
	$Id: mjd.c,v 1.3 2006-05-01 23:19:47 terben Exp $
	$Author: terben $
	$Date: 2006-05-01 23:19:47 $
	$Revision: 1.3 $
*/

/*----------------------------------------------------------------------------
                                History Coments
 ---------------------------------------------------------------------------*/

/*
   12.01.2005:
   I added printout of MJD. So far, only JD was given.
*/

/*----------------------------------------------------------------------------
                                Includes and Defines
 ---------------------------------------------------------------------------*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifndef MAXSTRINGLENGTH
#define MAXSTRINGLENGTH 256
#endif

#define __USAGE__ "\
\n\
USAGE:\n\
       mjd     -t time (hh:mm:ss.ss)\n\
               -d date (DD/MM/YYYY) \n\
  OR\n\
       mjd     -f DATE-OBS (YYYY-MM-DDThh:mm:ss, FITS convention)\n\n\
DESCRIPTION:\n\
The program mjd calculates and prints to screen the Julian \n\
Date (JD) and Modified Julian Date (MJD) of a given calendar\n\
date. The calendar date has to be provided at Universal Time\n\
(UT).\n\n\
EXAMPLES:\n\
    mjd -t 18:00:37 -d 20/08/2001\n\
    mjd -f 2001-08-20T18:00:37\n\n\
AUTHOR:\n\
      Thomas Erben\n\
      Mischa Schirmer\n\n"

/*----------------------------------------------------------------------------
                            Main code
 ---------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
  char date[MAXSTRINGLENGTH];
  char time[MAXSTRINGLENGTH];
  char dateobs[MAXSTRINGLENGTH];
  double month=0.0, day=0.0, year=0.0;
  double hour=0.0, min=0.0;
  double sec=0.0, timefrac=0.0;
  double julian_date=0.0, modified_julian_date=0.0;
  double A, B, C, D, E, F;
  int i, dateobsgiven = 0;
  char *dummy, *date_do, *time_do;

  /* read command line parameters */

  if(argc < 2)
  {
    printf("%s", __USAGE__);
    exit(1);
  }

  for (i=1; i<argc; i++)
  {
    if (argv[i][0] == '-')
    {
       switch(tolower((int)argv[i][1]))
       {
         case 't': strncpy(time, argv[++i], MAXSTRINGLENGTH-1);
                   break;
         case 'd': strncpy(date, argv[++i], MAXSTRINGLENGTH-1);
                   break;
         case 'f': strncpy(dateobs, argv[++i], MAXSTRINGLENGTH-1);
	           dateobsgiven = 1;
                   break;
         default:
                   printf("Wrong command line syntax !!\n %s", __USAGE__);
                   exit(1);
                   break;
       }
    }
    else
    {
      printf("Wrong command line syntax !!\n %s", __USAGE__);
      exit(1);
    }
  }            

  /* decode date and time string; date is assumed to be
     DD/MM/YYYY and time hh:mm:ss.ss 
  */

  if (dateobsgiven == 0) {
      if((dummy = strtok(date, "/")) != NULL)
	  day   = atof(dummy);
      if((dummy = strtok(NULL, "/")) != NULL)
	  month = atof(dummy);
      if((dummy = strtok(NULL, "/")) != NULL)
	  year  = atof(dummy);
      
      if((dummy = strtok(time, ":")) != NULL)
	  hour  = atof(dummy);
      if((dummy = strtok(NULL, ":")) != NULL)
	  min   = atof(dummy);
      if((dummy = strtok(NULL, ":")) != NULL)
	  sec   = atof(dummy);
  }
  else {
      date_do = strtok(dateobs, "T");
      time_do = strtok(NULL, "T");
      if (date_do != NULL) {
	  if((dummy  = strtok(date_do, "-")) != NULL)
	      year  = atof(dummy);
	  if((dummy = strtok(NULL, "-")) != NULL)
	      month = atof(dummy);
	  if((dummy = strtok(NULL, "-")) != NULL)
	      day   = atof(dummy);
      }
      if (time_do != NULL) {
	  if((dummy  = strtok(time_do, ":")) != NULL)
	      hour  = atof(dummy);
	  if((dummy = strtok(NULL, ":")) != NULL)
	      min   = atof(dummy);
	  if((dummy = strtok(NULL, ":")) != NULL)
	      sec   = atof(dummy);
      }
  }


  /* for an explanation of the formulas below visit
     http://aa.usno.navy.mil/faq/docs/JD_Formula.html
  */
  timefrac = hour + min / 60.0 + sec / 3600.0;

  A = year * 367.0;
  B = floor((month + 9.0) / 12.0);
  C = floor(((year + B) * 7.0) / 4.0);
  D = floor((275.0 * month) / 9.0);
  E = day + 1721013.5 + (timefrac / 24.0);
  F = (((100.0 * year) + month - 190002.5) >= 0) ? 1.0 : -1.0;
  julian_date = A - C + D + E - (0.5 * F) + 0.5;
  modified_julian_date=julian_date - 2400000.5;

  printf("Julian Date: %f, Modified Julian Date: %f\n", 
         julian_date, modified_julian_date);

  return 0;
}
