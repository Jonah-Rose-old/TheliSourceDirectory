BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits NICI@GEMINI_SOUTH images

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

mkdir /$1/$2_R
mkdir /$1/$2_B

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

  RA=`get_key ${FILE} "RA"`
  if [ "${RA}" = "" ]; then
      RA=0.001
  fi

  DEC=`get_key ${FILE} "DEC"`
  if [ "${DEC}" = "" ]; then
      DEC=0.0
  fi

  DIT1=`get_key ${FILE} "ITIME_R"`
  DIT2=`get_key ${FILE} "ITIME_B"`
  NDIT1=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} "NCOADD_R" | ${P_GAWK} '($1!="FILE") {print $2}'`
  NDIT2=`${P_DFITS} -x 2 ${FILE} | ${P_FITSORT} "NCOADD_B" | ${P_GAWK} '($1!="FILE") {print $2}'`
  EXPTIME1=`echo ${DIT1} ${NDIT1} | ${P_GAWK} '{print $1*$2}'`
  EXPTIME2=`echo ${DIT2} ${NDIT2} | ${P_GAWK} '{print $1*$2}'`
  if [ "${EXPTIME1}" = "" ] || [ "${EXPTIME1}" = "KEY_N/A" ]; then
      EXPTIME1=1.0
  fi
  if [ "${EXPTIME2}" = "" ] || [ "${EXPTIME2}" = "KEY_N/A" ]; then
      EXPTIME2=1.0
  fi

  OBJECT=`get_key ${FILE} "OBJECT"`
  if [ "${OBJECT}" = "" ]; then
      OBJECT=UNKNOWN
  fi

  EQUINOX=`get_key ${FILE} "EQUINOX" | sed 's/[^0-9.]//g'`
  if [ "${EQUINOX}" = "" ] || [ "${EQUINOX}" = "0." ]; then
      EQUINOX=2000.0
  fi

  MJD=`get_key ${FILE} "MJD_OBS"`
  if [ "${MJD}" = "" ]; then
      MJD=52000.0
  fi

  FILTER1=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} "FILTER_R" | ${P_GAWK} '($1!="FILE") {print $2}'`
  if [ "${FILTER1}" = "" ]; then
      FILTER1=UNKNOWN
  fi

  FILTER2=`${P_DFITS} -x 2 ${FILE} | ${P_FITSORT} "FILTER_B" | ${P_GAWK} '($1!="FILE") {print $2}'`
  if [ "${FILTER2}" = "" ]; then
      FILTER2=UNKNOWN
  fi

  LST=`get_key ${FILE} "ST"`
  if [ "${LST}" = "" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "AMSTART"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "-9999." ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  cd11_1=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} "CD1_1" | ${P_GAWK} '($1!="FILE") {print $2}'`
  cd12_1=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} "CD1_2" | ${P_GAWK} '($1!="FILE") {print $2}'`
  cd21_1=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} "CD2_1" | ${P_GAWK} '($1!="FILE") {print $2}'`
  cd22_1=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} "CD2_2" | ${P_GAWK} '($1!="FILE") {print $2}'`
  cd11_2=`${P_DFITS} -x 2 ${FILE} | ${P_FITSORT} "CD1_1" | ${P_GAWK} '($1!="FILE") {print $2}'`
  cd12_2=`${P_DFITS} -x 2 ${FILE} | ${P_FITSORT} "CD1_2" | ${P_GAWK} '($1!="FILE") {print $2}'`
  cd21_2=`${P_DFITS} -x 2 ${FILE} | ${P_FITSORT} "CD2_1" | ${P_GAWK} '($1!="FILE") {print $2}'`
  cd22_2=`${P_DFITS} -x 2 ${FILE} | ${P_FITSORT} "CD2_2" | ${P_GAWK} '($1!="FILE") {print $2}'`
  cpx1_1=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} "CRPIX1" | ${P_GAWK} '($1!="FILE") {print $2}'`
  cpx2_1=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} "CRPIX2" | ${P_GAWK} '($1!="FILE") {print $2}'`
  cpx1_2=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} "CRPIX1" | ${P_GAWK} '($1!="FILE") {print $2}'`
  cpx2_2=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} "CRPIX2" | ${P_GAWK} '($1!="FILE") {print $2}'`


  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${cpx1_1},${cpx1_2}" \
     -CRPIX2 "${cpx2_2},${cpx2_2}" \
     -CD11 "${cd11_1},${cd11_2}" \
     -CD12 "${cd12_1},${cd12_2}" \
     -CD21 "${cd21_1},${cd21_2}" \
     -CD22 "${cd22_1},${cd22_2}" \
     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME1} -AIRMASS ${AIRMASS} \
     -GABODSID ${GABODSID} -FILTER ${FILTER1} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
     -M11 "1,1" \
     -M12 "0,0" \
     -M21 "0,0" \
     -M22 "1,1" \
     ${FILE}

  dateobs=`get_key ${FILE} "DATE-OBS"`
  timeobs=`get_key ${FILE} "TIME-OBS"`
  dateobs=${dateobs}T${timeobs}
  ${P_REPLACEKEY} ${BASE}_1.fits "DATE-OBS= ${dateobs}" DUMMY1
  ${P_REPLACEKEY} ${BASE}_2.fits "DATE-OBS= ${dateobs}" DUMMY1
  ${P_REPLACEKEY} ${BASE}_1.fits "FILENAME= ${FILE}" DUMMY2
  ${P_REPLACEKEY} ${BASE}_2.fits "FILENAME= ${FILE}" DUMMY2
  ${P_REPLACEKEY} ${BASE}_1.fits "FILTER  = ${FILTER1}" FILTER "EXPTIME = ${EXPTIME1}" EXPTIME 
  ${P_REPLACEKEY} ${BASE}_2.fits "FILTER  = ${FILTER2}" FILTER "EXPTIME = ${EXPTIME2}" EXPTIME 
  ${P_REPLACEKEY} ${BASE}_1.fits "MJD-OBS = ${MJD}" DUMMY3
  ${P_REPLACEKEY} ${BASE}_2.fits "MJD-OBS = ${MJD}" DUMMY3

  mv ${BASE}_1.fits /$1/$2_R/NICI.R.${dateobs}_1.fits
  mv ${BASE}_2.fits /$1/$2_B/NICI.B.${dateobs}_1.fits

  # no image scaling wrt. NDIT has to be done as the image is not averaged but summed.

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}
done
