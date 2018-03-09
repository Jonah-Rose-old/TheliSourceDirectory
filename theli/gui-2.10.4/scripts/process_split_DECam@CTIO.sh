#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits DECam@CTIO images

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

NCHIPS=62

. bash.include

DIR=`pwd`

cd /$1/$2

FILES=`p_ls`

mkdir ORIGINALS

for FILE in ${FILES}
do
  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  BASE=`basename $FILE ${SUFFIX}`
  mv ${FILE} ${BASE}.fits
  FILE=${BASE}.fits

  ${P_SPLITDECAM} -i ${FILE} \
      -p ${DIR}/../ExtCalTables/${INSTRUMENT}_xtalk.txt \
      -q ${DIR}/../ExtCalTables/${INSTRUMENT}_nlin.fits \
      -n ${NPARA}

  MJD=`get_key ${FILE} "MJD-OBS"`
  if [ "${MJD}" = "" ]; then
      MJD=52000.0
  fi

  DATEOBS=`get_key ${BASE}_1.fits "DATE-OBS"`
  FILTER=`get_key ${BASE}_1.fits "FILTER"`

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    ${P_REPLACEKEY} ${BASE}_${k}.fits "ORIGFILE= ${FILE}" DUMMY1
    mv ${BASE}_${k}.fits DECAM.${FILTER}.${DATEOBS}_${k}.fits
    k=$(( $k + 1 ))
  done

  # Detectors 2 and 61 are dead
  # rename chip #62 to #2
  # delete chip #61
  mv DECAM.${FILTER}.${DATEOBS}_62.fits DECAM.${FILTER}.${DATEOBS}_2.fits
  rm DECAM.${FILTER}.${DATEOBS}_61.fits

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}
done
