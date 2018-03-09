#!/bin/bash -xv

# this script inserts RA and DEC values in degrees
# into already existing CRVAL1 and CRVAL2 keywords.

# $1: main dir
# $2: Science dir
# $3: extension before .fits (before the extension we have the chip number _i
# $4: either of "crval+cd" or "crval"
# $5: chips to be processed


INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

ra=${V_AP_REFRA}
dec=${V_AP_REFDEC}
mode=$4

# check for the coordinate format
ra_format=`echo ${ra} | ${P_GAWK} '{if ($1 ~ ":") print "hhmmss"; else print "decimal"}'`
dec_format=`echo ${dec} | ${P_GAWK} '{if ($1 ~ ":") print "ddmmss"; else print "decimal"}'`

if [ "${ra_format}" != "decimal" ]; then
    ra=`${P_HMSTODECIMAL} ${ra}` 
fi
if [ "${dec_format}" != "decimal" ]; then
    dec=`${P_DMSTODECIMAL} ${dec}` 
fi

for CHIP in $5
do
  ls -1 /$1/$2/*_${CHIP}$3.fits > ${TEMPDIR}/corrcrval_$$

  cat ${TEMPDIR}/corrcrval_$$ |\
  {
    while read IMAGE
    do
  	echo "updating CRVAL1/2 keyword of ${IMAGE}"

        # take into account possible link
	REALIMAGE=${IMAGE}
    	DIR=`dirname ${IMAGE}`

	if [ -L ${IMAGE} ]; then
             REALIMAGE=`${P_READLINK} ${IMAGE}`
             DIR=`dirname ${REALIMAGE}`
	fi
     
	# if requested, insert CRVAL1 and CRVAL2
	if [ ! "${ra}" = "header" ] && [ ! "${dec}" = "header" ]; then
	    ${P_REPLACEKEY} ${REALIMAGE} "CRVAL1  = ${ra}" "CRVAL1"
	    ${P_REPLACEKEY} ${REALIMAGE} "CRVAL2  = ${dec}" "CRVAL2"

	    # if requested, also change the CD matrix
	    if [ "${mode}" = "crval+cd" ]; then
  	        # check if CDELT1/2 have the correct signs for north up and east left
		cd11=`echo ${PIXSCALE} | ${P_GAWK} '{printf "%e\n", -1.0*$1/3600.}'`
		cd22=`echo ${PIXSCALE} | ${P_GAWK} '{printf "%e\n", +1.0*$1/3600.}'`

		${P_REPLACEKEY} ${REALIMAGE} "CD1_1   = ${cd11}" "CD1_1"
		${P_REPLACEKEY} ${REALIMAGE} "CD2_2   = ${cd22}" "CD2_2"
		${P_REPLACEKEY} ${REALIMAGE} "CD1_2   = 0.0" "CD1_2"
		${P_REPLACEKEY} ${REALIMAGE} "CD2_1   = 0.0" "CD2_1"
	    fi
	fi
    done
  }
done
