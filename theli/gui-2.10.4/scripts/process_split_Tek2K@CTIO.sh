#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits Y4KCam@CTIO images

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

  ${P_PASTEQUADCHANNEL} -i ${FILE} -o ${BASE}p.fits -m TeK2K
  mv ${FILE} ORIGINALS
  FILE=${BASE}p.fits
  BASE=`basename $FILE .fits`

  RA=`get_key ${FILE} "RA"`
  if [ "${RA}" != "" ]; then
      RA=`${P_HMSTODECIMAL} ${RA}`
  fi
  if [ "${RA}" = "" ]; then
      RA=0.001
  fi

  DEC=`get_key ${FILE} "DEC"`
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

  EQUINOX=2000.0

  DATEOBS=`get_key ${FILE} "DATE-OBS"`
  MJD=`${P_MJD} -f ${DATEOBS} | ${P_GAWK} '{print $7}'`

  FILTER=`get_key ${FILE} "FILTERS"`
  FILTER=`echo ${FILTER} | sed 's/dia//g'`
  FILTER=`echo ${FILTER} | sed 's/ //g'`
  if [ "${FILTER}" = "" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "ST"`
  if [ "${LST}" = "" ]; then
      LST=0.0
  fi

  SECONDS=`${P_HMSTODECIMAL} ${LST} | ${P_GAWK} '{print $1/360.*86400}'`
  AIRMASS=`${P_AIRMASS} -t ${SECONDS} -e ${EXPTIME} -r ${RA} -d ${DEC} -l ${OBSLAT}`
  testair=`echo ${AIRMASS} | awk '{if ($1<1) print "N"; else print "Y"}'`
  if [ "${AIRMASS}"_A = "_A" ] || [ ${testair} = "N" ]; then
      AIRMASS=1.0
  fi
  echo 

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${REFPIXX[1]}" \
     -CRPIX2 "${REFPIXY[1]}" \
     -CD11 "-0.000111389" \
     -CD12 "0.0" \
     -CD21 "0.0" \
     -CD22 "0.000111389" \
     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
     -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
     -M11 "1" \
     -M12 "0" \
     -M21 "0" \
     -M22 "1" \
     ${FILE}

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    tmp=`get_key ${FILE} "DATE-OBS"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${tmp}" DUMMY1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "ORIGFILE= '${FILE}'" DUMMY2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY3
    mv ${BASE}_${k}.fits Tek2K.${FILTER}.${tmp}_${k}.fits
    k=$(( $k + 1 ))
  done

  rm ${FILE}
done
