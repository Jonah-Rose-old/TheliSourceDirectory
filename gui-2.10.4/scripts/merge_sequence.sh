BASHPATH -xv

# this script moves back images that were spread with
# spread_sequence.sh previously.

# THIS SCRIPT DOES NOT SUPPORT A LINKING STRUCTURE

# $1: main dir
# $2: science dir
# $3: image extension
# $4: number of groups

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

# move the superflat, illumination and fringe correction images
# back to where they would normally be.
# we use the ones from the last group which is the most stable one.

CHIP=1
while [ "${CHIP}" -le "${NCHIPS}" ]
do
  mv /$1/$2_S$4/$2_S$4_${CHIP}.fits /$1/$2/$2_${CHIP}.fits
  mv /$1/$2_S$4/$2_S$4_${CHIP}_illum.fits /$1/$2/$2_${CHIP}_illum.fits
  mv /$1/$2_S$4/$2_S$4_${CHIP}_fringe.fits /$1/$2/$2_${CHIP}_fringe.fits
  CHIP=$(( ${CHIP} + 1 ))
done

# move the various stages of the processed data back to where
# it came from

GROUP=1
while [ "${GROUP}" -le "$4" ]
do
  if [ ! -d /$1/$2/SPLIT_IMAGES ]; then
      mkdir /$1/$2/SPLIT_IMAGES
  fi
  if [ ! -d /$1/$2/OFC_IMAGES ]; then
      mkdir /$1/$2/OFC_IMAGES
  fi
  mv /$1/$2_S${GROUP}/SPLIT_IMAGES/* /$1/$2/SPLIT_IMAGES
  mv /$1/$2_S${GROUP}/OFC_IMAGES/* /$1/$2/OFC_IMAGES
  mv /$1/$2_S${GROUP}/*$3.fits /$1/$2/

  if [ "$3" = "OFCSF" ]; then
      if [ ! -d /$1/$2/OFCS_IMAGES ]; then
	  mkdir /$1/$2/OFCS_IMAGES
      fi
      mv /$1/$2_S${GROUP}/OFCS_IMAGES/* /$1/$2/OFCS_IMAGES
  fi

  mv /$1/$2_S${GROUP}/ /$1/$2/

  GROUP=$(( ${GROUP} + 1 ))
done
