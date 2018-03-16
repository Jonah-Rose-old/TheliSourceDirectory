BASHPATH -xv

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

for CHIP in $4
do
  RESULTDIR[${CHIP}]="$1/$2"
done

. bash.include

if [ "${V_COLLMASKACTION}" != "0" ]; then
    if [ ! -d $1/$2/MASK_IMAGES ]; then
	mkdir $1/$2/MASK_IMAGES
    fi
fi

if [ ! -d $1/$2/$3_IMAGES ]; then
    mkdir $1/$2/$3_IMAGES
fi

# check if an area should be masked
maskregion=""
if [ ${V_COLLXMIN}_A != "_A" ] && [ ${V_COLLXMAX}_A != "_A" ] && \
   [ ${V_COLLYMIN}_A != "_A" ] && [ ${V_COLLYMAX}_A != "_A" ]; then
    maskregion="-x ${V_COLLXMIN} ${V_COLLXMAX} ${V_COLLYMIN} ${V_COLLYMAX}"
fi

for CHIP in $4
do

  # delete a globalweight_dummy file (region masking) if existing in the WEIGHTS directory
  # It might be for a different instrument, or the polygons might have changed
  test -e /$1/WEIGHTS/globalweight_dummy_${CHIP}.fits && rm /$1/WEIGHTS/globalweight_dummy_${CHIP}.fits

  ls -1 /$1/$2/*_${CHIP}$3.fits > ${TEMPDIR}/collapse_${CHIP}_$$

  NUMTHREAD=`${SCRIPTDIR}/get_nthreads.sh ${NCHIPS} ${NPARA} ${CHIP}`

  FILE=`${P_GAWK} '(NR==1) {print $0}' ${TEMPDIR}/collapse_${CHIP}_$$`

  if [ -L ${FILE} ]; then
      LINK=`${P_READLINK} ${FILE}`
      RESULTDIR[${CHIP}]=`dirname ${LINK}`
      mkdir ${RESULTDIR[${CHIP}]}/MASK_IMAGES
  else
      RESULTDIR[${CHIP}]="$1/$2"
  fi
  
  get_statminsection

  cat ${TEMPDIR}/collapse_${CHIP}_$$ |\
      {
      while read FILE
      do
	  BASE=`basename ${FILE} .fits`
	  BASE2=`basename ${FILE} $3.fits`
	  
	  if [ "${V_COLLAUTOTHRESHOLD}" = "1" ]; then
              # estimate the amplitude of the reset anomaly
	      amp=`${P_FITSCOLLAPSE} -i ${FILE} -c ${V_COLLDIRECTION} -k 0.5 -${maskregion} -r`
	      
              # estimate the rms in the image
	      rms=`${P_IMSTATS} -d 6 ${FILE} -s ${statminsection_imstats} | \
		  ${P_GAWK} '($1!~/#/) {print $7}'`
	      
              # the sextractor detection threshold (scaled with a factor of 1.2 for safety)
	      # if it is smaller than the user-provided threshold, use the user threshold
	      V_COLLDETECTTHRESH=`echo ${amp} ${rms} ${V_COLLDETECTTHRESH} |\
              ${P_GAWK} '{res=1.2 * $1 / $2; if (res>$3) print res; else print $3}'`
	  fi

	  # check if there is a region file, and prep sextractor for using it
	  get_region
	  prepare_sex_for_region $1 $2 # why do I have a second argument there? Not used...
	  
          # fitscollapse with object masks
	  if [ "${V_COLLDETECTMINAREA}_A" != "_A" ] && [ "${V_COLLDETECTTHRESH}_A" != "_A" ]; then
	      ${P_SEX} ${FILE} -c ${DATACONF}/background_masks.sex \
		  -CATALOG_NAME ${RESULTDIR[${CHIP}]}/${BASE}.mask.cat \
		  -CHECKIMAGE_NAME ${RESULTDIR[${CHIP}]}/${BASE}.mask.fits \
		  -DETECT_MINAREA ${V_COLLDETECTMINAREA} \
		  -DETECT_THRESH ${V_COLLDETECTTHRESH} \
		  -CHECKIMAGE_TYPE OBJECTS \
		  -NTHREADS ${NUMTHREAD} \
		  ${backstring} ${weightstring}

	      # expand the sextractor mask and overwrite the input image
	      if [ "${V_COLLMASKEXPAND}_A" != "_A" ]; then 
		  wstring="-w /$1/WEIGHTS/globalweight_dummy_${CHIP}.fits"
		  if [ ! -e /$1/WEIGHTS/globalweight_dummy_${CHIP}.fits ]; then
		      mkdir -p /$1/WEIGHTS/
		      ${P_IC} -p -32 -c ${SIZEX[${CHIP}]} ${SIZEY[${CHIP}]} '1' > \
			  /$1/WEIGHTS/globalweight_dummy_${CHIP}.fits
		  fi
		  ${P_EXPANDSEXMASK} \
		      -i ${RESULTDIR[${CHIP}]}/${BASE}.mask.fits \
		      -o ${RESULTDIR[${CHIP}]}/${BASE}.mask.fits \
		      -c ${RESULTDIR[${CHIP}]}/${BASE}.mask.cat \
		      -r ${V_COLLMASKEXPAND} ${wstring}
	      fi

	      ${P_FITSCOLLAPSE} \
		  -i ${FILE} -s \
		  -o ${RESULTDIR[${CHIP}]}/${BASE}"C.fits" \
		  -m $1/$2/${BASE}".mask.fits" \
		  -c ${V_COLLDIRECTION} \
		  -k ${V_COLLREJECTTHRESH} \
		  ${maskregion} \
		  ${region}

	  else
              # fitscollapse without object masks
	      ${P_FITSCOLLAPSE} \
		  -i ${FILE} -s \
		  -o ${RESULTDIR[${CHIP}]}/${BASE}"C.fits" \
		  -c ${V_COLLDIRECTION} \
		  -k ${V_COLLREJECTTHRESH} \
		  ${maskregion} \
		  ${region}
	  fi

	  # cleanup
	  mv ${FILE} /$1/$2/$3_IMAGES
	  if [ "${RESULTDIR[${CHIP}]}" != "$1/$2" ]; then
	      ln -s ${RESULTDIR[${CHIP}]}/${BASE}"C.fits" \
		  $1/$2/${BASE}"C.fits"
	  fi

	  # if the object mask should be deleted
	  if [ "${V_COLLMASKACTION}" = "0" ]; then
	      test -e ${RESULTDIR[${CHIP}]}/${BASE}".mask.fits" && \
		   rm ${RESULTDIR[${CHIP}]}/${BASE}".mask.fits"
	      test -e $1/$2/${BASE}".mask.fits" && \
		   rm $1/$2/${BASE}".mask.fits"
	      test -e ${RESULTDIR[${CHIP}]}/${BASE}".mask.cat" && \
		   rm ${RESULTDIR[${CHIP}]}/${BASE}".mask.cat"
	      test -e $1/$2/${BASE}".mask.cat" && \
		   rm $1/$2/${BASE}".mask.cat"
	  else
	      if [ "${RESULTDIR[${CHIP}]}" != "$1/$2" ]; then
		  ln -s ${RESULTDIR[${CHIP}]}/${BASE}".mask.fits" \
		      $1/$2/MASK_IMAGES/${BASE}".mask.fits"
	      else
		  mv $1/$2/${BASE}".mask.fits" $1/$2/MASK_IMAGES/
		  mv $1/$2/${BASE}".mask.cat" $1/$2/MASK_IMAGES/
	      fi
	  fi
      done
  }
done
