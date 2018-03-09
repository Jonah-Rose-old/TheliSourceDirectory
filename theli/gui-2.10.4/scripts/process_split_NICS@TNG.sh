#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits NICS@TNG images

# NOTE: crosstalk amplitude (correction factor) for NICS: -0.045

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

  RA=`${P_DFITS} ${FILE} | ${P_FITSORT} "RA-HOURS" | ${P_GAWK} '($1!="FILE") {print 15.*$2}'`
  if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
      RA=0.0
  fi

  DEC=`get_key ${FILE} "DEC-DEG"`
  if [ "${DEC}" = "" ] || [ "${DEC}" = "KEY_N/A" ]; then
      DEC=0.0
  fi

  DIT=`get_key ${FILE} DIT`
  NDIT=`get_key ${FILE} NDIT`
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
# The modified julian day is 4 digits in NICS, therefore it is added with 50000 !!!
  fi

  MJD=`${P_DFITS} ${FILE} | ${P_FITSORT} "J-DAY" | ${P_GAWK} '($1!="FILE") {printf "%5.6f", $2+50000}'`
  if [ "${MJD}" = "" ] || [ "${MJD}" = "KEY_N/A" ]; then
      MJD=52000.0
  fi

  FILTER=`get_key ${FILE} "FILTER" | ${P_CLEANSTRING}`
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

  GABODSID=`${P_NIGHTID} -t 12:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  DATEOBS1=`get_key ${FILE} "DATE"`
  DATEOBS2=`get_key ${FILE} "TIME"`
  DATEOBS=${DATEOBS1}"T"${DATEOBS2}

  PIXSCALE=`get_key ${FILE} PIXSCALE`
  PA=`get_key ${FILE} POSANG`

  cdmatrix=`${P_GETCDMATRIX} -s ${PIXSCALE} -p ${PA} -n`
  CD11=`echo ${cdmatrix} | ${P_GAWK} '{print $1}'`
  CD12=`echo ${cdmatrix} | ${P_GAWK} '{print $2}'`
  CD21=`echo ${cdmatrix} | ${P_GAWK} '{print $3}'`
  CD22=`echo ${cdmatrix} | ${P_GAWK} '{print $4}'`

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${REFPIXX[1]}" \
     -CRPIX2 "${REFPIXY[1]}" \
     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
     -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
     -CD11 "${CD11}" \
     -CD12 "${CD12}" \
     -CD21 "${CD21}" \
     -CD22 "${CD22}" \
     -M11 "1" \
     -M12 "0" \
     -M21 "0" \
     -M22 "1" \
     ${FILE}

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${DATEOBS}" DUMMY1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "EXPT_DIT= ${DIT}" DUMMY3
    ${P_REPLACEKEY} ${BASE}_${k}.fits "ORIGNAME= ${FILE}" DUMMY4

    # scale the images such that they reflect the total exposure time.
    # otherwise we get problems with the weights when combining images
    # with different EXPTIME/NDIT settings
    if [ "${NDIT}" -gt "1" ]; then
	${P_IC} '%1 '${NDIT}' *' ${BASE}_${k}.fits > ${BASE}_${k}.fits2
	mv ${BASE}_${k}.fits2 ${BASE}_${k}.fits
    fi
    mv ${BASE}_${k}.fits NICS.${FILTER}.${DATEOBS}_${k}.fits

    k=$(( $k + 1 ))
  done

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}

done
