#!/bin/bash -xv

# $1: main dir
# $2: science dir
# $3: extension
# $4: threshold
# $5: chips to work on

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

for CHIP in $5
do
  RESULTDIR[${CHIP}]="$1/$2"
done

for CHIP in $5
do

  ls -1 $1/$2/*_${CHIP}$3.fits > ${TEMPDIR}/debloomimages${CHIP}_$$

  FILE=`${P_GAWK} '(NR==1) {print $0}' ${TEMPDIR}/debloomimages${CHIP}_$$`

  if [ -L ${FILE} ]; then
    LINK=`${P_READLINK} ${FILE}`
    RESULTDIR[${CHIP}]=`dirname ${LINK}`
  else
    RESULTDIR[${CHIP}]="$1/$2"
  fi  

  cat ${TEMPDIR}/debloomimages${CHIP}_$$ |\
  {
    while read file
    do
      BASE=`basename ${file} .fits`

      # debloom the image
      ${P_DEBLOOM} -i ${file} -o ${RESULTDIR[${CHIP}]}/${BASE}D.fits -t $4

      echo "${file} debloomed"
    done
  }
  if [ ! -d $1/$2/$3_IMAGES ]; then
     mkdir $1/$2/$3_IMAGES
  fi
  mv $1/$2/*_${CHIP}$3.fits $1/$2/$3_IMAGES
done
