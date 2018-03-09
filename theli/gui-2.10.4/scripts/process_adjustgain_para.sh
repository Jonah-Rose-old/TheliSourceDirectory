#!/bin/bash -xv

# adjusts the gain towards the highest gain of a mosaic
# camera, using sky background levels.

#$1: main directory (filter)
#$2: Science directory
#$3: chips to be processed

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
for CHIP in $3
do
  RESULTDIR[${CHIP}]="$1/$2"
done

# we create object-subtracted images to avoid dark regions
# around bright objects in later images

if [ "${V_DO_SUPERFLAT}" = "Y" ]; then

  SUB=""
  if [ "${V_CAL_DETECTMINAREA}" != "" ] || [ "${V_CAL_DETECTTHRESH}" != "" ]; then
    SUB=".mask"
  fi

  for CHIP in $3
  do
      get_statminsection
      # do statistics from all science frames.
      # This is necessary to get the mode
      # of the combination from all images, so that
      # the combination where images have been excluded
      # can be scaled accordingly.
      ${P_IMSTATS} -d 6 `ls /$1/$2/*_${CHIP}OFC${SUB}.fits` \
	  -o ${TEMPDIR}/science_images_$$ -t -1e8 1e8 \
	  -s ${statminsection_imstats}

      RESULTMODE=`${P_GAWK} 'BEGIN {mean=0.0; n=0} ($1!="#") {
	  n=n+1; mean=mean+$2} END {print mean/n}' ${TEMPDIR}/science_images_$$`
      
      # modify the input list of images
      # in case we have to reject files for the superflat:
      if [ -s /$1/$2/background_exclusion ]; then
	  ${P_GAWK} 'BEGIN {nex = 0; while (getline <"/'$1/$2'/background_exclusion" > 0) {
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
      # listed in the /$1/$2/background_exclusion file

      ${P_IMCOMBFLAT_IMCAT} -i ${TEMPDIR}/science_coadd_images_$$ \
	  -o ${RESULTDIR[${CHIP}]}/$4_${CHIP}.fits -c ${V_CAL_COMBINEMETHOD} \
	  -s 1 -e ${V_CAL_SCIENCENLOW} ${V_CAL_SCIENCENHIGH} -m ${RESULTMODE}
      
      if [ "${SUB}" = ".mask" ]; then
	  if [ ! -d /$1/$2/MASK_IMAGES ]; then
	      mkdir /$1/$2/MASK_IMAGES
	  fi
	  mv /$1/$2/*_${CHIP}OFC${SUB}.fits /$1/$2/MASK_IMAGES/
	  
	  if [ ! -d /$1/$2/MASK_IMAGES ]; then
	      mkdir /$1/$2/MASK_IMAGES
	  fi
	  mv /$1/$2/*_${CHIP}OFC${SUB}.fits /$1/$2/MASK_IMAGES/
      fi
  done
fi
