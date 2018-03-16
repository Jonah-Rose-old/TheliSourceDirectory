BASHPATH -xv

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

SCRIPTDIR=`pwd`

DIR=""
FILTER="*"
NAME=""
DETECTTHRESH=""
DETECTMINAREA=""
NLOW=""
NHIGH=""
METHOD=""
RESCALE=""
LEVEL=""
runsex="no"

GoOn=0
while [ $GoOn = 0 ]
do
   case $1 in
   -m)
       DIR=${2}
       shift
       shift
       ;;
   -f)
       FILTER=${2}
       shift
       shift
       ;;
   -n)
       NAME=${2}
       shift
       shift
       ;;
   -d)
       DETECTTHRESH=${2}
       DETECTMINAREA=${3}
       runsex="yes"
       shift
       shift
       shift
       ;;
   -r)
       NLOW=${2}
       NHIGH=${3}
       shift
       shift
       shift
       ;;
   -c)
       METHOD=${2}
       shift
       shift
       ;;
   -s)
       RESCALE=${2}
       shift
       shift
       ;;
   -l)
       LEVEL=${2}
       shift
       shift
       ;;
    *)
       GoOn=1
       ;;
   esac
done

CHIPS=$*


# the resultdir is where the output coadded images
# will go. If ONE image of the corresponding chip
# is a link the image will go to THIS directory
for CHIP in ${CHIPS}
do
  RESULTDIR[${CHIP}]="${DIR}/"
done

SUB=""

if [ "${runsex}" = "yes" ]; then  

  # we create object-subtracted images to avoid dark regions
  # around bright objects in later images

  for CHIP in ${CHIPS}
  do
    ls -1 /${RESULTDIR[${CHIP}]}/*_${CHIP}${FILTER}.fits \
	> ${TEMPDIR}/images-objects_$$
    
    NUMTHREAD=`${SCRIPTDIR}/get_nthreads.sh ${NCHIPS} ${NPARA} ${CHIP}`

    cat ${TEMPDIR}/images-objects_$$ |\
    {
      while read file
      do
	BASE=`basename ${file} .fits`
        # now run sextractor to subtract objects from the image

	${P_SEX} ${file} -c ${DATACONF}/image-objects.sex\
	    -CHECKIMAGE_NAME ${RESULTDIR[${CHIP}]}/${BASE}".mask.fits" \
	    -CHECKIMAGE_TYPE -OBJECTS \
	    -DETECT_MINAREA ${DETECTMINAREA} \
	    -DETECT_THRESH ${DETECTTHRESH} \
	    -ANALYSIS_THRESH ${DETECTTHRESH} \
	    -FILTER Y \
	    -NTHREADS ${NUMTHREAD}

	${P_IC} '%1 -1e09 %2 fabs 1.0e-06 > ?' ${file} \
	    ${RESULTDIR[${CHIP}]}/${BASE}".mask.fits" \
	    > ${RESULTDIR[${CHIP}]}/${BASE}".mask1.fits"
	mv ${RESULTDIR[${CHIP}]}/${BASE}".mask1.fits" \
	    ${RESULTDIR[${CHIP}]}/${BASE}".mask.fits"
	
	if [ "${RESULTDIR[${CHIP}]}" != "${DIR}/" ]; then
	    ln -s ${RESULTDIR[${CHIP}]}/${BASE}".mask.fits" \
		${DIR}/${BASE}".mask.fits"
	fi
      done
    }
  done
  
  SUB=".mask"
fi

for CHIP in ${CHIPS}
do
  if [ "${RESULTDIR[${CHIP}]}" != "${DIR}/" ]; then
      ln -s ${RESULTDIR[${CHIP}]}/${NAME}_${CHIP}.fits \
	  ${DIR}/${NAME}_${CHIP}.fits
  fi
  
  get_statminsection
  # do statistics from all science frames.
  # This is necessary to get the mode
  # of the combination from all images, so that
  # the combination where images have been excluded
  # can be scaled accordingly.
  ${P_IMSTATS} -d 6 `ls /${DIR}/*_${CHIP}${FILTER}${SUB}.fits` \
      -o ${TEMPDIR}/science_images_$$ -t -1e8 1e8 \
      -s ${statminsection_imstats}
  
  RESULTMODE=`${P_GAWK} 'BEGIN {mean=0.0; n=0} ($1!="#") {
     n=n+1; mean=mean+$2} END {print mean/n}' ${TEMPDIR}/science_images_$$`

  if [ "${LEVEL}" != "" ]; then
      RESULTMODE=${LEVEL}
  fi
  
  # modify the input list of images
  # in case we have to reject files for the superflat:
  if [ -s /${DIR}/background_exclusion ]; then
      ${P_GAWK} 'BEGIN {nex = 0; while (getline <"/'${DIR}'/background_exclusion" > 0) {
               ex[nex]=$1; nex++; }}
               {exclude = 0; i=0;
                while (i<nex) {
                  if (index($1, ex[i])!=0) {
                    exclude = 1;
                  }
	          i++;
                }
                if(exclude == 0) {print $0}}' ${TEMPDIR}/science_images_$$ >\
		    ${TEMPDIR}/science_coadd_images_$$
  else
      cp ${TEMPDIR}/science_images_$$ ${TEMPDIR}/science_coadd_images_$$
  fi
  
  # do the combination; do not include images
  # listed in the /${DIR}/background_exclusion file

  ${P_IMCOMBFLAT_IMCAT} -i ${TEMPDIR}/science_coadd_images_$$\
      -o ${RESULTDIR[${CHIP}]}/${NAME}_${CHIP}.fits \
      -s ${RESCALE} -e ${NLOW} ${NHIGH} -m ${RESULTMODE} \
      -c ${METHOD} -t -1e8 1e8
  
  if [ "${SUB}" = ".mask" ]; then
      if [ ! -d /${DIR}/MASK_IMAGES ]; then
	  mkdir /${DIR}/MASK_IMAGES
      fi
      mv /${DIR}/*_${CHIP}${FILTER}${SUB}.fits /${DIR}/MASK_IMAGES/
      
      if [ ! -d /${DIR}/MASK_IMAGES ]; then
	  mkdir /${DIR}/MASK_IMAGES
      fi
      mv /${DIR}/*_${CHIP}${NAME}${SUB}.fits /${DIR}/MASK_IMAGES/
  fi

done
