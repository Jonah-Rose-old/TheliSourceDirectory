#!/bin/bash -xv

# the script creates flag maps for images; all pixels being non-zero
# will be set to 1 and all zeros remain zeros; the output image is an
# 8-bit FITS frame.

# HISTORY COMMENTS
# ================
#
# 12.02.2013:
# I included the new THELI logging scheme into the script.

# $1: main directory
# $2: science directory
# $3: image ending (including a . before the fits extension)
# $4: new image ending (including a . before the fits extension)
# $5: chips to be processed

. ./${INSTRUMENT:?}.ini
. ./bash_functions.include
theli_start "$*" "${!#}"


# the chips that are to be processed
for CHIP in ${!#}
do
  ${P_FIND} /$1/$2/ -maxdepth 1 -name \*_${CHIP}${3}fits > \
            ${TEMPDIR}/flag_images${CHIP}_$$

  if [ -s ${TEMPDIR}/flag_images${CHIP}_$$ ]; then
    while read file
    do
      if [ -L ${file} ]; then
        LINK=`${P_READLINK} ${file}`
        RESULTDIR[${CHIP}]=`dirname ${LINK}`
      else
        RESULTDIR[${CHIP}]="/$1/$2"
      fi
  
      BASE=`basename ${file} ${3}fits`
  
      ${P_IC} -p 8 '0 1 %1 fabs 1.0e-09 < ?' \
        ${RESULTDIR[${CHIP}]}/${BASE}"${3}fits" > \
        ${RESULTDIR[${CHIP}]}/${BASE}"${4}fits"
  
    done < ${TEMPDIR}/flag_images${CHIP}_$$
  else
    theli_warn "No images to process for chip ${CHIP}!"
  fi  

  test -f ${TEMPDIR}/flag_images${CHIP}_$$ && \
       rm ${TEMPDIR}/flag_images${CHIP}_$$
done

theli_end "${!#}"
exit 0;
