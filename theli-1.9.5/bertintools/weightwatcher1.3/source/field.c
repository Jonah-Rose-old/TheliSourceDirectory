 /*
 				field.c

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	WeightWatcher
*
*	Author:		E.BERTIN (IAP, Leiden observatory & ESO)
*
*	Contents:	Handling of field structures.
*
*	Last modify:	25/07/97
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

/*
  29.01.2005:
  - I added information about the ww processing history
  - I added the utility function get_basename

  03.09.2010:
  The time put into the 'ww' history was GMT. I changed that
  to the local time. The program name put into histories is now
  'ww_theli' instead of 'ww'.
*/

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include        <time.h>

#include	"define.h"
#include	"globals.h"
#include	"field.h"
#include	"fitscat.h"

/********************************* newfield **********************************/
/*
Returns a pointer to a new field, ready to go!
*/
picstruct	*newfield(char *filename, int flags, picstruct *mfield)

  {
   picstruct	*field;
   struct tm	*tm;
   double	dval;
   int		ival;
   char         tmpstring[72];
   int          headnblocks;
   time_t	thetime;
   int          i;

/* First allocate memory for the new field (and nullify pointers) */
  QCALLOC(field, picstruct, 1);
  if (mfield)
    *field = *mfield;
  field->flags = flags;
  strcpy (field->filename, filename);
/* A short, "relative" version of the filename */
  if (!(field->rfilename=strrchr(field->filename, '/')))
    field->rfilename = field->filename;

  if (mfield)
    {
    sprintf(gstr, "Opening %s", field->rfilename);
    NFPRINTF(OUTPUT, gstr);
    if (!(field->file = fopen(field->filename, "wb")))
    {
      error(EXIT_FAILURE, "*Error*: Cannot open for output ",field->filename);
    }
/*-- keep some marging for HISTORY jeywords */
    headnblocks = field->fitsheadsize/FBSIZE;
    QMALLOC(field->fitshead, char, (headnblocks + 2) * FBSIZE);
    memset(field->fitshead, ' ', (headnblocks + 2) * FBSIZE);
    memcpy(field->fitshead, mfield->fitshead, field->fitsheadsize);
/*-- Neutralize possible scaling factors */
    dval = 1.0; fitswrite(field->fitshead, "BSCALE  ",&dval,H_FLOAT,T_DOUBLE);
    dval = 0.0; fitswrite(field->fitshead, "BZERO   ",&dval,H_FLOAT,T_DOUBLE);
/*-- Add HISTORY and log information */
    /* add OBJECT key if it does not yet exist */
    fitsadd(field->fitshead, "OBJECT  ", "");
    fitsadd(field->fitshead, "HISTORY ", "");
    sprintf(tmpstring, "ww: %s", __PIPEVERS__);
    fitsadd(field->fitshead, "HISTORY ", tmpstring);
    thetime = time(NULL);
    tm = localtime(&thetime);
    sprintf(tmpstring,"ww_theli: called at: %04d-%02d-%02dT%02d:%02d:%02d",
                        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
                        tm->tm_hour, tm->tm_min, tm->tm_sec);    
    fitsadd(field->fitshead, "HISTORY ", tmpstring);
    /* Add info on the input weight and flag images and
       the polygon files
    */
    if(prefs.nweight_name)
    {
      sprintf(tmpstring, "ww: Input Weight Images:");
      fitsadd(field->fitshead, "HISTORY ", tmpstring);
      for(i=0; i < prefs.nweight_name; i++)
      {
	  snprintf(tmpstring, 71, "%s; low: %f; high: %f", 
                   get_basename(prefs.weight_name[i]),
                   prefs.weight_threshd[i], prefs.weight_threshu[i]);
	  fitsadd(field->fitshead, "HISTORY ", tmpstring);
      }
    }

    if(prefs.nflag_name)
    {
      sprintf(tmpstring, "ww: Input Flag Images:");
      fitsadd(field->fitshead, "HISTORY ", tmpstring);
      for(i=0; i < prefs.nflag_name; i++)
      {
	  snprintf(tmpstring, 71, "%s; wmask: 0x%x; fmask: 0x%x; ofmask: %d", 
                   get_basename(prefs.flag_name[i]), prefs.flag_wmask[i], 
                   prefs.flag_fmask[i], prefs.flag_ofmask[i]);
	  fitsadd(field->fitshead, "HISTORY ", tmpstring);
      }
    }
    
    if(prefs.nvec_name)
    {
      sprintf(tmpstring, "ww: Input Polygon Files:");
      fitsadd(field->fitshead, "HISTORY ", tmpstring);
      for(i=0; i < prefs.nvec_name; i++)
      {
	  snprintf(tmpstring, 71, "%s; outflag: %d", 
                   get_basename(prefs.vec_name[i]), prefs.vec_mask[i]);
	  fitsadd(field->fitshead, "HISTORY ", tmpstring);
      }      
    }

    if (field->compress_type != COMPRESS_NONE)
      {
      field->compress_type = COMPRESS_NONE;
      fitswrite(field->fitshead, "IMAGECOD", "NONE", H_STRING, T_STRING);
      }
    fitswrite(field->fitshead, "ORIGIN  ", BANNER, H_STRING, T_STRING);
    if (flags & FLAG_FIELD)
      {
      ival = 1; fitswrite(field->fitshead, "BITSGN  ",&ival, H_INT, T_LONG);
      fitswrite(field->fitshead, "BITPIX  ", &field->bitpix, H_INT, T_LONG);
      fitswrite(field->fitshead, "OBJECT  ", "FLAG MAP", H_STRING,T_STRING);
      }
    else
      {
      ival = 1; fitswrite(field->fitshead, "BITSGN  ",&ival, H_INT, T_LONG);
      fitswrite(field->fitshead, "OBJECT  ", "WEIGHT MAP", H_STRING,T_STRING);
      field->bitpix = BP_FLOAT;
      }
    fitswrite(field->fitshead, "BITPIX  ", &field->bitpix, H_INT, T_LONG);
    field->bytepix = (field->bitpix>0?field->bitpix:-field->bitpix)>>3;

/*-- Determine real size of image header and write it */
    headnblocks = ((fitsfind(field->fitshead, "END     ")+36)*80)/FBSIZE;
    QREALLOC(field->fitshead, char, headnblocks*FBSIZE);
    QFWRITE(field->fitshead,headnblocks*FBSIZE,field->file,field->filename);
    }
  else
    {
    sprintf(gstr, "Looking for %s", field->rfilename);
    NFPRINTF(OUTPUT, gstr);
/*-- Check the image exists and read important info (image size, etc...) */
    readimagehead(field);
    if (prefs.verbose_type != QUIET)
    fprintf(OUTPUT, "Frame:    \"%.20s\" / %d x %d / %d bits %s data\n",
	field->ident,
	field->width, field->height, field->bytepix*8,
	field->bitpix>0?
		(field->compress_type!=COMPRESS_NONE?"COMPRESSED":"INTEGER")
		:"FLOATING POINT");

/*-- Provide a buffer for compressed data */
    if (field->compress_type != COMPRESS_NONE)
      QMALLOC(field->compress_buf, char, FBSIZE);
    }

  field->stripheight = prefs.mem_bufsize;
  if (field->stripheight>field->height)
    field->stripheight = field->height;
  if (!(field->strip=malloc(field->stripheight*field->width*4)))
    error(EXIT_FAILURE,"Not enough memory for the image buffer in ",
	field->rfilename);

  return field;
  }


/********************************* endfield **********************************/
/*
Free and close everything related to a field structure.
*/
void	endfield(picstruct *field)

  {
  fclose(field->file);

  free(field->fitshead);
  free(field->strip);
  free(field->compress_buf);
  free(field);

  return;
  }

/********************************* get_basename **********************************/
/*
utility routine to find out the basename of a file
(i.e. the filename without a prefix path)
*/
char * get_basename(const char *filename)
{
  char *p ;
  p = strrchr (filename, '/');
  return p ? p + 1 : (char *) filename;
}
