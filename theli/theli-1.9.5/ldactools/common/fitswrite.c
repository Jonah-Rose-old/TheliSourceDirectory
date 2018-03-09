/*
 				fitswrite.c

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	The LDAC Tools
*
*	Author:		E.BERTIN, DeNIS/LDAC
*
*	Contents:	low-level functions for writing LDAC FITS catalogs.
*
*	Last modify:	13/01/98
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

/*
 * 09.12.2002:
 * function save_tab:
 * the close_cat commands in this function are now only
 * executed if catalogs REALLY have been opened before.
 *
 * 05.05.03:
 * included the tsize.h file
 *
 * 03.01.05:
 * removed compiler warnings with gcc and optimisation
 * (-O3) enabled.
 *
 * I made the global declaration of padbuf 'extern' as the
 * variable is defined in fitswrite.c already.
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
 * 24.07.2007:
 * - I forgot to change a a call to 'ftell' to QFTELL (changes of
 *   04.06.2007)
 * - I changed the definition of two variables that can exceed
 *   the range of ints in case of tables with a size larger than
 *   2GB.
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>

#include	"fitscat_defs.h"
#include	"fitscat.h"
#include	"tsize.h"

char	*lineout_buf; 
int	lineout_size, nlineout;

/* padbuf is defined in fitsread.c */
extern char *padbuf[FBSIZE];

/****** save_cat ***************************************************************
PROTO	void save_cat(catstruct *cat, char *filename)
PURPOSE	Save a FITS catalog with name filename.
INPUT	catalog structure,
	filename.
OUTPUT	-.
NOTES	Any preexisting file with name filename is overwritten.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	07/04/97
 ***/
void	save_cat(catstruct *cat, char *filename)

  {
   tabstruct	*tab;
   int		i;

  strcpy(cat->filename, filename);
  if (open_cat(cat, WRITE_ONLY) != RETURN_OK)
    error(EXIT_FAILURE, "*Error*: cannot open for writing ", filename);

  tab = cat->tab;
/*Go through each segment in the right order to save data*/

  for (i=cat->ntab; i--;)
    {
    save_tab(cat, tab);
    while (!((tab=tab->nexttab)->nseg));
    }

  if (close_cat(cat) != RETURN_OK)
    error(EXIT_FAILURE, "*Error*: Problem while closing", cat->filename);

  return;
  }


/****** save_tab **************************************************************
PROTO	void save_tab(catstruct *cat, tabstruct *tab)
PURPOSE	Save a FITS catalog with name filename.
INPUT	catalog structure,
	filename.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	13/01/99
 ***/
void	save_tab(catstruct *cat, tabstruct *tab)

  {
   catstruct	*tabcat=NULL;
   keystruct	*key;
   tabstruct	*keytab;
   int		b,j,k,o, nbytes,nkey,nobj,spoonful,
		tabflag, larrayin=0,larrayout;
   OFF_T        size,tabsize;
   char		*buf, *inbuf=NULL, *outbuf, *fptr,*ptr;
   int          openedfile = 0;
#ifdef	BSWAP
   int		esize;
#endif

/*  Make the table parameters reflect its content*/
  update_tab(tab);
/*  The header itself*/
  tabflag = update_head(tab)==RETURN_OK?1:0;
  QFWRITE(tab->headbuf, tab->headnblock*FBSIZE, cat->file, cat->filename);
/*  Allocate memory for the output buffer */
  tabsize = 0;
  if (tabflag)
    {
     QFTELL(tab->bodypos, cat->file, cat->filename); 
/*-- If segment is a binary table, save it row by row */
    QMALLOC(outbuf, char, (larrayout = tab->naxisn[0]));
    nkey = tab->nkey;
    tabsize = 0;
    for (j=tab->nseg; j--;)
      {
      update_tab(tab);
/*---- Scan keys to find the reference tab and other things*/
      keytab = NULL;
      key = tab->key;
      for (k=nkey; k--; key = key->nextkey)
        if (!key->ptr)
          {
          keytab = key->tab;
          tabcat = keytab->cat;
          }
/*---- If table contains some keys with no ptrs, we have to access a file */
      if (keytab)
        {
        QMALLOC(inbuf, char, (larrayin = keytab->naxisn[0]));
	if(!tabcat->file)
	{
	  openedfile = 1;
	}
        if (open_cat(tabcat, READ_ONLY) != RETURN_OK)
          error(EXIT_FAILURE, "*Error*: Cannot access ", tabcat->filename);
        QFSEEK(tabcat->file, keytab->bodypos, SEEK_SET, tabcat->filename);
        }
      nobj = tab->naxisn[1];
      for (o=0; o<nobj; o++)
        {
        if (keytab)
          QFREAD(inbuf, larrayin, tabcat->file, tabcat->filename);
        fptr = outbuf;
        for (k=nkey; k--; key = key->nextkey)
          {
          nbytes = key->nbytes;
          ptr = key->ptr? (char *)key->ptr+nbytes*o:inbuf+key->pos;
          for (b=nbytes; b--;)
            *(fptr++) = *(ptr++);
#ifdef	BSWAP
          if (key->ptr)
            {
            esize = t_size[key->ttype];
            swapbytes(fptr-nbytes, esize, nbytes/esize);
            }
#endif
          }
        QFWRITE(outbuf, larrayout, cat->file, cat->filename);
        }
      if (keytab)
      {
        QFREE(inbuf);
	if(openedfile != 0)
	{
          if (close_cat(tabcat) != RETURN_OK)
	  {
            error(EXIT_FAILURE, "*Error*: Problem while closing",
		  tabcat->filename);
	  }
	  openedfile = 0;
        }
      }
      tabsize += tab->tabsize;
      tab = tab->nexttab;
      }
    QFREE(outbuf);
    }
  else
    {
/*-- If segment is not a binary table, save it ``as it is'' */
/*-- We use a limited-size buffer ``in case of'' */
    tabsize = size = tab->tabsize;
    if (tabsize)
      {
      if (tab->bodybuf)
        {
/*------ A body is present in memory and needs to be written */
#ifdef BSWAP
        swapbytes(tab->bodybuf, tab->bytepix, tabsize/tab->bytepix);
#endif
        QFWRITE(tab->bodybuf, tabsize, cat->file, cat->filename);
#ifdef BSWAP
        swapbytes(tab->bodybuf, tab->bytepix, tabsize/tab->bytepix);
#endif
        }
      else
/*------ The body should be copied from the source tab */
        {
        tabcat = tab->cat;
        spoonful = size<DATA_BUFSIZE?size:DATA_BUFSIZE;
        QMALLOC(buf, char, spoonful);
	if(!tabcat->file)
	{
	  openedfile = 1;
	}
        if (open_cat(tabcat, READ_ONLY) != RETURN_OK)
          error(EXIT_FAILURE, "*Error*: Cannot access ", tabcat->filename);
        QFSEEK(tabcat->file, tab->bodypos, SEEK_SET, tabcat->filename);
        for (;size>0; size -= spoonful)
          {
          if (spoonful>size)
          spoonful = size;
          QFREAD(buf, spoonful, tabcat->file, tabcat->filename);
          QFWRITE(buf, spoonful, cat->file, cat->filename);
          }
        QFREE(buf);
	if(openedfile != 0)
	 {
          if (close_cat(tabcat) != RETURN_OK)
	  {
            error(EXIT_FAILURE, "*Error*: Problem while closing",
		  tabcat->filename);
	  }
	  openedfile = 0;
	 }
        }
      }
    }

/* FITS padding*/
  size = PADEXTRA(tabsize);
  if (size)
    QFWRITE(padbuf, size, cat->file, cat->filename);

  return;
  }


/****** init_writeobj *********************************************************
PROTO	void init_writeobj(catstruct *cat, tabstruct *tab)
PURPOSE	Prepare the writing of individual sources in a FITS table
INPUT	catalog structure,
	table structure.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	23/01/97
 ***/
void	init_writeobj(catstruct *cat, tabstruct *tab)

  {
   keystruct	*key;
   int		k;

/* Make the table parameters reflect its content*/
  update_tab(tab);
/* The header itself */
  if (update_head(tab) != RETURN_OK)
    error(EXIT_FAILURE, "*Error*: Not a binary table: ", tab->extname);

  QFWRITE(tab->headbuf, tab->headnblock*FBSIZE, cat->file, cat->filename);
/* Store the current position */
  QFTELL(tab->bodypos, cat->file, cat->filename);

/* Allocate memory for the output buffer  (or increase it if done already) */
  if (lineout_buf)
    {
    if (lineout_size < tab->naxisn[0])
      {
      QREALLOC(lineout_buf, char, tab->naxisn[0]);
      lineout_size = tab->naxisn[0];
      }
    }
  else
    {
    QMALLOC(lineout_buf, char, tab->naxisn[0]);
    lineout_size = tab->naxisn[0];
    }

  nlineout++;

/* Scan keys to check that memory has been allocated */
  key = tab->key;
  for (k=tab->nkey; k--; key = key->nextkey)
    if (!key->ptr)
      error(EXIT_FAILURE, "*Error*: no memory allocated for ", key->name);

/* No object written yet: initialize counter */
  tab->naxisn[1] = 0;

  return;
  }


/****** write_obj *************************************************************
PROTO	int write_obj(tabstruct *tab)
PURPOSE	Write one individual source in a FITS table
INPUT	Destination catalog
	Table structure.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	23/01/97
 ***/
int	write_obj(tabstruct *tab)

  {
   keystruct	*key;
   char		*pin, *pout;
   int		b,k;
#ifdef	BSWAP
   int		esize;
#endif

  key = tab->key;
  pout = lineout_buf;
  for (k=tab->nkey; k--; key = key->nextkey)
    {
    pin = key->ptr;
#ifdef BSWAP
    esize = t_size[key->ttype];
    swapbytes(pin, esize, key->nbytes/esize);
#endif
    for (b=key->nbytes; b--;)
      *(pout++) = *(pin++);
    }

  QFWRITE(lineout_buf, *tab->naxisn, tab->cat->file, tab->cat->filename);

  return ++tab->naxisn[1];
  }


/****** end_writeobj **********************************************************
PROTO	void end_writeobj(catstruct *cat, tabstruct *tab)
PURPOSE	End the writing of individual sources in a FITS table
INPUT	catalog structure,
	table structure.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	13/11/97
 ***/
void	end_writeobj(catstruct *cat, tabstruct *tab)

  {
   keystruct	*key;
   int		k, size;
   OFF_T        pos;
/* Make the table parameters reflect its content*/
  key = tab->key;
  for (k=tab->nkey; k--; key = key->nextkey)
    key->nobj = tab->naxisn[1];
  update_tab(tab);
/* The header itself */
  if (update_head(tab) != RETURN_OK)
    error(EXIT_FAILURE, "*Error*: Not a binary table: ", tab->extname);

/*--FITS padding*/
  size = PADEXTRA(tab->tabsize);
  if (size)
  {
    QFWRITE(padbuf, size, cat->file, cat->filename);
  }
  QFTELL(pos, cat->file, cat->filename);
  QFSEEK(cat->file,tab->bodypos-FBSIZE*tab->headnblock,SEEK_SET,cat->filename);
  QFWRITE(tab->headbuf, FBSIZE*tab->headnblock, cat->file, cat->filename);
  QFSEEK(cat->file, pos, SEEK_SET, cat->filename);

  if (!(--nlineout))
    {
    QFREE(lineout_buf);
    lineout_buf = NULL;
    lineout_size = 0;
    }

  return;
  }


/****** print_obj *************************************************************
PROTO	void print_obj(FILE *stream, tabstruct *tab)
PURPOSE	Print one individual source to the output stream.
INPUT	Output stream
	Table structure.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP & Leiden observatory)
VERSION	13/06/97
 ***/
void	print_obj(FILE *stream, tabstruct *tab)

  {
   keystruct	*key;
   char		*ptr;
   int		i,k, esize;

  if (!(key = tab->key))
    error(EXIT_FAILURE, "*Error*: no key to print in table ", tab->extname);

  for (k=tab->nkey; k--; key = key->nextkey)
    {
    esize = t_size[key->ttype];
    ptr = key->ptr;
    for (i = key->nbytes/esize; i--; ptr += esize)
      switch(key->ttype)
        {
        case T_SHORT:
          fprintf(stream, *key->printf?key->printf:"%d", *(short *)ptr);
          if (i)
            putc(' ', stream);
          break;
        case T_LONG:
          fprintf(stream, *key->printf?key->printf:"%d", *(int *)ptr);
          if (i)
            putc(' ', stream);
          break;
        case T_FLOAT:
          fprintf(stream, *key->printf?key->printf:"%g", *(float *)ptr);
          if (i)
            putc(' ', stream);
          break;
        case T_DOUBLE:
          fprintf(stream, *key->printf?key->printf:"%f", *(double *)ptr);
          if (i)
            putc(' ', stream);
          break;
        case T_BYTE:
          if (key->htype==H_BOOL)
            {
            if (*ptr)
              fprintf(stream, "T");
            else
              fprintf(stream, "F");
            }
          else
            fprintf(stream, key->printf?key->printf:"%d", (int)*ptr);
          if (i)
            putc(' ', stream);
          break;
        case T_STRING:
          fprintf(stream, "%c", (int)*ptr);
          break;
        default:
          error(EXIT_FAILURE, "*FATAL ERROR*: Unknown FITS type in ",
		"show_keys()");
        }
    if (k)
      putc(' ', stream);
    }

  putc('\n', stream);

  return;
  }

