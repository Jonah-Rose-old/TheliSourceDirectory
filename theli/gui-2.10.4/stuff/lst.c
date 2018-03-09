/*----------------------------------------------------------------------------
   
   File name    :   lst.c
   Author       :   Thomas Erben
   Created on   :   30.05.2004
   Description  :   calculate the Local Siderial Time for a given date/time.

 ---------------------------------------------------------------------------*/

/*
	$Id: lst.c,v 1.4 2006-05-19 13:43:14 terben Exp $
	$Author: terben $
	$Date: 2006-05-19 13:43:14 $
	$Revision: 1.4 $
*/

/*----------------------------------------------------------------------------
                                History Coments
 ---------------------------------------------------------------------------*/

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
USAGE:\n\
       lst         -t time (hh:mm:ss.ss)\n\
                   -d date (DD/MM/YYYY) \n\
                   -l longitude of the observation site in deg (West counts negative!)\n\n\
  OR\n\
       mjd     -f DATE-OBS (YYYY-MM-DDThh:mm:ss, FITS convention)\n\n\
DESCRIPTION:\n\
The program lst calculates the Local Siderial Time for a given\n\
date/time (that has to be given in UT !!!) at an observing site\n\
at a specified longitude. The obtained LST value is accurate up\n\
to a few seconds up to 2099 (what is completely sufficient for\n\
my lifetime)!!!!\n\n\
EXAMPLE:\n\
  lst -t 04:11:29.52 -d 29/08/2000 -l -70.73333\n\
  lst -f 2001-08-20T18:00:37 -l -70.73333\n\n\
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
  float day=0., month=0., year=0.;
  float hour=0., min=0., sec=0., ut;
  float longitude = -70.73333; /* this is La Silla */

  float dwhole, dfrac, d;

  float lst;
  int i, idummy, dateobsgiven = 0;;
  char *cdummy;
  char *dummy, *date_do, *time_do;

  /* read command line parameters */

  if(argc < 5)
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
         case 'l': longitude=atof(argv[++i]);
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
    if((cdummy = strtok(date, "/")) != NULL)
      day   = atof(cdummy);
    if((cdummy = strtok(NULL, "/")) != NULL)
      month = atof(cdummy);
    if((cdummy = strtok(NULL, "/")) != NULL)
      year  = atof(cdummy);
    
    if((cdummy = strtok(time, ":")) != NULL)
      hour  = atof(cdummy);
    if((cdummy = strtok(NULL, ":")) != NULL)
      min   = atof(cdummy);
    if((cdummy = strtok(NULL, ":")) != NULL)
      sec   = atof(cdummy);
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

  /*get the UT in decimal hours */
  ut = hour+(min/60.)+(sec/3600.);

  /* The formulas used for the following calcuations 
     were obtained from: http://www.xylem.f2s.com/kepler/index.html#top
  */

  /* get the complete and fractional days from J2000 */
  dwhole = 367*year-(int)(7*(year+(int)((month+9)/12))/4)+
           (int)(275*month/9)+day-730531.5;

  dfrac = (ut)/24.;

  d = dwhole + dfrac;

  lst = 100.46 + 0.985647*d + longitude + 15.*ut;  

  /* bring the LST in the range of 0 to 360 degrees */
  idummy=(int)(lst/360.);
  
  if(lst > 0)
  {
    lst=lst-(float)(idummy*360.);
  }
  else
  {
    lst=lst-(float)((idummy-1)*360.);
  }

  /* bring LST from degrees to seconds */
  lst = (lst/15.)*3600.;

  printf("LST: %f\n", lst);

  return 0;
}



