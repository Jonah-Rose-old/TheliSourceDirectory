BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits FLI-PL16801@WISE

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

FILES=`p_ls`

mkdir ORIGINALS

for FILE in ${FILES}
do
  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  BASE=`basename $FILE ${SUFFIX}`
  mv ${FILE} ${BASE}.fits
  FILE=${BASE}.fits

  RA=`${P_GETKEY} ${FILE} "OBJCTRA"`
  RA=`echo $RA | sed 's/ /:/g'`
  if [ "${RA}" != "" ]; then
      RA=`${P_HMSTODECIMAL} ${RA}`
  fi
  if [ "${RA}" = "" ]; then
      RA=0.001
  fi

  DEC=`${P_GETKEY} ${FILE} "OBJCTDEC"`
  DEC=`echo $DEC | sed 's/ /:/g'`
  if [ "${DEC}" != "" ]; then
      DEC=`${P_DMSTODECIMAL} ${DEC}`
  fi
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
  DATEOBS2=`get_key ${FILE} "TIME-OBS"`
  DATEOBS=${DATEOBS1}"T"${DATEOBS2}

  MJD=`${P_MJD} -f ${DATEOBS} | ${P_GAWK} '{print $7}'`
  if [ "${MJD}" = "" ]; then
      MJD=52000.0
  fi

  FILTER=`get_key ${FILE} "FILTER" | ${P_CLEANSTRING}`
  if [ "${FILTER}" = "" ]; then
      FILTER=UNKNOWN
  fi

  LST=`${P_LST} -f ${DATEOBS} -l 34.75 | ${P_GAWK} '{print $2}'`
  if [ "${LST}" = "" ]; then
      LST=0.0
  fi

  AIRMASS=`${P_AIRMASS} -t ${LST} -e ${EXPTIME} -r ${RA} -d ${DEC} -l 30.6`
  AIRMASS=`echo ${AIRMASS} | ${P_GAWK} '{if ($1<1.0) print 1.0; else print $1}'`
  if [ "${AIRMASS}" = "" ]; then
      AIRMASS=1.0
  fi
  

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  # Preserve the CD matrix if present.
  # If this does not work, replace the 'get_wcs' function below
  # with the explicit code in 'bash.include', and then modify it
  # according to your needs.
  cpx1=2048
  cpx2=2048
  cd11=-2.31077E-04
  cd12=1.87021E-06
  cd21=-1.63209E-06
  cd22=-2.31451E-04

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
     -M22 "-1" \
     ${FILE}

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${DATEOBS}" DUMMY1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY2
    k=$(( $k + 1 ))
  done

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}

done
