#!/usr/bin/bash

# the script processes a set of Science frames the images are overscan
# corrected, debiased, flatfielded and stacked with mode rescaling
# (Superflat). We assume that we do NOT work with a normalised flat
# The script uses the 'preprocess' program to perform the reduction.
# It assumes that all images from ONE chip reside in the same physical
# directory
#
# 30.05.04:
# temporary files go to a TEMPDIR directory
#
# 24.08.04:
# - in bands containing strong fringes (I and Z), we have
#   to use special detection/pixel threshholds in the
#   creation of sky subtracted images (In introduced a new
#   argument to indicate this feature)
# - new RESCALE flag; it was introduced so that the user can decide
#   whether the gain equalisation is done here with skyflats
#   or in the process_science_illum step with superflats.
#
# 25.11.04:
# We introduced the possibility to exclude certain chips in the
# creation of a superflat. To this end a list with the chips to be
# excluded has to reside in $1/superflat_exclusion.  Each line of this
# file has to contain a filename with blabla_CHIP, where CHIP
# represents the chipnumber to be excluded.
#
# 28.11.2004:
# corrected a bug in the case where the gain equalisation is performed
# here. There was no FLAT_SCALEIMAGE specified.
#
# 14.02.2005:
# the 'stats' program is now called via the P_STATS variable to avoid
# that it is not executed if it is not in the users path.
#
# 25.02.2005:
# I removed a '-COMBINE_MINVAL "-69999"' statement so that the default
# minimum value of -10000 is used. -69999 seems to lead to wrong
# problems if we have -70000 in some images.
#
# 26.02.2005:
# I substituted the call to the Python script filter.py by a gawk
# call. This enables the use of this script on machines where Python
# is not available.
#
# 02.05.2005:
# I rewrote the script to use the MAXIMAGE parameter in preprocess
# and the imcat-based imcombflat for the coaddition of the superflat.
#
# 14.08.2005:
# The preprocess calls now explicitely declare the statistics
# region on the command line.
#
# 05.12.2005:
# - Chips whose NOTPROCESS flag is set are not processed at all.
# - Chips whose NOTUSE flag is set are not used in the set of
#   Flatscale images.
# - The superflat combination now uses the clipped mean algorithm
#   from imcombflat.
# - I corrected a bug in the indexing of NOTPROCESS flags.
#
# 22.12.2005:
# - I changed back the final co-addition to median stacking
#   (with initial rejection of the highest value at each pixel
#   position).
#   The clipped mean turned out to be too affected by outliers.
# - background statistics for the superflat are now estimated
#   from the whole frame and not only from a subsection in the
#   middle of a chip.
#   Bright features in the middle of the chip tended to bias
#   the final result.
#
# 10.02.2006:
# - I corrected a major bug in the selection of images entering the
#   superflats. In cameras with more than 9 chips too many images
#   were rejected.
# - I made the reading of the superflat_exclusion file more robust
#   against empty lines
#
# 25.09.2008:
# temporary files are deleted at the end of the script!
#
# 25.01.2012:
# I included the possibility to check for suitable superflat images
# Chips from images who have objects in more than 8% of their pixels
# can be excluded from superflat creation (set 7th command line
# argument to SUPERTEST to enable this feature).
#
# 12.02.2012:
# If the superflat rejection removes all images from a chip we create
# a constant value image with the approprate mode as SCIENCE image for
# that chip.
#
# 31.12.2012:
# I made the script more robust against non-existing files
#
# 04.02.2013:
# superflats are no longer created if they already are present!
#
# 13.02.2013:
# I corrected a bug in the creation of the initial image list to
# process.  It was only correct if the science directory actually had
# the name 'SCIENCE...'
#
# 15.05.2014:
# I include the possibility to remove images 'completely' from
# superflat consideration. Similar to the 'regular' superflat check
# this is done within a file 'superflat_complete_exclusion' listing
# the basenames of exposures to exclude. We saw that, in rare
# circumstances, very bright stars even scre up the superflat if the
# corresponding chips are rejected with the 'regular' superlfat
# rejection scheme. These images would still 'contribute' the the
# resulting lfat with their mode.  This feature is also enabled by
# 'SUPERTEST' as 7th argument.

#$1: main directory (filter)
#$2: Bias directory
#$3: Flat directory
#$4: Science directory
#$5: RESCALE/NORESCALE
#$6: FRINGE/NOFRINGE
#$7: SUPERTEST/NOSUPERTEST
#$8: chips to be processed

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
    RESULTDIR[${CHIP}]="$1/$4"
  else
    theli_warn "Chip ${CHIP} will not be processed!" "${!#}"
  fi
done

# perform preprocessing (overscan correction, BIAS subtraction, first
# flatfield pass)
for CHIP in ${!#}
do
  if [ ${NOTPROCESS[${CHIP}]:=0} -eq 0 ]; then
    FILES=`ls $1/$4/*_${CHIP}.fits | grep -v "$1/$4/$4"`

    DOPROC[${CHIP}]=0 # wll be set to 1 if we need to do soemthing
                      # at all
    if [ "${FILES}" != "" ]; then
      DOPROC[${CHIP}]=1

      for FILE in ${FILES}
      do
        if [ -L ${FILE} ]; then
          LINK=`${P_READLINK} ${FILE}`
          BASE=`basename ${LINK} .fits`
          DIR=`dirname ${LINK}`
          ln -s ${DIR}/${BASE}OFC.fits $1/$4/${BASE}OFC.fits
          RESULTDIR[${CHIP}]=`dirname ${LINK}`
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
        if [ ${NOTUSE[${i}]:=0} -eq 0 ] && [ ${NOTPROCESS[${i}]:=0} -eq 0 ]
        then
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
      if [ "$5" = "RESCALE" ]; then
        FLATFLAG="-FLAT_SCALE Y -FLAT_SCALEIMAGE ${FLATSTR}"
      fi
  
      # overscan correct, bias subtract and flatfield science images:
      ${P_IMRED_ECL:?} ${FILES} \
        -MAXIMAGES ${NFRAMES}\
        -STATSSEC ${STATSXMIN},${STATSXMAX},${STATSYMIN},${STATSYMAX} \
        -OVERSCAN Y \
        -OVERSCAN_REGION ${OVSCANX1[${CHIP}]},${OVSCANX2[${CHIP}]} \
        -BIAS Y -BIAS_IMAGE /$1/$2/$2_${CHIP}.fits \
        -FLAT Y -FLAT_IMAGE /$1/$3/$3_${CHIP}.fits \
        -COMBINE N \
        -OUTPUT Y -OUTPUT_DIR /$1/$4/ -OUTPUT_SUFFIX OFC.fits \
        -TRIM Y \
        -TRIM_REGION ${CUTX[${CHIP}]},${MAXX},${CUTY[${CHIP}]},${MAXY} \
        ${FLATFLAG}

      test -d /$1/$4/SPLIT_IMAGES || mkdir /$1/$4/SPLIT_IMAGES
      mv ${FILES} /$1/$4/SPLIT_IMAGES
    else
      theli_warn "No files for Chip ${CHIP} present!" "${!#}"
    fi  
  fi
done

# do not create superflat images if they are present already.  We
# assume that superflat files exist for all chips if they exist for
# some:
DOSUPER=1
for CHIP in ${!#}
do
  if [ -s $1/$4/$4_${CHIP}.fits ]; then
    theli_warn "Superflat $1/$4/$4_${CHIP}.fits already exists!"
    DOSUPER=0
  fi
done

if [ ${DOSUPER} -eq 1 ]; then
  # we create object-subtracted images to avoid dark regions around
  # bright objects in later images (superflats)
  for CHIP in ${!#}
  do
    if [ ${NOTPROCESS[${CHIP}]:=0} -eq 0 ] && [ ${DOPROC[${CHIP}]} -eq 1 ]; then
      ls -1 /${RESULTDIR[${CHIP}]}/*_${CHIP}OFC.fits > \
        ${TEMPDIR}/images-objects_$$
  
      while read FILE
      do
        BASE=`basename ${FILE} .fits`
        #
        # now run sextractor to subtract objects from the image
        #
        # if the SCIENCE images contain fringes ($6=FRINGE) we have to
        # use special detection/pixel threshholds to keep the fringes.
        if [ "$6" = "FRINGE" ]; then
          ${P_SEX} ${FILE} -c ${DATACONF}/image-objects.sex\
            -CHECKIMAGE_NAME ${RESULTDIR[${CHIP}]}/${BASE}"_sub.fits"\
                  -DETECT_MINAREA 7 -DETECT_THRESH 5 -ANALYSIS_THRESH 5
        else
          ${P_SEX} ${FILE} -c ${DATACONF}/image-objects.sex\
            -CHECKIMAGE_NAME ${RESULTDIR[${CHIP}]}/${BASE}"_sub.fits"
        fi
        ${P_IC} '%1 -70000 %2 fabs 1.0e-06 > ?' ${FILE} \
                ${RESULTDIR[${CHIP}]}/${BASE}"_sub.fits" \
          > ${RESULTDIR[${CHIP}]}/${BASE}"_sub1.fits"
  
        mv ${RESULTDIR[${CHIP}]}/${BASE}"_sub1.fits" \
           ${RESULTDIR[${CHIP}]}/${BASE}"_sub.fits"
  
        if [ "${RESULTDIR[${CHIP}]}" != "$1/$4" ]; then
          ln -s ${RESULTDIR[${CHIP}]}/${BASE}"_sub.fits" \
                $1/$4/${BASE}"_sub.fits"
        fi
      done < ${TEMPDIR}/images-objects_$$
    fi
  done
  
  # see whether we need to exclude images (individual chips) from the
  # superflat creation process. We do not need to check for images to
  # be excluded if we already did this at some point. In this case
  # there should be a file 'superflat_exclusion' listing, on each
  # line, a chip to be excluded from the superflat. The lines in the
  # file should read as 'bla_CHIP' (bla is the basename of the image
  # and CHIP the chip number)
  if [ -f $1/superflat_exclusion ]; then
    cp $1/superflat_exclusion $1/superflat_exclusion_$$
  else
    if [ "$7" = "SUPERTEST" ]; then  
      test -f ${TEMPDIR}/superflattest.txt_$$ && \
           rm ${TEMPDIR}/superflattest.txt_$$
  
      for CHIP in ${!#}
      do
        ${P_FIND} /$1/$4/ -maxdepth 1 -name \*_${CHIP}OFC_sub.fits >> \
          ${TEMPDIR}/superflattest.txt_$$
      done
  
      if [ -s ${TEMPDIR}/superflattest.txt_$$ ]; then
        ${P_PYTHON} ${S_SUPERFLATEXCLUSION} \
            ${TEMPDIR}/superflattest.txt_$$ -50 0.08 1 > \
            ${TEMPDIR}/superflatexclude.txt_$$
    
        ${P_GAWK} -F/ '{print $NF}' ${TEMPDIR}/superflatexclude.txt_$$ |\
          sed -e 's/OFC_sub.fits//' > $1/superflat_exclusion_$$
      fi
    fi
  fi
  
  # create superflat
  for CHIP in ${!#}
  do
    if [ ${NOTPROCESS[${CHIP}]:=0} -eq 0 ] && [ ${DOPROC[${CHIP}]} -eq 1 ]; then
      if [ "${RESULTDIR[${CHIP}]}" != "$1/$4" ]; then
         ln -s ${RESULTDIR[${CHIP}]}/$4_${CHIP}.fits $1/$4/$4_${CHIP}.fits
      fi
  
      # do statistics from all science frames.  This is necessary to get
      # the mode of the combination from all images, so that the
      # combination where images have been excluded can be scaled
      # accordingly. At this stage only reject images where all the
      # chips need to be removed from the superflat process.
      ${P_FIND} /$1/$4/ -maxdepth 1 -name \*_${CHIP}OFC_sub.fits |\
          sed -e 's!/.*/!!' | sed -e 's/_'${CHIP}'OFC_sub.fits//' > \
          ${TEMPDIR}/superflat_candidates_$$

      if [ -f $1/superflat_complete_exclusion ]; then
        ${P_GAWK} '{print $1}' ${TEMPDIR}/superflat_candidates_$$ \
            $1/superflat_complete_exclusion | sort | uniq -c | \
            ${P_GAWK} '($1 == 1) {
                       print "/'$1'/'$4'/" $2 "_'${CHIP}'OFC_sub.fits"}' > \
              ${TEMPDIR}/superflat_images_$$
      else
        ${P_GAWK} '{print "/'$1'/'$4'/" $1 "_'${CHIP}'OFC_sub.fits"}' \
           ${TEMPDIR}/superflat_candidates_$$ > ${TEMPDIR}/superflat_images_$$
      fi

      ${P_IMSTATS} -t -60000 60000 `cat ${TEMPDIR}/superflat_images_$$`\
                   -o ${TEMPDIR}/science_images_$$
  
      RESULTMODE=`${P_GAWK} 'BEGIN {mean = 0.0; n = 0} ($1!="#") {
                             n = n + 1;
                             mean = mean + $2}
                             END {print mean / n}' \
                             ${TEMPDIR}/science_images_$$`
  
      # modify the input list of images in case we have to reject files
      # for the superflat:
      if [ -s /$1/superflat_exclusion_$$ ]; then
        ${P_GAWK} 'BEGIN {nex = 0;
                     while (getline <"/'$1'/superflat_exclusion_'$$'" > 0) {
                       gsub(" ", "", $1);
                       if (length($1) >0) {
                         ex[nex]=$1; nex++;
                       }
                     }}
                   {exclude = 0; i=0;
                    while (i<nex) {
                      if ((ind=index($1, ex[i]))!=0) {
                        tmp=$1;
                        gsub(ex[i],"",tmp);
                        first=substr(tmp,ind,1);
                        if (first !~ /[0-9]/) {
                          exclude = 1;
                        }
                      }
                      i++;
                    }
                    # we exclude comment lines from the output. Thus
                    # the result file is empty if the superflat exclusion
                    # should remove all images:
                    if ((exclude == 0) && ($0 !~ /^#/)) {print $0}}' \
        ${TEMPDIR}/science_images_$$ > ${TEMPDIR}/science_coadd_images_$$
      else
        cp ${TEMPDIR}/science_images_$$ ${TEMPDIR}/science_coadd_images_$$
      fi
  
      # do the combination: first test whether a possible superflat
      # exclusion removed all the images. In this case just create a
      # constant value image with the mode:
      if [ -s ${TEMPDIR}/science_images_$$ ] && \
         [ ! -s ${TEMPDIR}/science_coadd_images_$$ ]; then
        ${P_IC} -c ${SIZEX[${CHIP}]} ${SIZEY[${CHIP}]} \
              -p -32 ''${RESULTMODE}'' > ${RESULTDIR[${CHIP}]}/$4_${CHIP}.fits
      else
        ${P_IMCOMBFLAT_IMCAT} -i ${TEMPDIR}/science_coadd_images_$$\
                        -o ${RESULTDIR[${CHIP}]}/$4_${CHIP}.fits \
                        -s 1 -e 0 1 -m ${RESULTMODE}
      fi
  
      test -d /$1/$4/SUB_IMAGES || mkdir /$1/$4/SUB_IMAGES
      mv /$1/$4/*_${CHIP}OFC_sub.fits /$1/$4/SUB_IMAGES/
    fi
  done
fi # if [ ${DOSUPER} ] ....

# clean up and bye
test -f ${TEMPDIR}/images-objects_$$ && \
        rm ${TEMPDIR}/images-objects_$$
test -f ${TEMPDIR}/science_coadd_images_$$ && \
        rm ${TEMPDIR}/science_coadd_images_$$
test -f ${TEMPDIR}/science_images_$$ && \
        rm ${TEMPDIR}/science_images_$$
test -f ${TEMPDIR}/superflattest.txt_$$ && \
        rm ${TEMPDIR}/superflattest.txt_$$
test -f ${TEMPDIR}/superflatexclude.txt_$$ && \
        rm ${TEMPDIR}/superflatexclude.txt_$$
test -f ${TEMPDIR}/superflat_images_$$ && \
        rm ${TEMPDIR}/superflat_images_$$
test -f ${TEMPDIR}/superflat_candidates_$$ && \
        rm ${TEMPDIR}/superflat_candidates_$$

theli_end "${!#}"
exit 0;
