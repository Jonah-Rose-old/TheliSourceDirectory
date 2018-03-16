BASHPATH -xv

# the script creates sky subtracted images.
# It puts them in the same dir. as the science frames

# $1: main directory
# $2: science directory
# $3: extension
# $4: chips to be processed

SCRIPTDIR=`pwd`

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash.include

# the chips to process

for CHIP in $4
do
  ls -1 /$1/$2/*_${CHIP}$3.fits > ${TEMPDIR}/skysub_images${CHIP}_$$

  NUMTHREAD=`${SCRIPTDIR}/get_nthreads.sh ${NCHIPS} ${NPARA} ${CHIP}`

  FILE=`${P_GAWK} '(NR==1) {print $0}' ${TEMPDIR}/skysub_images${CHIP}_$$`

  if [ -L ${FILE} ]; then
      LINK=`${P_READLINK} ${FILE}`
      RESULTDIR[${CHIP}]=`dirname ${LINK}`
  else
      RESULTDIR[${CHIP}]="$1/$2"
  fi  
  
  cat ${TEMPDIR}/skysub_images${CHIP}_$$ |\
  {
  while read file
    do
      BASE=`basename ${file} .fits`

      # create a segmentation image
      ${P_SEX} ${file} -c ${DATACONF}/background_masks.sex \
	  -DETECT_THRESH ${V_SKYSUBDETECTTHRESH} \
	  -DETECT_MINAREA ${V_SKYSUBDETECTMINAREA} \
	  -CATALOG_NAME    ${RESULTDIR[${CHIP}]}/${BASE}"_seg.cat" \
	  -CHECKIMAGE_NAME ${RESULTDIR[${CHIP}]}/${BASE}"_seg.fits" \
	  -CHECKIMAGE_TYPE SEGMENTATION \
	  -DEBLEND_MINCONT 0.05 \
	  -NTHREADS ${NUMTHREAD} \
	  -WEIGHT_TYPE MAP_WEIGHT \
	  -WEIGHT_IMAGE /$1/WEIGHTS/${BASE}.weight.fits

      # merge global and individual region files, if they exist
      get_region

      # do the sky subtraction
      skymodel=""
      if [ "${V_SAVESKYMODEL}" = "Y" ]; then
	  skymodel="-m ${RESULTDIR[${CHIP}]}/${BASE}.sky.fits"
      fi

      maskexpand=""
      if [ "${V_SKYSUBMASKEXPAND}"_A != "_A" ]; then
	  maskexpand="-a ${RESULTDIR[${CHIP}]}/${BASE}_seg.cat -b ${V_SKYSUBMASKEXPAND}"
      fi

      if [ "${regionfile}"_A = "_A" ]; then
	  regionstring=""
      else
	  regionstring="-p ${regionfile}"
      fi 

      ${P_SUBSKY} \
	  -i ${file} \
	  -s ${RESULTDIR[${CHIP}]}/${BASE}"_seg.fits" \
	  -o ${RESULTDIR[${CHIP}]}/${BASE}.sub.fits \
	  -n 5000 \
	  -c ${NUMTHREAD} \
	  -f ${V_SKYSUBBACKSIZE} \
	  -t -1e8 1e8 \
	  -w /$1/WEIGHTS/${BASE}.weight.fits \
	  -d \
	  ${maskexpand} ${regionstring} ${skymodel} 

      if [ "${RESULTDIR[${CHIP}]}" != "$1/$2" ]; then
        ln -s ${RESULTDIR[${CHIP}]}/${BASE}.sub.fits /$1/$2/${BASE}.sub.fits
        ln -s ${RESULTDIR[${CHIP}]}/${BASE}.sky.fits /$1/$2/${BASE}.sky.fits
      fi

      rm ${RESULTDIR[${CHIP}]}/${BASE}_seg.fits ${RESULTDIR[${CHIP}]}/${BASE}_seg.cat

      if [ "${RESULTDIR[${CHIP}]}" != "$1/$2" ]; then
	  rm /$1/$2/${BASE}_seg.fits
      fi
    done
  }
done
