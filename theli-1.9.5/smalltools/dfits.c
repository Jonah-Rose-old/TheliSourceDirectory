
/*---------------------------------------------------------------------------
                        European Southern Observatory
 ----------------------------------------------------------------------------
   File name    :   dfits.c
   Author       :   Nicolas Devillard
   Created on   :   30 Mar 2000
   Language     :   ANSI C
   Description  :   FITS header display

   Initial version from 1996.
   Rewritten from scratch to support FITS extensions.
 --------------------------------------------------------------------------*/

/*

 $Id: dfits.c,v 1.3 2004/11/01 21:02:20 terben Exp $
 $Author: terben $
 $Date: 2004/11/01 21:02:20 $
 $Revision: 1.3 $
 $Log: dfits.c,v $
 Revision 1.3  2004/11/01 21:02:20  terben
 01.11.2004:
 solved problems with too lonmg strings for the filename
 in a subsequent call ofitsort:
 - For the filename, now the basename is given (instead of
   the filename with the whole path)
 - Printing the filename we now ensure that the string is
   not longer than 128 characters

 Revision 1.2  2004/02/05 13:33:31  terben
 function dump_fits_filter:
 The buffer reading the fits lines has now a 0 at the 80th
 position before being passed to rstrip. Not doing this led
 sometimes to binary output at the end of the FITS cards.

 Revision 1.1  2001/03/13 15:55:33  erben
 first checkin of atandalone external utility programs
 (they consist of single C-files that can be compiled standalone
 without the need for any library etc.)

 Revision 1.5  2000/03/30 15:19:54  ndevilla
 cleaned out unused variables

 Revision 1.4  2000/03/30 15:11:39  ndevilla
 rewritten from scratch to support extensions

 05.02.2004:
 function dump_fits_filter:
 The buffer reading the fits lines has now a 0 at the 80th
 position before being passed to rstrip. Not doing this led 
 sometimes to binary output at the end of the FITS cards.

 01.11.2004:
 solved problems with too lonmg strings for the filename
 in a subsequent call ofitsort:
 - For the filename, now the basename is given (instead of
   the filename with the whole path)
 - Printing the filename we now ensure that the string is
   not longer than 128 characters
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LGTH    	80
#define MAGIC   	"SIMPLE  ="


void	usage(char * pname);
void	parse_cmd_line(int, char **, int*, int*, int*);
int		dump_fits_filter(FILE*, int);
int		dump_fits(char *, int);
char *	rstrip(char *);
char * get_basename(const char *);


/*--------------------------------------------------------------------------*/

int main(int argc, char *argv[])
{
	int		xtnum ;
	int		c_arg ;
	int		filter ;
	int		err ;

	/* No arguments prints out a usage message */
	if (argc<2) {
		usage(argv[0]);
		return 1 ;
	}

	/* Parse command-line options */
	parse_cmd_line(argc, argv, &xtnum, &filter, &c_arg);

	/* Filter mode: process data received from stdin */
	if (filter)
		return dump_fits_filter(stdin, xtnum);

	/* Normal mode: loop on all file names given on command-line */
	err = 0 ;
	while (c_arg < argc) {
		err += dump_fits(argv[c_arg], xtnum);
		c_arg++;
	}
	return err ; /* Returns number of errors during process */
}

void usage(char * pname)
{
	printf("\n\n");
	printf("usage: %s [-x xtnum] <list of FITS files>\n", pname);
	printf("usage: %s [-x xtnum] -\n", pname);
	printf("\n");
	printf("The former version expects file names.\n");
	printf("The latter expects data coming in from stdin.\n");
	printf("\n");
	printf("-x xtnum specifies the extension header to print\n");
	printf("-x 0     specifies main header + all extensions\n");
	printf("\n\n");
}

void parse_cmd_line(
	int		argc,
	char ** argv,
	int *	xtnum,
	int *	filter,
	int *	c_arg
)
{
	*filter = 0;
	*xtnum  = -1 ;
	*c_arg  = argc-1 ;

	/* If '-' is on the command-line, it must be the last argument */
	if (!strcmp(argv[argc-1], "-")) {
		*filter = 1 ;
	}
	/*
	 * If -x xtnum is on the command-line, it must be the first two
	 * arguments
	 */
	if (!strcmp(argv[1], "-x")) {
		*xtnum = atoi(argv[2]);
		*c_arg = 3 ;
	} else {
		*c_arg = 1 ;
	}
	return ;
}

/*
 * Strip off all blank characters in a string from the right-side.
 */
char * rstrip(char * s)
{
    int len ;
    if (s==NULL) return s ;
    len = strlen(s);
    if (len<1) return s ;
    len -- ;
    while (s[len]== ' ') {
        s[len]=(char)0 ;
        len --;
    }
    return s ;
}

/*
 * Dump the requested header (main or extension) from a filename.
 */
int dump_fits(char * name, int xtnum)
{
	FILE	*	in ;
	int			err ;

	if ((in=fopen(name, "r"))==NULL) {
		fprintf(stderr, "error: cannot open file [%s]\n", name);
		return 1 ;
	}

	printf("====> file %.100s (main) <====\n", get_basename(name)) ;
	err = dump_fits_filter(in, xtnum);
	fclose(in);
	return err ;
}

/*
 * Dump the requested header (main or extension) from a FILE *
 */
int dump_fits_filter(FILE * in, int xtnum)
{
	int		n_xt ;
	char	buf[LGTH+1];
	int		err ;

	/* Try getting the first 80 chars */
	if (fread(buf, sizeof(char), LGTH, in)!=LGTH) {
		fprintf(stderr, "error reading input\n");
		return 1;
	}
	/* Check that it is indeed FITS */
	if (strncmp(buf, MAGIC, strlen(MAGIC))) {
		fprintf(stderr, "not a FITS file\n");
		return 1 ;
	}
	if (xtnum<1) {
		/* Output main header */
		buf[LGTH]=(char)0;
		printf("%s\n", rstrip(buf));
		while ((err=fread(buf, sizeof(char), LGTH, in))==LGTH) {
			buf[LGTH]=(char)0;
			printf("%s\n", rstrip(buf));
			if (buf[0]=='E' &&
				buf[1]=='N' &&
				buf[2]=='D') {
				break ;
			}
		}
		if (err!=LGTH) /* Read error */
			return 1 ;
	}
	if (xtnum<0)
		return 0 ;

	n_xt=0 ;
	while (1) {
		/* Look for next XTENSION keyword */
		while ((err=fread(buf, sizeof(char), LGTH, in))==LGTH) {
			if (buf[0]=='X' &&
				buf[1]=='T' &&
				buf[2]=='E' &&
				buf[3]=='N' &&
				buf[4]=='S' &&
				buf[5]=='I' &&
				buf[6]=='O' &&
				buf[7]=='N') break ;
		}
		if (err==0)	/* Nothing more to read */
			break ;

		if (err!=LGTH)	/* Read error */
			return 1 ;

		n_xt++ ;
		if (xtnum==0 || xtnum==n_xt) {
			printf("====> xtension %d\n", n_xt) ;
			buf[LGTH]=(char)0;
			printf("%s\n", rstrip(buf));
			while ((err=fread(buf, sizeof(char), LGTH, in))==LGTH) {
				buf[LGTH]=(char)0;
				printf("%s\n", rstrip(buf));
				if (buf[0]=='E' &&
					buf[1]=='N' &&
					buf[2]=='D') break ;
			}
		}
		if (n_xt==xtnum)
			break ;
	}
	return 0 ;
}

/*
 * Find out the base name of a file (i.e. without prefix path)
 */

char * get_basename(const char *filename)
{
  char *p ;
  p = strrchr (filename, '/');
  return p ? p + 1 : (char *) filename;
}
