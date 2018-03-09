#!/bin/bash -xv

# the script coadds images with
# Emmanuels swarp program. It takes
# the resampled images created by
# resample_coadd_swarp_para.sh.
# So far it cannot be parallelised!
#
# Note that a coaddition with another header
# than that created by prepare_coadd_swarp.sh
# and later used by resample_coadd_swarp_para.sh
# leads to false results !!!

# 02.03.2004:
# The ending of the resampled FITS images is now
# "COADIDENT".resamp.fits instead of simply .resamp.fits
#

#$1: main dir.
#$2: science dir.

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

DIR=`pwd`

PSCALE=${PIXSCALE}
if [ ! "${V_COADD_PIXSCALE}_A" = "_A" ]; then
    PSCALE=${V_COADD_PIXSCALE}
fi


USERDIR=~/.theli/scripts/

if [ "${V_COADD_IDENT}_A" = "_A" ]; then
   V_COADD_IDENT="default"
fi

cd /$1/$2/coadd_${V_COADD_IDENT}

if [ ! -f coadd.head ]; then
    cp ${USERDIR}/coadd.head .
fi

# and simply go!!!


SWARPCONF=${DATACONF}/create_coadd_swarp2.swarp

clipped=""
if [ "${V_COADD_COMBINETYPE}" = "CLIPPED" ]; then
    clipped="-CLIP_AMPFRAC ${V_COADD_CLIPAMPFRAC} -CLIP_SIGMA ${V_COADD_CLIPSIGMA}"
fi

${P_SWARP} -c ${SWARPCONF} \
    *.${V_COADD_IDENT}.resamp.fits \
    -RESAMPLE N -COMBINE Y -IMAGEOUT_NAME coadd.fits \
    -PIXEL_SCALE ${PSCALE} \
    -WEIGHTOUT_NAME coadd.weight.fits \
    -WEIGHT_TYPE MAP_WEIGHT \
    -CELESTIAL_TYPE ${V_COADD_CELESTIALTYPE} \
    -PROJECTION_TYPE ${V_COADD_PROJECTION} \
    -RESAMPLING_TYPE ${V_COADD_KERNEL} \
    -COMBINE_TYPE ${V_COADD_COMBINETYPE} \
    -NTHREADS ${NPARA} \
    -RESCALE_WEIGHTS ${V_COADD_RESCALEWEIGHTS} \
    -BLANK_BADPIXELS Y $clipped

rm coadd.head

if [ ! "${V_COADD_SKYPOSANGLE}_A" = "_A" ]; then
    ${P_REPLACEKEY} coadd.fits "CROTA2  = ${V_COADD_SKYPOSANGLE}" "DUMMY1"
fi

cd ${DIR}
