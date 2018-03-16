BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits SUSI2old@NTT images (2 detectors in 1 fits extension)

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

  MJD=`get_key ${FILE} "MJD-OBS"`
  if [ "${MJD}" = "" ] || [ "${MJD}" = "KEY_N/A" ]; then
      MJD=52000.0
  fi

  FILTER=`get_key ${FILE} "HIERARCH ESO INS FILT1 NAME" | ${P_CLEANSTRING}`
  if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "LST"`
  if [ "${LST}" = "" ] || [ "${LST}" = "KEY_N/A" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "HIERARCH ESO TEL AIRM START"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  cd11=`get_key ${FILE} "CD1_1"`
  cd12=`get_key ${FILE} "CD1_2"`
  cd21=`get_key ${FILE} "CD2_1"`
  cd22=`get_key ${FILE} "CD2_2"`

  ${P_FITSSPLIT_ECL} \
      -CRPIX1 "${REFPIXX[1]}" \
      -CRPIX2 "${REFPIXY[1]}" \
      -CD11 "${cd11}" \
      -CD12 "${cd12}" \
      -CD21 "${cd21}" \
      -CD22 "${cd22}" \
      -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
      -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
      -M11 "-1" \
      -M12 "0" \
      -M21 "0" \
      -M22 "1" \
      ${FILE}

  tmp=`get_key ${FILE} "DATE-OBS"`
  ${P_REPLACEKEY} ${BASE}_1.fits "DATE-OBS= ${tmp}" DUMMY1
  ${P_REPLACEKEY} ${BASE}_1.fits "MJD-OBS = ${MJD}" DUMMY2

  # split the image into the two extensions, fix the header
  ${P_FITSCUT} -i ${BASE}_1.fits -o ${BASE}_tmp1.fits -x 1 1072 1 2048
  ${P_FITSCUT} -i ${BASE}_1.fits -o ${BASE}_tmp2.fits -x 1073 2144 1 2048

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}

  mv  ${BASE}_tmp1.fits  ${BASE}_1.fits
  mv  ${BASE}_tmp2.fits  ${BASE}_2.fits

  ${P_REPLACEKEY} ${BASE}_1.fits "CRPIX1  = 1038" CRPIX1
  ${P_REPLACEKEY} ${BASE}_1.fits "CRPIX2  = 1145" CRPIX2
  ${P_REPLACEKEY} ${BASE}_2.fits "CRPIX1  = -45" CRPIX1
  ${P_REPLACEKEY} ${BASE}_2.fits "CRPIX2  = 1145" CRPIX2
  ${P_REPLACEKEY} ${BASE}_2.fits "IMAGEID = 2" IMAGEID

done
