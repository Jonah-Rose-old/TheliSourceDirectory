#!/bin/bash

# $1: script dir
# $2: image dir
# $3: file extension

cd $1
DIR=$1

# preliminary work:
INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

# get the CD matrix, NAXIS1/2 and CRPIX1/2 from the first exposure
cd $2
image=`ls *_1$3.fits | ${P_GAWK} '(NR==1)'`
base=`basename ${image} _1$3.fits`
# a list of all CRVALs
${P_DFITS} *_1$3.fits | fitsort CRVAL1 CRVAL2 | ${P_GAWK} '(NR>=2) {print $2, $3}' > \
    ${TEMPDIR}/positions.txt
# the detector array geometry
${P_DFITS} ${base}_*$3.fits | ${P_FITSORT} NAXIS1 NAXIS2 CRPIX1 CRPIX2 CD1_1 CD1_2 CD2_1 CD2_2 | \
    ${P_GAWK} '(NR>=2) {print $2, $3, $4, $5, $6, $7, $8, $9}' > ${TEMPDIR}/geometries.txt
# get the refcat radius
rad=`${P_GETREFCATRADIUS} -i ${TEMPDIR}/geometries.txt -l ${TEMPDIR}/positions.txt`
echo "${rad}"
