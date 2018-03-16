#!/usr/bin/bash -u

# First step to create catalogues suitable for absolute photometric
# calibration with 'create_abs_photo_info.sh'. The script takes
# astrometric information from a scamp processing and calculates
# 'correct' sky coordinates from objects catalogues (extracted from
# standard star fields). Afterwards the standard star object
# catalogues are merged with a reference standard star sources.
# (e.g. from Landolt, Stetson, Sloan ....)
# This last step is to be performed with the script
# 'create_stadphotom_merge.sh'

# SCRIPT HISTORY:
#
# 23.11.2012:
# The merging of extracted source lists and the standard star catalogue
# needed an update because names of magnitudes are different for
# different filter systems. They are now read dynamically from the
# standardstar catalogue and a config file for 'make_ssc' is created on
# the fly.
#
# 07.01.2013:
# I made the script more robust to non-existing files.
#
# 01.06.2013:
# Bug fix: objects that were associated with the standardstarcatalogue
# but who had associations with more than one object entered the processing.
# This caused huge problems in using long-exposed KIDS fields for photometric
# calibration with SDSS. Up to now this probably never showed up when we
# calibrated with short exposed standard star fields who only had 'isolated'
# objects listed as standard sources.
#
# 04.06.2013:
# I completely rewrote this script by splitting it. The task of estimating
# correct astrometry for standard star observations and merging them
# with a standard star catalogue is now done in two separate scripts.
# The merging is no longer performed chip-by-chip but on an exposure level.
# This dramatically reduces the number of necessary associations. The
# associations became a very long process when we used also KiDS science
# data (overlapping with SDSS) in the calibration process.

#$1: main directory
#$2: science dir. (the catalogues are assumed to be in $1/$2/cat)
#$3: image extension
#$4: astrometry standardstar catalogue used for the scamp calibration
#    (the script needs the scamp headers which are assumed to be in 
#    $1/$2/headers_scamp_$4)
#$5: chips to work on

. ./${INSTRUMENT:?}.ini
. ./bash_functions.include
theli_start "$*" "${!#}"

# check number of command line arguments:
if [ $# -ne 5 ]; then
  theli_error "Number of command line argument not correct!" "${!#}"
  exit 1;
fi

# give meaningful names to command line arguments:
MD=$1
SD=$2
EXTENSION=$3
STANDARDCAT=$4

# start script task:
for CHIP in ${!#}
do
  CATS=`find ${MD}/${SD}/cat -name \*_${CHIP}${EXTENSION}.cat`

  if [ "${CATS}" != "" ]; then
    for CAT in ${CATS}
    do
      BASE=`basename ${CAT} .cat`
      HEADBASE=`basename ${CAT} ${EXTENSION}.cat`
      ${P_LDACCONV} -i ${CAT} -o ${MD}/${SD}/cat/${BASE}_ldac.cat \
                    -b ${CHIP} -c ${INSTRUMENT} -f dummy 
  
      # get 'correct' astrometric scamp information to the catalogues:    
      ${S_APLASTROMSCAMP} -i ${MD}/${SD}/cat/${BASE}_ldac.cat \
           -o ${MD}/${SD}/cat/${BASE}_ldac_corr.cat \
           -s ${MD}/${SD}/headers_scamp_${STANDARDCAT}/${HEADBASE}.head \
           -t OBJECTS -p Xpos Ypos -r Ra Dec
  
      ${P_MAKEJOIN} -i /${MD}/${SD}/cat/${BASE}_ldac_corr.cat \
                    -o ${TEMPDIR}/tmp.cat0_$$ \
  		    -c ${DATACONF}/stdphotom_prepare_make_join.conf

      # calculate object magnitudes with a magzeropoint of 0 and
      # an exposure time normalisation of 1s (1.08574 is 2.5 / log(10)):
      ${P_LDACCALC} -i ${TEMPDIR}/tmp.cat0_$$ \
                    -o ${TEMPDIR}/tmp.cat00_$$ \
                    -t OBJECTS -c "(-1.08574*log(FLUX_AUTO/EXPTIME));" \
                    -n MAG_AUTO_corr "exposure time corr. MAG_AUTO" -k FLOAT
      ${P_LDACRENTAB} -i ${TEMPDIR}/tmp.cat00_$$\
                      -o ${TEMPDIR}/tmp.cat1_$$ -t OBJECTS STDTAB
      ${P_LDACDELTAB} -i ${TEMPDIR}/tmp.cat1_$$\
                      -o ${TEMPDIR}/tmp.cat11_$$ -t FIELDS
      ${P_LDACRENKEY} -i ${TEMPDIR}/tmp.cat11_$$ \
                      -o ${MD}/${SD}/cat/${BASE}_photprep.cat \
                      -t STDTAB -k A_WORLD A_WCS B_WORLD B_WCS \
                                   THETA_J2000 THETAWCS
    done
  else # if [ "${CATS}" != "" ]
    theli_warn "No catalogues for Chip ${CHIP} available!" "${!#}"
  fi
done

# clean up and bye
rm  ${TEMPDIR}/*_$$

theli_end "${!#}"
exit 0;
