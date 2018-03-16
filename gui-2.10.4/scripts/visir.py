#!/usr/bin/env python

import sys
import numpy as np
import scipy as sp
from astropy.io import fits
from numpy import std, float32

image=sys.argv[1]
mode=sys.argv[2]

base=image.split(".fits")[0]

hdulist = fits.open(image)
header = hdulist[0].header

# Print file information and leave
if mode == "info":
    fits.info(image)

    print "OBJECT  = ", header['OBJECT']
    print "EXPTIME = ", header['EXPTIME']
    print "NEXP    = ", header['HIERARCH ESO TPL NEXP']
    print "NDIT    = ", header['HIERARCH ESO DET NDIT']
    print "NODPOS  = ", header['HIERARCH ESO SEQ NODPOS']
    print "FILTER1 = ", header['FILTER1']
    print "FILTER2 = ", header['FILTER2']
    print "NSLICES = ", header['NAXIS3']

    exit

data = hdulist[0].data
data = np.nan_to_num(data)
n_slice = int(header['NAXIS3'])
ndit = int(header['HIERARCH ESO DET NDIT'])
exptime = float(header['EXPTIME'])

exptime = exptime*ndit*n_slice

# Make a theli header
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
crval1=headercopy["RA"]
header["CRVAL1"] = crval1
crval2=headercopy["DEC"]
header["CRVAL2"] = crval2
header["EXPTIME"] = exptime
dateobs=headercopy["DATE-OBS"]
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
header["CTYPE1"] = 'RA---TAN'
header["CTYPE2"] = 'DEC--TAN'

for i in range (1,51):
    header["DUMMY"+str(i)] = 0

# Extract the 2nd last slice
# extract the second last slice from the cube
data_slice = data[n_slice-1]
fits.writeto(base+"_1.fits", data_slice, header, clobber=True)
