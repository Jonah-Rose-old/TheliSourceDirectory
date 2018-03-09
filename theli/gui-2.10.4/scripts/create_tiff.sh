#!/bin/bash -xv

# $1: main dir
# $2: science dir
# $3: image extension

# preliminary work:

INSTDIR=instruments_professional               
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then 
    INSTDIR=instruments_commercial             
fi                                             
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then 
    INSTDIR=~/.theli/instruments_user/         
fi                                             
. ${INSTDIR}/${INSTRUMENT:?}.ini               


cd $1/$2

test ! -d BINNED_TIFF && mkdir BINNED_TIFF
test ! -d BINNED_FITS && mkdir BINNED_FITS

ls -1 *$3.fits   > ${TEMPDIR}/tiffalllist_$$
ls -1 *_1$3.fits > ${TEMPDIR}/tiffmosaiclist_$$

leave=0

section=""

exec < ${TEMPDIR}/tiffmosaiclist_$$
while read file
do
    base=`basename ${file} _1$3.fits`
    if [ -f BINNED_FITS/${base}"_"$3"mosaic.fits" ]; then
	if [ "${INSTRUMENT}" = "DECam@CTIO" ] || [ "${INSTRUMENT}" = "FLAMINGOS2@GEMINI" ]; then
	    n1min=`${P_DFITS} BINNED_FITS/${base}"_"$3"mosaic.fits" | ${P_FITSORT} NAXIS1 | ${P_GAWK} '(NR==2) {printf "%d\n", $2*0.2}'` 
	    n1max=`${P_DFITS} BINNED_FITS/${base}"_"$3"mosaic.fits" | ${P_FITSORT} NAXIS1 | ${P_GAWK} '(NR==2) {printf "%d\n", $2*0.8}'` 
	    n2min=`${P_DFITS} BINNED_FITS/${base}"_"$3"mosaic.fits" | ${P_FITSORT} NAXIS2 | ${P_GAWK} '(NR==2) {printf "%d\n", $2*0.2}'` 
	    n2max=`${P_DFITS} BINNED_FITS/${base}"_"$3"mosaic.fits" | ${P_FITSORT} NAXIS2 | ${P_GAWK} '(NR==2) {printf "%d\n", $2*0.8}'` 
	    section="-s ${n1min} ${n1max} ${n2min} ${n2max}"
	fi
	sky=`${P_IMSTATS} -d 6 ${section} BINNED_FITS/${base}"_"$3"mosaic.fits" | ${P_GAWK} '($0!~/#/) {print $2}'`
	${P_IC} '%1 '$sky' -' BINNED_FITS/${base}"_"$3"mosaic.fits" > BINNED_FITS/${base}_"nosky.fits"
	${P_FITS2TIFF} -i BINNED_FITS/${base}_"nosky.fits" -b 8 \
	    -o BINNED_TIFF/${base}"_"$3"mosaic.tif" -r ${V_WEIGHTBINMIN} ${V_WEIGHTBINMAX}
	\rm BINNED_FITS/${base}_"nosky.fits"
	leave=1
    fi
done


if [ ! "${leave}" = "1" ]; then
    exec < ${TEMPDIR}/tiffalllist_$$
    while read file
    do
	base=`basename ${file} .fits`
	if [ "${V_WEIGHTBINSIZE}" != "1" ]; then
	    ${P_FITSBIN} -i ${file} -o BINNED_FITS/${base}"binned.fits" -b ${V_WEIGHTBINSIZE}
	else
	    cd BINNED_FITS
	    ln -s ../${file} ${base}"binned.fits"
	    cd ..
	fi    
	if [ "${INSTRUMENT}" = "DECam@CTIO" ] || [ "${INSTRUMENT}" = "FLAMINGOS2@GEMINI" ]; then
	    n1min=`${P_DFITS} BINNED_FITS/${base}"binned.fits" | ${P_FITSORT} NAXIS1 | ${P_GAWK} '(NR==2) {printf "%d\n", $2*0.2}'` 
	    n1max=`${P_DFITS} BINNED_FITS/${base}"binned.fits" | ${P_FITSORT} NAXIS1 | ${P_GAWK} '(NR==2) {printf "%d\n", $2*0.8}'` 
	    n2min=`${P_DFITS} BINNED_FITS/${base}"binned.fits" | ${P_FITSORT} NAXIS2 | ${P_GAWK} '(NR==2) {printf "%d\n", $2*0.2}'` 
	    n2max=`${P_DFITS} BINNED_FITS/${base}"binned.fits" | ${P_FITSORT} NAXIS2 | ${P_GAWK} '(NR==2) {printf "%d\n", $2*0.8}'` 
	    section="-s ${n1min} ${n1max} ${n2min} ${n2max}"
	fi
	sky=`${P_IMSTATS} -d 6 ${section} BINNED_FITS/${base}"binned.fits" | ${P_GAWK} '($0!~/#/) {print $2}'`
	${P_IC} '%1 '$sky' -' BINNED_FITS/${base}"binned.fits" > BINNED_FITS/${base}"nosky.fits"
	${P_FITS2TIFF} -i BINNED_FITS/${base}"nosky.fits" -b 8 \
	    -o BINNED_TIFF/${base}"binned.tif" -r ${V_WEIGHTBINMIN} ${V_WEIGHTBINMAX}
	\rm  BINNED_FITS/${base}"nosky.fits"
    done
fi
