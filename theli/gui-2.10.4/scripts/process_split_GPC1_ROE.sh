#!/bin/bash -xv

# splits GPC1@PS1 images

# $1: directory with the raw data
# $2: file containing list of files to be split

# create image list: we assume that ONLY unsplit
# images are in the directory

INSTDIR=instruments_professional
. ${INSTDIR}/GPC1@PS1.ini >& /dev/null

. bash.include


FILES=`cat /$1/$2`

cd /$1

if [ ! -d "WEIGHTS" ]; then
    mkdir WEIGHTS
fi
if [ ! -d "SCIENCE" ]; then
    mkdir SCIENCE
fi

test ! -d ORIGINALS && mkdir ORIGINALS

for FILE in ${FILES}
do
  FILEORIG=${FILE}
  BASE=`basename $FILE .ch.fits`

  # get a THELI conform chip number
  CHIPTMP=`echo ${BASE} | ${P_GAWK} 'BEGIN{FS="."} {print $5}' | \
      ${P_GAWK} 'BEGIN {FS="XY"} {print $2}'`
  CHIP=`echo ${CHIPTMP} | ${P_GAWK} 'BEGIN{FS=""} {
    if ($2==0) print $1+8*$2
    if ($2>0 && $2<7) print $1+8*$2-1
    if ($2==7) print $1+8*$2-2}'`

  ROOTBASE=`basename ${FILE} .XY${CHIPTMP}.ch.fits`

  if [ ! -f ${ROOTBASE}_s1CD.dat ] && [ "${CHIPTMP}" -lt 40 ]; then
      ${P_DFITS} -x 1 ${ROOTBASE}.XY[0-3]*.ch.fits | \
	  ${P_FITSORT} CD1_1A CD1_2A CD2_1A CD2_2A | \
	  ${P_GAWK} '(NR>1) {print $2, $4, $3, $5}' > ${ROOTBASE}_s1CD.dat
 
     num=`grep "KEY_N/A" ${ROOTBASE}_s1CD.dat | wc | ${P_GAWK} '{print $1}'`
     if [ ! "${num}" = "0" ]; then
	 ${P_DFITS} -x 1 ${ROOTBASE}.XY[0-3]*.ch.fits | \
             ${P_FITSORT} CD1_1 CD1_2 CD2_1 CD2_2 | \
             ${P_GAWK} '(NR>1) {print $2, $3, $4, $5}' > ${ROOTBASE}_s1CD.dat
     fi

     ${P_GAWK} '{print $1}' ${ROOTBASE}_s1CD.dat > ${ROOTBASE}_s1CD11.dat
     ${P_GAWK} '{print $2}' ${ROOTBASE}_s1CD.dat > ${ROOTBASE}_s1CD12.dat
     ${P_GAWK} '{print $3}' ${ROOTBASE}_s1CD.dat > ${ROOTBASE}_s1CD21.dat
     ${P_GAWK} '{print $4}' ${ROOTBASE}_s1CD.dat > ${ROOTBASE}_s1CD22.dat
  fi
  if [ "${CHIPTMP}" -lt 40 ]; then
      cd11=`${P_GETSTATS} -i ${ROOTBASE}_s1CD11.dat -m median`
      cd12=`${P_GETSTATS} -i ${ROOTBASE}_s1CD12.dat -m median`
      cd21=`${P_GETSTATS} -i ${ROOTBASE}_s1CD21.dat -m median`
      cd22=`${P_GETSTATS} -i ${ROOTBASE}_s1CD22.dat -m median`
  fi

  if [ ! -f ${ROOTBASE}_s2CD.dat ] && [ "${CHIPTMP}" -ge 40 ]; then
      ${P_DFITS} -x 1 ${ROOTBASE}.XY[4-7]*.ch.fits | \
	  ${P_FITSORT} CD1_1A CD1_2A CD2_1A CD2_2A | \
	  ${P_GAWK} '(NR>1) {print $2, $4, $3, $5}' > ${ROOTBASE}_s2CD.dat

      num=`grep "KEY_N/A" ${ROOTBASE}_s2CD.dat | wc | ${P_GAWK} '{print $1}'`
      if [ ! "${num}" = "0" ]; then
          ${P_DFITS} -x 1 ${ROOTBASE}.XY[4-7]*.ch.fits | \
              ${P_FITSORT} CD1_1 CD1_2 CD2_1 CD2_2 | \
              ${P_GAWK} '(NR>1) {print $2, $3, $4, $5}' > ${ROOTBASE}_s2CD.dat
      fi

      ${P_GAWK} '{print $1}' ${ROOTBASE}_s2CD.dat > ${ROOTBASE}_s2CD11.dat
      ${P_GAWK} '{print $2}' ${ROOTBASE}_s2CD.dat > ${ROOTBASE}_s2CD12.dat
      ${P_GAWK} '{print $3}' ${ROOTBASE}_s2CD.dat > ${ROOTBASE}_s2CD21.dat
      ${P_GAWK} '{print $4}' ${ROOTBASE}_s2CD.dat > ${ROOTBASE}_s2CD22.dat
  fi
  if [ "${CHIPTMP}" -ge 40 ]; then
      cd11=`${P_GETSTATS} -i ${ROOTBASE}_s2CD11.dat -m median`
      cd12=`${P_GETSTATS} -i ${ROOTBASE}_s2CD12.dat -m median`
      cd21=`${P_GETSTATS} -i ${ROOTBASE}_s2CD21.dat -m median`
      cd22=`${P_GETSTATS} -i ${ROOTBASE}_s2CD22.dat -m median`
  fi

  # extract the image
#  /disk1/ps1/local/getstars_para/fitsuncompress ${FILE} ${BASE}_ext.fits -32
  /usr/bin/fitsuncompress ${FILE} ${BASE}_ext.fits -32

  FILE=${BASE}_ext.fits

  FILTER=`get_key ${FILE} "FILTER" | ${P_CLEANSTRING}`
  RA=`get_key ${FILE} "CRVAL1A"`
  DEC=`get_key ${FILE} "CRVAL2A"`
  MJD=`get_key ${FILE} "MJD-OBS"`
  EXPTIME=`get_key ${FILE} "EXPTIME"`
  EQUINOX=`get_key ${FILE} "EQUINOX" | sed 's/[^0-9.]//g'`
  AIRMASS=`get_key ${FILE} "AIRMASS"`
  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`
  CRPIX1=`get_key ${FILE} "CRPIX1"`
  CRPIX2=`get_key ${FILE} "CRPIX2"`
  OBJECT=`get_key ${FILE} "QUEUEID"`

  # half of the GPC1 images are rotated by 180 degrees
  if [ "${CHIPTMP}" -lt 40 ]; then
      ${P_FITSSPLIT_ECL} \
	  -CRPIX1 "${CRPIX1}" \
          -CRPIX2 "${CRPIX2}" \
          -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
          -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
	  -CD11 "${cd11}" \
	  -CD12 "${cd12}" \
          -CD21 "${cd21}" \
	  -CD22 "${cd22}" \
          -M11 "1" \
	  -M12 "0" \
	  -M21 "0" \
	  -M22 "1" \
          ${FILE}
  else
      ${P_FITSSPLIT_ECL} \
	  -CRPIX1 "${CRPIX1}" \
          -CRPIX2 "${CRPIX2}" \
          -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
          -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
	  -CD11 "${cd11}" \
	  -CD12 "${cd12}" \
          -CD21 "${cd21}" \
	  -CD22 "${cd22}" \
          -M11 "-1" \
	  -M12 "0" \
	  -M21 "0" \
	  -M22 "-1" \
          ${FILE}
  fi

  mv ${FILEORIG} ORIGINALS/
  mv ${BASE}_ext_1.fits SCIENCE/${ROOTBASE}_${CHIP}OFC.fits
  ${P_REPLACEKEY} SCIENCE/${ROOTBASE}_${CHIP}OFC.fits "MJD-OBS = ${MJD}" DUMMY1
  rm ${BASE}_ext.fits

  # do the weight:
  ${P_FITSNANMASK} -i /$1/SCIENCE/${ROOTBASE}_${CHIP}OFC.fits -o /$1/WEIGHTS/${ROOTBASE}_${CHIP}glob.fits -n
  ${P_FITSNANMASK} -i /$1/SCIENCE/${ROOTBASE}_${CHIP}OFC.fits -o /$1/SCIENCE//${ROOTBASE}_${CHIP}OFC_nonan.fits
  mv /$1/SCIENCE//${ROOTBASE}_${CHIP}OFC_nonan.fits /$1/SCIENCE//${ROOTBASE}_${CHIP}OFC.fits

  # run sextractor to identify cosmic rays:
  RESULTDIR="$1/WEIGHTS"
  mkdir -p /$1/WEIGHTS
  ${P_SEX} /$1/SCIENCE/${ROOTBASE}_${CHIP}OFC.fits -c ${CONF}/cosmic.conf.sex -CHECKIMAGE_NAME \
      ${RESULTDIR}/cosmic_${CHIP}_$$.fits \
      -CATALOG_NAME ${TEMPDIR}/cosmic.cat_$$ \
      -FILTER_THRESH 3.0 \
      -DETECT_THRESH 3 \
      -DETECT_MINAREA 2 \
      -BACK_SIZE 120 \
      -ANALYSIS_THRESH 0.1
  
  V_WEIGHTLOWTHRESHOLD="-1e9"
  V_WEIGHTHIGHTHRESHOLD="1e9"
  INFLAGS="-FLAG_NAMES \"\""
  OUTFLAGS="-OUTFLAG_NAME \"\""
  ${P_WW} -c ${CONF}/weights.ww \
      -WEIGHT_NAMES ${RESULTDIR}/${ROOTBASE}_${CHIP}glob.fits,${RESULTDIR}/cosmic_${CHIP}_$$.fits,/$1/SCIENCE/${ROOTBASE}_${CHIP}OFC.fits \
      ${INFLAGS} ${OUTFLAGS} \
      -OUTWEIGHT_NAME ${RESULTDIR}/${ROOTBASE}_${CHIP}OFC.weight.fits \
      -WEIGHT_MIN "-1e9,-1e9,-1e9" \
      -WEIGHT_MAX "1e9,0.1,1e9"
  rm ${RESULTDIR}/cosmic_${CHIP}_$$.fits ${RESULTDIR}/${ROOTBASE}_${CHIP}glob.fits ${TEMPDIR}/cosmic.cat_$$

done

\rm *_s[1,2]CD*.dat
