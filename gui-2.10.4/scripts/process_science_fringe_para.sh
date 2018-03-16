BASHPATH -xv

# $1: main directory (filter)
# $2: science directory
# $3: chips to be processed

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

for CHIP in $3
do
  RESULTDIR[${CHIP}]="$1/$2"
done

for CHIP in $3
do
  FILES=`ls /$1/$2/*_${CHIP}OFC.fits`

  for FILE in ${FILES}
  do
    if [ -L ${FILE} ]; then
	 LINK=`${P_READLINK} ${FILE}`
	 BASE=`basename ${LINK} .fits`
	 DIR=`dirname ${LINK}`
	 ln -s ${DIR}/${BASE}B.fits $1/$2/${BASE}B.fits
	 RESULTDIR[${CHIP}]=`dirname ${LINK}`  
    fi  
  done 

  get_statminsection

  ${P_IMRED_ECL:?} ${FILES} \
      -c ${CONF}/imred.conf \
      -MAXIMAGES ${NFRAMES} \
      -OVERSCAN N \
      -BIAS N \
      -FLAT N \
      -COMBINE N \
      -FRINGE Y \
      -FRINGE_IMAGE /$1/$2/$2_${CHIP}_fringe.fits \
      -FRINGE_REFIMAGE /$1/$2/$2_${CHIP}_illum.fits \
      -FRINGE_SCALE ${V_FRINGESCALE} \
      -OUTPUT Y \
      -OUTPUT_DIR /$1/$2/ \
      -OUTPUT_SUFFIX B.fits \
      -TRIM N \
      -STATSSEC ${statminsection}

  if [ ! -d /$1/$2/OFC_IMAGES ]; then
     mkdir /$1/$2/OFC_IMAGES
  fi
  mv /$1/$2/*_${CHIP}OFC.fits /$1/$2/OFC_IMAGES

done
