#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits SAMI_2x2@SOAR images

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
      if [ "${newname}" != "" ]; then
         mv ${FILE} ${newname}
      else
         echo ERROR: Could not find ${V_RENAME_FITSKEY} in the FITS header
      fi
    done
  }
fi


FILES=`ls -1l $1/$2/ | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

cd /$1/$2

FILES=`p_ls`

mkdir ORIGINALS

for FILE in ${FILES}
do
  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  BASE=`basename $FILE ${SUFFIX}`
  mv ${FILE} ${BASE}.fits
  FILE=${BASE}.fits

  ${P_PASTESAMI} -i ${FILE}

  DATEOBS1=`get_key ${FILE} "DATE-OBS"`
  DATEOBS2=`get_key ${FILE} "TIME-OBS"`
  DATEOBS=${DATEOBS1}"T"${DATEOBS2}

  MJD=`${P_MJD} -f ${DATEOBS} | ${P_GAWK} '{print $7}'`

  FILTER=`get_key ${FILE} "FILTER1" | ${P_GAWK} 'BEGIN{FS="_"} {print $2}'`
  if [ "${FILTER}" = "" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "LSTHDR"`
  if [ "${LST}" = "" ]; then
      LST=0.0
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  ${P_REPLACEKEY} ${BASE}_1.fits "DATE-OBS= '${DATEOBS}'" DUMMY1
  ${P_REPLACEKEY} ${BASE}_1.fits "GABODSID= '${GABODSID}'" DUMMY2
  ${P_REPLACEKEY} ${BASE}_1.fits "MJD-OBS= ${MJD}" MJD-OBS
  ${P_REPLACEKEY} ${BASE}_1.fits "FILTER = ${FILTER}" FILTER

  mv ${FILE} ORIGINALS/
  mv ${BASE}_1.fits SAMI.${FILTER}.${DATEOBS}_1.fits
done
