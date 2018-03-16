#!/usr/bin/bash -u

# ----------------------------------------------------------------
# File Name:           check_science_PSF_plot.sh
# Author:              Thomas Erben (terben@astro.uni-bonn.de)
# Last modified on:    18.02.2013
# Description:         creates PSF anisotropy checkplots for
#                      'regularly shaped' multi-chip cameras
# ----------------------------------------------------------------

# Script history information:
# 19.09.2007:
# script written
#
# 16.07.2009:
# The catalogue subdirectory now has to be provided
# as command line argument
#
# 29.07.2009:
# I fixed various bugs connected with the changes from 
# 16.07.2009
#
# 18.04.2012:
# The script now works in parallel producing plots simultaneously
# for several images
#
# 04.01.2013:
# I made the script more robust to non-existing files
#
# 18.02.2013:
# sm cannot handle too long strings/filenames. I therefore shortened
# significantly all filenames that are handed to sm.

. ./${INSTRUMENT:?}.ini
. ./bash_functions.include
theli_start "$*"

# define THELI_DEBUG because of the '-u' script flag
# (the use of undefined variables will be treated as errors!)
# THELI_DEBUG is used in the cleanTmpFiles function.
THELI_DEBUG=${THELI_DEBUG:-""}

# function definitions:
function printUsage 
{
    echo "SCRIPT NAME:"
    echo "    check_science_PSF_plot.sh"
    echo ""
    echo "ARGUMENTS:"
    echo "    1. main_dir."
    echo "    2. science (run)_dir."
    echo "    3. image_extension"
    echo "    4. catalogue directory"
    echo "    5. Super Mongo Postscript device (OPTIONAL)"
    echo "       Default is 'postportfile'"
    echo ""
}

function cleanTmpFiles
{
  if [ -z ${THELI_DEBUG} ]; then
    echo "Cleaning temporary files for script $0"
    test -f ${TEMPDIR}/psfimages_plot_$$ && rm ${TEMPDIR}/psfimages_plot_$$
    find ${TEMPDIR} -name PSF_allellip.asc\*_$$ | xargs -r rm
    find ${TEMPDIR} -name \*PSFplot.asc\*_$$    | xargs -r rm
    find ${TEMPDIR} -name PSFplot.sm\*_$$       | xargs -r rm
  else
    echo "Variable THELI_DEBUG set! No cleaning of temp. files in script $0"    
  fi
}

# Handling of program interruption by CRTL-C
trap "echo 'Script $0 interrupted!! Cleaning up and exiting!'; \
      cleanTmpFiles; exit 1" INT

# check whether we have the external 'sm' program at all:
if [ -z ${P_SM} ]
then
  echo "You need the external Super Mongo (sm) program to"
  echo "use this script! At least the necessary variable in"
  echo "your progs.ini seems not to be set! Exiting!!"
  exit 1;
fi

# check validity of command line arguments:
if [ $# -lt 4 ] || [ $# -gt 5 ] ; then
  printUsage
  exit 1
fi

# Here the main script begins

# set SM postscript device
SMPSDEVICE="postportfile"

if [ $# -eq 5 ]; then
  SMPSDEVICE=$5
fi

# decompose the CHIPGEOMETRY variable:
NCHIPSX=`echo ${CHIPGEOMETRY} | ${P_GAWK} '{print $1}'`
NCHIPSY=`echo ${CHIPGEOMETRY} | ${P_GAWK} '{print $2}'`
NAXIS1=`echo ${CHIPGEOMETRY} | ${P_GAWK} '{print $3}'`
NAXIS2=`echo ${CHIPGEOMETRY} | ${P_GAWK} '{print $4}'`

# If something is wrong with the CHIPGEOMETRY var.
# probably NAXIS2 is not set!
test -z ${NAXIS2} && \
  { theli_error "Malformed CHIPGEOMETRY variable! Exiting !!"; exit 1; }

# immediately quit if we do not yet have a catalogue directory!
if [ ! -d /$1/$2/$4/ ]; then
  theli_error "Directory /$1/$2/$4/ doesnot exist! Exiting!"
  exit 1;
fi

if [ ! -d "/$1/$2/$4/PSFcheck" ]; then
  mkdir /$1/$2/$4/PSFcheck || \
  { theli_error "creation of dir. /$1/$2/$4/PSFcheck failed!"; exit 1; } 
fi

# first get a list of all images for which to create PSF plots:
${P_FIND} /$1/$2/ -name \*_1$3.fits > ${TEMPDIR}/psfimages_plot_$$

# do we need to do something at all?
if [ -s ${TEMPDIR}/psfimages_plot_$$ ]; then
  # do the job in parallel:
  NIMA=`wc -l ${TEMPDIR}/psfimages_plot_$$ | ${P_GAWK} '{print $1}'`
  
  if [ ${NPARA} -gt ${NIMA} ]; then
    NPARA=${NIMA}
  fi
  
  PROC=0
  while [ ${PROC} -lt ${NPARA} ]
  do
    NFIELDS[${PROC}]=0
    FIELDS[${PROC}]=""
    PROC=$(( ${PROC} + 1 ))
  done
  
  i=1
  while read FIELD
  do
    PROC=$(( $i % ${NPARA} ))
    FIELDS[${PROC}]="${FIELDS[${PROC}]} ${FIELD}"
    NFIELDS[${PROC}]=$(( ${NFIELDS[${PROC}]} + 1 ))
    i=$(( $i + 1 ))
  done < ${TEMPDIR}/psfimages_plot_$$
  
  PROC=0
  while [ ${PROC} -lt ${NPARA} ]
  do
    CURRPROC=$(( ${PROC} + 1 )) 
    echo -e "Starting Job ${CURRPROC}. It has ${NFIELDS[${PROC}]} files to process!\n" 
    {
      for FIELD in ${FIELDS[${PROC}]}
      do
        BASE=`basename ${FIELD} _1$3.fits`
      
        NCATS=0  # the number of available KSB catalogues for
                 # the current image
      
        # The file for global ellipticity statistics:
        test -f  ${TEMPDIR}/PSF_allellip.asc_${CURRPROC}_$$ && \
              rm ${TEMPDIR}/PSF_allellip.asc_${CURRPROC}_$$ 
      
        i=1
        while [ "${i}" -le ${NCHIPS} ]
        do
          if [ -f /$1/$2/$4/${BASE}_${i}$3_ksb.cat2 ]
          then
            ${P_LDACTOASC} -i /$1/$2/$4/${BASE}_${i}$3_ksb.cat2 \
                -b -t OBJECTS\
                -k Xpos Ypos e1 e2 > \
                 ${TEMPDIR}/${i}_PSFplot.asc_${CURRPROC}_$$
      
            # gather all ellipticity values for global statistics:
            ${P_GAWK} '{print $3, $4}' \
                      ${TEMPDIR}/${i}_PSFplot.asc_${CURRPROC}_$$ >> \
                      ${TEMPDIR}/PSF_allellip.asc_${CURRPROC}_$$ 
      
            NCATS=$(( ${NCATS} + 1 ))
          fi
          i=$(( $i + 1 ))
        done
      
        if [ ${NCATS} -gt 0 ]
        then
          # Ok, create a sm script file for the current image
          # First the sickplots for each chip:
          {
            echo 'macro read "'${SMMACROS}'/shearfield.sm"'
            echo 'device "'${SMPSDEVICE}' '${TEMPDIR}'/tmp.ps_'${CURRPROC}'_'$$'"'
            echo "relocate (17600 32500)"
            echo "putlabel 5 '${BASE}'"
            echo "limits 0 ${NAXIS1} 0 ${NAXIS2}"      
            echo "lweight 1.5"
            echo "expand 0.5"
      
            i=1
            j=1
            k=1
            while [ ${j} -le ${NCHIPSY} ]
            do
              while [ ${k} -le ${NCHIPSX} ]
              do
                echo "window -${NCHIPSX} -${NCHIPSY} ${k} ${j}"
                
      	        # The box command specifying whether and where
                # to put lablels depends on where we are in the
                # mosaic:
                if [ ${j} -eq 1 ] && [ ${k} -eq 1 ]; then
                  echo "box"
                else
                  if [ ${j} -eq 1 ]; then
                    echo "box 1 0"
                  else
                    if [ ${k} -eq 1 ]; then
                      echo "box 0 2"
                    else
                      echo "box 0 0"
                    fi
                  fi
                fi
      
                if [ -f ${TEMPDIR}/${i}_PSFplot.asc_${CURRPROC}_$$ ]
                then
                  echo 'shearfield "'${TEMPDIR}'/'${i}'_PSFplot.asc_'${CURRPROC}'_'$$'" 2000'
                fi
      
                i=$(( $i + 1 ))
                k=$(( $k + 1 ))
              done
              j=$(( $j + 1 )) 
              k=1
            done
      
            # global statistics on the PSF ellipticity
            # distribution:
            echo 'data "'${TEMPDIR}'/PSF_allellip.asc_'${CURRPROC}'_'$$'"'
            echo "read { e1 1 e2 2 }"
            echo "stats e1 m1 s1 k1"
            echo "stats e2 m2 s2 k2"
            echo "relocate ( 2000 1500 )"
            echo "define s (sprintf('<e1>: %.2f +/- ', \$m1))"
            echo 'label $s'
            echo "define s (sprintf('%.2f;', \$s1))"
            echo 'label $s'
            echo "relocate ( 8000 1500 ) "
            echo "define s (sprintf('<e2>: %.2f +/- ', \$m2))"
            echo 'label $s'
            echo "define s (sprintf('%.2f;', \$s2))"
            echo 'label $s'
            echo "set eabs=sqrt(e1*e1+e2*e2)"
            echo "sort {eabs}"
            echo "relocate ( 15000 1500 )"
            echo "define s (sprintf('|e| min = %.2f;', \$(eabs[0])))"
            echo 'label $s'
            echo "relocate ( 20000 1500 )"
            echo "define n (dimen(eabs)-1)"
            echo "define s (sprintf('|e| max = %.2f', \$(eabs[\$n])))"
            echo 'label $s'
      
            echo "sort {e1}"
            echo "relocate ( 2000 1000 )"
            echo "define s (sprintf('e1 min = %.2f;', \$(e1[0])))"
            echo 'label $s'
            echo "relocate ( 8000 1000 )"
            echo "define n (dimen(e1)-1)"
            echo "define s (sprintf('e1 max = %.2f', \$(e1[\$n])))"
            echo 'label $s'
      
            echo "sort {e2}"
            echo "relocate ( 15000 1000 )"
            echo "define s (sprintf('e2 min = %.2f;', \$(e2[0])))"
            echo 'label $s'
            echo "relocate ( 20000 1000 )"
            echo "define n (dimen(e2)-1)"
            echo "define s (sprintf('e2 max = %.2f', \$(e2[\$n])))"
            echo 'label $s'
      
            echo "expand 1.0"
            echo "lweight 1.0"
            echo "hardcopy"      
          } > ${TEMPDIR}/PSFplot.sm_${CURRPROC}_$$
          cat ${TEMPDIR}/PSFplot.sm_${CURRPROC}_$$ | ${P_SM}

          mv ${TEMPDIR}/tmp.ps_${CURRPROC}_$$ /$1/$2/$4/PSFcheck/${BASE}.ps
        else
          theli_warn "Not a single chip KSB catalogue for exposure ${BASE}!"
        fi        
      done  
    } &
    PROC=$(( ${PROC} + 1 ))
  done
  
  # wait for all processes to end:
  wait;
else # if [ -s .... ]
  theli_error "No files to process!"
  cleanTmpFiles
  exit 1;
fi

# clean temporary files and bye
cleanTmpFiles

theli_end
exit 0;

