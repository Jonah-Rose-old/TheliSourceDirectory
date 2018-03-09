#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits SOI@SOAR images

# THIS SCRIPT DEVIATES VERY SIGNIFICANTLY FROM THE
# STANDARD SPLITTING PROCEDURE BY THELI

# $1: main directory
# $2: science directory

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash.include


# rename the images to the value of a FITS key (if requested)

if [ "${V_PRE_RENAME_CHECKED}" = "1" ]; then

  cd /$1/$2
  mkdir ORIGINALS


  p_ls > ${TEMPDIR}/fitskey_$$

  cat ${TEMPDIR}/fitskey_$$ |\
  {
    while read FILE
    do
      newname=`get_key ${FILE} ${V_RENAME_FITSKEY}`
      if [ "${newname}" != "" ]; then
         mv ${FILE} ${newname}
      else
         echo ERROR: Could not find ${V_RENAME_FITSKEY} in the FITS header
         mv ${FILE} ORIGINALS/
      fi
    done
  }
fi


cd /$1/$2

FILES=`p_ls`

mkdir ORIGINALS

for FILE in ${FILES}
do
  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  BASE=`basename $FILE ${SUFFIX}`
  mv ${FILE} ${BASE}.fits
  FILE=${BASE}.fits

  ${P_PASTEDUALCHANNEL} -i ${FILE} -o ${BASE}p.fits -m SOI
  mv ${FILE} ORIGINALS
  FILE=${BASE}p.fits
  BASE=`basename $FILE .fits`

  RA=`get_key ${FILE} "RA"`
  RA=`${P_HMSTODECIMAL} ${RA}`
  if [ "${RA}" = "" ]; then
      RA=0.001
  fi

  DEC=`get_key ${FILE} "DEC"`
  DEC=`${P_DMSTODECIMAL} ${DEC}`
  if [ "${DEC}" = "" ]; then
      DEC=0.0
  fi

  EXPTIME=`get_key ${FILE} "EXPTIME"`
  if [ "${EXPTIME}" = "" ]; then
      EXPTIME=`get_key ${FILE} "EXPOSURE"`
  fi
  if [ "${EXPTIME}" = "" ]; then
      EXPTIME=1.0
  fi

  OBJECT=`get_key ${FILE} "OBJECT"`
  if [ "${OBJECT}" = "" ]; then
      OBJECT=UNKNOWN
  fi

  EQUINOX=`get_key ${FILE} "RADECEQ" | sed 's/[^0-9.]//g'`
  if [ "${EQUINOX}" = "" ]; then
      EQUINOX=2000.0
  fi

  DATEOBS1=`get_key ${FILE} "DATE-OBS"`
  DATEOBS2=`get_key ${FILE} "TIME-OBS"`
  DATEOBS=${DATEOBS1}"T"${DATEOBS2}

  MJD=`${P_MJD} -f ${DATEOBS} | ${P_GAWK} '{print $7}'`
  if [ "${MJD}" = "" ]; then
      MJD=52000.0
  fi

  FILT1=`get_key ${FILE} "FILTER1" | ${P_CLEANSTRING}`
  FILT2=`get_key ${FILE} "FILTER2" | ${P_CLEANSTRING}`
  FILTER=${FILT1}"+"${FILT2}
  if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "LSTHDR"`
  LST=`${P_DMSTODECIMAL} ${LST} | ${P_GAWK} '{print $1*3600.}'`
  if [ "${LST}" = "" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  POSANGLE=`get_key ${FILE} "DECPANGL"`
  cdmatrix=`${P_GETCDMATRIX} -s ${PIXSCALE} -p ${POSANGLE} -n`
  cd11=`echo ${cdmatrix} | ${P_GAWK} '{print $1}'`
  cd12=`echo ${cdmatrix} | ${P_GAWK} '{print $2}'`
  cd21=`echo ${cdmatrix} | ${P_GAWK} '{print $3}'`
  cd22=`echo ${cdmatrix} | ${P_GAWK} '{print $4}'`

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${REFPIXX[1]},${REFPIXX[2]}" \
     -CRPIX2 "${REFPIXY[1]},${REFPIXY[2]}" \
     -CD11 "${cd11},${cd11}" \
     -CD12 "${cd12},${cd12}" \
     -CD21 "${cd21},${cd21}" \
     -CD22 "${cd22},${cd22}" \
     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
     -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
     -M11 "1,1" \
     -M12 "0,0" \
     -M21 "0,0" \
     -M22 "1,1" \
     ${FILE}

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${DATEOBS}" DUMMY1
    tmp=`get_key ${FILE} "OBSID"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "OBSID   = ${tmp}" DUMMY2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY3
    mv ${BASE}_${k}.fits SOI.${DATEOBS}_${k}.fits
    k=$(( $k + 1 ))
  done

  rm ${FILE}

done
