
/*----------------------------------------------------------------------------
   
   File name    :   imcombflat.c
   Author       :   Thomas Erben
   Created on   :   01.02.2005
   Description  :   stacks FITS images

 ---------------------------------------------------------------------------*/

/*
	$Id: imcombflat.c,v 1.7 2013/04/03 14:45:36 thomas Exp $
	$Author: thomas $
	$Date: 2013/04/03 14:45:36 $
	$Revision: 1.7 $
*/

/*----------------------------------------------------------------------------
                                History Coments
 ---------------------------------------------------------------------------*/

/*
  01.02.2005:
  project started.

  09.04.2005:
  A warning message is printed if only 1 or zero images effectively
  enter the coaddition. This is the case if many low and high ranks
  are rejected.

  28.06.2005:
  I fixed a major bug. Sorting of the input pixel array (before being coadded)
  had to be at a different place.

  06.12.2005:
  I introduced a clipmean combination mode (parameter -c CLIPMEAN)). 
  It estimates mean and sigma
  of the pixel samples and rejects all pixels lying below and above user
  defined sigma values (parameter -l lo_clip hi_clip). This procedure
  is repeated until the array is not changed anymore (after each iteration
  the complete initial array is considered. i.e. also old points are
  allowed to return to the pixel sample) or until a maximum number
  of iterations is reached (20; variable maxiterations in the new
  function meanclip).

  23.03.2006:
  I substituted the quicksort algorithm by a faster one.

  16.01.2008:
  I corrected a bug in the definition of the two-dimensional
  imagenames array.

  03.04.2013:
  xmemory is no longer used by default but only if the variable
  __XMEM_DEBUG__ is defined at compile time. Otherwise, regular malloc
  etc. calls are used for memory management.
 */

/*----------------------------------------------------------------------------
                                Includes and Defines
 ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <stdlib.h>

#include "error.h"
#include "arrays.h"
#include "fits.h"
#include "global_vers.h"
#include "t_iso8601.h"

#ifdef __XMEM_DEBUG__
#include "xmemory.h"
#endif

#ifndef MAXCHAR
#define MAXCHAR 255
#endif 

#define MAXIMAGE 1000 /* the maximum number of input images allowed */

/* uncomment the following line to obtain a status on allocated/freed
   memory at the end of the program. Only available if also __XMEM_DEBUG__
   is defined at compile time.
*/

/* #define MEMORY_DEBUG */

#define __USAGE__ "\
USAGE:\n\
   imcombflat options\n\n\
OPTIONS:\n\
-h                            print this and leave\n\
-i                            file listing input images\n\
                              and their modes (mandatory\n\
                              parameter)\n\
-o                            output result file\n\
     (defaulted to imcombflat.fits)\n\
-b                            output bitpix (16 or -32)\n\
     (defaulted to -32)\n\
-e   lo_rank hi_rank          low and high ranks to exclude\n\
                              during coaddition\n\
     (defaulted to 0 0)\n\
-t   lo_rej hi_rej            define low/high threshholds\n\
                              for pixels to be considered\n\
                              in the coaddition process\n\
-l   lo_clip hi_clip          define low/high sigma clip values\n\
                              for pixels to be considered\n\
                              in the CLIPMEAN coaddition.\n\
     (defaulted to 3.0 3.0)\n\
-c                            coaddition method\n\
                              'MEAN', 'CLIPMEAN' or 'MEDIAN'\n\
     (defaulted to MEDIAN)\n\
-s                            rescale input images before\n\
                              coadditon (1=YES, 0=NO)\n\
     (defaultled to 0)\n\
-m                            define mode of result image\n\
                              in case of rescaling turned on\n\
     (defaulted to 0: mean of modes from input data)\n\n\
DESCRIPTION:\n\
The program stacks a set of 2d FITS images of equal size.\n\
The stacking is done by collecting all input pixels on a given\n\
array position, rejecting the 'low_rank' lowest and 'high_rank'\n\
highest values. In this process, only pixels with values between\n\
'lo_rej' and 'hi_rej' are at all considered. From the remaining\n\
pixel values, a straight median, mean or a sigma clipped mean is \n\
given as result. In the latter case mean and sigma are estimated,\n\
pixels below and above defined thresholds are rejected and the process\n\
is repeated until the array does not change anymore or until 20\n\
iterations are reached. In the rejection process all points entering\n\
are again considered, i.e. previously rejected points are again\n\
allowed to reenter the process.\n\
The input images are given in a file containing on each line one\n\
input image and optionally its mode. Such a file can directly be\n\
produced with the 'imstats' program. An example file could look like:\n\n\
# imstats: Pipeline Global Version: 0.9\n\
# imstats: called at 2005-02-07T00:21:24\n\
# imstats: estimating statistics in\n\
# the whole frames within -60000.000000 < pixvalue < 60000.000000\n\
#\n\
# imstats: filename     mode    median  mean    sigma\n\
test_1.fits       1.01    1.06    1.02    2.00\n\
test_2.fits       1.99    2.02    1.99    2.00\n\
test_3.fits       3.00    2.99    3.01    2.01\n\
test_4.fits       4.00    3.94    4.01    2.01\n\
test_5.fits       5.00    5.03    5.01    1.9.\n\n\
The input images can be rescaled to a provided level (options -s and\n\
-m) if desired.\n\n\
AUTHOR:\n\
   Thomas Erben (terben@astro.uni-bonn.de)\n\
   The code is heavily based on Nick Kaiser's 'combineimages' \n\
   routine within imcat.\n"


#define __PROGRAM__ "imcombflat: stack images"
#define __VERS__ "1.0"


static void usage() ;

/* sort a float array by increasing number */
void float_qsort(float * arr, int l, int r);

/* return the median of an array */
float median(float *, int);

/* return the mean of an array */
float mean(float *, int);

/* return the mean of an array after iertatively clipping */
float meanclip(float *, int);

/* global variables for efficiency reasons !! */
float Glo_clip, Ghi_clip; /* low and high sigma clip values (CLIPMEAN coaddition method) */

/*----------------------------------------------------------------------------
                            Main code
 ---------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
   int		a, i, j;               /* loop variables */
   int          N1first=0, N2first=0, N1=0, N2=0;
   int          nimages = 0;           /* the total number of input
                                          images */
   fitsheader	**fitsin = NULL, *fitsout = NULL;
   FILE         *ilist;
   FILE		**ipf, *opf;           /* File handles for input and
                                          result image data */
   float        lo_rej = -60000.0;     /* low pixel reject. threshhold for
                                          statistics */
   float        hi_rej = 60000.0;      /* high pixel reject. threshhold for
                                          statistics */
   int          lo_rank = 0;           /* low pixels to be rejected before coaddition */
   int          hi_rank = 0;           /* high pixels to be rejected before coaddition */
   int          rescale = 0;           /* should images be rescaled before 
                                          coaddition (1=YES) */
   char         inputlist[MAXCHAR]=""; /* list with input images */
   char         imagenames[MAXIMAGE][MAXCHAR]; /* names of input images */
   float        inputmodes[MAXIMAGE];
   float        resultmode = 0.0;      
   char         outfilename[MAXCHAR]="imcombflat.fits";
   int          outbitpix = -32;       /* bitpix of result image */
   char         coadditionmode[MAXCHAR]="MEDIAN";
   float        ***inputline = NULL;   /* 'batch' lines of all input images */
   float        *outputline = NULL;    /* the currently considered line from the
                                          result */
   float        *pixstack;             /* contains pixel values from the input
                                          images that are to be stacked by median
                                          or mean */
   int          batchsize = 64;        /* the number of lines to be read from
                                          the input images in one go (to save I/O)
				       */
   int          origbatchsize;         /* contains the same value as batchsize;
                                          as batchsize is changed we need a copy
                                          to cleanly free memory at the end
				       */
   int          nlinesprocessed = 0;
   int          done = 0;
   int          nactupix = 0;          /* the number of remaining pixels (after 
                                          rejections due to thresshholds) at the
                                          current stack position */
   float        (*coaddfunc)(float *, int); /* pointer to the coaddition function:
                                               mean or median */
   char         tmpstring[MAXCHAR];    /* temporary strings used for output or out header
                                          construction */
    
   /* set further default values */
   origbatchsize = batchsize;
   Glo_clip = Ghi_clip = 3.0;

   /* read comand line arguments */
   if(argc < 2)
   {
       usage();
   }

   for (a=1; a<argc; a++)
   {
     if (argv[a][0] == '-')
     {
         switch((int)tolower((int)argv[a][1]))
         {
 /*-------- Config filename */
           case 'h':    usage(); 
  		        break;
           case 'i':	if(a<(argc-1))
	                {
			    strncpy(inputlist, argv[++a], MAXCHAR-1);
			}
	                else
		        {
			    usage();  
			}
  			break;
           case 'o':	if(a<(argc-1))
	                {
			    strncpy(outfilename, argv[++a], MAXCHAR-1);
			}
	                else
		        {
			    usage();  
			}
  			break;
           case 'e':	if(a<(argc-2))
	                {
			    lo_rank = atoi(argv[++a]);
			    hi_rank = atoi(argv[++a]);
			}
	                else
		        {
			    usage();  
			}
  			break;
           case 't':	if(a<(argc-2))
	                {
			    lo_rej = atof(argv[++a]);
			    hi_rej = atof(argv[++a]);
			}
	                else
		        {
			    usage();  
			}
  			break;
           case 'l':	if(a<(argc-2))
	                {
			    Glo_clip = atof(argv[++a]);
			    Ghi_clip = atof(argv[++a]);
			}
	                else
		        {
			    usage();  
			}
  			break;
           case 'c':	if(a<(argc-1))
	                {
			   strncpy(coadditionmode, argv[++a], MAXCHAR-1);
			}
	                else
		        {
			    usage();  
			}
  			break;
           case 'm':	if(a<(argc-1))
	                {
			    resultmode = atof(argv[++a]);
			}
	                else
		        {
			    usage();  
			}
  			break;
           case 's':	if(a<(argc-1))
	                {
			    rescale = atoi(argv[++a]);
			}
	                else
		        {
			    usage();  
			}
  			break;
           case 'b':	if(a<(argc-1))
	                {
			    outbitpix = atoi(argv[++a]);
			}
	                else
		        {
			    usage();  
			}
  			break;
           default:	usage(argv[0]);
		        break;
         }
     }
   }

   /* check if an input list was given */
   if(strcmp(inputlist, "") == 0)
   {
       usage(argv[0]);
   }

   /* read input images */
   if((ilist = fopen(inputlist, "r")) == NULL)
   {
       error_exit("Could not read input list\n");
   }

   while(fgets(tmpstring, MAXCHAR-1, ilist) && nimages < MAXIMAGE)
   {
       i = sscanf(tmpstring, "%s %f", imagenames[nimages], &(inputmodes[nimages]));

       /* do not consider comment lines; see if the first character
          in imagesnames is '#' */
       j=0;
       while (imagenames[j][0] == ' ')
       {
	 j++;
       }

       if(imagenames[j][0] != '#')
       {
         if(i < 2)
         {
  	     if(i==0)
  	     {
  	         error_exit("Errorneous input list: aborting!!");
  	     }
  	     else
  	     {
  	         /* no mode is found: disable rescaling !! */
  	         if(rescale == 1)
  	         {
  	             rescale = 0;
  	             fprintf(stderr, "Warning: no modes in input file; rescaling disabled !!\n");
  	         }
  	     }
          }
	  nimages++;
       }
   }


   /* allocate input stuff */
   IMCAT_CALLOC(fitsin, fitsheader *, nimages);
   IMCAT_CALLOC(ipf, FILE *, nimages);
   IMCAT_CALLOC(inputline, float **, nimages);

   for(i=0; i < nimages; i++)
   {
       IMCAT_CALLOC(inputline[i], float *, batchsize);
   }

   /* open input images */
   for (i=0; i < nimages; i++)
   {
       if((ipf[i] = fopen(imagenames[i], "r")) == NULL)
       {
	   fprintf(stderr, "Cannot read input image %s: aborting !!\n", imagenames[i]);
	   exit(1);
       } 
       fitsin[i] = readfitsheader(ipf[i]); 
       N1 = fitsin[i]->n[0];
       N2 = fitsin[i]->n[1];

       if(i == 0)
       {
	   N1first = N1;
	   N2first = N2;
       }
       if( (N1 != N1first) || (N2 != N2first) )
       {
	   error_exit("Inconsistent image sizes in input images\n");
       }
       
       /* allocate memory for input lines */
       for(j=0; j < batchsize; j++)
       {
	   IMCAT_CALLOC(inputline[i][j], float, N1first);	   
       }    
   }

   /* output result image */
   if((opf = fopen(outfilename, "w")) == NULL)
   {
       fprintf(stderr, "error opening output image %s: aborting!\n", outfilename);
       exit(1);
   }

   switch(outbitpix)
   {
     case 16:
       fitsout = new2Dfitsheader(N1, N2, SHORT_PIXTYPE);
       fitsout->bscaling = 1;
       fitsout->bzero = 32768.0;
       fitsout->bscale = 1.0;
       break;
     default:
       fitsout = new2Dfitsheader(N1, N2, FLOAT_PIXTYPE);
   }
   fitsout->opstream = opf;

   /* determine scaling factors for images; in case
      where no rescaling is applied this is set
      to 1 */
   if(rescale == 1)
   {
       if(resultmode < 1.0e-24)
       {
           for(i=0; i < nimages; i++)
           {
	       resultmode += inputmodes[i];
           }
	   resultmode /= nimages;
       }
       /* save the scaling factors in the input mode
          variables */
       for(i=0; i < nimages; i++)
       {
	   inputmodes[i] = resultmode / inputmodes[i];
       }
   }
   else
   {
       for(i=0; i < nimages; i++)
       {
	   inputmodes[i] = 1.0;
       }
   }

   /* write comments into the new FITS header and on screen */
   appendcomment(newtextcomment("HISTORY", "", NULL), fitsout);
   sprintf(tmpstring, "imcombflat: %s", __PIPEVERS__);
   fprintf(stdout, "\n%s\n", tmpstring);
   appendcomment(newtextcomment("HISTORY", tmpstring, NULL), fitsout);
   sprintf(tmpstring, "imcombflat: called at %s", get_datetime_iso8601());
   fprintf(stdout, "\n%s\n", tmpstring);
   appendcomment(newtextcomment("HISTORY", tmpstring, NULL), fitsout);
   add_comment(argc, argv, fitsout);
   
   if(strncmp(coadditionmode, "MEAN", 4) == 0)
   {
       sprintf(tmpstring, "imcombflat: mean of %d images", nimages);     
   }
   else
   {
       if(strncmp(coadditionmode, "CLIPMEAN" ,8) == 0)
       {
           sprintf(tmpstring, "imcombflat: meanclip of %d images", nimages);     
       }
       else
       {
           sprintf(tmpstring, "imcombflat: median of %d images", nimages);     
       }
   }
   appendcomment(newtextcomment("HISTORY", tmpstring, NULL), fitsout);
   fprintf(stdout, "%s\n", tmpstring);

   sprintf(tmpstring, "imcombflat: rejecting low/high pixels: %d/%d", lo_rank, hi_rank);     
   appendcomment(newtextcomment("HISTORY", tmpstring, NULL), fitsout);
   fprintf(stdout, "%s\n", tmpstring);
   sprintf(tmpstring, "imcombflat: Threshholding: [%f; %f]", lo_rej, hi_rej);     
   appendcomment(newtextcomment("HISTORY", tmpstring, NULL), fitsout);
   fprintf(stdout, "%s\n", tmpstring);

   if(rescale == 1)
   {
       sprintf(tmpstring, "imcombflat: rescaling with resultmode %f", resultmode);
   }
   else
   {
       sprintf(tmpstring, "imcombflat: no rescaling");
   }
   appendcomment(newtextcomment("HISTORY", tmpstring, NULL), fitsout);
   fprintf(stdout, "%s\n", tmpstring);

   for(i=0; i< nimages; i++)
   {
       if(rescale == 1)
       {
	   sprintf(tmpstring, "imcombflat: %s %f", 
                   get_basename(imagenames[i]), resultmode/inputmodes[i]);
       }
       else
       {
	   sprintf(tmpstring, "imcombflat: %s", get_basename(imagenames[i]));
       }
       appendcomment(newtextcomment("HISTORY", tmpstring, NULL), fitsout);
       fprintf(stdout, "%s\n", tmpstring);
   }

   writefitsheader(fitsout);

   /* some allocations for the coaddition process */
   IMCAT_CALLOC(outputline, float, N1first);
   IMCAT_CALLOC(pixstack, float, nimages);

   /* set the coaddition fuction pointer */
   if(strncmp(coadditionmode, "MEAN", 4) == 0)
   {
       coaddfunc=mean;
   }
   else
   {
       if(strncmp(coadditionmode, "CLIPMEAN" ,8) == 0)
       {
           coaddfunc=meanclip;
       }
       else
       {
           coaddfunc=median;
       }
   }

   /* first give a warning if the result only consists
      of one value */
   if(nimages-lo_rank-hi_rank<=1)
   {
     fprintf(stdout,
             "Only 0 or 1 images effectively enter the coaddition !!!!\n");
   }

   /* main loop for image coaddition */
   while(done != 1)
   {
       if(nlinesprocessed + batchsize > N2first)
       {
	   batchsize = N2first - nlinesprocessed;
	   done = 1;
       }
       for(i=0; i < nimages ; i++)
       {
	   for(j=0; j < batchsize; j++)
	   {
	       readfitsline(inputline[i][j], fitsin[i]);
	   }
       }  
       for(i=0; i < batchsize; i++)
       {
	   for(j=0; j < N1first; j++)
	   {
	       nactupix = 0;
	       for(a=0; a < nimages; a++)
	       {
		   pixstack[nactupix]=inputline[a][i][j];
		   if(pixstack[nactupix] > lo_rej && pixstack[nactupix] < hi_rej)
		   {
		       pixstack[nactupix] *= inputmodes[a];
		       nactupix++;
		   }
	       }
	       if((nactupix-lo_rank-hi_rank) <=0)
	       {
		   outputline[j] = resultmode;
	       }
	       else
	       {
                   /* sort array */
                   float_qsort(pixstack, 0, nactupix-1);
	           outputline[j] = coaddfunc(&(pixstack[lo_rank]),
                                             nactupix-lo_rank-hi_rank);
	       }
	   }
	   writefitsline(outputline, fitsout);
       } 
       nlinesprocessed += batchsize;
       fprintf(stderr, " %d lines done\r", nlinesprocessed);
   }

   /* close files and release memory */
   for(i=0; i < nimages; i++)
   {
       if(0 != fclose(ipf[i]))
       {
	   error_exit("error closing input files!");
       }
   }

   writefitstail(fitsout);

   if(0 != fclose(opf))
   {
        error_exit("error closing output file!"); 
   }

   IMCAT_FREE(ipf);
   IMCAT_FREE(pixstack);
   IMCAT_FREE(outputline);
   delfitsheader(fitsout);

   for(i=0; i < nimages; i++)
   {
       for(j=0; j < origbatchsize; j++)
       {
	   IMCAT_FREE(inputline[i][j]);
       }
       IMCAT_FREE(inputline[i]);	 
       delfitsheader(fitsin[i]);
   }
   IMCAT_FREE(fitsin);
   IMCAT_FREE(inputline);

#ifdef MEMORY_DEBUG
   xmemory_status() ;
#endif    

   return 0;
}

/*
 * This function only gives the usage for the program
 */

static void usage()
{
    printf("%s (V%s; %s)\n\n%s", __PROGRAM__, __VERS__, __PIPEVERS__, 
                                 __USAGE__) ;
    exit(1) ;
} 

/*-------------------------------------------------------------------------*/
/**
  @brief	Estimate the median of an array.
  @param	array	Array of floats to consider
  @param	n	Number of floats in the array.
  @return	the median of 'array'

  Calculate the median of an array of float numbers. The median is
  estimated by sorting the array and returning the 'middle' element.
  If the number of elements is even, the mean of the two 'middle'
  elements is returned.
 */
/*--------------------------------------------------------------------------*/

float median(float *array, int n)
{
    float result;
    
    int middle;

    middle = n/2;
    
    if(n % 2 == 0)
    {
      result = (array[middle] + array[middle-1])/2.;
    }
    else
    {
      result = array[middle];
    }

    return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Calculate the mean of an array.
  @param	array	Array of floats to consider
  @param	n	Number of floats in the array.
  @return	the mean of 'array'

  Calculate the mean of an array of float numbers.
 */
/*--------------------------------------------------------------------------*/

float mean(float *array, int n)
{
    float result = 0.0;
    
    int i;

    for(i=0; i<n; i++)
    {
        result += array[i];
    }

    result /=n ;
    return result;
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Calculate the mean of an array after sigma clipping.
  @param	array	 Array of floats to consider
  @param	n	 Number of floats in the array.
  @param	lowclip	 sigma for clipping on the low end of the array.
  @param	highclip sigma for clipping at the high end of the arraz.
  @return	the mean of 'array'

  Calculate the mean of an array of float numbers after ierative sigma
  clipping. From the initial array, mean and sigma is estimated. Points
  lying lowclip*sigma below or highclip*sigma above the mean are rejected
  and a new mean and sigma are estimated. This process is repeated until
  no more points are rejected or added to the array or until 20 iertations
  are done.  
 */
/*--------------------------------------------------------------------------*/

float meanclip(float *array, int n)
{
   /* the variables Glo_clip and Ghi_clip are global !! */
   int cont = 1;
   int nstart = 0, nend = n;
   int i; 
   int nactu;
   int nstartnew, nendnew;
   float meanactu, sigmaactu;
   int iterations = 0;
   int maxiterations = 20;

   /* in case that less than 4 elements are in the array
      we return a straight mean */

   if(n<=3) return mean(array, n);

   nactu = nend - nstart;

   while(cont == 1)
   {
     meanactu = sigmaactu = 0.0;
     iterations ++;

     for(i=0; i < nactu; i++)
     {
       meanactu += array[nstart+i];
       sigmaactu += array[nstart+i]*array[nstart+i];
     }

     meanactu /= nactu;

     /* sanity check if all array elements are the same */
     if(sigmaactu < 1.0e-06)
     {
       sigmaactu = 0.0;
     }
     else
     {
       sigmaactu = sqrt((1./(nactu-1))*(sigmaactu-nactu*meanactu*meanactu));
     }


     /* determine new nstart and nend values;
        note that the array is sorted !!
     */
     nstartnew = nendnew = 0;
     i=0;
     while((array[i] < meanactu+Ghi_clip*sigmaactu) && (i < n))
     {
       if(array[i] < meanactu-Glo_clip*sigmaactu) nstartnew++;
       i++;
     }

     nendnew = i;

     /* leave the loop if either:
	- if max iterations is reached
	- if no more points are rejected or added to the array
	- if 2 points or less are left in the process
     */
     if((nactu=(nendnew-nstartnew)) < 3 || (iterations >= maxiterations) || (nstartnew == nstart && nendnew == nend))
     {
       cont = 0;
     }
     nstart = nstartnew;
     nend = nendnew;
   }

   return meanactu;
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Sort an array of floats by increasing value.
  @param	a 	Array of floats to consider
  @param	l	lower array index
  @param	r	higher array index
  @return	void

  Fast sort on an array of floats. The array is sorted between the
  indices l and r. The alorithm is a modified quicksort which
  uses insertion sort for subfields with less than 'M' elements.
  The pivot element for each subdivision is chosen with a 'median
  of three' strategy. 
 */
/*--------------------------------------------------------------------------*/

#define M 10
void float_qsort(float * a, int l, int r)
{
	int i, j, m; 
        float tmp; /* change the type of this variable if you
                      rewrite the sort for another variable type */
	if(r-l > M){
	  /* quicksort with a median of three strategy
             to choose the pivot element
	  */
		i=l-1; j=r;
		m=l+(r-l)/2;
		if(a[l]>a[m]){
		        tmp=a[l]; a[l]=a[m]; a[m]=tmp;
		}
		if(a[l]>a[r]){
		        tmp=a[l]; a[l]=a[r]; a[r]=tmp;
		}
		else if(a[r]>a[m]){
		        tmp=a[r]; a[r]=a[m]; a[m]=tmp;
		}

		for(;;){
			while(a[++i]<a[r]);
			while(a[--j]>a[r]);
			if(i>=j) break;
			tmp=a[i]; a[i]=a[j]; a[j]=tmp;
		}
		tmp=a[i]; a[i]=a[r]; a[r]=tmp;

		float_qsort(a, l, i-1);
		float_qsort(a, i+1, r);
	}
	else{
	  /* insertion sort */
		for(i=l+1; i<=r; ++i){
			tmp=a[i];
			for(j=i-1; j>=l && tmp<a[j]; --j)
				a[j+1]=a[j];
			a[j+1]=tmp;
		}
	}
}
#undef M

