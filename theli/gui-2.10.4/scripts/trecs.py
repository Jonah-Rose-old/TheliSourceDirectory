#!/usr/bin/env python

import sys
import numpy as np
import scipy as sp
from astropy.io import fits
from numpy import std, float32

image=sys.argv[1]
mode=sys.argv[2]

base=image.split(".fits")[0]

# Check for the presence of NANs in a data array
def has_nan( array ):
    for x in np.nditer(array):
        if np.isnan(x): 
            return True
    return False

# The HDU
header=fits.getheader(image)

# Print file information and leave
if mode == "info":
    fits.info(image)

    print "OBSMODE = ", header['OBSMODE']
    print "FRMCOADD= ", header['FRMCOADD']
    print "NSAVSETS= ", header['NSAVSETS']
    print "NNODSETS= ", header['NNODSETS']
    print "NODSETS = ", header['NODSETS']
    print "OBJTIME = ", header['OBJTIME']
    print "FILTER1 = ", header['FILTER1']
    print "FILTER2 = ", header['FILTER2']

    exit

# Some other consistency checks
if header['OBSMODE'] != 'chop-nod':
    print "Only chop-nod mode is supported."
    exit

# the number of image extensions in the file
n_ext = len(fits.open(image))-1

# the image geometry
naxis1 = (fits.getheader(image,1))['NAXIS1']
naxis2 = (fits.getheader(image,1))['NAXIS2']

# initialise the sky and the science arrays
sci1 = np.ndarray(shape=(naxis2,naxis1), dtype=float32)
sci2 = np.ndarray(shape=(naxis2,naxis1), dtype=float32)
sky1 = np.ndarray(shape=(naxis2,naxis1), dtype=float32)
sky2 = np.ndarray(shape=(naxis2,naxis1), dtype=float32)
tot1 = np.ndarray(shape=(naxis2,naxis1), dtype=float32)
tot  = np.ndarray(shape=(naxis2,naxis1), dtype=float32)

theliheader = header
headercopy = header.copy()

thelikeys=['SIMPLE','BITPIX','NAXIS','NAXIS1','NAXIS2','INSTRUME','CTYPE1',
           'CTYPE2','CRPIX1','CRPIX2','CD1_1','CD2_2','CD2_1','CD1_2','CRVAL1',
           'CRVAL2','RADECSYS','OBJECT','AIRMASS','EQUINOX','MJD-OBS']

headerkeys=header.keys()

for keyword in headerkeys:
    k = 0
    for thelikey in thelikeys:
        if keyword == thelikey:
            k = 1
    if k == 0:
        del header[keyword]

# reconstruct missing header keywords

exptime=headercopy["OBJTIME"]
header["EXPTIME"] = exptime
date=headercopy["DATE"]
time=headercopy["UT"]
dateobs=date+"T"+time
header["DATEOBS"] = dateobs
filter1=headercopy["FILTER1"]
filter2=headercopy["FILTER2"]
if filter1 == "Open":
    filter1 = ""
if filter2 == "Open":
    filter2 = ""
filter=filter1+filter2
header["FILTER"] = filter
gabodsid=int(headercopy["MJD-OBS"])-51180
header["GABODSID"] = gabodsid

for i in range (1,51):
    header["DUMMY"+str(i)] = 0

exptime_ind=exptime/n_ext*2.

header_ind = header.copy()
header_ind["EXPTIME"] = exptime_ind

# Average the nod and chop savesets
k = 0
u = 1
skip = False
skipind = False
nadd = 0
for i in range(1,n_ext+1):
    data = fits.getdata(image,i)
    nimg = data.shape[0]
    nodpos = (fits.getheader(image,i))['NOD']
    # If we are in NOD=A, then dim3=1 contains the on-source position 
    # and dim3=2 the sky position. If we are in NOD=B, it is reversed
    for j in range(0,nimg):
        if nodpos == 'A':
            sci1 += data[j,0,:,:]
            sky1 += data[j,1,:,:]
        if nodpos == 'B':
            sci2 += data[j,1,:,:]
            sky2 += data[j,0,:,:]

    # check for the presence of NaN values; increment only if absent!
    if (not has_nan(sci1)) and  (not has_nan(sci2)) and (not has_nan(sky1)) and (not has_nan(sky2)):
        tot1 += sci1 - sky1 + sci2 - sky2
        tot += sci1 - sky1 + sci2 - sky2
        nadd = nadd + 1
        skipind = False
    else:
        skipind = True
        print "INFO: Image in extension ", i, "has NaN values. Skipping over it..."
        
    # Logically combine the skip values for the individual extension ('skipind')
    # and for the combination of the two ('skip'). If one of them is true, the 
    # result has to be true so that we can skip over it
    skip = skipind or skip;

    sci1[:] = 0.
    sci2[:] = 0.
    sky1[:] = 0.
    sky2[:] = 0.

    if mode ==  "split":
        if k == 1:
            if skip == False:
                header_ind.update("BACK_RMS",std(tot1))
                fits.writeto(base+"_"+str(u)+"_1.fits", tot1, header_ind, output_verify='fix',clobber=True)
                tot1[:] = 0.
                k = 0
                u += 1
                skipind = False
            if skip == True:
                skip = False
        else:
            k += 1

if mode == "normal":
    exptime = exptime_ind * nadd/2
    header["EXPTIME"] = exptime

    fits.writeto(base+"_1.fits", tot, header, clobber=True)
