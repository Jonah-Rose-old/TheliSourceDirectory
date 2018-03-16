BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits MEGACAM_2x2@LCO images

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

  RA=`get_keyext ${FILE} "CRVAL1"`
  if [ "${RA}" = "" ]; then
      RA=0.001
  fi

  DEC=`get_keyext ${FILE} "CRVAL2"`
  if [ "${DEC}" = "" ]; then
      DEC=0.0
  fi

  EXPTIME=`get_keyext ${FILE} "EXPTIME"`
  if [ "${EXPTIME}" = "" ]; then
      EXPTIME=`get_keyext ${FILE} "EXPOSURE"`
  fi
  if [ "${EXPTIME}" = "" ]; then
      EXPTIME=1.0
  fi

  OBJECT=`get_keyext ${FILE} "OBJECT"`
  if [ "${OBJECT}" = "" ]; then
      OBJECT=UNKNOWN
  fi

  EQUINOX=`get_keyext ${FILE} "EQUINOX" | sed 's/[^0-9.]//g'`
  if [ "${EQUINOX}" = "" ]; then
      EQUINOX=2000.0
  fi

  MJD=`get_keyext ${FILE} "MJD"`
  if [ "${MJD}" = "" ]; then
      MJD=52000.0
  fi

  FILTER=`get_keyext ${FILE} "FILTER" | ${P_CLEANSTRING}`
  if [ "${FILTER}" = "" ]; then
      FILTER=UNKNOWN
  fi
  FILTER=`echo ${FILTER} | sed 's/gblk//g' | sed 's/,//g'`

  LST=`get_keyext ${FILE} "ST"`
  if [ "${LST}" = "" ]; then
      LST=0.0
  fi

  AIRMASS=`get_keyext ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  # make arguments for mefsplit
  k=1
  cpx1=""
  cpx2=""
  cd11=""
  cd12=""
  cd21=""
  cd22=""
  cd11a=`get_keyext ${FILE} CD1_1`
  cd12a=`get_keyext ${FILE} CD1_2`
  cd21a=`get_keyext ${FILE} CD2_1`
  cd22a=`get_keyext ${FILE} CD2_2`
  cd11b=`${P_DFITS} -x 19 ${FILE} | fitsort CD1_1 | ${P_GAWK} '(NR==2) { print $2}'`
  cd12b=`${P_DFITS} -x 19 ${FILE} | fitsort CD1_2 | ${P_GAWK} '(NR==2) { print $2}'`
  cd21b=`${P_DFITS} -x 19 ${FILE} | fitsort CD2_1 | ${P_GAWK} '(NR==2) { print $2}'`
  cd22b=`${P_DFITS} -x 19 ${FILE} | fitsort CD2_2 | ${P_GAWK} '(NR==2) { print $2}'`
  while [ "${k}" -le "18" ]
  do
    cpx1="${cpx1},${REFPIXX[${k}]}"
    cpx2="${cpx2},${REFPIXY[${k}]}"
    cd11="${cd11},${cd11a}"
    cd12="${cd12},${cd12a}"
    cd21="${cd21},${cd21a}"
    cd22="${cd22},${cd22a}"
    k=$(( $k + 1 ))
  done
  while [ "${k}" -le "36" ]
  do
    cpx1="${cpx1},${REFPIXX[${k}]}"
    cpx2="${cpx2},${REFPIXY[${k}]}"
    cd11="${cd11},${cd11b}"
    cd12="${cd12},${cd12b}"
    cd21="${cd21},${cd21b}"
    cd22="${cd22},${cd22b}"
    k=$(( $k + 1 ))
  done

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${cpx1}" \
     -CRPIX2 "${cpx2}" \
     -CD11 "${cd11}" \
     -CD12 "${cd12}" \
     -CD21 "${cd21}" \
     -CD22 "${cd22}" \
     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
     -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
     -M11 "1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1" \
     -M12 "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0" \
     -M21 "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0" \
     -M22 "1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1" \
     ${FILE}

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    tmp=`get_keyext ${FILE} "DATE-OBS"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${tmp}" DUMMY1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY2
    mv ${BASE}_${k}.fits MEGACAM.${FILTER}.${tmp}_${k}.fits
    k=$(( $k + 1 ))
  done

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}

done
