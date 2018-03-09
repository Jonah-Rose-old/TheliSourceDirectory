#!/usr/bin/bash -u

# the script processes a set of Standard star frames the images are
# overscan corrected, debiased and flatfielded.  For further
# processing superflats are NOT created from the preprocessed
# standards but the corresponding files from the SCIENCE frames are
# copied.  The script uses the 'preprocess' program to perform the
# reduction.  It assumes that all images from ONE chip reside in the
# same physical directory
#

# HISTORY COMMENTS:
#
# 31.12.2012:
# I made the script more robust to non-exitsting data

#$1: main directory (filter)
#$2: Bias directory
#$3: Flat directory
#$4: Science directory
#$5: Standard directory
#$6: RESCALE/NORESCALE
#$7: chips to be processed

# preliminary work:
. ./${INSTRUMENT:?}.ini
. ./bash_functions.include
theli_start "$*" "${!#}"

# the resultdir is where the output coadded images will go. If ONE
# image of the corresponding chip is a link the image will go to THIS
# directory
for CHIP in ${!#}
do
  if [ ${NOTPROCESS[${CHIP}]:=0} -eq 0 ]; then
    SCIRESULTDIR[${CHIP}]="$1/$4/"
    STDRESULTDIR[${CHIP}]="$1/$5/"
  else
    theli_warn "Chip ${CHIP} will not be processed!" "${!#}"
  fi
done

# perform preprocessing (overscan correction, BIAS subtraction, first
# flatfield pass)
for CHIP in ${!#}
do
  if [ ${NOTPROCESS[${CHIP}]:=0} -eq 0 ]; then
    FILES=`ls $1/$5/*_${CHIP}.fits`

    # if there are no files we do not need to do anything!
    if [ "${FILES}" != "" ]; then
      for FILE in ${FILES}
      do
        if [ -L ${FILE} ]; then
          LINK=`${P_READLINK} ${FILE}`
          BASE=`basename ${LINK} .fits`
          DIR=`dirname ${LINK}`
          ln -s ${DIR}/${BASE}OFC.fits $1/$5/${BASE}OFC.fits
          STDRESULTDIR[${CHIP}]=`dirname ${LINK}`    
        fi
      done 
  
      MAXX=$(( ${CUTX[${CHIP}]} + ${SIZEX[${CHIP}]} - 1 ))
      MAXY=$(( ${CUTY[${CHIP}]} + ${SIZEY[${CHIP}]} - 1 ))
  
      # build up list of all flatfields necessary for rescaling
      # when gains between chips are equalised here.
      i=1
      j=1
      FLATSTR=""
      while [ "${i}" -le "${NCHIPS}" ]
      do
        if [ ${NOTUSE[${i}]:=0} -eq 0 ] && \
           [ ${NOTPROCESS[${i}]:=0} -eq 0 ]; then
          if [ "${j}" = "1" ]; then
            FLATSTR="/$1/$3/$3_${i}.fits"
	    j=2
          else
            FLATSTR="${FLATSTR},/$1/$3/$3_${i}.fits"
          fi
        fi    
        i=$(( $i + 1 ))
      done
  
      FLATFLAG=""
      if [ "$6" = "RESCALE" ]; then   
        FLATFLAG="-FLAT_SCALE Y -FLAT_SCALEIMAGE ${FLATSTR}" 
      fi
  
      # overscan correct, bias subtract and flatfield
      # science images:
      ${P_IMRED_ECL:?} `ls /$1/$5/*_${CHIP}.fits` \
        -MAXIMAGES ${NFRAMES}\
        -STATSSEC ${STATSXMIN},${STATSXMAX},${STATSYMIN},${STATSYMAX} \
        -OVERSCAN Y \
        -OVERSCAN_REGION ${OVSCANX1[${CHIP}]},${OVSCANX2[${CHIP}]} \
        -BIAS Y \
        -BIAS_IMAGE /$1/$2/$2_${CHIP}.fits \
        -FLAT Y \
        -FLAT_IMAGE /$1/$3/$3_${CHIP}.fits \
        -COMBINE N \
        -OUTPUT Y \
        -OUTPUT_DIR /$1/$5/ \
        -OUTPUT_SUFFIX OFC.fits \
        -TRIM Y \
        -TRIM_REGION ${CUTX[${CHIP}]},${MAXX},${CUTY[${CHIP}]},${MAXY} \
        ${FLATFLAG}

      # move splitted images:
      test -d /$1/$5/SPLIT_IMAGES || mkdir /$1/$5/SPLIT_IMAGES
      mv /$1/$5/*_${CHIP}.fits /$1/$5/SPLIT_IMAGES

      # copy superflat images from the SCIENCE directory:
      cp /${SCIRESULTDIR[${CHIP}]}/$4_${CHIP}.fits \
         /${STDRESULTDIR[${CHIP}]}/$5_${CHIP}.fits

      # create link for superflat images if necessary
      if [ ! -f $1/$5/$5_${CHIP}.fits ]; then
        ln -s /${STDRESULTDIR[${CHIP}]}/$5_${CHIP}.fits \
            $1/$5/$5_${CHIP}.fits
      fi
    fi
  fi
done

theli_end "${!#}"
exit 0;
