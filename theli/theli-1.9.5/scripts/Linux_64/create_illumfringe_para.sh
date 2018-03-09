#!/bin/bash

# script that creates an illumination correction and
# fringe image out of a blank sky field (superflat)

# HISTORY COMMENTS:
# =================
#
# 31.12.2012:
# If the file from that fringe and illumination image should be extracted
# did not exist the script created empty links - corrected.

# $1: main dir (filter)
# $2: Science directory
# $3: chips to be processed

# preliminary work:
. ./${INSTRUMENT:?}.ini
. ./bash_functions.include
theli_start "$*" "${!#}"

for CHIP in ${!#}
do
  FILE=`ls /$1/$2/$2_${CHIP}.fits`

  if [ "${FILE}" != "" ]; then
    if [ -L ${FILE} ]; then
      LINK=`${P_READLINK} ${FILE}`
      BASE=`basename ${LINK} .fits`
      DIR=`dirname ${LINK}`
      ln -s ${DIR}/$2_${CHIP}_illum.fits $1/$2/$2_${CHIP}_illum.fits
      ln -s ${DIR}/$2_${CHIP}_fringe.fits $1/$2/$2_${CHIP}_fringe.fits
      RESULTDIR[${CHIP}]=`dirname ${LINK}`    
    else
      RESULTDIR[${CHIP}]="/$1/$2"
    fi
  
    ${P_SEX} /$1/$2/$2_${CHIP}.fits -c ${CONF}/illumfringe_back.sex \
      -CHECKIMAGE_NAME ${RESULTDIR[${CHIP}]}/$2_${CHIP}_illum.fits \
      -BACK_SIZE 256
  
    ${P_SEX} /$1/$2/$2_${CHIP}.fits -c ${CONF}/illumfringe_fringe.sex \
      -CHECKIMAGE_NAME ${RESULTDIR[${CHIP}]}/$2_${CHIP}_fringe.fits \
      -BACK_SIZE 256
  else
    theli_error "/$1/$2/$2_${CHIP}.fits does not exist!" "${!#}"
    exit 1;
  fi  
done

theli_end "${!#}"
exit 0;
