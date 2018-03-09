#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits SITe#T5b@TLS images

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

  # RA is in decimal hours ...
  RA=`get_key ${FILE} "RA" | awk '{print $1/24.*360.}'`
  if [ "${RA}" = "" ]; then
      RA=0.001
  fi

  DEC=`get_key ${FILE} "DEC"`
  if [ "${DEC}" = "" ]; then
      DEC=0.0
  fi

  EXPTIME=`get_key ${FILE} "EXP_TIME"`
  if [ "${EXPTIME}" = "" ]; then
      EXPTIME=`get_key ${FILE} "EXPOSURE"`
  fi
  if [ "${EXPTIME}" = "" ]; then
      EXPTIME=`get_key ${FILE} "EXPTIME"`
  fi
  if [ "${EXPTIME}" = "" ]; then
      EXPTIME=1.0
  fi

  OBJECT=`get_key ${FILE} "OBJECT"`
  if [ "${OBJECT}" = "" ]; then
      OBJECT=UNKNOWN
  fi

  EQUINOX=`get_key ${FILE} "EQUINOX" | sed 's/[^0-9.]//g'`
  if [ "${EQUINOX}" = "" ] || [ "${EQUINOX}" = "0.0" ]; then
      EQUINOX=2000.0
  fi
  EQUINOX=`echo ${EQUINOX} | awk '{if ($1<1950) printf "%.1f\n", 2000.0; else print $1}'`

  DATEOBS=`get_key ${FILE} "DATE-OBS"`

  MJD=`${P_MJD} -f ${DATEOBS} | ${P_GAWK} '{print $7}'`

  FILTER=`get_key ${FILE} "FLTRNAME" | ${P_CLEANSTRING}`
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

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "1024" \
     -CRPIX2 "1024" \
     -CD11 "${PIXSCX}" \
     -CD12 "0.0" \
     -CD21 "0.0" \
     -CD22 "${PIXSCY}" \
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
    tmp=`get_key ${FILE} "DATE-OBS"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${tmp}" DUMMY1
    tmp=`get_key ${FILE} "MJD"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${tmp}" DUMMY2
    # Image is rotated 90 degrees counter-clockwise, North left East down
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD1_1   = 0.0" CD1_1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD2_2   = 0.0" CD2_2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD1_2   = -3.42500E-04" CD1_2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD2_1   = -3.42500E-04" CD2_1
    k=$(( $k + 1 ))
  done

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}

  mv ${BASE}_1.fits SITe.${FILTER}.${DATEOBS}_1.fits
done
