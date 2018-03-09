BASHPATH -xv

# the script processes a set of FLAT frames
# the images are overscan corrected, debiased and stacked with
# mode rescaling.

#$1: main directory (filter)
#$2: Bias directory
#$3: Flat directory
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

DIR=`pwd`

#
# the resultdir is where the output coadded images
# will go. If ONE image of the corresponding chip
# is a link the image will go to THIS directory
for CHIP in $4
do
  RESULTDIR[${CHIP}]="$1/$3"
done

# Correction overscan; create config file on the fly
# we do not assume that the overscan is the same for all chips

for CHIP in $4
do

# check whether the master calibration file
# exists already. if yes, then delete it.

  test -f $1/$3/$3_${CHIP}.fits && rm $1/$3/$3_${CHIP}.fits

  FILES=`ls $1/$3/*_${CHIP}.fits`

  i=1
  for FILE in ${FILES}
  do
      BAYERPATTERN=`get_key ${FILE} BAYERPAT`
      if [ -L ${FILE} ]; then
	  LINK=`${P_READLINK} ${FILE}`
	  BASE=`basename ${LINK} .fits`
	  DIR=`dirname ${LINK}`
	  ln -s ${DIR}/${BASE}OC.fits $1/$2/${BASE}OC.fits
	  RESULTDIR[${CHIP}]=`dirname ${LINK}`
      fi
      # Get the filter keyword
      if [ ${i} -eq 1 ]; then
	  filter=`get_key ${FILE} FILTER`
      fi
      i=$((${i}+1))
  done

  MAXX=$(( ${CUTX[${CHIP}]} + ${SIZEX[${CHIP}]} - 1 ))
  MAXY=$(( ${CUTY[${CHIP}]} + ${SIZEY[${CHIP}]} - 1 ))

  do_overscan=Y
  if [ "${OVSCANX1[${CHIP}]}" = "0" ] && \
      [ "${OVSCANX2[${CHIP}]}" = "0" ]; then
      do_overscan=N
  fi

  # check the number of exposures
  numexp=`ls $1/$3/*_${CHIP}.fits | wc | ${P_GAWK} '{print $1}'`

  # if less than 3 exposures are available:
  if [ "${numexp}" -lt 3 ]; then
      echo PREPOROCESS_FAILURE: Need at least three images of chip ${CHIP} to create master flat
      continue
  fi

  get_statminsection

  # if numexp <= NFRAMES combine the images right away,
  # otherwise write them down and then combine them.

  if [ "${numexp}" -gt "${NFRAMES}" ] || [ "${V_NONLINCORR}" = "Y" ]; then
      # overscan correct, trim and bias correct FLAT frames
      ${P_IMRED_ECL:?} `ls /$1/$3/*_${CHIP}.fits` \
	  -c ${CONF}/imred.conf \
	  -MAXIMAGES ${NFRAMES} \
	  -OVERSCAN ${do_overscan} \
	  -OVERSCAN_REGION ${OVSCANX1[${CHIP}]},${OVSCANX2[${CHIP}]} \
	  -OVERSCAN_REJECT ${V_CAL_OVSCANNLOW},${V_CAL_OVSCANNHIGH} \
	  -BIAS ${V_DO_BIAS} \
	  -BIAS_IMAGE /$1/$2/$2_${CHIP}.fits \
	  -OUTPUT Y \
	  -OUTPUT_DIR /$1/$3/ \
	  -TRIM Y \
	  -OUTPUT_SUFFIX OC.fits \
	  -TRIM_REGION ${CUTX[${CHIP}]},${MAXX},${CUTY[${CHIP}]},${MAXY} \
	  -STATSSEC ${statminsection}
      
      # correct for nonlinearity
      if [ "${V_NONLINCORR}" = "Y" ] && [ -f ${INSTDIR}/${INSTRUMENT}".nldat" ]; then
	  ls /$1/$3/*_${CHIP}OC.fits > ${TEMPDIR}/nonlinlist_$$
	  cat ${TEMPDIR}/nonlinlist_$$ |\
          {
	    while read file
	    do
		BASE=`basename ${file} _${CHIP}OC.fits`
		${P_FITSNONLINEARITY} -i ${file} -o ${RESULTDIR[${CHIP}]}/${BASE}_${CHIP}OCnl.fits \
		    -c ${CHIP} -f ${INSTDIR}/${INSTRUMENT}".nldat"
		mv ${RESULTDIR[${CHIP}]}/${BASE}_${CHIP}OCnl.fits \
		    ${RESULTDIR[${CHIP}]}/${BASE}_${CHIP}OC.fits 
	    done
	  }
      fi
      
      # and combine them
      ${P_IMSTATS} -d 6 `ls /$1/$3/*_${CHIP}OC.fits` \
	  -o ${TEMPDIR}/flat_images_$$ -t -1e8 1e8 \
	  -s ${statminsection_imstats}
      ${P_IMCOMBFLAT_IMCAT} -i ${TEMPDIR}/flat_images_$$\
	  -o ${RESULTDIR[${CHIP}]}/$3_${CHIP}.fits \
	  -s 1 -e ${V_CAL_FLATNLOW} ${V_CAL_FLATNHIGH} 
  fi
  
  if [ "${numexp}" -le "${NFRAMES}" ] && [ "${V_NONLINCORR}" = "N" ]; then
  # overscan correct, trim and bias correct FLAT frames
      ${P_IMRED_ECL:?} `ls /$1/$3/*_${CHIP}.fits` \
	  -c ${CONF}/imred.conf \
	  -MAXIMAGES ${NFRAMES} \
	  -OVERSCAN ${do_overscan} \
	  -OVERSCAN_REGION ${OVSCANX1[${CHIP}]},${OVSCANX2[${CHIP}]} \
	  -OVERSCAN_REJECT ${V_CAL_OVSCANNLOW},${V_CAL_OVSCANNHIGH} \
	  -BIAS ${V_DO_BIAS} \
	  -BIAS_IMAGE /$1/$2/$2_${CHIP}.fits \
	  -OUTPUT N \
	  -OUTPUT_DIR ${RESULTDIR[${CHIP}]}/ \
	  -TRIM Y \
	  -OUTPUT_SUFFIX OC.fits \
	  -TRIM_REGION ${CUTX[${CHIP}]},${MAXX},${CUTY[${CHIP}]},${MAXY} \
	  -COMBINE Y \
	  -COMBINE_RESCALE Y \
	  -COMBINE_OUTPUT ${RESULTDIR[${CHIP}]}/$3_${CHIP}.fits \
	  -COMBINE_REJECT ${V_CAL_FLATNLOW},${V_CAL_FLATNHIGH} \
	  -STATSSEC ${statminsection}
  fi
      
  # merge global and individual region files, if they exist
  get_region

  # mask the flat
  if [ "${region}"_A != "_A" ]; then
      # we mask it with a very small negative value, so that the
      # corresponding pixels in the flat-fielded data will be VERY
      # negative, and thus outside the range where statistics is probed
      ${P_FITSPOLYGON} \
	  -i ${RESULTDIR[${CHIP}]}/$3_${CHIP}.fits \
	  -o ${RESULTDIR[${CHIP}]}/$3_${CHIP}.masked.fits \
	  -p ${regionfile} \
	  -v -1e-9
     mv ${RESULTDIR[${CHIP}]}/$3_${CHIP}.masked.fits ${RESULTDIR[${CHIP}]}/$3_${CHIP}.fits
  fi
  
  if [ ! "${BAYERPATTERN}_A" = "_A" ]; then
      FILE=$1/$3/$3_${CHIP}.fits
      BASE=`basename ${FILE} _${CHIP}.fits`
      ${P_EQUALISEBAYERFLAT} -i ${FILE} -o $1/$3/${BASE}_tmp.fits
      mv $1/$3/${BASE}_tmp.fits ${FILE}
  fi
  
  # Add the filter name to the flatfield
  ${P_FITSADDKEY} -i $1/$3/$3_${CHIP}.fits -k FILTER -v ${filter}

  # Apply the illumination correction if available
  illumcorrdir=${DIR}/../../ExtCal/${INSTRUMENT}/illumcorr/
  if [ -f ${illumcorrdir}/illumcorr_${filter}_1.fits ]; then
      ${P_IC} '%1 %2 *' ${RESULTDIR[${CHIP}]}/$3_${CHIP}.fits \
	  ${illumcorrdir}/illumcorr_${filter}_${CHIP}.fits > \
	  ${RESULTDIR[${CHIP}]}/$3_${CHIP}.tmp.fits
      mv ${RESULTDIR[${CHIP}]}/$3_${CHIP}.tmp.fits ${RESULTDIR[${CHIP}]}/$3_${CHIP}.fits
  fi

  if [ "${RESULTDIR[${CHIP}]}" != "$1/$3" ]; then
      ln -s ${RESULTDIR[${CHIP}]}/$3_${CHIP}.fits $1/$3/$3_${CHIP}.fits
  fi
  
  \rm -f /$1/$3/*_${CHIP}OC.fits
  if [ "${RESULTDIR[${CHIP}]}" != "$1/$3" ]; then
      \rm -f ${RESULTDIR[${CHIP}]}/*_${CHIP}OC.fits
  fi
  
done
