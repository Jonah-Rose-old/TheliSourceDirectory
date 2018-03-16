#!/usr/bin/bash -xv

# splits WFI Fits extension images into the eight chips. Uses the
# eclipse utilities and also updates the image headers
#
# 16.02.2005:
# I now transfer also the OBJECTS keyword from the original files.
#
# 30.01.2006:
# The raw images now have to be already in a ORIGINALS subdirectory
# (from /$1/$2). They are no longer moved after extraction of
# individual chips.
#
# 20.07.2007:
# I added the IMAGEID command line option to the mefsplit call. It is
# necessary due to a new numbering scheme for WFI chips.
#
# 06.02.2012:
# some code beautifying
#
# 23.11.2012:
# values for GAIN and saturation level are now added to the image headers
# of splitted data.
#
# 24.03.2014:
# All images get a BADCCD header keyword so that we are able to mark
# bad CCDs.
#
# 29.04.2014:
# I simplified the reading of header keywords.

# $1: main directory
# $2: science directory

# create image list: we assume that ONLY unsplit
# images are in the directory

. ./WFI.ini

FILES=`${P_FIND} $1/$2/ORIGINALS -maxdepth 1 -name \*.fits`

cd /$1/$2/ORIGINALS

for FILE in ${FILES}
do
  # The filter name for WFI is either stored in the header keyword
  # 'HIERARCH ESO INS FILT NAME' or in 'HIERARCH ESO INS FILT1 NAME':
  FILTNAM=`${P_DFITS} ${FILE} | ${P_FITSORT} -d \
          "HIERARCH ESO INS FILT NAME" "HIERARCH ESO INS FILT1 NAME" |\
          sed -e 's!KEY_N/A!!' | ${P_GAWK} '{print $2}'`
  FILTNAM="'${FILTNAM}'"

  RA=`${P_DFITS}  ${FILE} | ${P_FITSORT} -d RA | cut -f 2`
  DEC=`${P_DFITS} ${FILE} | ${P_FITSORT} -d DEC | cut -f 2`
  LST=`${P_DFITS} ${FILE} | ${P_FITSORT} -d LST | cut -f 2`
  MJD=`${P_DFITS} ${FILE} | ${P_FITSORT} -d "MJD-OBS" | cut -f 2`
  OBJECT=`${P_DFITS} ${FILE} | ${P_FITSORT} -d OBJECT | cut -f 2`
  EXPTIME=`${P_DFITS} ${FILE} | ${P_FITSORT} -d EXPTIME | cut -f 2`
  AIRMASS=`${P_AIRMASS} -t ${LST} -e ${EXPTIME} \
                        -r ${RA} -d ${DEC} -l -29.25694444`
  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
           ${P_GAWK} ' ($1 ~ /Days/) {print $6}' |\
           ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`


  # The GAIN of WFI images is stored in keywords with different names
  # in each chip for WFI. Therefore the eight 'HEADTRANSFER' options
  # in the following command:
  ${P_FITSSPLIT_ECL} -CRPIX1 \
    "${REFPIXX[1]},${REFPIXX[2]},${REFPIXX[3]},${REFPIXX[4]},${REFPIXX[5]},${REFPIXX[6]},${REFPIXX[7]},${REFPIXX[8]}"\
                     -CRPIX2 \
    "${REFPIXY[1]},${REFPIXY[2]},${REFPIXY[3]},${REFPIXY[4]},${REFPIXY[5]},${REFPIXY[6]},${REFPIXY[7]},${REFPIXY[8]}"\
                     -IMAGEID \
    "${IMAGEID[1]},${IMAGEID[2]},${IMAGEID[3]},${IMAGEID[4]},${IMAGEID[5]},${IMAGEID[6]},${IMAGEID[7]},${IMAGEID[8]}"\
                     -CRVAL1 ${RA} -CRVAL2 ${DEC}\
                     -EXPTIME ${EXPTIME}\
                     -AIRMASS ${AIRMASS}\
                     -GABODSID ${GABODSID}\
                     -FILTER ${FILTNAM}  \
                     -OBJECT ${OBJECT} \
                     -HEADER BADCCD 0 "Is_CCD_Bad_(1=Yes)" \
                     -HEADTRANSFER "HIERARCH ESO DET OUT1 CONAD" GAIN \
                     -HEADTRANSFER "HIERARCH ESO DET OUT2 CONAD" GAIN \
                     -HEADTRANSFER "HIERARCH ESO DET OUT3 CONAD" GAIN \
                     -HEADTRANSFER "HIERARCH ESO DET OUT4 CONAD" GAIN \
                     -HEADTRANSFER "HIERARCH ESO DET OUT5 CONAD" GAIN \
                     -HEADTRANSFER "HIERARCH ESO DET OUT6 CONAD" GAIN \
                     -HEADTRANSFER "HIERARCH ESO DET OUT7 CONAD" GAIN \
                     -HEADTRANSFER "HIERARCH ESO DET OUT8 CONAD" GAIN \
                     -OUTPUT_DIR .. \
                     ${FILE}

  # get an estimate for the saturation level in the images and put it
  # into the image headers:
  BASE=`basename ${FILE} .fits`
  ${P_IMSTATS} -t 50000 100000 ../${BASE}_*fits > \
      satlevels.txt_$$
  ${P_GAWK} '$1 !~ /#/ {
       image = $1;
       if ($2 > 50000) {
         satlev = sprintf(" \"SATLEVEL= %20.2f / saturation level\"",
                          int($2));
       } else {
         satlev = sprintf(" \"SATLEVEL= %20.2f / saturation level\"",
                          60000.00);
       }
       system("'${P_REPLACEKEY}' " image satlev " DUMMY1");
       }' satlevels.txt_$$
  rm satlevels.txt_$$
done
