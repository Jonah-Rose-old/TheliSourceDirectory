 /*
 				types.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	WeightWatcher
*
*	Author:		E.BERTIN (IAP, Leiden observatory & ESO)
*
*	Contents:	global type definitions.
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

#include <stdio.h>

/*-------------------------------- flags ------------------------------------*/

/*--------------------------------- typedefs --------------------------------*/
typedef	unsigned char	BYTE;			/* a byte */
typedef	float		PIXTYPE;		/* WEIGHT pixel type */
typedef	int		FLAGTYPE;		/* FLAG pixel type */
typedef	unsigned short	USHORT;			/* 0 to 65535 integers */

#ifdef INT64
typedef	int		LONG;
typedef	unsigned int	ULONG;
#else
typedef	long		LONG;
typedef	unsigned long	ULONG;
#endif

/*----------------------------- image parameters ----------------------------*/
typedef struct
  {
  char		filename[MAXCHAR];	/* pointer to the image filename */
  char		*rfilename;		/* pointer to the reduced image name */
  char		ident[MAXCHAR];		/* field identifier (read from FITS)*/
  char		rident[MAXCHAR];	/* field identifier (relative) */
  FILE		*file;			/* pointer the image file structure */
  char		*fitshead;		/* pointer to the FITS header */
  int		fitsheadsize;		/* FITS header size */
/* ---- main image parameters */
  int		bitpix, bytepix;	/* nb of bits and bytes per pixel */
  int		bitsgn;			/* non-zero if signed integer data */
  int		width, height;		/* x,y size of the field */
  size_t	npix;			/* total number of pixels */
  double	bscale, bzero;		/* FITS scale and offset */
  void		*strip;			/* pointer to the image buffer */
  int		stripheight;		/* height  of a strip (in lines) */
/* ---- image (de-)compression */
  enum {COMPRESS_NONE, COMPRESS_BASEBYTE, COMPRESS_PREVPIX}
		compress_type;		/* image compression type */
  char		*compress_buf;		/* de-compression buffer */
  char		*compress_bufptr;	/* present pixel in buffer */
  int		compress_curval;	/* current pixel or checksum value */
  int		compress_checkval;	/* foreseen pixel or checksum value */
  int		compress_npix;		/* remaining pixels in buffer */
  int		flags;			/* flags defining the field type */
/*---- WEIGHT-stuff */
  PIXTYPE	threshd, threshu;	/* lower and upper thresholds */
  FLAGTYPE	wmask;			/* threshold detection mask */
/*---- FLAG-stuff */
  FLAGTYPE	ofmask[32];		/* output mask (max 1 per bit) */
  FLAGTYPE	fmask;			/* FLAG detection mask */
  FLAGTYPE	fmask2[32];		/* Positions of active FLAG bits */
  int		nfmask;			/* number of active FLAG bits */
  }	picstruct;


/*------------------------------- preferences -------------------------------*/
typedef struct
  {
  char		prefs_name[MAXCHAR];			/* prefs filename*/
  char		*(weight_name[MAXNIMAGE]);		/* WEIGHT filenames */
  int		nweight_name;				/* number of params */
  char		*(flag_name[MAXNIMAGE]);		/* WEIGHT filenames */
  int		nflag_name;				/* number of params */
  char		*(vec_name[MAXNIMAGE]);			/* POLY filenames */
  int		nvec_name;				/* number of params */
  double	weight_threshd[MAXNIMAGE];		/* minimum WEIGHT */
  int		nweight_threshd;			/* number of params */
  double	weight_threshu[MAXNIMAGE];		/* maximum WEIGHT */
  int		nweight_threshu;			/* number of params */
  FLAGTYPE	weight_mask[MAXNIMAGE];			/* WEIGHT flags */
  int		nweight_mask;				/* number of params */
  FLAGTYPE	vec_mask[MAXNIMAGE];			/* POLY flags */
  int		nvec_mask;				/* number of params */
  FLAGTYPE	flag_wmask[MAXNIMAGE];			/* FLAG weight-mask */
  int		nflag_wmask;				/* number of params */
  FLAGTYPE	flag_fmask[MAXNIMAGE];			/* intput FLAG-mask */
  int		nflag_fmask;				/* number of params */
  FLAGTYPE	flag_ofmask[32];			/* output FLAGs */
  int		nflag_ofmask;				/* number of params */
  char          *intersec;                              /* flag intersection?*/
  char		*oweight_name;				/* output WEIGHT */
  char		*oflag_name;				/* output FLAG */
  int		mem_bufsize;				/* # of buffer lines */

  enum	{QUIET, NORM, FULL}		verbose_type;	/* display type */
  }	prefstruct;

