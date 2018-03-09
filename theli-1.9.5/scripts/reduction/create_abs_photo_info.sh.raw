#!/bin/bash -u

# script to estimate absolute photometric calibration on a night
# basis. It substitutes the old create_photoinfo.sh script.

# $1 main dir
# $2 standard dir
# $3 science dir
# $4 image extension 
# $5 filter
# $6 filter name in standardstar catalog
# $7 color index (e.g. VmI)
# $8 Extinction for fits with constant extinction
# $9 color for fit with constant color term
# $10 NIGHTCALIB or RUNCALIB (determination of photometric
#                             quantities on night- or runbasis)
# $11 AUTOMATIC or INTERACTIVE
# $12 Lower acceptable zeropoint in AUTOMATIC mode
#     (provide a dummy if you use INTERACTIVE mode)
# $13 Upper acceptable zeropoint in AUTOMATIC mode
#     (provide a dummy if you use INTERACTIVE mode)
# $14 should headers of science image be updated with photometric
#     inforation (1=Yes; 0=No)
# $15 lower limit for magnitudes to be considered
#     (standard star catalog) (OPTIONAL argument;
#      set to 0.0 by default, i.e. we use all objects)


# 03.11.2004 Filter measured magnitudes for nonsense
#
# 25.11.2004:
# we adapted the script to the new output of fitsort
# that no longer gives the file including the whole
# but only the filename.
#
# 07.12.2004:
# I corrected a bug in the writing of ZP and COEFF keyowrds (FITS
# standard issues)
#
# 10.12.2004:
# Now passes the new label argument to phot_abs.py
#
# 26.02.2005:
# I introduced a new argument to account for a different 'filter' name
# and the name for that filter in the standard star catalog.
#
# 01.03.2005:
# I introduced temporary output files for files created by
# photo_abs.py. photo_abs.py uses pgplot that cannot handle too long
# file names.
#
# 11.03.2005:
# We now write all photometric solutions to the header.  In case the
# night was estimated non-photometric the ZPCHOICE keyword 0
#
# 13.03.2005:
# In the header updates the next free dummy keyword is now searched
# automatically. This part is written more modular now.
#
# 17.03.2005:
# If the night is marked as non-photometric, ZPCHOICE is set to 0 now.
#
# 19.03.2005:
# I corrected a syntax error in an if statement (that probably
# was introduced in one of the last commits)
#
# 14.08.2005:
# The call of the UNIX 'sort' program is now done via a variable
# 'P_SORT'.
#
# 05.12.2005
# Chips whose NOTUSE or NOTPROCESS flag is set are not considered in
# the determination of absolute photometric zeropoints.
#
# 23.01.2006:
# I introduced a new command line argument giving the minimum
# magnitude for standard stars to be considered. This argument is
# optional. This change should help to better reject bright objects
# with saturated features.
#
# 08.02.2008:
# - I removed the 'fifo' concept to ask for user input
# - I introduced an automatic calibration mode. If it is chosen
#   over the interactive one (10th command line argument) the 
#   script checks whether the zeropoint of the second solution
#   (fit of zeropoint and colour term) is within predefined values
#   (11th and 12th command line argument). If this condition is 
#   fulfilled this solution is taken.
#
# 18.05.2009:
# Modification of the script to reflect changes in the photo_abs.py
# module. There, we now use matplotlib instead of ppgplot.
#
# 03.02.2010:
# some code cleaning
#
# 28.03.2010:
# BUG FIX: The condition for accepting a zeropoint in the AUTOMATIC 
# mode was wrong.
#
# 26.11.2012:
# - calibrations can now be done on night basis (this is how the script
#   worked up to now), or on a run basis. In the latter case all standard
#   stars from all present nights are used to determine one single set
#   of photometric quantities for all science images.
# - the photo_abs.py script no longer requires a 'night' argument.
#
# 30.12.2012:
# In RUNCALIBRATION mode we now use the dummy night 0 to indicate this.
# Before we used the string 'RUN' but it was problematic to have a string
# or a numeric value as indicator for the calibration mode.
#
# 31.12.2012:
# I made the script more robust against non-existing files
#
# 18.01.2013:
# I renamed the variable CHOICE to the more meaningful ZPCHOICE.
#
# 17.06.2013:
# Slight adaptions to the script because now catalogues, merged from
# individual exposures, not from individual chips, enter this script.
#
# 27.08.2013:
# the temporary file 'photo_res' did not have the process number included
# (potential problems if this script was called multiple times with the
# same 'TEMPDIR' setting).
#
# 07.09.2013:
# I substituded 'direct listings' of directories with the 'find' command
# (command too long issues with very big directories).
#
# 23.03.2014:
# I introduced a new command line options indicating whether science
# images should be updated with magnitude zeropoint information.
# For many images this updates takes some time and we do not always need it
# (e.g. 'science' image processing for z-band in KIDS).

. ./${INSTRUMENT:?}.ini
. ./bash_functions.include
theli_start "$*"

# give meaningful names to some command line arguments:
SCIENCEUPDATE=${14}
if [ $# -eq 15 ]; then
  MINMAG=${15}
else
  MINMAG=0.0
fi

# list the exposure catalogues that were merged with the
# a standardstar catalogue:
CATS=`${P_FIND} /$1/$2/cat/ -name \*all_photprep_merg.cat`

# check whether something needs to be done at all:
if [ "${CATS}" != "" ]; then
  ${P_LDACPASTE} -i ${CATS} -t PSSC\
                 -o ${TEMPDIR}/tmp_exp_$$.cat
  
  ${P_LDACFILTER} -i ${TEMPDIR}/tmp_exp_$$.cat -t PSSC\
      -c "(((${6}mag < 99)AND($7 < 99))AND(Mag < 0))AND(${6}mag > ${MINMAG});"\
      -o /$1/$2/cat/allexp_merg.cat
  
  ${P_LDACFILTER} -i /$1/$2/cat/allexp_merg.cat -t PSSC\
                  -c "($7 > -10) AND ($7 < 10);"\
                  -o /$1/$2/cat/allexp_tmp.cat
  
  LABEL=`echo $7 | sed 's/m/-/g'`
  
  # Get a list of all nights:
  NIGHTS=`${P_LDACTOASC} -i /$1/$2/cat/allexp_tmp.cat  -t PSSC\
      -b -k GABODSID | ${P_SORT} | uniq | awk '{printf("%s ", $1)}'`
  
  # In case of run calibration we do not loop through each night
  if [ "${10}" = "RUNCALIB" ]; then
    CALIBNIGHT="${NIGHTS}" # variable that is  written as CALIBNIGHT
                           # to image header
    NIGHTS="0" # we use the dummy night 0 to indicate RUN calibration.
               # The value of this variable is used below for the filename
               # of the result file.
  fi
   
  for NIGHT in ${NIGHTS}
  do
    if [ "${10}" = "RUNCALIB" ]; then
      echo
      echo "       ---==== Calibrating the whole RUN ====---"
      echo
    else
      echo
      echo "       ---==== Calibrating night ${NIGHT} ====---"
      echo
    fi
  
    if [ "${10}" = "RUNCALIB" ]; then
      cp /$1/$2/cat/allexp_tmp.cat /$1/$2/cat/night_${NIGHT}.cat
    else
      ${P_LDACFILTER} -i /$1/$2/cat/allexp_tmp.cat  -t PSSC\
    	-o /$1/$2/cat/night_${NIGHT}.cat -c "(GABODSID=${NIGHT});"
    fi
  
    ${P_LDACTOASC} -i /$1/$2/cat/night_${NIGHT}.cat  -t PSSC\
  	-b -k Mag ${6}mag ${7} AIRMASS > ${TEMPDIR}/night_$5_${NIGHT}_$$.asc
  
  
    if [ "${11}" == "AUTOMATIC" ]; then
      BATCH="--batch"
    fi
  
    ${S_PHOTOABS} --input=${TEMPDIR}/night_$5_${NIGHT}_$$.asc \
        --output=${TEMPDIR}/photo_res_$$ --extinction="$8" \
        --color="$9" --label=${LABEL} ${BATCH}
  
    # in 'photo_abs.py' we 'abuse' the return value to give the interactive
    # choice back to the shell:
    ZPCHOICE=$?

    test -d /$1/$2/calib || mkdir /$1/$2/calib

    mv ${TEMPDIR}/photo_res_$$.asc /$1/$2/calib/night_${NIGHT}_$5_result.asc
    mv ${TEMPDIR}/photo_res_$$.png /$1/$2/calib/night_${NIGHT}_$5_result.png

    ZP[0]=-1.0
    COEFF[0]=-1.0
  
    i=1
    while read -a SOL
    do
      ZP[$i]=${SOL[0]}
      COEFF[$i]=${SOL[1]}
      COL[$i]=${SOL[2]}
      i=$(($i + 1))
    done < /$1/$2/calib/night_${NIGHT}_$5_result.asc
  
    if [ "${11}" = "AUTOMATIC" ]; then
      ZPCHOICE=`${P_GAWK} 'BEGIN{if ('${ZP[2]}' > '${12}' &&
                                   '${ZP[2]}' < '${13}') {
                                 print 2;
                               } else {
                                 print 0;
                               }}'`
    fi

    if [ ${SCIENCEUPDATE} -eq 1 ]; then
      echo
      echo "Updating image headers ..."
      echo
    
      if [ "${10}" = "RUNCALIB" ]; then
        ${P_FIND} /$1/$3/ -maxdepth 1 -name \*$4.fits |\
          ${P_GAWK} -F/ '{print $NF}' > \
          ${TEMPDIR}/night_${NIGHT}_img_list_$$.asc
      else
        ${P_FIND} /$1/$3/ -maxdepth 1 -name \*$4.fits |\
            xargs ${P_DFITS} | ${P_FITSORT} -d GABODSID | \
            ${P_GAWK} '{if ($2 == '${NIGHT}') print $1}' > \
            ${TEMPDIR}/night_${NIGHT}_img_list_$$.asc
    
        CALIBNIGHT="${NIGHT}" # variable that is  written as CALIBNIGHT
                              # to image header; for RUNCALIB it was set
                              # above.
      fi
    
      while read IMG
      do
        echo $IMG
        # Write the choice to the header
        value ${ZPCHOICE}
        writekey /$1/$3/${IMG} ZPCHOICE "${VALUE}" REPLACE
    
        value ${ZP[$ZPCHOICE]}
        writekey /$1/$3/${IMG} ZP "${VALUE} / Magnitude zeropoint" REPLACE
    
        value ${COEFF[$ZPCHOICE]}
        writekey /$1/$3/${IMG} COEFF "${VALUE} / Extinction coefficient" \
            REPLACE
    
        i=1
        while [ "${i}" -le "3" ]
        do
          value ${ZP[$i]}
          writekey /$1/$3/${IMG} ZP${i} "${VALUE} / Magnitude zeropoint" \
              REPLACE
    
          value ${COEFF[$i]}
          writekey /$1/$3/${IMG} COEFF${i} \
             "${VALUE} / Extinction coefficient" REPLACE
    
          i=$(( $i + 1 ))
        done
        
        # put information on the calibration mode (night or run) into
        # the header:
        if [ "${10}" = "RUNCALIB" ]; then
          value "'RUN'"
        else
          value "'NIGHT'"
        fi  
    
        writekey /$1/$3/${IMG} PHOTCALM \
            "${VALUE} / photometric calibration mode (RUN or NIGHT)" REPLACE
        writecommhis /$1/$3/${IMG} COMMENT \
             "The following nights were used for photometric parameters:"
        writecommhis /$1/$3/${IMG} COMMENT "${CALIBNIGHT}"
      done < ${TEMPDIR}/night_${NIGHT}_img_list_$$.asc
    fi # if [ ${SCIENCEUPDATE} -eq 1 ]; then
  done
else
  theli_error "No catalogues available for absolute phot. calibration!"
  exit 1;
fi

# clean up and bye:
rm ${TEMPDIR}/*_$$*

theli_end
exit 0;
