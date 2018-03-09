BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits FourStar@LCO images

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

cd /$1/$2

FILES=`ls -1l $1/$2/*.fits | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

mkdir ORIGINALS

for FILE in ${FILES}
do
  BASE=`basename $FILE .fits`
  FILE=${BASE}.fits

  RA=`get_key ${FILE} "RA"`
  if [ "${RA}" = "" ]; then
      RA=0.001
  fi

  DEC=`get_key ${FILE} "DEC"`
  if [ "${DEC}" = "" ]; then
      DEC=0.0
  fi

  EXPTIME=`get_key ${FILE} "EXPTIME"`
  if [ "${EXPTIME}" = "" ]; then
      EXPTIME=`get_key ${FILE} "EXPOSURE"`
  fi
  if [ "${EXPTIME}" = "" ]; then
      EXPTIME=1.0
  fi

  OBJECT=`get_key ${FILE} "OBJECT"`
  if [ "${OBJECT}" = "" ]; then
      OBJECT=UNKNOWN
  fi

  EQUINOX=`get_key ${FILE} "EQUINOX" | sed 's/[^0-9.]//g'`
  if [ "${EQUINOX}" = "" ]; then
      EQUINOX=2000.0
  fi

  DATEOBS1=`get_key ${FILE} "DATE-OBS"`
  DATEOBS2=`get_key ${FILE} "UT-TIME"`
  DATEOBS=${DATEOBS1}"T"${DATEOBS2}

  MJD=`get_key ${FILE} "MJD"`
  if [ "${MJD}" = "" ]; then
      MJD=52000.0
  fi

  FILTER=`get_key ${FILE} "FILTER" | ${P_CLEANSTRING}`
  if [ "${FILTER}" = "" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "ST"`
  LST=`${P_LST} -f ${DATEOBS} -l -70.73333`
  if [ "${LST}" = "" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  ROTANGLE=`get_key ${FILE} "ROTANGLE"`
  CHIP=`get_key ${FILE} "CHIP"`

  CDSTRING=`${P_GETCDMATRIX} -s 0.159 -p ${ROTANGLE} -n`
  cd11=`echo ${CDSTRING} | gawk '{print $1}'`
  cd12=`echo ${CDSTRING} | gawk '{print $2}'`
  cd21=`echo ${CDSTRING} | gawk '{print $3}'`
  cd22=`echo ${CDSTRING} | gawk '{print $4}'`

  if [ "${CHIP}" = "1" ]; then
      crpix1=1011
      crpix2=1002
  fi
  if [ "${CHIP}" = "2" ]; then
      crpix1=1020
      crpix2=3185
  fi
  if [ "${CHIP}" = "3" ]; then
      crpix1=3240
      crpix2=3174
  fi
  if [ "${CHIP}" = "4" ]; then
      crpix1=3197
      crpix2=1013
  fi

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${crpix1}" \
     -CRPIX2 "${crpix2}" \
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

  ${P_REPLACEKEY} ${BASE}_1.fits "DATE-OBS= ${DATEOBS}" DUMMY1
  ${P_REPLACEKEY} ${BASE}_1.fits "ORIGFILE= ${FILE}" DUMMY2
  ${P_REPLACEKEY} ${BASE}_1.fits "MJD-OBS = ${MJD}" DUMMY3

  mv ${BASE}_1.fits FourStar.${FILTER}.${DATEOBS}_${CHIP}.fits

  mv ${FILE} ORIGINALS/
done
