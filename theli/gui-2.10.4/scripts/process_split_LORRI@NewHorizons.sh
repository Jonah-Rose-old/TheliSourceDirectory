#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits LORRI@NewHorizons images

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


# rename the LORRI images (longer than 64 characters)

cd /$1/$2

mkdir ORIGINALS

ls -l | grep ^- | ${P_GAWK} '{print $NF}' > ${TEMPDIR}/fitskey_$$

cat ${TEMPDIR}/fitskey_$$ |\
{
  while read FILE
  do
      newname=`get_key ${FILE} UTCMID`
      if [ "${newname}" != "" ]; then
          cp ${FILE} LORRI.${newname}.fits
	  mv ${FILE} ORIGINALS/
      else
          echo ERROR: Could not find UTCMID in the FITS header
	  mv ${FILE} ORIGINALS/	  
      fi
  done
}

FILES=`ls -1l $1/$2/LORRI.*.fits | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

for FILE in ${FILES}
do
  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  BASE=`basename $FILE ${SUFFIX}`
  ${P_MISSFITS} ${FILE} -OUTFILE_TYPE SPLIT -WRITE_XML N
  rm ${FILE} ${BASE}.0[1-2].fits
  mv ${BASE}.00.fits ${BASE}.fits
  FILE=${BASE}.fits

  RA=`get_key ${FILE} "CRVAL1"`
  if [ "${RA}" = "" ]; then
      RA=0.001
  fi

  DEC=`get_key ${FILE} "CRVAL2"`
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

  OBJECT=`get_key ${FILE} "TARGET"`
  if [ "${OBJECT}" = "" ]; then
      OBJECT=UNKNOWN
  fi

  EQUINOX=`get_key ${FILE} "EQUINOX | sed 's/[^0-9.]//g'"`
  if [ "${EQUINOX}" = "" ]; then
      EQUINOX=2000.0
  fi

  MJD=`get_key ${FILE} "MJD-OBS"`
  if [ "${MJD}" = "" ]; then
      MJD=52000.0
  fi

  FILTER=`get_key ${FILE} "FILTER" | ${P_CLEANSTRING}`
  if [ "${FILTER}" = "" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "LST"`
  if [ "${LST}" = "" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  # Preserve the CD matrix if present.
  # If this does not work, replace the 'get_wcs' function below
  # with the explicit code in 'bash.include', and then modify it
  # according to your needs.
  get_wcs
  cdelt1=`get_key ${FILE} CDELT1`
  cdelt2=`get_key ${FILE} CDELT2`
  pc11=`get_key ${FILE} PC1_1`
  pc12=`get_key ${FILE} PC1_2`
  pc21=`get_key ${FILE} PC2_1`
  pc22=`get_key ${FILE} PC2_2`

  cd11=`echo ${cdelt1} ${pc11} | ${P_GAWK} '{printf "%g", $1*$2}'`
  cd12=`echo ${cdelt1} ${pc12} | ${P_GAWK} '{printf "%g", $1*$2}'`
  cd21=`echo ${cdelt2} ${pc21} | ${P_GAWK} '{printf "%g", $1*$2}'`
  cd22=`echo ${cdelt2} ${pc22} | ${P_GAWK} '{printf "%g", $1*$2}'`

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

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    tmp=`get_key ${FILE} "SPCUTCID"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${tmp}" DUMMY1
    k=$(( $k + 1 ))
  done

  rm ${FILE}

done
