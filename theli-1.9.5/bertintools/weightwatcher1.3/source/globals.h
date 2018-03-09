 /*
 				globals.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	WeightWatcher
*
*	Author:		E.BERTIN (IAP, Leiden observatory & ESO)
*
*	Contents:	global declarations.
*
*	Last modify:	25/09/2000
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

/*
  29.01.2005:
  I included a definition for the utility routine get_basename
*/

#include	"types.h"

/*----------------------- miscellaneous variables ---------------------------*/

prefstruct		prefs;
char			gstr[MAXCHAR];
int			bswapflag;

/*------------------------------- functions ---------------------------------*/
extern void	error(int, char *, char *),
		makeit(void),
		readdata(picstruct *, PIXTYPE *, int),
		readidata(picstruct *, FLAGTYPE *, int),
		readimagehead(picstruct *),
		readprefs(char *, char **, char **, int),
		swapbytes(void *, int, int),
		useprefs(void),
		warning(char *, char *);

extern int	cistrcmp(char *, char *, int);

extern char	*readfitshead(FILE *, char *, int *);

extern picstruct	*newfield(char *, int, picstruct *);

extern char             *get_basename(const char *filename);
