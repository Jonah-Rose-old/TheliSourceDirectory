 /*
 				main.c

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	WeightWatcher
*
*	Author:		E.BERTIN (IAP, Leiden observatory & ESO).
*
*	Contents:	parsing.
*
*	Last modify:	24/07/97
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#include	<ctype.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#include	"define.h"
#include	"globals.h"

#define		SYNTAX \
"ww [-c <configuration_file>] [-<keyword> <value>]"

/********************************** main ************************************/
int	main(int argc, char *argv[])

  {
   int		a, narg;
   char		**argkey, **argval;

  if (argc<2)
    {
    fprintf(OUTPUT, "\n		%s  Version %s\n", BANNER, VERSION);
    fprintf(OUTPUT, "		(%s)\n", __PIPEVERS__);
    fprintf(OUTPUT, "\nFor information, please contact: %s\n", COPYRIGHT);
    error(EXIT_SUCCESS, "SYNTAX: ", SYNTAX);
    }

  QMALLOC(argkey, char *, argc);
  QMALLOC(argval, char *, argc);

/*default parameters */

  strcpy(prefs.prefs_name, "default.ww");
  narg = 0;

  for (a=1; a<argc; a++)
    {
    if (argv[a][0] == '-' && a<(argc-1))
      {
      if (strlen(argv[a])<3)
        switch((int)tolower((int)argv[a][1]))
          {
          case 'c':	strcpy(prefs.prefs_name, argv[++a]);
			break;
          default:	error(EXIT_SUCCESS,"SYNTAX: ", SYNTAX);
          }
      else
        {
        argkey[narg] = &argv[a][1];
        argval[narg++] = argv[++a];
        }       
      }
    else
      error(EXIT_SUCCESS,"SYNTAX: ", SYNTAX);
    }

  readprefs(prefs.prefs_name, argkey, argval, narg);
  free(argkey);
  free(argval);

  makeit();

  NFPRINTF(OUTPUT, "All done");
  NPRINTF(OUTPUT, "\n");

  exit(EXIT_SUCCESS);
  }
