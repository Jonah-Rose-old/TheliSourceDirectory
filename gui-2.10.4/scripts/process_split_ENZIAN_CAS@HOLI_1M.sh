BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits ENZIAN_CAS@HOLI_1M images

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

  MJD=`get_key ${FILE} "MJD-OBS"`
  if [ "${MJD}" = "" ] || [ "${MJD}" = "KEY_N/A" ] || [ "${MJD}" = "0." ]; then
      MJD=52000.0
  fi

  FILTER=`get_key ${FILE} "FILTERS" | ${P_CLEANSTRING}`
  if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "LST"`
  if [ "${LST}" = "" ] || [ "${LST}" = "KEY_N/A" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ] || [ "${AIRMASS}" = "0." ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${REFPIXX[1]}" \
     -CRPIX2 "${REFPIXY[1]}" \
     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
     -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
     -CD11 "${PIXSCX}" \
     -CD12 "0" \
     -CD21 "0" \
     -CD22 "${PIXSCY}" \
     -M11 "0" \
     -M12 "1" \
     -M21 "-1" \
     -M22 "0" \
     ${FILE}

  k=1
  CD11=`echo ${PIXSCX} | ${P_GAWK} '{if ($1>0) print $1*(-1.); else print $1}'`
  CD22=`echo ${PIXSCY} | ${P_GAWK} '{if ($1<0) print $1*(-1.); else print $1}'`
  while [ "${k}" -le "${NCHIPS}" ]
  do
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD1_1   = ${CD11}" CD1_1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD2_2   = ${CD22}" CD2_2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD1_2   = 0.0" CD1_2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD2_1   = 0.0" CD2_1
    k=$(( $k + 1 ))
  done

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    tmp=`get_key ${FILE} "DATE-OBS"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${tmp}" DUMMY2
    tmp=`get_key ${FILE} "OBSERVER"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "OBSERVER= ${tmp}" DUMMY3
    tmp=`get_key ${FILE} "IMGTYPE"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "IMGTYPE = ${tmp}" DUMMY4
    tmp=`get_key ${FILE} "PIC_NAME"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "PIC_NAME= ${tmp}" DUMMY5
    tmp=`get_key ${FILE} "SEQUENCE"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "SEQUENCE= ${tmp}" DUMMY6
    tmp=`get_key ${FILE} "TEL_FOC "`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "TEL_FOC = ${tmp}" DUMMY7
    tmp=`get_key ${FILE} "CAM_FOC"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CAM_FOC = ${tmp}" DUMMY8
    tmp=`get_key ${FILE} "CCD_TEMP"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CCD_TEMP= ${tmp}" DUMMY9
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY10
    k=$(( $k + 1 ))
  done

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}

done
