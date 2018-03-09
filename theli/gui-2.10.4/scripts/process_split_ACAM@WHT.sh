#!/bin/bash -xv

# splits the ACAM@WHT images

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
  mv ${FILE} ${BASE}.fits
  FILE=${BASE}.fits

  RA=`get_key ${FILE} "RA"`
  RA=`${P_HMSTODECIMAL} ${RA}`
  DEC=`get_key ${FILE} "DEC"`
  DEC=`${P_DMSTODECIMAL} ${DEC}`

# deal with the funny aperture stuff of the WHT system:
  ra_off1=`get_key ${FILE} "XAPNOM"`
  ra_off2=`get_key ${FILE} "XAPOFF"`
  dec_off1=`get_key ${FILE} "YAPNOM"`
  dec_off2=`get_key ${FILE} "YAPOFF"`
  RA=`echo ${RA} ${ra_off1} ${ra_off2} | ${P_GAWK} '{print $1 + $2 + $3}'`
  DEC=`echo ${DEC} ${dec_off1} ${dec_off2} | ${P_GAWK} '{print $1 + $2 + $3}'`

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

  MJD=`get_key ${FILE} "MJD-OBS"`
  if [ "${MJD}" = "" ] || [ "${MJD}" = "KEY_N/A" ]; then
      MJD=52000.0
  fi

  FILT1=`get_key ${FILE} "ACAMWH1" | ${P_CLEANSTRING}`
  FILT2=`get_key ${FILE} "ACAMWH2" | ${P_CLEANSTRING}`
  FILT3=`get_key ${FILE} "ACAMFSLI" | ${P_CLEANSTRING}`
  FILTER=""
  if [ "${FILT1}" != "CLEAR" ]; then
      FILTER=${FILTER}+${FILT1}
  fi
  if [ "${FILT2}" != "CLEAR" ]; then
      FILTER=${FILTER}+${FILT2}
  fi
  if [ "${FILT3}" != "CLEAR" ]; then
      FILTER=${FILTER}+${FILT3}
  fi
  if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "ST"`
  LST=`${P_DMSTODECIMAL} ${LST} | ${P_GAWK} '{print $1*3600}'`
  if [ "${LST}" = "" ] || [ "${LST}" = "KEY_N/A" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 12:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  ${P_FITSSPLIT_ECL} \
      -CRPIX1 "${REFPIXX[1]}" \
      -CRPIX2 "${REFPIXY[1]}" \
      -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
      -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
      -CD11 "${PIXSCX}" -CD12 "0.0" \
      -CD21 "0.0" -CD22 "${PIXSCY}" \
      -M11 "1" -M12 "0" -M21 "0" -M22 "1" \
      ${FILE}

  tmp2=`get_key ${FILE} "DATE-OBS"`
  ${P_REPLACEKEY} ${BASE}_1.fits "DATE-OBS= ${tmp2}" DUMMY1
  ${P_REPLACEKEY} ${BASE}_1.fits "MJD-OBS = ${MJD}" DUMMY2

  # crop the ACAM image if full frame:
  naxis1=`get_key ${BASE}_1.fits "NAXIS1"`
  naxis2=`get_key ${BASE}_1.fits "NAXIS2"`
  if [ "${naxis1}" = "2148" ] &&  [ "${naxis2}" = "4200" ]; then
      ${P_FITSCUT} -i ${BASE}_1.fits -o ${BASE}_1cut.fits -x 1 2148 800 3300
      mv ${BASE}_1cut.fits ${BASE}_1.fits
  fi

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}

done
