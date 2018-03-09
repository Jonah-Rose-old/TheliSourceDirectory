#!/bin/bash

# $1: script dir
# $2: image dir
# $3: file extension

cd $1
DIR=$1

. progs.ini

cd $2

${P_DFITS} *_1$3.fits | ${P_FITSORT} CRVAL1 CRVAL2 | \
    ${P_GAWK} '(NR>1) {print $2, $3}' > ${TEMPDIR}/ditherradius.dat

${P_GAWK} '{print $1}' ${TEMPDIR}/ditherradius.dat \
    > ${TEMPDIR}/ditherradius_ra.dat
${P_GAWK} '{print $2}' ${TEMPDIR}/ditherradius.dat \
    > ${TEMPDIR}/ditherradius_dec.dat

DECCORRFAC=`${P_GETSTATS} -i ${TEMPDIR}/ditherradius_dec.dat -m mean | \
    ${P_GAWK} '{print cos($1*3.14159/180)}'`

ra_range=`${P_GAWK} 'BEGIN{min=360;max=0} {if ($1<min) min=$1; if ($1>max) max=$1} END {print '"${DECCORRFAC}"'*(max-min)*60}' ${TEMPDIR}/ditherradius_ra.dat`

dec_range=`${P_GAWK} 'BEGIN{min=90;max=-90} {if ($1<min) min=$1; if ($1>max) max=$1} END {print (max-min)*60}' ${TEMPDIR}/ditherradius_dec.dat`

ditherradius=`echo ${ra_range} ${dec_range} | ${P_GAWK} '{print 0.5*sqrt($1*$1+$2*$2)}'`

echo ${ditherradius}
