BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits MOIRCS@SUBARU images

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
  CD11=`get_key ${FILE} "CD1_1"`
  CD12=`get_key ${FILE} "CD1_2"`
  CD21=`get_key ${FILE} "CD2_1"`
  CD22=`get_key ${FILE} "CD2_2"`
  DETID=`get_key ${FILE} "DET-ID"`
  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  if [ "${DETID}" = "1" ]; then
      NAME1=`basename ${FILE} ${SUFFIX}`
  fi
  BASE=`basename ${FILE} ${SUFFIX}`
  mv ${FILE} ${BASE}.fits
  FILE=${BASE}.fits

  RA=`get_key ${FILE} "CRVAL1"`
  if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
      RA=0.0
  fi

  DEC=`get_key ${FILE} "CRVAL2"`
  if [ "${DEC}" = "" ] || [ "${DEC}" = "KEY_N/A" ]; then
      DEC=0.0
  fi

  DIT=`get_key ${FILE} "K_DETEP1"`
  NDIT=`get_key ${FILE} "COADD"`
  EXPTIME=`echo ${DIT} ${NDIT} | ${P_GAWK} '{print $1*$2}'`
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

  FILT1=`get_key ${FILE} "FILTER01" | ${P_CLEANSTRING}`
  FILT2=`get_key ${FILE} "FILTER02" | ${P_CLEANSTRING}`
  FILTER=${FILT1}"+"${FILT2}
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

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${REFPIXX[${DETID}]}" \
     -CRPIX2 "${REFPIXY[${DETID}]}" \
     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
     -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
     -CD11 "${CD11}"\
     -CD12 "${CD12}" \
     -CD21 "${CD21}" \
     -CD22 "${CD22}"\
     -M11 "1" \
     -M12 "0" \
     -M21 "0" \
     -M22 "1" \
      ${FILE}

  tmp=`get_key ${FILE} "DATE-OBS"`
  ${P_REPLACEKEY} ${BASE}_1.fits "DATE-OBS= ${tmp}" DUMMY2
  ${P_REPLACEKEY} ${BASE}_1.fits "MJD-OBS = ${MJD}" DUMMY3

  # scale the images such that they reflect the total exposure time.
  # otherwise we get problems with the weights when combining images
  # with different EXPTIME/NDIT settings
  if [ "${NDIT}" -gt "1" ]; then
      ${P_IC} '%1 '${NDIT}' *' ${BASE}_1.fits > ${BASE}_1.fits2
      mv ${BASE}_1.fits2 ${BASE}_1.fits
  fi

  # lastly, rename the 2nd image such that it complies with the THELI naming convention
  if [ "${DETID}" = "2" ]; then
      mv ${BASE}_1.fits ${NAME1}"_2.fits"
  fi

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}

done
