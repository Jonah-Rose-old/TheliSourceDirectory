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
    ${P_ALBUM} -f ${DIR}/album_${INSTRUMENT}.conf ${ARGUMENT} -b ${V_WEIGHTBINSIZE} \
	-p -32 24600 29000 \
	${BASE}1$3.fits \
        ${BASE}2$3.fits \
	${BASE}3$3.fits \
        ${BASE}4$3.fits \
	${BASE}5$3.fits \
        ${BASE}6$3.fits \
	${BASE}7$3.fits \
        ${BASE}8$3.fits \
        ${BASE}9$3.fits \
        ${BASE}10$3.fits \
	${BASE}11$3.fits \
        ${BASE}12$3.fits \
	${BASE}13$3.fits \
        ${BASE}14$3.fits \
	${BASE}15$3.fits \
        ${BASE}16$3.fits \
	${BASE}17$3.fits \
        ${BASE}18$3.fits \
        ${BASE}19$3.fits \
        ${BASE}20$3.fits \
	${BASE}21$3.fits \
        ${BASE}22$3.fits \
	${BASE}23$3.fits \
        ${BASE}24$3.fits \
	${BASE}25$3.fits \
        ${BASE}26$3.fits \
	${BASE}27$3.fits \
        ${BASE}28$3.fits \
        ${BASE}29$3.fits \
        ${BASE}30$3.fits \
	${BASE}31$3.fits \
        ${BASE}32$3.fits \
	${BASE}33$3.fits \
        ${BASE}34$3.fits \
	${BASE}35$3.fits \
        ${BASE}36$3.fits \
	${BASE}37$3.fits \
        ${BASE}38$3.fits \
        ${BASE}39$3.fits \
        ${BASE}40$3.fits \
	${BASE}41$3.fits \
        ${BASE}42$3.fits \
	${BASE}43$3.fits \
        ${BASE}44$3.fits \
	${BASE}45$3.fits \
        ${BASE}46$3.fits \
	${BASE}47$3.fits \
        ${BASE}48$3.fits \
        ${BASE}49$3.fits \
        ${BASE}50$3.fits \
	${BASE}51$3.fits \
        ${BASE}52$3.fits \
	${BASE}53$3.fits \
        ${BASE}54$3.fits \
	${BASE}55$3.fits \
        ${BASE}56$3.fits \
	${BASE}57$3.fits \
        ${BASE}58$3.fits \
        ${BASE}59$3.fits \
        ${BASE}60$3.fits \
	> BINNED_FITS/${BASE}$3mosaic.fits
  done
}
