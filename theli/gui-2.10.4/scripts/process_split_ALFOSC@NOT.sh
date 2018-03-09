#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits ALFOSC@NOT images

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
  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  BASE=`basename $FILE $SUFFIX`
  mv ${FILE} ${BASE}.fits
  FILE=${BASE}.fits

  RA=`get_key ${FILE} "RA"`
  if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
      RA=0.0
  fi

  DEC=`get_key ${FILE} "DEC"`
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

  DATEOBS=`get_key ${FILE} "DATE-OBS"`
  MJD=`${P_MJD} -f ${DATEOBS} | ${P_GAWK} '{print $7}'`

  FILTER=`get_key ${FILE} "ALFLTNM" | ${P_CLEANSTRING}`
  if [ "${FILTER}" = "Open" ]; then
      FILTER=`get_key ${FILE} "FAFLTNM" | ${P_CLEANSTRING}`
  fi
  if [ "${FILTER}" = "Open" ]; then
      FILTER=`get_key ${FILE} "FBFLTNM" | ${P_CLEANSTRING}`
  fi
  if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "ST"`
  if [ "${LST}" = "" ] || [ "${LST}" = "KEY_N/A" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  cd11=`get_key ${FILE} "CD1_1"`
  cd12=`get_key ${FILE} "CD1_2"`
  cd21=`get_key ${FILE} "CD2_1"`
  cd22=`get_key ${FILE} "CD2_2"`
  crpix1=`get_key ${FILE} "CRPIX1"`
  crpix2=`get_key ${FILE} "CRPIX2"`

  if [ "${cd11}"_A = _A ]; then
      cd11=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CD1_1 | ${P_GAWK} '(NR==2) {print $2}'`
  fi
  if [ "${cd12}"_A = _A ]; then
      cd12=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CD1_2 | ${P_GAWK} '(NR==2) {print $2}'`
  fi
  if [ "${cd21}"_A = _A ]; then
      cd21=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CD2_1 | ${P_GAWK} '(NR==2) {print $2}'`
  fi
  if [ "${cd22}"_A = _A ]; then
      cd22=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CD2_2 | ${P_GAWK} '(NR==2) {print $2}'`
  fi
  if [ "${crpix1}"_A = _A ]; then
      crpix1=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CRPIX1 | ${P_GAWK} '(NR==2) {print $2}'`
  fi
  if [ "${crpix2}"_A = _A ]; then
      crpix2=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CRPIX2 | ${P_GAWK} '(NR==2) {print $2}'`
  fi

  ${P_FITSSPLIT_ECL} \
      -CRPIX1 "${crpix1}" \
      -CRPIX2 "${crpix2}" \
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
  ${P_REPLACEKEY} ${BASE}_1.fits "DATE-OBS= ${tmp}" DUMMY1
  ${P_REPLACEKEY} ${BASE}_1.fits "MJD-OBS = ${MJD}" DUMMY2

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}

done
