#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits SuprimeCam_200101-200104@SUBARU images

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

  p_ls > ${TEMPDIR}/fitskey_$$

  cat ${TEMPDIR}/fitskey_$$ |\
  {
    while read FILE
    do
      newname=`get_key ${FILE} ${V_RENAME_FITSKEY}`
      if [ "${newname}" != "KEY_N/A" ] && [ "${newname}" != "" ]; then
         mv ${FILE} ${newname}
      else
         echo ERROR: Could not find ${V_RENAME_FITSKEY} in the FITS header
      fi
    done
  }
fi


FILES=`ls -1l $1/$2/ | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

cd /$1/$2

mkdir ORIGINALS

for FILE in ${FILES}
do
  ORIGNAME=${FILE}

  # rename the image to FRAMEID
  FRAMEID=`get_key ${FILE} "FRAMEID"`
  BASEIMG=`echo ${FRAMEID} | ${P_GAWK} '{l=length($1); print substr($1,1,l-1)}'`
  mv ${FILE} a_${BASEIMG}.fits
  FILE=a_${BASEIMG}.fits

  DETID0=`get_key ${FILE} "DET-ID"`
  DETID1=$((${DETID0}+1))
  # skip the bad CCD #6
  if [ "${DETID0}" = "6" ]; then
      mv ${FILE} ORIGINALS/
      continue
  fi
  if [ "${DETID0}" -ge "7" ]; then
      DETID0=$((${DETID0}-1))
  fi
  DETID0=$((${DETID0}+1))

  RA=`get_key ${FILE} "CRVAL1"`
  if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
      RA=0.0
  fi

  DEC=`get_key ${FILE} "CRVAL2"`
  if [ "${DEC}" = "" ] || [ "${DEC}" = "KEY_N/A" ]; then
      DEC=0.0
  fi

  EXPTIME=`get_key ${FILE} "EXPTIME"`
  if [ "${EXPTIME}" = "" ] || [ "${EXPTIME}" = "KEY_N/A" ]; then
      EXPTIME=1.0
  fi

  OBJECT=`get_key ${FILE} "OBJECT"`
  if [ "${OBJECT}" = "" ] || [ "${OBJECT}" = "KEY_N/A" ]; then
      OBJECT=UNKNOWN
  fi

  EQUINOX=`get_key ${FILE} "EQUINOX" | sed 's/[^0-9.]//g'`
  if [ "${EQUINOX}" = "" ] || [ "${EQUINOX}" = "KEY_N/A" ]; then
      EQUINOX=2000.0
  fi

  MJD=`get_key ${FILE} "MJD"`
  if [ "${MJD}" = "" ] || [ "${MJD}" = "KEY_N/A" ]; then
      MJD=52000.0
  fi

  FILTER=`get_key ${FILE} "FILTER01" | ${P_CLEANSTRING}`
  if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "LST"`
  if [ "${LST}" = "" ] || [ "${LST}" = "KEY_N/A" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 22:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  cdmatrix=`${P_DFITS} ${FILE} | ${P_FITSORT} CD1_1 CD1_2 CD2_1 CD2_2 | ${P_GAWK} '($1!="FILE") {print $2, $3, $4, $5}'`
  refpixel=`${P_DFITS} ${FILE} | ${P_FITSORT} CRPIX1 CRPIX2 | ${P_GAWK} '($1!="FILE") {print $2, $3}'`
  # the CD1_1 keyword has the wrong sign in early suprime cam data
  # (critical date: march 2001-03-01 (ESTIMATED)
  mjdint=`echo ${MJD} | ${P_GAWK} '{print int($1)}'`
  if [ "${mjdint}" -le "51969" ]; then
      cd11=`echo ${cdmatrix} | ${P_GAWK} '{print -1*$1}'`
  else
      cd11=`echo ${cdmatrix} | ${P_GAWK} '{print $1}'`
  fi
  cd12=`echo ${cdmatrix} | ${P_GAWK} '{print $2}'`
  cd21=`echo ${cdmatrix} | ${P_GAWK} '{print $3}'`
  cd22=`echo ${cdmatrix} | ${P_GAWK} '{print $4}'`
  cpx1=`echo ${refpixel} | ${P_GAWK} '{print $1}'`
  cpx2=`echo ${refpixel} | ${P_GAWK} '{print $2}'`

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${cpx1}" \
     -CRPIX2 "${cpx2}" \
     -CD11 "${cd11}" \
     -CD12 "${cd12}" \
     -CD21 "${cd21}" \
     -CD22 "${cd22}" \
     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
     -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
     -M11 "1" \
     -M12 "0" \
     -M21 "0" \
     -M22 "1" \
     ${FILE}

  tmp=`get_key ${FILE} "DATE-OBS"`
  ${P_REPLACEKEY} a_${BASEIMG}_1.fits "DATE-OBS= ${tmp}" DUMMY1
  ${P_REPLACEKEY} a_${BASEIMG}_1.fits "MJD-OBS = ${MJD}" DUMMY2

  mv -f a_${BASEIMG}_1.fits ${BASEIMG}_${DETID0}.fits 

  mv ${FILE} ORIGINALS/${ORIGNAME}
done
