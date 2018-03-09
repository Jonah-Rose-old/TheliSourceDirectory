#!/bin/bash -u

# the script corrects a set of Science frames
# for illumination pattern. This is an effective
# second flat-fielding. 
# The script uses the 'preprocess' program to perform the
# reduction.
# It assumes that all images from ONE chip reside
# in the same physical directory
#
# 28.07.2004:
# new RESCALE flag; it was introduced so that the user can decide
# whether the gain equalisation is done here with Superflats
# or in the process_science step with skyflats. 
#
# 14.08.2005:
# The preprocess calls now explicitely declare the statistics
# region on the command line.
#
# 02.05.2005:
# The preprocess call now uses the MAXIMAGES parameter.
#
# 05.12.2005:
# - Chips whose NOTPROCESS flag is set are not processed at
#   all
# - Chips whose NOTUSE flag is set are not considered in the
#   Flatscale images.
# - New command line argument: The 4th command line argument now
#   determined whether images are superflatted with the original
#   (parameter value SUPER),
#   unsmoothed superflat or the illumination correction image
#   (parameter value ILLUM).
# - I corrected a bug in the indexing of NOTPROCESS arrays.
#
# 01.02.2011:
# I introduced the minimum threshhold for a good flatfield as a
# command line argument. We saw for medium band filters (WFI) that we
# need to accept values as low as 20 counts for the superflat!
#
# 31.12.2012:
# I made the script more robust against non-existing files.

# $1: main directory (filter)
# $2: Science directory
# $3: RESCALE/NORESCALE
# $4: ILLUM/SUPER (Is the illumination correction done with
#     the 'illumination correction image' or with the unsmoothed
#     superflat?)
# $5: threshhold to accept a flat field (OPTIONAL; default: 50)
# $#: chips to be processed


# preliminary work:
. ./${INSTRUMENT:?}.ini
. ./bash_functions.include
theli_start "$*" "${!#}"

ILLUMFLAG=""

if [ "$4" = "ILLUM" ]; then
  ILLUMFLAG="_illum"
fi

# set threshhold to accept flat fields:
FLATTHRESH=50
if [ $# -gt 5 ]; then
  FLATTHRESH=$5
fi

for CHIP in ${!#}
do
  if [ ${NOTPROCESS[${CHIP}]:=0} -eq 0 ]; then
    RESULTDIR[${CHIP}]="$1/$2"
  else
    theli_warn "Chip ${CHIP} will not be processed!" "${!#}"
  fi
done

for CHIP in ${!#}
do
  if [ ${NOTPROCESS[${CHIP}]:=0} -eq 0 ]; then
    FILES=`ls /$1/$2/*_${CHIP}OFC.fits`

    if [ "${FILES}" != "" ]; then
      for FILE in ${FILES}
      do
        if [ -L ${FILE} ]; then
          LINK=`${P_READLINK} ${FILE}`
  	  BASE=`basename ${LINK} .fits`
  	  DIR=`dirname ${LINK}`
  	  ln -s ${DIR}/${BASE}S.fits $1/$2/${BASE}S.fits
  	  RESULTDIR[${CHIP}]=`dirname ${LINK}`    
        fi
      done 
  
      i=1
      j=1
      FLATSTR=""
      while [ "${i}" -le "${NCHIPS}" ]
      do
        if [ ${NOTUSE[${i}]:=0} -eq 0 ] && \
            [ ${NOTPROCESS[${i}]:=0} -eq 0 ]; then
          if [ "${j}" = "1" ]; then
            FLATSTR="/$1/$2/$2_${i}${ILLUMFLAG}.fits"
            j=2
          else
            FLATSTR="${FLATSTR},/$1/$2/$2_${i}${ILLUMFLAG}.fits"
          fi   
        fi 
        i=$(( $i + 1 ))
      done
   
  
      RESCALEFLAG=""
      if [ "$3" = "RESCALE" ]; then   
        RESCALEFLAG="-FLAT_SCALE Y -FLAT_SCALEIMAGE ${FLATSTR}"
      fi
  
      ${P_IMRED_ECL:?} ${FILES} \
         -MAXIMAGES ${NFRAMES}\
         -STATSSEC ${STATSXMIN},${STATSXMAX},${STATSYMIN},${STATSYMAX} \
         -OVERSCAN N \
         -BIAS N \
         -FLAT Y \
         -FLAT_IMAGE /$1/$2/$2_${CHIP}${ILLUMFLAG}.fits \
         -COMBINE N \
         -OUTPUT Y \
         -OUTPUT_DIR /$1/$2/ \
         -OUTPUT_SUFFIX S.fits \
         -FLAT_THRESHHOLD ${FLATTHRESH} \
         -TRIM N ${RESCALEFLAG}
  
      test -d /$1/$2/OFC_IMAGES || mkdir /$1/$2/OFC_IMAGES
      mv /$1/$2/*_${CHIP}OFC.fits /$1/$2/OFC_IMAGES
    else
      theli_warn "No files for Chip ${CHIP} to process!" "${!#}"
    fi
  fi
done

theli_end "${!#}"
exit 0;

