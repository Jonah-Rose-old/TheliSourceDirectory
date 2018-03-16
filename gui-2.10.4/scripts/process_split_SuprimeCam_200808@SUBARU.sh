BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits SuprimeCam@SUBARU images

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


##############################################################################
#                                                                            #
# Overscan correct and gain correct the images, trim overscan and paste back #
#                                                                            #
##############################################################################

cd /$1/$2

mkdir ORIGINALS

FILES=`ls -1l $1/$2/*.fits | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

for FILE in ${FILES}
do
    base=`basename ${FILE} .fits`
    ${P_SPLITSUPRIMECAM} -i ${FILE} -o ${base}s.fits
    mv ${FILE} ORIGINALS
done

FILES=`ls -1l $1/$2/*s.fits | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

for FILE in ${FILES}
do
  DETID0=`get_key ${FILE} "DET-ID"`
  DETID1=`${P_DFITS} ${FILE} | ${P_FITSORT} "DET-ID" | ${P_GAWK} '($1!="FILE") {print $2+1}'`
  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  BASE=`basename $FILE s${SUFFIX}`
  TRUEBASE=`basename $FILE ${DETID0}s${SUFFIX}`

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
  cd11=`echo ${cdmatrix} | ${P_GAWK} '{print $1}'`
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
  ${P_REPLACEKEY} ${BASE}s_1.fits "DATE-OBS= ${tmp}" DUMMY1
  ${P_REPLACEKEY} ${BASE}s_1.fits "MJD-OBS = ${MJD}" DUMMY2

  mv ${BASE}s_1.fits ${TRUEBASE}_${DETID1}.fits 
  rm ${FILE}

done
