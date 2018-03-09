#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits PAUCam@WHT images

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

mkdir ORIGINALS

for FILE in ${FILES}
do
  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  BASE=`basename $FILE ${SUFFIX}`
  mv ${FILE} ${BASE}.fits
  FILE=${BASE}.fits

  ${P_SPLITPAUCAM} -i ${FILE}

  MJD=`get_keyext ${FILE} MJD-OBS`

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  k=1
  FILTER=`get_key ${BASE}_${k}.fits "FILTER" | sed 's/FT_//g'`
  DATEOBS=`get_key ${BASE}_${k}.fits "DATE-OBS"`
  while [ "${k}" -le ${NCHIPS} ]
  do
    ${P_REPLACEKEY} ${BASE}_${k}.fits "GABODSID= ${GABODSID}" DUMMY1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "ORIGFILE= ${FILE}" DUMMY2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CRPIX1  = ${REFPIXX[${k}]}" CRPIX1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CRPIX2  = ${REFPIXY[${k}]}" CRPIX2
    mv ${BASE}_${k}.fits PAUCAM.${FILTER}.${DATEOBS}_${k}.fits
    k=$(( $k + 1 ))
  done

  mv ${FILE} ORIGINALS/

#  rm PAUCAM.${FILTER}.${DATEOBS}_10.fits
#  rm PAUCAM.${FILTER}.${DATEOBS}_12.fits
#  rm PAUCAM.${FILTER}.${DATEOBS}_15.fits
#  rm PAUCAM.${FILTER}.${DATEOBS}_17.fits

#  mv PAUCAM.${FILTER}.${DATEOBS}_18.fits PAUCAM.${FILTER}.${DATEOBS}_10.fits
#  mv PAUCAM.${FILTER}.${DATEOBS}_16.fits PAUCAM.${FILTER}.${DATEOBS}_12.fits
done
