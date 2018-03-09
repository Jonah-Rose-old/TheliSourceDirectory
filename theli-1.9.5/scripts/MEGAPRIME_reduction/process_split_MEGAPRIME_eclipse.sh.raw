#!/bin/bash

# Splits Elixir processed CFHT Megaprime Fits extension images into
# the 36 chips. Uses the eclipse utilities and also updates the image
# headers.  Assumes that ONLY unsplit images are in the directory.
#
# If available, the script uses Skyprobe information to decide whether
# an image was obtained under photometric conditions and to transfer
# Elixir/QSO quality information to the image headers.  This is the
# case for CFHTLS data for instance.

# History comments:
#
# 20.07.06:
# I add Elixir/QSO quality information to the
# image headers.
#
# 25.06.2007:
# I merged the script with process_split_CFHTLS_eclipse.sh.
# That script split MEGAPRIME images and, in addition, 
# transfered skyprobe information to the image headers.
# This task is done by this script if the optional,
# third command line argument is given. This argument
# points to an LDAC catalogue with skyprobe information
# on the images.
#
# 02.10.2008:
# We estimate a saturation level for Elixir processed images;
# we note that Elixir images carry a saturation level 
# header keyword. However, this turned out to be useless 
# (much too high values in many cases).
#
# 12.10.2008:
# I make sure that the SATLEVEL keyword is alwasys a float with
# two digits accuracy.
#
# 10.11.2008:
# I corrected the printing of SATLEVEL (two digits) at one more place
# in the script; if the estimated SATLEVEL is smaller than 50000 (for
# short exposures for instance) I set ot to 60000 by default.
#
# 25.11.2008:
# I add the civil date of observation (keyword DATE-OBS) to the
# headers. The date is given as 'local' time at the telescope, NOT
# as UT!
#
# 19.12.2008:
# - The program caldate is now called via a variable, not directly
# - For the u*-band filter we apply a zeropoint correction of 0.2
#   from 19/05/2006 - 28/09/2006
#
# 05.02.2009:
# We make sure that the seeing which is written to headers always
# is a float with two digits; we had problem with a seeing of exactly
# 1 arcsec which was stored as integer.
#
# 04.01.2010:
# The command line arguments now have to be given with a keyword.
# I introduced the possibility to give a second, optional argument.
# This did not permit anymore handling of command line arguments via
# simple positional variables. The new optional argument  is a file
# with image names, zeropoints and a 'is photometric' flag. It overrides
# photometric information in a photometric catalogue. This feature
# is probably only useful with the special Elixir data at Terapix! 
#
# 26.03.2010:
# I make sure that the saturation files represent integer numbers (though
# given as float header keywords) as the input images come in Integer 
# format.
# 
# 28.09.2010:
# I made the script more efficient. Header entries are now treated within
# the mefsplit call whereever possible. The 'mefsplit' program was modified
# for that purpose. Also the estimation of saturation levels and their
# transfer to image headers was optimised.
#
# 28.11.2010:
# - I introduced an internal parallelisation to this script (simultaneous
#   splitting of files). Usefult e.g. on a RAMDISK
# - The header keyword GAIN is now transfered from the original images
#   to the splitted data.
#
# 16.12.2010:
# I made the construction of unique temporary filenames in subshells more
# robust. The old method of using BASHPID seems not to have worked properly!
#
# 19.04.2011:
# Possibility to use 'zeropoint files' provided by Jean-Charles to process
# 'non-official' Elixir processed images (used also for T07). The format
# of his files is 'imagename ZP ext.'; one image per line.
#
# 19.11.2012:
# adaption of the mefsplit call: The HEADTRANFER option now needs to
# specify the name of the output keyword.
#
# 21.03.2013:
# The Median Julian date (MJD-OBS) is now transfered to splitted
# images. It is needed for variability studies based on single frames.

# -md : main directory
# -sd : science directory
# -cat: LDAC catalog with SkyProbe information (OPTIONAL)
# -yannickzp: ASCII file with zeropoints from Yannick;
#             this option probably only makes sense at Terapix!!
# -jcczp: ASCII file with zeropoints and extinction coefficients;
#         from Jean-Charles. this option probably only makes sense
#         with T07 data at UBC!!

. ./MEGAPRIME.ini
. ./bash_functions.include

# On some systems we might consider to split data in parallel;
# e.g. on a RAMDISK. As the systems where this is possible without
# different mefsplit processes blocking each other we keep this
# functionality VERY internal!
NPROC=6

#
# parse command line arguments
#
MD=
SD=
PHOTCAT=
ZPFILE=
JCC=0
GoOn=0
while [ $GoOn = 0 ]
do
  case $1 in
  -md)
      MD=${2}
      shift 2
      ;;
  -sd)       
      SD=${2}
      shift 2
      ;; 
  -cat)       
      PHOTCAT=${2}
      shift 2
      ;; 
  -yannickzp)       
      ZPFILE=${2}
      shift 2
      ;; 
  -jcczp)       
      ZPFILE=${2}
      JCC=1
      shift 2
      ;; 
   *)
      if [ $# -ge 1 ]; then
        echo "Unknown command line option: ${1}"
        exit 1	   
      else
        GoOn=1
      fi
      ;;
  esac
done

# sanity checks:
if [ -z ${MD} ] || [ -z ${SD} ]; then
  echo "You need to provide at least main directory and science directory!"
  exit 1;
fi

if [ ! -z ${PHOTCAT} ] && [ ! -f ${PHOTCAT} ]; then
  echo "Error reading ${PHOTCAT}" 
fi

if [ ! -z ${ZPFILE} ] && [ ! -f ${ZPFILE} ]; then
  echo "Error reading ${ZPFILE}" 
fi

# Create image list: we assume that ONLY unsplit
# images are in the directory.

if [ ! -z ${PHOTCAT} ]; then
  ${P_LDACTOASC} -b -i ${PHOTCAT} -t OBJECTS -s -k IMAGENAME PHOTOMETRIC \
                 > phot_$$.asc
  ${P_LDACTOASC} -b -i ${PHOTCAT} -t OBJECTS -s -k IMAGENAME QUALITY \
                 > qual_$$.asc

  # in cases where we 'only' have one seeing value for the whole mosaic,
  # we must fake the SEEINGCENTRE and SEEINGCORNER values with that,
  # one value:
  ${P_LDACTESTEXIST} -i ${PHOTCAT} -t OBJECTS -k SEEINGCENTRE

  if [ $? -eq 0 ]; then
    ${P_LDACTOASC} -b -i ${PHOTCAT} -t OBJECTS -s -k IMAGENAME SEEINGCENTRE \
                   > seeingcentre_$$.asc
    ${P_LDACTOASC} -b -i ${PHOTCAT} -t OBJECTS -s -k IMAGENAME SEEINGCORNER \
                   > seeingcorner_$$.asc
  else
    ${P_LDACTOASC} -b -i ${PHOTCAT} -t OBJECTS -s -k IMAGENAME SEEING \
                   > seeingcentre_$$.asc
    ${P_LDACTOASC} -b -i ${PHOTCAT} -t OBJECTS -s -k IMAGENAME SEEING \
                   > seeingcorner_$$.asc
  fi
fi

DIR=`pwd`

# split files 'in parallel':
FILES=`${P_FIND} /${MD}/${SD}/ORIGINALS/ -name \*p.fits`

i=0
while [ ${i} -lt ${NPROC} ]
do
  NFIELDS[${i}]=0
  i=$(( $i + 1 ))
done

i=1
for FIELD in ${FILES}
do
  PROC=$(( $i % ${NPROC} ))
  FIELDS[${PROC}]="${FIELDS[${PROC}]} ${FIELD}"
  NFIELDS[${PROC}]=$(( ${NFIELDS[${PROC}]} + 1 ))
  i=$(( $i + 1 ))
done

cd /${MD}/${SD}/ORIGINALS

IPARA=0
while [ ${IPARA} -lt ${NPROC} ]
do
  JPARA=$(( ${IPARA} + 1 )) 
  echo -e "Starting Job ${JPARA}. It has ${NFIELDS[${IPARA}]} files to process!\n"
  (
    for FILE in ${FIELDS[${IPARA}]}
    do
      BASEELIXIR=`basename ${FILE} p.fits`
    
      # If a catalogue with Skyprobe information is given the image
      # under consideration HAS to be listed in order to be processed!!
      ISPRESENT=""
    
      if [ ! -z ${PHOTCAT} ]; then
        ISPRESENT=`${P_GAWK} '($1 == "'${BASEELIXIR}'") {print $1}' \
                   ${DIR}/phot_$$.asc`
      fi
    
      if [ "${ISPRESENT}_A" != "_A" ] || [ -z ${PHOTCAT} ]; then
        FILTNAM=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} -d FILTER  | cut -f 2`
        # The 'awk' command for OBJECT ensures that also OBJECT names with
        # a space (such as 'Abell 1835') can be treated. Spaces within the 
        # OBJECTS keyword are substituted with underscores. Spaces would be 
        # deleted by the following 'mefsplit' program call.
        OBJECT=`${P_DFITS} -x 1  ${FILE} | ${P_FITSORT} OBJECT  |\
                ${P_GAWK} '($1 != "FILE") {gsub($1, ""); string = ""; 
                      for(i = 1; i < NF; i++) {string = string $i "_"}; 
                      string = string $NF; print string}'`
        RA=`${P_DFITS} -x 1  ${FILE} | ${P_FITSORT} -d RA_DEG  | cut -f 2`
        DEC=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} -d DEC_DEG | cut -f 2`
        LST=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} -d LST-OBS | cut -f 2 |\
             ${P_GAWK} -F: '{print $1 * 3600 + $2 * 60 + $3}'`
        MJD=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} -d MJD-OBS  | cut -f 2`
        EXPTIME=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} -d EXPTIME | cut -f 2`
        AIRMASS=`${P_AIRMASS} -t ${LST} -e ${EXPTIME} \
                              -r ${RA} -d ${DEC} -l ${OBSLAT}`
        GABODSID=`${P_NIGHTID} -t ${REFERENCETIME} -d 31/12/1998 -m ${MJD} |\
                  ${P_GAWK} ' ($1 ~ /Days/) {print $6}' |\
                  ${P_GAWK} 'BEGIN{ FS = "."} {print $1}'`
    
        # we recalculate the civil observation date from the GABODSID because
        # we want the local time at the telescope, NOT UT.
        DATEOBS=`${P_CALDATE} -d 31/12/1998 -i ${GABODSID} | cut -d ' ' -f 3`
    
        # RUN information
        RUN=`${P_DFITS} ${FILE} | ${P_FITSORT} -d QRUNID  | cut -f 2`
      
        # Photometric information; all images will be marked as photometric!!
        PHOTINFO=1 # by default we have photometric information; this may not
                   # be the case for special filters
        PHOT_C=`${P_DFITS} ${FILE} | ${P_FITSORT} -d PHOT_C  | cut -f 2`
        PHOT_K=`${P_DFITS} ${FILE} | ${P_FITSORT} -d PHOT_K  | cut -f 2`

        # check if we have special zero point files from Yannick or
        # Jean-Charles:
        if [ ! -z ${ZPFILE} ]; then
          grep ${BASEELIXIR} ${ZPFILE} > /dev/null
    
          if [ $? -eq 0 ]; then
            PHOT_C_NEW=`grep ${BASEELIXIR} ${ZPFILE} | awk 'END {print $2}'`
            PHOT_C=`awk 'BEGIN {if ('${PHOT_C_NEW}' > 90) { 
                                  print '${PHOT_C}' 
                                } else {
                                  print '${PHOT_C_NEW}'}
                                }'`
            # Jean-Charles data also have new extinction coefficients!
            if [ ${JCC} -eq 1 ]; then
              PHOT_K=`grep ${BASEELIXIR} ${ZPFILE} | awk 'END {print $3}'`
            fi
          fi        
        fi

        if [ "${PHOT_C}" != "KEY_N/A" ] && [ "${PHOT_K}" != "KEY_N/A" ]; then 
          # our magnitude zeropoint (at airmass 0) is PHOT_C-PHOT_K
          ZP=`${P_GAWK} 'BEGIN {print '${PHOT_C}' - ('${PHOT_K}')}'`
    
          # If the filter is 'u.MP9301' and the observation between 
          # 19.05.2006 and 28.09.2006 we subtract 0.2 from the CFHT zeropoint!
          # we do this only if zeropoints were not given directly by Yannick
          # or Jean-Charles:
          if [ -z ${ZPFILE} ]; then
            if [ "${FILTNAM}" = "u.MP9301" ]; then
              if [ ${GABODSID} -ge 2695 ] && [ ${GABODSID} -le 2827 ]; then
                ZP=`${P_GAWK} 'BEGIN {print '${ZP}' - 0.2}'`
              fi    
            fi
          fi
        else
          PHOTINFO=0  
        fi
    
        # collect some more info for image header keywords:
        HEADERSTRING="-HEADER RUN ${RUN} CFHT_Run_ID"
        PHOTOMETRIC=1 # default for all images!
    
        # get Skyprobe info if available:    
        if [ ! -z ${PHOTCAT} ]; then
          # get Elixir/QSO data and save it together with
          # photometric infomration,to the headers:
          PHOTOMETRIC=`${P_GAWK} '($1 == "'${BASEELIXIR}'") {print $2}' \
                       ${DIR}/phot_$$.asc`
       
          if [ "${PHOTOMETRIC}_A" = "_A" ]; then
            PHOTOMETRIC=0
          fi
    
          ELQUAL=`${P_GAWK} '($1 == "'${BASEELIXIR}'") {print int($2)}' \
                  ${DIR}/qual_$$.asc`
    
          if [ "${ELQUAL}_A" = "_A" ]; then
            ELQUAL=-1
          fi
       
          ELSEECEN=`${P_GAWK} '($1 == "'${BASEELIXIR}'") {printf("%.2f", $2)}' \
                    ${DIR}/seeingcentre_$$.asc`
    
          if [ "${ELSEECEN}_A" = "_A" ]; then
            ELSEECEN=-1.0
          fi
       
          ELSEECOR=`${P_GAWK} '($1 == "'${BASEELIXIR}'") {printf("%.2f", $2)}' \
                    ${DIR}/seeingcorner_$$.asc`
    
          if [ "${ELSEECOR}_A" = "_A" ]; then
            ELSEECOR=-1.0
          fi
    
          HEADERSTRING="${HEADERSTRING} -HEADER ELQUAL ${ELQUAL} Elixir_Quality_Flag"
          HEADERSTRING="${HEADERSTRING} -HEADER ELSEECEN ${ELSEECEN} Elixir_Seeing_in_Image_Center"
          HEADERSTRING="${HEADERSTRING} -HEADER ELSEECOR ${ELSEECOR} Elixir_Seeing_in_Image_Corners"
        fi
    
        if [ ${PHOTINFO} -eq 0 ]; then 
          # if no photometric information is available we set the
          # zeropoints to '-1.0' and the extinction to '0.0'  
          ZP=-1.0
          PHOT_K=0.0
          PHOTOMETRIC=0
        fi
    
        HEADERSTRING="${HEADERSTRING} -HEADER ZP ${ZP} Magnitude_Zero_Point"
        HEADERSTRING="${HEADERSTRING} -HEADER COEFF ${PHOT_K} Extinction_Coefficient"
    
        for i in `seq 1 3`
        do
          HEADERSTRING="${HEADERSTRING} -HEADER ZP${i} ${ZP} Magnitude_Zero_Point"  
          HEADERSTRING="${HEADERSTRING} -HEADER COEFF${i} ${PHOT_K} Extinction_Coefficient"  
        done
        HEADERSTRING="${HEADERSTRING} -HEADER ZPCHOICE ${PHOTOMETRIC} Is_Image_Photometric_(1=Yes)"  
        HEADERSTRING="${HEADERSTRING} -HEADER BADCCD 0 Is_CCD_Bad_(1=Yes)"  
        HEADERSTRING="${HEADERSTRING} -HEADER DATE-OBS ${DATEOBS} obs._date_(YYYY-MM-DD;_local_time_at_tel.)"  
    
        ${P_FITSSPLIT_ECL} -CRPIX1 \
        "${REFPIXX[1]},${REFPIXX[2]},${REFPIXX[3]},${REFPIXX[4]},${REFPIXX[5]},${REFPIXX[6]},${REFPIXX[7]},${REFPIXX[8]},${REFPIXX[9]},${REFPIXX[10]},${REFPIXX[11]},${REFPIXX[12]},${REFPIXX[13]},${REFPIXX[14]},${REFPIXX[15]},${REFPIXX[16]},${REFPIXX[17]},${REFPIXX[18]},${REFPIXX[19]},${REFPIXX[20]},${REFPIXX[21]},${REFPIXX[22]},${REFPIXX[23]},${REFPIXX[24]},${REFPIXX[25]},${REFPIXX[26]},${REFPIXX[27]},${REFPIXX[28]},${REFPIXX[29]},${REFPIXX[30]},${REFPIXX[31]},${REFPIXX[32]},${REFPIXX[33]},${REFPIXX[34]},${REFPIXX[35]},${REFPIXX[36]}"\
                           -CRPIX2 \
        "${REFPIXY[1]},${REFPIXY[2]},${REFPIXY[3]},${REFPIXY[4]},${REFPIXY[5]},${REFPIXY[6]},${REFPIXY[7]},${REFPIXY[8]},${REFPIXY[9]},${REFPIXY[10]},${REFPIXY[11]},${REFPIXY[12]},${REFPIXY[13]},${REFPIXY[14]},${REFPIXY[15]},${REFPIXY[16]},${REFPIXY[17]},${REFPIXY[18]},${REFPIXY[19]},${REFPIXY[20]},${REFPIXY[21]},${REFPIXY[22]},${REFPIXY[23]},${REFPIXY[24]},${REFPIXY[25]},${REFPIXY[26]},${REFPIXY[27]},${REFPIXY[28]},${REFPIXY[29]},${REFPIXY[30]},${REFPIXY[31]},${REFPIXY[32]},${REFPIXY[33]},${REFPIXY[34]},${REFPIXY[35]},${REFPIXY[36]}"\
                           -CD11 \
        "${CD11[1]},${CD11[2]},${CD11[3]},${CD11[4]},${CD11[5]},${CD11[6]},${CD11[7]},${CD11[8]},${CD11[9]},${CD11[10]},${CD11[11]},${CD11[12]},${CD11[13]},${CD11[14]},${CD11[15]},${CD11[16]},${CD11[17]},${CD11[18]},${CD11[19]},${CD11[20]},${CD11[21]},${CD11[22]},${CD11[23]},${CD11[24]},${CD11[25]},${CD11[26]},${CD11[27]},${CD11[28]},${CD11[29]},${CD11[30]},${CD11[31]},${CD11[32]},${CD11[33]},${CD11[34]},${CD11[35]},${CD11[36]}"\
                           -CD22 \
        "${CD22[1]},${CD22[2]},${CD22[3]},${CD22[4]},${CD22[5]},${CD22[6]},${CD22[7]},${CD22[8]},${CD22[9]},${CD22[10]},${CD22[11]},${CD22[12]},${CD22[13]},${CD22[14]},${CD22[15]},${CD22[16]},${CD22[17]},${CD22[18]},${CD22[19]},${CD22[20]},${CD22[21]},${CD22[22]},${CD22[23]},${CD22[24]},${CD22[25]},${CD22[26]},${CD22[27]},${CD22[28]},${CD22[29]},${CD22[30]},${CD22[31]},${CD22[32]},${CD22[33]},${CD22[34]},${CD22[35]},${CD22[36]}"\
                           -CD12 \
        "${CD12[1]},${CD12[2]},${CD12[3]},${CD12[4]},${CD12[5]},${CD12[6]},${CD12[7]},${CD12[8]},${CD12[9]},${CD12[10]},${CD12[11]},${CD12[12]},${CD12[13]},${CD12[14]},${CD12[15]},${CD12[16]},${CD12[17]},${CD12[18]},${CD12[19]},${CD12[20]},${CD12[21]},${CD12[22]},${CD12[23]},${CD12[24]},${CD12[25]},${CD12[26]},${CD12[27]},${CD12[28]},${CD12[29]},${CD12[30]},${CD12[31]},${CD12[32]},${CD12[33]},${CD12[34]},${CD12[35]},${CD12[36]}"\
                           -CD21 \
        "${CD21[1]},${CD21[2]},${CD21[3]},${CD21[4]},${CD21[5]},${CD21[6]},${CD21[7]},${CD21[8]},${CD21[9]},${CD21[10]},${CD21[11]},${CD21[12]},${CD21[13]},${CD21[14]},${CD21[15]},${CD21[16]},${CD21[17]},${CD21[18]},${CD21[19]},${CD21[20]},${CD21[21]},${CD21[22]},${CD21[23]},${CD21[24]},${CD21[25]},${CD21[26]},${CD21[27]},${CD21[28]},${CD21[29]},${CD21[30]},${CD21[31]},${CD21[32]},${CD21[33]},${CD21[34]},${CD21[35]},${CD21[36]}"\
                            -M11 \
       "${M11[1]},${M11[2]},${M11[3]},${M11[4]},${M11[5]},${M11[6]},${M11[7]},${M11[8]},${M11[9]},${M11[10]},${M11[11]},${M11[12]},${M11[13]},${M11[14]},${M11[15]},${M11[16]},${M11[17]},${M11[18]},${M11[19]},${M11[20]},${M11[21]},${M11[22]},${M11[23]},${M11[24]},${M11[25]},${M11[26]},${M11[27]},${M11[28]},${M11[29]},${M11[30]},${M11[31]},${M11[32]},${M11[33]},${M11[34]},${M11[35]},${M11[36]}"\
                            -M22 \
       "${M22[1]},${M22[2]},${M22[3]},${M22[4]},${M22[5]},${M22[6]},${M22[7]},${M22[8]},${M22[9]},${M22[10]},${M22[11]},${M22[12]},${M22[13]},${M22[14]},${M22[15]},${M22[16]},${M22[17]},${M22[18]},${M22[19]},${M22[20]},${M22[21]},${M22[22]},${M22[23]},${M22[24]},${M22[25]},${M22[26]},${M22[27]},${M22[28]},${M22[29]},${M22[30]},${M22[31]},${M22[32]},${M22[33]},${M22[34]},${M22[35]},${M22[36]}"\
                            -M12 \
       "${M12[1]},${M12[2]},${M12[3]},${M12[4]},${M12[5]},${M12[6]},${M12[7]},${M12[8]},${M12[9]},${M12[10]},${M12[11]},${M12[12]},${M12[13]},${M12[14]},${M12[15]},${M12[16]},${M12[17]},${M12[18]},${M12[19]},${M12[20]},${M12[21]},${M12[22]},${M12[23]},${M12[24]},${M12[25]},${M12[26]},${M12[27]},${M12[28]},${M12[29]},${M12[30]},${M12[31]},${M12[32]},${M12[33]},${M12[34]},${M12[35]},${M12[36]}"\
                            -M21 \
       "${M21[1]},${M21[2]},${M21[3]},${M21[4]},${M21[5]},${M21[6]},${M21[7]},${M21[8]},${M21[9]},${M21[10]},${M21[11]},${M21[12]},${M21[13]},${M21[14]},${M21[15]},${M21[16]},${M21[17]},${M21[18]},${M21[19]},${M21[20]},${M21[21]},${M21[22]},${M21[23]},${M21[24]},${M21[25]},${M21[26]},${M21[27]},${M21[28]},${M21[29]},${M21[30]},${M21[31]},${M21[32]},${M21[33]},${M21[34]},${M21[35]},${M21[36]}"\
                            -IMAGEID \
       "${IMAGEID[1]},${IMAGEID[2]},${IMAGEID[3]},${IMAGEID[4]},${IMAGEID[5]},${IMAGEID[6]},${IMAGEID[7]},${IMAGEID[8]},${IMAGEID[9]},${IMAGEID[10]},${IMAGEID[11]},${IMAGEID[12]},${IMAGEID[13]},${IMAGEID[14]},${IMAGEID[15]},${IMAGEID[16]},${IMAGEID[17]},${IMAGEID[18]},${IMAGEID[19]},${IMAGEID[20]},${IMAGEID[21]},${IMAGEID[22]},${IMAGEID[23]},${IMAGEID[24]},${IMAGEID[25]},${IMAGEID[26]},${IMAGEID[27]},${IMAGEID[28]},${IMAGEID[29]},${IMAGEID[30]},${IMAGEID[31]},${IMAGEID[32]},${IMAGEID[33]},${IMAGEID[34]},${IMAGEID[35]},${IMAGEID[36]}"\
                            -CRVAL1 ${RA} -CRVAL2 ${DEC}\
                            -EXPTIME ${EXPTIME}\
                            -AIRMASS ${AIRMASS}\
                            -GABODSID ${GABODSID}\
                            -FILTER ${FILTNAM}  \
                            -OBJECT "${OBJECT}"  \
                            ${HEADERSTRING} \
                            -HEADTRANSFER GAIN GAIN\
                            -HEADTRANSFER MJD-OBS MJD-OBS\
                            -OUTPUT_DIR .. \
                            ${FILE}
       
          
        # update image headers of unsplit images with the saturation level:
        # the following is optimised for speed, not for readability!
    
        # saturation level:
        # The saturation level given in Elixir processed images is often
        # too high. Instead of using the value in the headers 
        # we estimate the mode of high pixel values and subtract 5000.
        # This turned out empirically to be a reasonable estimate. 
        BASE=`basename ${FILE} .fits`
        # as we are in a subshell '$$' still contains the process ID from 
        # the calling script. We therefore include the parallel channel
        # to construct unique names for temporary files.
        IDENTIFIER=job_${JPARA}_$$
        ${P_IMSTATS} -t 50000 100000 ../${BASE}_*fits > \
            satlevels.txt_${IDENTIFIER}
        ${P_GAWK} '$1 !~ /#/ { 
                image = $1;
                if ($2 > 50000) {       
                  satlev = sprintf(" \"SATLEVEL= %20.2f / saturation level\"", 
                                   int($2) - 5000.);
                } else {
                  satlev = sprintf(" \"SATLEVEL= %20.2f / saturation level\"", 
                                   60000.00);
                }
                system("'${P_REPLACEKEY}' " image satlev " DUMMY1");
                }' satlevels.txt_${IDENTIFIER}
        rm satlevels.txt_${IDENTIFIER}
    
        # if the Elixir quality flag is '-1' or larger than 2 we move
        # the image immedialtely to a 'BADELQUAL' subdirectory.
        if [ ! -z ${PHOTCAT} ]; then
          if [ ${ELQUAL} -eq -1 ] || [ ${ELQUAL} -gt 2 ]; then
            if [ ! -d ../BADELQUAL ]; then
      	      mkdir ../BADELQUAL  
            fi
      
            i=1
            while [ ${i} -le 36 ]
            do
      	      mv ../${BASE}_${i}.fits ../BADELQUAL
      	      i=$(( $i + 1 ))
            done
          fi
        fi
      else
        if [ ! -z ${PHOTCAT} ]; then
          echo "No Skyprobe info for ${FILE}."
          echo "No splitting performed!!"  
        fi
      fi
    done
  ) 2> ${DIR}/process_split_job_${IPARA}.log &
  IPARA=$(( ${IPARA} + 1 ))
done

wait

cd ${DIR}

# clean up!!
test -f phot_$$.asc         && rm phot_$$.asc
test -f qual_$$.asc         && rm qual_$$.asc
test -f seeingcentre_$$.asc && rm seeingcentre_$$.asc
test -f seeingcorner_$$.asc && rm seeingcorner_$$.asc
