 /*
 				prefs.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	WeightWatcher
*
*	Author:		E.BERTIN (IAP, Leiden observatory & ESO)
*
*	Contents:	Keywords for the configuration file.
*
*	Last modify:	06/11/97
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

/*
  25.03.2007:
  I introduced the new command line parameter 'POLY_INTERSEC'. If 'Y'
  areas within overlapping polygons are masked; before those areas were
  not masked. This behaviour is active if 'POLY_INTERSEC' is 'N'.
  The default value is 'POLY_INTERSEC=Y'.

  The code changes are mostly a transfer from Emmanuel Bertin's v1.7
  of the weightwatcher software.
 */

/*----------------------------- Internal constants --------------------------*/

#define	MAXLIST		32		/* max. nb of list members */

/* NOTES:
One must have:	MAXLIST >= 1 (preferably >= 16!)
*/
/*-------------------------------- initialization ---------------------------*/
int	idummy;

pkeystruct key[] =
 {
  {"WEIGHT_NAMES", P_STRINGLIST, prefs.weight_name, 0,0,0.0,0.0,
    {""},0,MAXLIST, &prefs.nweight_name},
  {"WEIGHT_MIN", P_FLOATLIST, prefs.weight_threshd, 0,0,-BIG,BIG,
    {""}, 0, MAXLIST, &prefs.nweight_threshd},
  {"WEIGHT_MAX", P_FLOATLIST, prefs.weight_threshu, 0,0,-BIG,BIG,
    {""}, 0, MAXLIST, &prefs.nweight_threshu},
  {"WEIGHT_OUTFLAGS", P_INTLIST, prefs.weight_mask, 0,1<<30,0.0,0.0,
    {""}, 0, MAXLIST, &prefs.nweight_mask},
  {"FLAG_NAMES", P_STRINGLIST, prefs.flag_name, 0,0,0.0,0.0,
    {""},0,MAXLIST, &prefs.nflag_name},
  {"FLAG_WMASKS", P_INTLIST, prefs.flag_wmask, 0,1<<30,0.0,0.0,
    {""}, 0, MAXLIST, &prefs.nflag_wmask},
  {"FLAG_MASKS", P_INTLIST, prefs.flag_fmask, 0,1<<30,0.0,0.0,
    {""}, 0, MAXLIST, &prefs.nflag_fmask},
  {"FLAG_OUTFLAGS", P_INTLIST, prefs.flag_ofmask, 0,1<<30,0.0,0.0,
    {""}, 0, MAXLIST, &prefs.nflag_ofmask},
  {"POLY_NAMES", P_STRINGLIST, prefs.vec_name, 0,0,0.0,0.0,
    {""},0,MAXLIST, &prefs.nvec_name},
  {"POLY_OUTFLAGS", P_INTLIST, prefs.vec_mask, 0,1<<30,0.0,0.0,
    {""}, 0, MAXLIST, &prefs.nvec_mask},
  {"POLY_INTERSECT", P_BOOL, &prefs.intersec, 0, 0,0.0,0.0,
    {""}, 1, 1, &idummy},
  {"OUTWEIGHT_NAME", P_STRINGLIST, &prefs.oweight_name, 0,0,0.0,0.0,
    {""},0,1, &idummy},
  {"OUTFLAG_NAME", P_STRINGLIST, &prefs.oflag_name, 0,0,0.0,0.0,
    {""},0,1, &idummy},
  {"MEMORY_BUFSIZE", P_INT, &prefs.mem_bufsize, 1,1<<30},
  {"VERBOSE_TYPE", P_KEY, &prefs.verbose_type, 0,0, 0.0,0.0,
   {"QUIET","NORMAL","FULL",""}},
  {""}
 };
 
char			keylist[sizeof(key)/sizeof(pkeystruct)][16];
static const char	notokstr[] = {" \t=,;\n\r\""};

char *default_prefs[] =
 {
  "MEMORY_BUFSIZE	16",
  "VERBOSE_TYPE		NORMAL",
  "POLY_INTERSECT	Y",
  ""
 };

