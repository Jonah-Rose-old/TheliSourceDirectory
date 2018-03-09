/*
 				fitscat.c

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	The LDAC Tools
*
*	Author:		E.BERTIN, DeNIS/LDAC
*
*	Contents:	low-level functions for handling LDAC FITS catalogs.
*
*	Last modify:	25/04/97
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>

#include	"fitscat_defs.h"
#include	"fitscat.h"
#include        "global_vers.h"

/* 15.07.01:
 * changed type access_type to access_type_def in 
 * the open_cat definition to relfect changes in
 * fitscat.h
 *
 * 09.12.01:
 * test for a null pointer in the close_cat function 
 * before trying to close a file
 *
 * 30.01.2005:
 * function addhistoryto_cat:
 * the function now has an additional argument
 * that indicates whether HISTORY entries should have
 * a timestamp or not.
 *
 * new funtion historyto_cat:
 * the function adds information on a program call
 * as HISTORY information to an output catalog generated
 * by that program.
 *
 * funtion historyto_cat:
 * I corrected an error in the HISTORY line where the
 * program name was not treated correctly.
 *
 * 04.06.2007:
 * Changes to include Large File Support to the LDAC tools
 * On 32-bit systems calls to fseek are changed to fseeko
 * which takes an argument of type 'off_t' as offset argument.
 * This argument can be turned into a 64 bit type with compiler
 * flags (_FILE_OFFSET_BITS). Similarily the command fseek is
 * changed to fseeko on 32-bit machines. The types of concerned
 * variables are changed accordingly.
 *
 * 13.08.2008:
 * I changed the prototype for the function historyto_cat (second
 * argument goes from char * to const char *) to avoid
 * a C++ warning.
 *
 * 24.08.2010:
 * I removed some remarks raised by the Intel icc compiler:
 * - I test for _FILE_OFFSET_BITS being defined now tih '#ifdef' instead
 *   of '#if'.
 * - I removed unused variables
 *
 * 26.08.2010:
 * I corrected a bug (I deleted too much) 
 * introduced with the changes on 24.08.
 *
 * 03.09.2010:
 * The time put into the HISTORY of catalogues was in GMT.
 * I changed that to the local time.
 */

/****** about_cat **************************************************************
PROTO	int about_cat(catstruct *cat, FILE *stream)
PURPOSE	Print some info about a catalog.
INPUT	Catalog structure,
	output stream.
OUTPUT	RETURN_OK if everything went as expected, RETURN_ERROR otherwise.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	20/03/96
 ***/
int	about_cat(catstruct *cat, FILE *stream)

  {
   tabstruct	*tab;
   int		i;

  fprintf(stream,"\n");

/*General info about the catalog itself*/
  fprintf(stream,
	"------------------Catalog information----------------\n");
  fprintf(stream,
	"Filename:..............%s\n", cat->filename);
  fprintf(stream,
	"Number of segments:....%d\n", cat->ntab);
  fprintf(stream,"\n");

/*Now for each table*/
  tab = cat->tab;
  for (i=0; i<cat->ntab; i++)
    {
    fprintf(stream,
	"******	Table #%d\n", i+1);
    fprintf(stream,
	"	Extension type:.........%s\n",
	tab->xtension[0]? tab->xtension: "(Primary HDU)");
    fprintf(stream,
	"	Extension name:.........%s\n", tab->extname);
    if (tab->naxis)
      {
      fprintf(stream,
	"	Number of dimensions:...%d\n", tab->naxis);
      fprintf(stream,
	"	Number of elements:.....%d\n", tab->naxisn[1]);
      if (tab->tfields)
        fprintf(stream,
	"	Number of data fields...%d\n", tab->tfields);
#ifdef _FILE_OFFSET_BITS
      fprintf(stream,
	"	Body size:..............%lld bytes\n", tab->tabsize);
#else
      fprintf(stream,
	"	Body size:..............%ld bytes\n", tab->tabsize);
#endif
      }
    fprintf(stream,"\n");
    while (!(tab=tab->nexttab)->nseg);
    }

  fprintf(stream,"\n");

  return RETURN_OK;
  }

/****** addhistoryto_cat *******************************************************
PROTO	int addhistoryto_cat(catstruct *cat, char *str)
PURPOSE	Add a HISTORY line to a FITS catalog.
INPUT	A pointer to catalog structure, the character string to insert, and a bool
        indicating whether the history should have a timestamp.
OUTPUT	RETURN_OK if everything went as expected, RETURN_ERROR otherwise.
NOTES	The pointer to the primary header might be reallocated if necessary.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	20/03/96
 ***/

int	addhistoryto_cat(catstruct *cat, char *str, int timestamp)

  {
   static time_t	thetime;
   static char		str2[82];
   tabstruct		*tab;
   int			i, l, n, headpos;

  tab = cat->tab;
  
  n = tab->headnblock;
  headpos = fitsfind(tab->headbuf, "END     ");
  if (headpos >= n*(FBSIZE/80) - 1)
  {
    QREALLOC(tab->headbuf, char, (n+1)*FBSIZE);
    memset(&tab->headbuf[n*FBSIZE], ' ', FBSIZE);
    tab->headnblock++;
  }

  if(timestamp == 1)
  {
    if (time(&thetime)==-1)
      warning("No time available for history","");

    if (!strftime(str2, 16, "%d/%m/%y %H:%M", localtime(&thetime)))
      error(EXIT_FAILURE, "*Internal Error*: Time/date string too long in ",
	  "addhistoryto_cat()");
    fitsadd(tab->headbuf, "HISTORY ", str2);
  }

  l = strlen(str);
  for (i=0;l-i*70>0;i++) {
    memset(str2, ' ', 70);
    strncpy(str2,&str[i*70],MIN(70,l-i*70));
    n = tab->headnblock;
    headpos = fitsfind(tab->headbuf, "END     ");
    if (headpos >= n*(FBSIZE/80) - 1)
    {
      QREALLOC(tab->headbuf, char, (n+1)*FBSIZE);
      memset(&tab->headbuf[n*FBSIZE], ' ', FBSIZE);
      tab->headnblock++;
    }
    fitsadd(tab->headbuf, "HISTORY ", str2);
  }

  return RETURN_OK;
  }


/****** close_cat **************************************************************
PROTO	int close_cat(catstruct *cat)
PURPOSE	Close a FITS catalog.
INPUT	catalog structure.
OUTPUT	RETURN_OK if everything went as expected, RETURN_ERROR otherwise.
NOTES	the file structure member is set to NULL;
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	20/03/96
 ***/
int	close_cat(catstruct *cat)

  {

    if ((cat->file != NULL))
    {
      if (fclose(cat->file))
        {
        cat->file = NULL;
        return RETURN_ERROR;
        }
    
      cat->file = NULL;
    }
    else
    {
      printf("\nWARNING: close_cat applied to NULL pointer !!!!\n");
    }
  return RETURN_OK;
  }


/****** free_cat ***************************************************************
PROTO	void free_cat(catstruct *cat, int ncat)
PURPOSE	Free all structures allocated for one or several FITS catalog.
INPUT	Pointer to a catalog structure,
	Number of catalogs.
OUTPUT	-.
NOTES	Unallocated pointers should have been put to NULL.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	25/04/97
 ***/
void	free_cat(catstruct *cat, int ncat)

  {
   catstruct	*thecat;
   int		i;

/*--free memory allocated within each catalog */
  thecat = cat;
  for (i=ncat; i--;)
    {
    if (thecat->file)
      close_cat(thecat);
    remove_tabs(thecat++);
    }

  free(cat);

  return;
  }


/****** historyto_cat *******************************************************
PROTO	int historyto_cat(catstruct *cat, char *programname, int argc, char **argv)
PURPOSE	Add HISTORY information on a program call to an output catalog.
INPUT	the output catalog, the name from the calling program, the number
        of command line arguments, the actual arguments 
OUTPUT	RETURN_OK if everything went as expected, RETURN_ERROR otherwise.
AUTHOR	T. Erben (IAEF Bonn)
VERSION	29/01/05
 ***/
int historyto_cat(catstruct *cat, const char *programname, 
                  int argc, char **argv)
{
    struct tm		*tm;
    time_t		thetime;
    char tmpstring[72];
    char *cline;
    int i, l;

    addhistoryto_cat(cat, " ", 0);
    snprintf(tmpstring, 71, "%s: %s", programname, __PIPEVERS__);
    addhistoryto_cat(cat, tmpstring, 0);

    /* get current date/time in ISO format */
    thetime = time(NULL);
    tm = localtime(&thetime);
    sprintf(tmpstring,"%s called at %04d-%02d-%02dT%02d:%02d:%02d",
            programname, tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
            tm->tm_hour, tm->tm_min, tm->tm_sec);
    addhistoryto_cat(cat, tmpstring, 0);

    /* build up command line from the given arguments 
       and add it as HISTORY */
    l=0;
    if(argc > 0 && argv != NULL)
    {
      l+=strlen(programname) + 1;

      for(i=1; i < argc; i++)
      {
        l += strlen(argv[i])+1;
      }
  
      snprintf(tmpstring, 71, "%s calling sequence:", programname);  
      addhistoryto_cat(cat, tmpstring, 0);

      /* build up full command line for the catalog HISTORY */
      QCALLOC(cline, char, l+1);
      strcpy(cline, programname);
      for (i=1; i<argc; i++) 
      {
        strcat(cline, " ");
        strcat(cline, argv[i]);
      }
      addhistoryto_cat(cat, cline, 0);
      free(cline);
    }

    return RETURN_OK;
}
/****** inherit_cat ************************************************************
PROTO	int inherit_cat(catstruct *catin, catstruct *catout)
PURPOSE	Copy the primary table, and all other informations from one catalog
	to another, except those related to the associated file itself
	(filename, etc...),
INPUT	A pointer to both catalog structures.
OUTPUT	RETURN_OK if at least one table was copied, RETURN_ERROR otherwise.
NOTES	The output catalog should be ``cleaned'' before call.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	25/04/96
 ***/
int	inherit_cat(catstruct *catin, catstruct *catout)

  {
   tabstruct	*tabin, *tabout, *prevtabout;
   int		j;

   catout->ntab = 1; 
   tabin = catin->tab;

/*copy only one table: well it could be simpler, but let's stay general!*/
  prevtabout = NULL;
  for (j=tabin->nseg; j--;)
    {
    QCALLOC(tabout, tabstruct, 1);
    *tabout = *tabin;
    if (tabin->naxis)
      QMEMCPY(tabin->naxisn, tabout->naxisn, int, tabin->naxis);
    if (tabin->headbuf)
      QMEMCPY(tabin->headbuf, tabout->headbuf, char,
	tabin->headnblock*FBSIZE);
    if (tabin->bodybuf)
      QMEMCPY(tabin->bodybuf, tabout->bodybuf, char, tabin->tabsize);
    if (prevtabout)
      {
      tabout->prevtab = prevtabout;
      prevtabout->nexttab = tabout;
      }
    else
      {
      catout->tab = tabout;
      }
    prevtabout = tabout;
    tabin = tabin->nexttab;
    }

  if (prevtabout)
    {
    prevtabout->nexttab = catout->tab;
    catout->tab->prevtab = prevtabout;
    }
  else
    return RETURN_ERROR;

  return RETURN_OK;
  }

/****** init_cat ***************************************************************
PROTO	int init_cat(catstruct *cat)
PURPOSE	Initialize a catalog, "cleaning" any content if present
	 and adding the primary header "table".
INPUT	A pointer to the catalog structure.
OUTPUT	RETURN_OK if everything went as expected, RETURN_ERROR otherwise.
NOTES	The output catalog should be ``cleaned'' before call.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	19/08/96
 ***/
int	init_cat(catstruct *cat)

  {
   static char	bintabtemplate[][80] = {
"SIMPLE  =                    T / LETS STAY SIMPLE",
"BITPIX  =                    8 / ",
"NAXIS   =                    0 / ",
"EXTEND  =                    T / MORE STUFF MAY FOLLOW",
"END                            "};
   tabstruct	*tab;
   char		*buf;
   int		i;

/* Initialize the primary header itself */
  QCALLOC(tab, tabstruct, 1);
  tab->naxis = 0;
  tab->bitpix = 8;
  tab->bytepix = 1;
  tab->pcount = 0;
  tab->gcount = 1;
  tab->seg = 1;
  tab->nseg = 1;
/* Provide a new header*/
  QCALLOC(tab->headbuf, char, FBSIZE);
  memcpy(tab->headbuf, bintabtemplate, sizeof(bintabtemplate));
  for (buf = tab->headbuf, i=0; i<FBSIZE; i++, buf++)
    if (!*buf)
      *buf = ' ';
  tab->headnblock = 1;
/* Clean catalog and add the table to it */
  remove_tabs(cat);
  cat->tab = tab->prevtab = tab->nexttab = tab;
  cat->ntab = 1;
  addhistoryto_cat(cat, "Catalog created", 1);

  return RETURN_OK;
  }


/****** map_cat ****************************************************************
PROTO	int map_cat(catstruct *cat)
PURPOSE	Explores the whole FITS file
	and gets information for each of the FITS tables it contains.
INPUT	catalog structure.
OUTPUT	RETURN_OK if at least one table was found, RETURN_ERROR otherwise.
NOTES	Memory space for the array of fits structures is reallocated.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	20/03/96
 ***/
int	map_cat(catstruct *cat)

  {
   int			ntab;
   tabstruct		*tab, *prevtab;

/*scan through the file until we reach the end*/
  prevtab = NULL;
  QCALLOC(tab, tabstruct, 1);
  tab->cat = cat;
  for (ntab=0; !get_head(tab); ntab++)
    {
    readbasic_head(tab);
    readbintabparam_head(tab);
    QFTELL(tab->bodypos, cat->file, cat->filename);
    tab->nseg = tab->seg = 1;
    if (tab->tabsize)
    {
      QFSEEK(cat->file, PADTOTAL(tab->tabsize), SEEK_CUR, cat->filename);
    }
    if (prevtab)
      {
      tab->prevtab = prevtab;
      prevtab->nexttab = tab;
      }
    else
      cat->tab = tab;
    prevtab = tab;
    QCALLOC(tab, tabstruct, 1);
    tab->cat = cat;
    }

  cat->ntab = ntab;
  free(tab);

  if (prevtab)
    {
    prevtab->nexttab = cat->tab;
    cat->tab->prevtab = prevtab;
    }
  else
    return RETURN_ERROR;

/*rewind to the beginning*/
/*
  QFSEEK(cat->file, 0, SEEK_SET, cat->filename);
*/

  return RETURN_OK;
  }


/****** new_cat ****************************************************************
PROTO	catstruct *new_cat(int ncat)
PURPOSE	Initialize a structure for a FITS catalog.
INPUT	Number of catalogs.
OUTPUT	A pointer to the catalog array.
NOTES	All fields are initialized to 0.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	20/03/96
 ***/
catstruct	*new_cat(int ncat)

  {
   catstruct	*cat;

  QCALLOC(cat, catstruct, ncat);

  cat->access_type = WRITE_ONLY;

  return cat;
  }


/****** open_cat ***************************************************************
PROTO	int open_cat(catstruct *cat, access_type at)
PURPOSE	Open a FITS catalog with name filename.
INPUT	catalog structure,
	access type (can be WRITE_ONLY or READ_ONLY).
OUTPUT	RETURN_OK if the cat is found, RETURN_ERROR otherwise.
NOTES	If the file was already opened by this catalog, nothing is done.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	22/08/96
 ***/
int	open_cat(catstruct *cat, access_type_def at)

  {

  if  (cat->access_type == READ_ONLY && at == WRITE_ONLY)
    error(EXIT_FAILURE, "*Internal Error*: Trying to write to the "
	"READ_ONLY catalog ", cat->filename);

  if (!cat->file)
    {
    if ((cat->file = fopen(cat->filename, at==WRITE_ONLY?"wb":"rb")) == NULL)
      return RETURN_ERROR;
    cat->access_type = at;
    }

  return RETURN_OK;
  }

