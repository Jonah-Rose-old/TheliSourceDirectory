#!/usr/bin/env python

import sys
import numpy as np
import scipy as sp
import re
from astropy.io import fits
from numpy import std, float32

# Check for the presence of NANs in a data array
def has_nan( array ):
    for x in np.nditer(array):
        if np.isnan(x): 
            return True
    return False

def split_datasection(datasec):
    """
    Return the x/y pixel boundaries of the data section.
    :param datasec: str e.g. '[33:2080,1:4612]'
    :return: ((xmin,xmax),(ymin,ymax))
    """
    datasec = re.findall(r'(\d+)', datasec)
    x1 = min(int(datasec[0]), int(datasec[1]))
    x2 = max(int(datasec[0]), int(datasec[1]))
    y1 = min(int(datasec[2]), int(datasec[3]))
    y2 = max(int(datasec[2]), int(datasec[3]))
    return (x1, x2), (y1, y2)

def do_overscan(data,biassec,n,m):
    oscan_x, oscan_y = split_datasection(biassec)
    # manual override
    oxmin1 = 3
    oxmax1 = 17
    oxmin2 = 1193
    oxmax2 = 1210
    dim_1 = oxmax1-oxmin1+1
    dim_2 = oxmax2-oxmin2+1
    oscan1 = np.zeros(dim_1, dtype=float32)
    oscan2 = np.zeros(dim_2, dtype=float32)
    slope = 0.
    for j in range(0,m):
        oscan1[0:dim_1] = data[j][oxmin1:oxmax1+1]
        oscan2[0:dim_2] = data[j][oxmin2:oxmax2+1]
        oscan_left  = oscan1[~np.isnan(oscan1)]
        oscan_right = oscan2[~np.isnan(oscan2)]
        oscan_median_left  = np.median(oscan_left, overwrite_input=True)
        oscan_median_right = np.median(oscan_right, overwrite_input=True)
        # subtract the mean overscan
        data[j][0:n] = data[j][0:n] - (oscan_median_left + oscan_median_right)/2.
            
def make_theli_header(image):
    phdu = fits.getheader(image)
    theliheader = phdu
    headercopy  = phdu.copy()
    thelikeys   = ['SIMPLE','BITPIX','NAXIS','NAXIS1','NAXIS2','INSTRUME','EXPTIME','CTYPE1',
                   'CTYPE2','CRPIX1','CRPIX2','CD1_1','CD2_2','CD2_1','CD1_2','CRVAL1',
                   'CRVAL2','RADECSYS','OBJECT','AIRMASS','EQUINOX','DATE-OBS','MJD-OBS',
                   'GABODSID']

    phdukeys = phdu.keys()

    # Keep THELI keys only
    for keyword in phdukeys:
        k = 0
        for thelikey in thelikeys:
            if keyword == thelikey:
                k = 1
        if (k == 0 and keyword in phdu):
            del phdu[keyword]
                    
    # Add 50 DUMMY keywords
    for i in range (1,51):
        phdu["DUMMY"+str(i)] = 0

    return phdu


image=sys.argv[1]
base=image.split(".fits")[0]

# the number of image extensions in the file
f = fits.open(image)
n_ext = len(f)-1

phdu = make_theli_header(image)

# Read the dimensions of one amplifier
header = fits.getheader(image,1)
datasec = header['DATASEC']
data_x, data_y = split_datasection(datasec)
dim_n = data_x[1] - data_x[0] + 1;
dim_m = data_y[1] - data_y[0] + 1;

# Array containing an entire chip
data_chip = np.zeros((dim_m,8*dim_n), dtype=float32)

# Relative gain correction factors (normalized to the median)
gain = np.zeros(32,dtype=float32)

crpix1 = np.zeros(16,dtype=float32)
crpix2 = np.zeros(16,dtype=float32)
cd11 = np.zeros(16,dtype=float32)
cd12 = np.zeros(16,dtype=float32)
cd21 = np.zeros(16,dtype=float32)
cd22 = np.zeros(16,dtype=float32)

gain[0]  = 0.96714
gain[1]  = 1.04102
gain[2]  = 0.99646
gain[3]  = 0.96988
gain[4]  = 0.94289
gain[5]  = 1.00063
gain[6]  = 1.00673
gain[7]  = 1.05860
gain[8]  = 0.97980
gain[9]  = 0.94911
gain[10] = 0.94306
gain[11] = 0.95201
gain[12] = 0.92052
gain[13] = 0.91607
gain[14] = 0.92155
gain[15] = 0.92343
gain[16] = 1.01826
gain[17] = 1.00285
gain[18] = 1.01113
gain[19] = 1.01860
gain[20] = 0.99937
gain[21] = 1.04536
gain[22] = 1.04696
gain[23] = 1.04268
gain[24] = 1.03726
gain[25] = 1.02898
gain[26] = 1.01170
gain[27] = 0.99178
gain[28] = 1.00645
gain[29] = 0.96610
gain[30] = 1.20100
gain[31] = 0.95036

crpix1[0] = 9519
crpix1[1] = 4839
crpix1[2] = 9560
crpix1[3] = 4861
crpix1[4] = -242
crpix1[5] = -4924
crpix1[6] = -258
crpix1[7] = -4964
crpix1[8] = 9553
crpix1[9] = 4854
crpix1[10] = 9511
crpix1[11] = 4833
crpix1[12] = -264
crpix1[13] = -4968
crpix1[14] = -243
crpix1[15] = -4929

crpix2[0] = -509
crpix2[1] = -489
crpix2[2] = -5221
crpix2[3] = -5185
crpix2[4] = -444
crpix2[5] = -464
crpix2[6] = -5139
crpix2[7] = -5177
crpix2[8] = 9852
crpix2[9] = 9813
crpix2[10] = 5136
crpix2[11] = 5114
crpix2[12] = 9778
crpix2[13] = 9815
crpix2[14] = 5079
crpix2[15] = 5100

cd11[0] = -1.092266e-04
cd11[1] = -1.107024e-04
cd11[2] = -1.089964e-04
cd11[3] = -1.101159e-04
cd11[4] = -1.106932e-04
cd11[5] = -1.091623e-04
cd11[6] = -1.101168e-04
cd11[7] = -1.088940e-04
cd11[8] = -1.089626e-04
cd11[9] = -1.100747e-04
cd11[10] = -1.092400e-04
cd11[11] = -1.106954e-04
cd11[12] = -1.101298e-04
cd11[13] = -1.088948e-04
cd11[14] = -1.106953e-04
cd11[15] = -1.091175e-04

cd12[0] = -6.962677e-07
cd12[1] = -4.818470e-07
cd12[2] = -1.323555e-06
cd12[3] = -6.863427e-07
cd12[4] = -2.362441e-07
cd12[5] = -2.321675e-08
cd12[6] = -8.855917e-08
cd12[7] = 5.793042e-07
cd12[8] = 3.183011e-07
cd12[9] = -3.053775e-07
cd12[10] = -3.240071e-07
cd12[11] = -5.024645e-07
cd12[12] = -8.968568e-07
cd12[13] = -1.520879e-06
cd12[14] = -6.931475e-07
cd12[15] = -9.242692e-07

cd21[0] = 3.488591e-08
cd21[1] = -1.598428e-07
cd21[2] = 6.583665e-07
cd21[3] = 1.607340e-08
cd21[4] = -5.679542e-07
cd21[5] = -7.702238e-07
cd21[6] = -7.746407e-07
cd21[7] = -1.396596e-06
cd21[8] = -1.692830e-06
cd21[9] = -1.004818e-06
cd21[10] = -1.055388e-06
cd21[11] = -8.450872e-07
cd21[12] = -1.653654e-07
cd21[13] = 4.720556e-07
cd21[14] = -3.731945e-07
cd21[15] = -1.485770e-07

cd22[0] = 1.101122e-04
cd22[1] = 1.106666e-04
cd22[2] = 1.088880e-04
cd22[3] = 1.090831e-04
cd22[4] = 1.106697e-04
cd22[5] = 1.101674e-04
cd22[6] = 1.091008e-04
cd22[7] = 1.088941e-04
cd22[8] = 1.088467e-04
cd22[9] = 1.090222e-04
cd22[10] = 1.100868e-04
cd22[11] = 1.106571e-04
cd22[12] = 1.090355e-04
cd22[13] = 1.088338e-04
cd22[14] = 1.106676e-04
cd22[15] = 1.101265e-04

amp_count = 0
for ex in range(1,n_ext+1):
    # which CCD are we processing
    if (ex>=1 and ex<=8):
        ccdnum = 1
    if (ex>=9  and ex<=16):
        ccdnum = 5
    if (ex>=17 and ex<=24): 
        ccdnum = 9
    if (ex>=25 and ex<=32):
        ccdnum = 13

    # Read the data and header units
    data, header = fits.getdata(image,ex,header=True)
    n = header['NAXIS1']
    m = header['NAXIS2']
    biassec = header['BIASSEC']
    datasec = header['DATASEC']
    
    # For some reason I have to do the gain correction first.
    # Otherwise, if applied to bias data, the outpout pixels concentrate around positive 
    # zero and 65535 (not sure why)

    # Gain correction
    data = data / gain[ex-1]

    # Overscan correction
    do_overscan(data,biassec,n,m)

    # Paste amplifiers
    data_chip[0:dim_m,amp_count*dim_n:(amp_count+1)*dim_n] = data[0:dim_m,data_x[0]-1:data_x[1]]

    dim_2n = 4*dim_n
    dim_2m = dim_m/2

    data_ll = data_chip[0:dim_2m,0:dim_2n]
    data_lr = data_chip[0:dim_2m,dim_2n:8*dim_n]
    data_ul = data_chip[dim_2m:dim_m,0:dim_2n]
    data_ur = data_chip[dim_2m:dim_m,dim_2n:8*dim_n]

    amp_count = amp_count + 1

    # Write chip into FITS file
    if (ex==8 or ex==16 or ex==24 or ex==32):
        print "Writing data for detector", ccdnum
        phdu['FILTER']  = header['FILTER']
        phdu['CRVAL1']  = header['CRVAL1']
        phdu['CRVAL2']  = header['CRVAL2']
        phdu['CTYPE1']  = header['CTYPE1']
        phdu['CTYPE2']  = header['CTYPE2']
        phdu['CRPIX1']  = crpix1[ccdnum-1]
        phdu['CRPIX2']  = crpix2[ccdnum-1]
        phdu['CD1_1']   = cd11[ccdnum-1]
        phdu['CD1_2']   = cd12[ccdnum-1]
        phdu['CD2_1']   = cd21[ccdnum-1]
        phdu['CD2_2']   = cd22[ccdnum-1]
        phdu['LST']     = header['ST']
        phdu['EQUINOX'] = 2000.0
        outfile = base+"_"+str(ccdnum)+".fits"
        fits.writeto(outfile, data_ll, phdu, clobber=True)
        ccdnum=ccdnum+1
        outfile = base+"_"+str(ccdnum)+".fits"
        fits.writeto(outfile, data_lr, phdu, clobber=True)
        ccdnum=ccdnum+1
        outfile = base+"_"+str(ccdnum)+".fits"
        fits.writeto(outfile, data_ul, phdu, clobber=True)
        ccdnum=ccdnum+1
        outfile = base+"_"+str(ccdnum)+".fits"
        fits.writeto(outfile, data_ur, phdu, clobber=True)
        amp_count = 0
f.close()
