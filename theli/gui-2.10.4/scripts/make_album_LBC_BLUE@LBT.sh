#!/bin/bash -xv

# $1: main dir
# $2: science dir
# $3: image extension

. progs.ini
 
ARGUMENT="-l"
if [ "${V_WEIGHT_BINOUTLIER}" = "TRUE" ]; then
   ARGUMENT="${ARGUMENT} -r 10 10"
fi
 

DIR=`pwd`
cd /$1/$2

test ! -d BINNED_FITS && mkdir BINNED_FITS

ls -1 *_1$3.fits > ${TEMPDIR}/imagelist_$$

cat ${TEMPDIR}/imagelist_$$ |\
{
  while read file
  do
    BASE=`basename $file 1$3.fits`
    ${P_FITSFLIP} -i ${BASE}4$3.fits -o BINNED_FITS/${BASE}4$3_rot.fits -r 90
    ${P_ALBUM} -f ${DIR}/album_${INSTRUMENT}.conf ${ARGUMENT} -b ${V_WEIGHTBINSIZE} \
        -p -32 6170 6680 \
	${BASE}1$3.fits \
        ${BASE}2$3.fits \
	${BASE}3$3.fits \
        BINNED_FITS/${BASE}4$3_rot.fits \
	> BINNED_FITS/${BASE}$3mosaic.fits
    rm BINNED_FITS/${BASE}4$3_rot.fits
  done
}
