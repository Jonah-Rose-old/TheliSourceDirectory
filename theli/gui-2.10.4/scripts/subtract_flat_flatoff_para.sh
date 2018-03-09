#!/bin/bash -xv

# the script subtracts flat-off exposures from the flat-on exposures

#$1: main directory (filter)
#$2: Flat directory
#$3: Flat off directory
#$4: chips to process

# preliminary work:
INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash.include

# the resultdir is where the output coadded images
# will go. If ONE image of the corresponding chip
# is a link the image will go to THIS directory
for CHIP in $4
do
  RESULTDIR[${CHIP}]="$1/$2"
done

for CHIP in $4
do

  ${P_IC} '%1 %2 -' $1/$2/$2_${CHIP}.fits $1/$3/$3_${CHIP}.fits > \
      ${RESULTDIR[${CHIP}]}/$2_DIFF_${CHIP}.fits
  mkdir -p $1/$2/NOFLATOFFSUB
  mv $1/$2/$2_${CHIP}.fits $1/$2/NOFLATOFFSUB/
  mv ${RESULTDIR[${CHIP}]}/$2_DIFF_${CHIP}.fits \
     ${RESULTDIR[${CHIP}]}/$2_${CHIP}.fits
  
  if [ "${RESULTDIR[${CHIP}]}" != "$1/$2" ]; then
      ln -s ${RESULTDIR[${CHIP}]}/$2_${CHIP}.fits $1/$2/$2_${CHIP}.fits
  fi
done
