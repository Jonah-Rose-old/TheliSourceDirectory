#!/bin/bash -xv

# creates swarp headers from the image headers

# $1: main dir
# $2: science dir
# $3: image extension

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash.include

DIR=`pwd`

# first create the zeroorder filelist:
ls -1 /$1/$2/*$3.fits > ${TEMPDIR}/headerastrom_list_$$

# create the header file
if [ ! -d $1/$2/headers ]; then
    mkdir $1/$2/headers
else 
    rm -rf $1/$2/headers
    mkdir $1/$2/headers
fi

cat ${TEMPDIR}/headerastrom_list_$$ |\
{
  while read file
  do
    BASE=`basename $file $3.fits`
    touch $1/$2/headers/${BASE}.head

    crpix1=`get_key $file CRPIX1`
    crpix2=`get_key $file CRPIX2`
    crval1=`get_key $file CRVAL1`
    crval2=`get_key $file CRVAL2`
    cd11=`get_key $file CD1_1`
    cd12=`get_key $file CD1_2`
    cd21=`get_key $file CD2_1`
    cd22=`get_key $file CD2_2`
    exptime=`get_key $file EXPTIME`
    equinox=`get_key $file EQUINOX`
    airmass=`get_key $file AIRMASS`
    flxscale=`echo 0.0 ${exptime} | ${P_GAWK} '{print 10^(-0.4*$1)/$2}'`

    {
    echo "CRPIX1  = ${crpix1}"
    echo "CRPIX2  = ${crpix2}"
    echo "CD1_1   = ${PIXSCX}"
    echo "CD2_2   = ${PIXSCY}"
    echo "CD1_2   = 0.0"
    echo "CD2_1   = 0.0"
    echo "CRVAL1  = ${crval1}"
    echo "CRVAL2  = ${crval2}"
    echo "EXPTIME = ${exptime}"
    echo "EQUINOX = ${equinox}"
#    echo "LINXRMS = ${xrms}"
#    echo "LINYRMS = ${yrms}"
#    echo "ASTXRMS = ${xrms}"
#    echo "ASTYRMS = ${yrms}"
#    echo "LINTOL  = 5"
    echo "CTYPE1  = 'RA---TAN'"
    echo "CTYPE2  = 'DEC--TAN'"
#    echo "PSORD   = 1"
#    echo "PSTOL   = 5"
    echo "PV1_0   = 0.0"
    echo "PV1_1   = 1.0"
    echo "PV1_2   = 0.0"
    echo "PV2_0   = 0.0"
    echo "PV2_1   = 1.0"
    echo "PV2_2   = 0.0"
    echo "AIRMASS = ${airmass}"
    echo "ZP0     = 0."
    echo "ZP      = 0."
    echo "RZP     = 0."
    echo "FLXSCALE= ${flxscale}"
    } >> $1/$2/headers/${BASE}.head

    i=$(( ${i} + 1 ))
  done
}
