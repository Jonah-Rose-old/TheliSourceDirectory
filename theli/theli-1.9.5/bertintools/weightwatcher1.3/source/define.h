 /*
 				define.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	WeightWatcher
*
*	Author:		E.BERTIN (IAP, Leiden observatory & ESO)
*
*	Contents:	global definitions.
*
*	Last modify:	25/09/2000
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

/*
 * 25.03.2004:
 * increased MAXCHAR from 256 to 1024
 *
 * 29.01.2005:
 * I included global_vers.h for Glval version number
 * information
 */

#include "global_vers.h"

/*------------------------ what, who, when and where ------------------------*/

#define		BANNER		"WeightWatcher"
#define		VERSION		"1.3 (September 25, 2000)"
#define		COPYRIGHT	"Emmanuel BERTIN (bertin@iap.fr)"
#define		INSTITUTE	"IAP,Leiden,ESO"


/*----------------------------- Internal constants --------------------------*/

#define	OUTPUT			stderr		/* where all msgs are sent */
#define	DATA_BUFSIZE		262144		/* data buffer size */
#define	MAXCHAR			1024		/* max. number of characters */
#define	MAXNIMAGE	       	32		/* max. # of imput images */
#define	BIG			1e+30		/* a huge number */

#ifndef PI
#define	PI			3.1415926535898	/* never met before? */
#endif

/* NOTES:

One must have	DATA_BUFSIZE >= 2880 with DATA_BUFSIZE%8 = 0
		MAXCHAR >= 16
		MAXNIMAGE >= MAXLIST		(see prefs.h)
*/

/*----- Set defines according to machine's specificities and customizing ----*/

#ifdef DEC_ALPHA
#define	BSWAP
#define	INT64
#endif

#ifdef HP_UX
#define	_HPUX_SOURCE
#endif

#ifdef PC_LINUX
#define	BSWAP
#endif

#ifdef SUN_ULTRASPARC
#define	INT64
#endif

#ifdef SUN_OS
#endif

#ifdef DEC_ULTRIX
#define	BSWAP
#endif

#ifdef MEMORY_LINK
#define MEMORY_READ
#endif

/*--------------------- in case of missing constants ------------------------*/

#ifndef		SEEK_SET
#define		SEEK_SET	0
#endif
#ifndef		SEEK_CUR
#define		SEEK_CUR	1
#endif

#ifndef	EXIT_SUCCESS
#define	EXIT_SUCCESS		0
#endif
#ifndef	EXIT_FAILURE
#define	EXIT_FAILURE		-1
#endif

/*---------------------------- return messages ------------------------------*/

#define		RETURN_OK		0
#define		RETURN_ERROR		(-1)
#define		RETURN_FATAL_ERROR	(-2)

/*------------------- a few definitions to read FITS parameters ------------*/

#define	FBSIZE	2880L	/* size (in bytes) of one FITS block */

#define	FITSTOF(k, def) \
			(st[0]=0,((point = fitsnfind(buf, k, n))? \
				 fixexponent(point), \
				atof(strncat(st, &point[10], 70)) \
				:(def)))

#define	FITSTOI(k, def) \
			(st[0]=0,(point = fitsnfind(buf, k, n))? \
				 atoi(strncat(st, &point[10], 70)) \
				:(def))

#define	FITSTOS(k, str, def) \
			{ if (point = fitsnfind(buf, k, n)) \
				{ \
				for (i=0,point+=11; (*point)!='\'';) \
					str[i++] = *(point++); \
				str[i] = '\0'; \
				} \
			  else\
				strcpy(str, def); \
			}


/*------------------------------- Other Macros -----------------------------*/

#define	DEXP(x)	exp(2.30258509299*(x))	/* 10^x */

#define QFREAD(ptr, size, afile, fname) \
		if (fread(ptr, (size_t)(size), (size_t)1, afile)!=1) \
		  error(EXIT_FAILURE, "*Error* while reading ", fname)

#define QFWRITE(ptr, size, afile, fname) \
		if (fwrite(ptr, (size_t)(size), (size_t)1, afile)!=1) \
		  error(EXIT_FAILURE, "*Error* while writing ", fname)

#define	QFSEEK(afile, offset, pos, fname) \
		if (fseek(afile, (offset), pos)) \
		  error(EXIT_FAILURE,"*Error*: file positioning failed in ", \
			fname)

#define	QFTELL(pos, afile, fname) \
		if ((pos=ftell(afile))==-1) \
		  error(EXIT_FAILURE,"*Error*: file position unknown in ", \
			fname)

#define	QCALLOC(ptr, typ, nel) \
		{if (!(ptr = (typ *)calloc((size_t)(nel),sizeof(typ)))) \
		  error(EXIT_FAILURE, "Not enough memory for ", \
			#ptr " (" #nel " elements) !");;}

#define	QMALLOC(ptr, typ, nel) \
		{if (!(ptr = (typ *)malloc((size_t)(nel)*sizeof(typ)))) \
		  error(EXIT_FAILURE, "Not enough memory for ", \
			#ptr " (" #nel " elements) !");;}

#define	QREALLOC(ptr, typ, nel) \
		{if (!(ptr = (typ *)realloc(ptr, (size_t)(nel)*sizeof(typ)))) \
		   error(EXIT_FAILURE, "Not enough memory for ", \
			#ptr " (" #nel " elements) !");;}

#define	RINT(x)	(int)(floor(x+0.5))

#define	PIX(pic, x, y)	pic->strip[(((int)y)%pic->stripheight) \
				*pic->width +(int)x]

#define	NPRINTF		if (prefs.verbose_type == NORM) fprintf

#define	NFPRINTF(w,x)	{if (prefs.verbose_type == NORM) \
				fprintf(w, "\33[1M> %s\n\33[1A",x); \
			else if (prefs.verbose_type == FULL) \
				fprintf(w, "%s.\n", x);}

#define	FPRINTF		if (prefs.verbose_type == FULL)	fprintf

#define FLAG(x)		(*((char *)&flag##x))
