BASHPATH -xv

# the script processes a set of DARK frames.
# The images are overscan corrected and stacked without
# any rescaling. This script is the parallel version
# and uses the 'preprocess' program to perform the
# reduction. In contrary to the script version based
# on FLIPS, the individual preprocessed images are NOT
# saved to disk but only the final coadded calibration
# frames.
# It assumes that all images from ONE chip reside
# in the same physical directory

# $1: master directory
# $2: subdirectory with the DARK images
# $3: chips to work on
 
# The result images are in the $1/$2 directory and have
# the same basic name as the subdirectory, f.I.
# The images are in .../DARK. Then the stacked 
# images have the names .../DARK/DARK_i.fits, where i=1..NCHIPS.

#
# 27.04.2005:
# I rewrote the script to use the MAXIMAGE parameter in preprocess
# and the imcat-based imcombflat for the actual coaddition.

# preliminary work:
INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

#
# the resultdir is where the output coadded images
# will go. If ONE image of the corresponding chip
# is a link the image will go to THIS directory
for CHIP in $3
do
  RESULTDIR[${CHIP}]="$1/$2"
done

. bash.include

for CHIP in $3
do

# check whether the master calibration file
# exists already. if yes, then delete it.

  test -f $1/$2/$2_${CHIP}.fits && rm $1/$2/$2_${CHIP}.fits

  FILES=`ls $1/$2/*_${CHIP}.fits`

  image=`ls $1/$2/*_${CHIP}.fits | ${P_GAWK} '(NR==1)'`
  EXPTIME=`get_key ${image} EXPTIME`

  for FILE in ${FILES}
  do
        if [ -L ${FILE} ]; then
          LINK=`${P_READLINK} ${FILE}`
          BASE=`basename ${LINK} .fits`
          DIR=`dirname ${LINK}`
          ln -s ${DIR}/${BASE}OC.fits $1/$2/${BASE}OC.fits
          RESULTDIR[${CHIP}]=`dirname ${LINK}`
        fi
  done

  MAXX=$(( ${CUTX[${CHIP}]} + ${SIZEX[${CHIP}]} - 1 ))
  MAXY=$(( ${CUTY[${CHIP}]} + ${SIZEY[${CHIP}]} - 1 ))

  do_overscan=Y
  if [ "${OVSCANX1[${CHIP}]}" = "0" ] && \
      [ "${OVSCANX2[${CHIP}]}" = "0" ]; then
      do_overscan=N
  fi

  # check the number of exposures
  numexp=`ls $1/$2/*_${CHIP}.fits | wc | ${P_GAWK} '{print $1}'`

  # if less than 3 exposures are available:
  if [ "${numexp}" -lt 3 ]; then
      echo PREPROCESS_FAILURE: Need at least three images of chip ${CHIP} to create master dark
      continue
  fi

  # if numexp <= NFRAMES combine the images right away,
  # otherwise write them down and then combine them.

  xmin=2
  xmax=$(( ${SIZEX[${CHIP}]}-1 ))
  ymin=2
  ymax=$(( ${SIZEY[${CHIP}]}-1 ))

  if [ "${numexp}" -gt "${NFRAMES}" ]; then
  # overscan correct and trim DARK frames
      ${P_IMRED_ECL:?} `ls $1/$2/*_${CHIP}.fits` \
	  -c ${CONF}/imred.conf \
	  -MAXIMAGES ${NFRAMES} \
	  -OVERSCAN ${do_overscan} \
	  -OVERSCAN_REGION ${OVSCANX1[${CHIP}]},${OVSCANX2[${CHIP}]} \
	  -OVERSCAN_REJECT ${V_CAL_OVSCANNLOW},${V_CAL_OVSCANNHIGH} \
	  -OUTPUT Y \
	  -OUTPUT_DIR ${RESULTDIR[${CHIP}]}/ \
	  -TRIM Y \
	  -OUTPUT_SUFFIX OC.fits \
	  -TRIM_REGION ${CUTX[${CHIP}]},${MAXX},${CUTY[${CHIP}]},${MAXY} \
	  -STATSSEC ${xmin},${xmax},${ymin},${ymax}
      
  # and combine them
      ls -1 $1/$2/*_${CHIP}OC.fits > ${TEMPDIR}/dark_images_$$
      ${P_IMCOMBFLAT_IMCAT} -i ${TEMPDIR}/dark_images_$$\
	  -o ${RESULTDIR[${CHIP}]}/$2_${CHIP}.fits \
	  -e ${V_CAL_DARKNLOW} ${V_CAL_DARKNHIGH} 
  fi

  if [ "${numexp}" -le "${NFRAMES}" ]; then
  # overscan correct and trim DARK frames
      ${P_IMRED_ECL:?} `ls $1/$2/*_${CHIP}.fits` \
	  -c ${CONF}/imred.conf \
	  -MAXIMAGES ${NFRAMES} \
	  -OVERSCAN ${do_overscan} \
	  -OVERSCAN_REGION ${OVSCANX1[${CHIP}]},${OVSCANX2[${CHIP}]} \
	  -OVERSCAN_REJECT ${V_CAL_OVSCANNLOW},${V_CAL_OVSCANNHIGH} \
	  -OUTPUT N \
	  -OUTPUT_DIR ${RESULTDIR[${CHIP}]}/ \
	  -TRIM Y \
	  -OUTPUT_SUFFIX OC.fits \
	  -TRIM_REGION ${CUTX[${CHIP}]},${MAXX},${CUTY[${CHIP}]},${MAXY} \
	  -STATSSEC ${xmin},${xmax},${ymin},${ymax} \
	  -COMBINE Y \
	  -COMBINE_MAXVAL 1e8 \
	  -COMBINE_RESCALE N \
	  -COMBINE_OUTPUT ${RESULTDIR[${CHIP}]}/$2_${CHIP}.fits \
	  -COMBINE_REJECT ${V_CAL_DARKNLOW},${V_CAL_DARKNHIGH} 
  fi

  MD5KEY=`get_key ${RESULTDIR[${CHIP}]}/$2_${CHIP}.fits DATAMD5`
  if [ "${MD5KEY}"_A != "_A" ]; then
      ${P_REPLACEKEY} ${RESULTDIR[${CHIP}]}/$2_${CHIP}.fits "EXPTIME = ${EXPTIME}" "DATAMD5"
  else
      ${P_FITSADDKEY} -i ${RESULTDIR[${CHIP}]}/$2_${CHIP}.fits -k EXPTIME -v ${EXPTIME}
  fi

  if [ "${RESULTDIR[${CHIP}]}" != "$1/$2" ]; then
    ln -s ${RESULTDIR[${CHIP}]}/$2_${CHIP}.fits $1/$2/$2_${CHIP}.fits
  fi

  \rm -f /$1/$2/*_${CHIP}OC.fits
  if [ "${RESULTDIR[${CHIP}]}" != "$1/$2" ]; then
    \rm -f ${RESULTDIR[${CHIP}]}/*_${CHIP}OC.fits
  fi  

done
