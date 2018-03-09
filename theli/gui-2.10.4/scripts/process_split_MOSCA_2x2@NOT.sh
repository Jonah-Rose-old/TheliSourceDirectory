#!/bin/bash -xv

# splits 2x2-binned MOSCA@NOT images, correct rotations.

# $1: main directory
# $2: science directory

# create image list: we assume that ONLY unsplit
# images are in the directory

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
  if [ ! "${SUFFIX}" = ".fits" ]; then
      mv ${FILE} ${BASE}.fits
      FILE=${BASE}.fits
  fi
  
  RA=`get_key ${FILE} "RA"`
  RA1=`${P_DFITS} ${FILE} | ${P_FITSORT} "RA" | ${P_GAWK} '($1!="FILE") {print int($2+0.5)}'`
  if [ "${RA1}" -gt "361" ]; then
      RA=0.0
  fi

  DEC=`get_key ${FILE} "DEC"`
  DEC1=`${P_DFITS} ${FILE} | ${P_FITSORT} "DEC" | ${P_GAWK} '($1!="FILE") {print int($2+0.5)}'`
  if [ "${DEC1}" -gt "90" ]; then
      DEC=0.0
  fi

  LST=`get_key ${FILE} "ST"`

  EXPTIME=`get_key ${FILE} "EXPTIME"`
  if [ "${EXPTIME}" = "" ] || [ "${EXPTIME}" = "KEY_N/A" ]; then
      EXPTIME=1.0
  fi

  OBJECT=`get_key ${FILE} "OBJECT"`
  if [ "${OBJECT}" = "" ] || [ "${OBJECT}" = "KEY_N/A" ]; then
      OBJECT=UNKNOWN
  fi

  FILT1=`get_key ${FILE} "FAFLTNM" | ${P_CLEANSTRING}`
  FILT2=`get_key ${FILE} "FBFLTNM" | ${P_CLEANSTRING}`
  FILTER=${FILT1}"+"${FILT2}
  if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
      FILTER=UNKNOWN
  fi

  EQUINOX=`get_key ${FILE} "EQUINOX" | sed 's/[^0-9.]//g'`
  if [ "${EQUINOX}" = "" ] || [ "${EQUINOX}" = "KEY_N/A" ]; then
      EQUINOX=2000.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
      AIRMASS=1.0
  fi

  GABODSID=1000

  ${P_FITSSPLIT_ECL} \
      -CRPIX1 "${REFPIXX[1]},${REFPIXX[2]},${REFPIXX[3]},${REFPIXX[4]}" \
      -CRPIX2 "${REFPIXY[1]},${REFPIXY[2]},${REFPIXY[3]},${REFPIXY[4]}" \
      -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
      -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
      -CD11 "${PIXSCX},${PIXSCX},${PIXSCX},${PIXSCX}" \
      -CD12 "0.0, 0.0, 0.0, 0.0" \
      -CD21 "0.0, 0.0, 0.0, 0.0" \
      -CD22 "${PIXSCY},${PIXSCY},${PIXSCY},${PIXSCY}" \
      -M11 "1,1,1,1" \
      -M12 "0,0,0,0" \
      -M21 "0,0,0,0" \
      -M22 "1,1,1,1" \
      ${FILE}
  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}
done
