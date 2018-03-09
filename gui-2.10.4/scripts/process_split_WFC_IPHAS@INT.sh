BASHPATH -xv

# splits WFC_IPHAS@INT images, correct rotations.

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

# override NCHIPS
NCHIPS=1

mkdir ORIGINALS

for FILE in ${FILES}
do
  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  BASE=`basename $FILE $SUFFIX`
  mv ${FILE} ${BASE}.fits
  FILE=${BASE}.fits

  RA=`get_keyext ${FILE} "CRVAL1"`
  DEC=`get_keyext ${FILE} "CRVAL2"`
  EXPTIME=`get_keyext ${FILE} "EXPTIME"`
  if [ "${EXPTIME}" = "" ] || [ "${EXPTIME}" = "KEY_N/A" ]; then
      EXPTIME=1.0
  fi

  OBJECT=`get_keyext ${FILE} "OBJECT"`
  if [ "${OBJECT}" = "" ] || [ "${OBJECT}" = "KEY_N/A" ]; then
      OBJECT=UNKNOWN
  fi

  EQUINOX=`get_keyext ${FILE} "EQUINOX" | sed 's/[^0-9.]//g'`
  if [ "${EQUINOX}" = "" ] || [ "${EQUINOX}" = "KEY_N/A" ]; then
      EQUINOX=2000.0
  fi

  FILTER=`get_keyext ${FILE} "WFFBAND" | ${P_CLEANSTRING}`
  if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
      FILTER=UNKNOWN
  fi

  MJD=`get_keyext ${FILE} "MJD-OBS"`
  if [ "${MJD}" = "" ] || [ "${MJD}" = "KEY_N/A" ]; then
      MJD=52000.0
  fi

  AIRMASS=`get_keyext ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
      AIRMASS=1.0
  fi
  AIRMASS=`echo ${AIRMASS} | ${P_GAWK} '{if ($1<1) {print 1.0} else {print $1}}'`

  GABODSID=`${P_NIGHTID} -t 12:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  DATEOBS=`get_keyext ${FILE} "DATE-OBS"`

  IMAGEID=`get_keyext ${FILE} "IMAGEID"`

  # the WCS information in these fits headers is totally screwed ...
  # transform the CCDs such that North is up and east is left, assuming the
  # INTs default rotator angle of 180 degrees (has been fixed around 2005).

  get_wcsext

  ${P_FITSSPLIT_ECL} -x 1 -s \
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

  # reset the coordinates such that scamp does not get confused
  # (optical axis != crpix by 4 arcminutes)
  ranew=`echo ${RA} ${DEC} | ${P_GAWK} '{ \
      ra=$1-0.0733/cos($2*3.14159/180.); \
      if (ra>360) ra=ra-360.; \
      if (ra<0) ra=ra+360.} END {print ra}'`
  decnew=`echo ${DEC} | ${P_GAWK} '{print $1-0.02907}'` 

#  ${P_REPLACEKEY} ${BASE}_1.fits "CRVAL1  = ${ranew}" CRVAL1
#  ${P_REPLACEKEY} ${BASE}_1.fits "CRVAL2  = ${decnew}" CRVAL2

  ${P_REPLACEKEY} ${BASE}_1.fits "DATE-OBS= ${DATEOBS}" DUMMY1
  ${P_REPLACEKEY} ${BASE}_1.fits "MJD-OBS = ${MJD}" DUMMY2
  tmp=`get_keyext ${FILE} "MAGZPT"`
  ${P_REPLACEKEY} ${BASE}_1.fits "MAGZPT  = ${tmp}" DUMMY3
  tmp=`get_keyext ${FILE} "MAGZRR"`
  ${P_REPLACEKEY} ${BASE}_1.fits "MAGZRR  = ${tmp}" DUMMY4
  tmp=`get_keyext ${FILE} "EXTINCT"`
  ${P_REPLACEKEY} ${BASE}_1.fits "EXTINCT = ${tmp}" DUMMY5
  tmp=`get_keyext ${FILE} "SEEING"`
  ${P_REPLACEKEY} ${BASE}_1.fits "SEEINGPX= ${tmp}" DUMMY6
  tmp=`get_keyext ${FILE} "PHOTZP"`
  ${P_REPLACEKEY} ${BASE}_1.fits "PHOTZP  = ${tmp}" DUMMY7
  tmp=`get_keyext ${FILE} "PHOTZPER"`
  ${P_REPLACEKEY} ${BASE}_1.fits "PHOTZPER= ${tmp}" DUMMY8

  mv ${BASE}_1.fits WFC.${FILTER}.${DATEOBS}_${IMAGEID}OFC.fits

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}
done
