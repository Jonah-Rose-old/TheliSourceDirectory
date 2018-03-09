#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits MEGAPRIME@CFHT images

# $1: main directory
# $2: science directory

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash.include


# # rename the images to the value of a FITS key (if requested)
# 
if [ "${V_PRE_RENAME_CHECKED}" = "1" ]; then
    
    cd /$1/$2
    
    p_ls > ${TEMPDIR}/fitskey_$$
    
    cat ${TEMPDIR}/fitskey_$$ |\
   {
	while read FILE
	do
	    newname=`get_key ${FILE} ${V_RENAME_FITSKEY}`
	    if [ "${newname}" != "KEY_N/A" ] && [ "${newname}" != "" ]; then
		mv ${FILE} ${newname}
	    else
		echo ERROR: Could not find ${V_RENAME_FITSKEY} in the FITS header
	    fi
	done
    }
fi



FILES=`ls -1l $1/$2/ | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

cd /$1/$2

mkdir ORIGINALS

for FILE in ${FILES}
do
   SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
   BASE=`basename $FILE ${SUFFIX}`
   mv ${FILE} ${BASE}.fits
   FILE=${BASE}.fits
 
   RA=`get_key ${FILE} "RA_DEG"`
   if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
       RA=0.001
   fi
 
   DEC=`get_key ${FILE} "DEC_DEG"`
   if [ "${DEC}" = "" ] || [ "${DEC}" = "KEY_N/A" ]; then
       DEC=0.0
   fi
 
   EXPTIME=`get_key ${FILE} "EXPTIME"`
   if [ "${EXPTIME}" = "" ] || [ "${EXPTIME}" = "KEY_N/A" ]; then
       EXPTIME=1.0
   fi
 
   OBJECT=`get_key ${FILE} "OBJECT"`
   if [ "${OBJECT}" = "" ] || [ "${OBJECT}" = "KEY_N/A" ]; then
       OBJECT=UNKNOWN
   fi
 
   EQUINOX=`get_key ${FILE} "EQUINOX" | sed 's/[^0-9.]//g'`
   if [ "${EQUINOX}" = "" ] || [ "${EQUINOX}" = "KEY_N/A" ]; then
       EQUINOX=2000.0
   fi
 
   MJD=`get_key ${FILE} "MJD-OBS"`
   if [ "${MJD}" = "" ] || [ "${MJD}" = "KEY_N/A" ]; then
       MJD=52000.0
   fi
 
   FILTER=`get_key ${FILE} "FILTER" | ${P_CLEANSTRING}`
   if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
       FILTER=UNKNOWN
   fi
 
   LST=`get_key ${FILE} "LST-OBS"`
   if [ "${LST}" = "" ] || [ "${LST}" = "KEY_N/A" ]; then
       LST=0.0
   fi
 
   AIRMASS=`get_key ${FILE} "AIRMASS"`
   if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
       AIRMASS=1.0
   fi

   PHOTOMETRIC=1
 
   GABODSID=`${P_NIGHTID} -t 22:00:00 -d 31/12/1998 -m ${MJD} |\
       ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

   DATEOBS=`${P_CALDATE} -d 31/12/1998 -i ${GABODSID} | cut -d ' ' -f 3`

   RUN=`${P_DFITS} ${FILE} | ${P_FITSORT} -d QRUNID  | cut -f 2`

   PHOTINFO=1

   PHOT_C=`${P_DFITS} ${FILE} | ${P_FITSORT} -d PHOT_C  | cut -f 2`
   PHOT_K=`${P_DFITS} ${FILE} | ${P_FITSORT} -d PHOT_K  | cut -f 2`

   if [ "${PHOT_C}" != "KEY_N/A" ] && [ "${PHOT_K}" != "KEY_N/A" ]; then 
       # our magnitude zeropoint (at airmass 0) is PHOT_C-PHOT_K
       ZP=`${P_GAWK} 'BEGIN {print '${PHOT_C}' - ('${PHOT_K}')}'`
       
       # If the filter is 'u.MP9301' and the observation between 
       # 19.05.2006 and 28.09.2006 we subtract 0.2 from the CFHT zeropoint!
       if [ "${FILTER}" = "u.MP9301" ]; then
           if [ ${GABODSID} -ge 2695 ] && [ ${GABODSID} -le 2827 ]; then
               ZP=`${P_GAWK} 'BEGIN {print '${ZP}' - 0.2}'`
           fi    
       fi
   else
       PHOTINFO=0  
   fi

   if [ ${PHOTINFO} -eq 0 ]; then 
       # if no photometric information is available we set the
       # zeropoints to '-1.0' and the extinction to '0.0'  
       ZP=-1.0
       PHOT_K=0.0
       PHOTOMETRIC=0
   fi


   HEADERSTRING="-HEADER RUN ${RUN} CFHT_Run_ID"
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

   k=1
   cd11=""
   cd12=""
   cd21=""
   cd22=""
   cpx1=""
   cpx2=""
 
   while [ "${k}" -le "${NCHIPS}" ]
   do
       if [ "${NCHIPS}" = "1" ]; then
           ext=0
       else
           ext=${k}
       fi
       cdmatrix=`${P_DFITS} -x ${ext} ${FILE} | ${P_FITSORT} CD1_1 CD1_2 CD2_1 CD2_2 | ${P_GAWK} '($1!="FILE") {print $2, $3, $4, $5}'`
       refpixel=`${P_DFITS} -x ${ext} ${FILE} | ${P_FITSORT} CRPIX1 CRPIX2 | ${P_GAWK} '($1!="FILE") {print $2, $3}'`
       c11=`echo ${cdmatrix} | ${P_GAWK} '{print $1}'`
       c12=`echo ${cdmatrix} | ${P_GAWK} '{print $2}'`
       c21=`echo ${cdmatrix} | ${P_GAWK} '{print $3}'`
       c22=`echo ${cdmatrix} | ${P_GAWK} '{print $4}'`
       cr1=`echo ${refpixel} | ${P_GAWK} '{print $1}'`
       cr2=`echo ${refpixel} | ${P_GAWK} '{print $2}'`
       if [ "${k}" = "1" ]; then
  	   cd11="${c11}"
  	   cd12="${c12}"
  	   cd21="${c21}"
  	   cd22="${c22}"
  	   cpx1="${cr1}"
  	   cpx2="${cr2}"
       else
   	   cd11="${cd11},${c11}"
  	   cd12="${cd12},${c12}"
  	   cd21="${cd21},${c21}"
  	   cd22="${cd22},${c22}"
  	   cpx1="${cpx1},${cr1}"
  	   cpx2="${cpx2},${cr2}"
       fi
       k=$(( $k + 1 ))
   done
   echo "cpx1 = "${cpx1}
   echo "cpx2 = "${cpx2}
   echo "CD11 = "${cd11}
   echo "CD12 = "${cd12} 
   echo "CD21 = "${cd21}
   echo "CD22 = "${cd22}
   ${P_FITSSPLIT_ECL} \
       -CRPIX1 "${cpx1}" \
       -CRPIX2 "${cpx2}" \
       -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
       -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
       -CD11 "${cd11}" \
       -CD12 "${cd12}" \
       -CD21 "${cd21}" \
       -CD22 "${cd22}" \
       -M11 "-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1" \
       -M12 "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0" \
       -M21 "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0" \
       -M22 "-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1" \
       -IMAGEID \
	   "${IMAGEID[1]},${IMAGEID[2]},${IMAGEID[3]},${IMAGEID[4]},${IMAGEID[5]},${IMAGEID[6]},${IMAGEID[7]},${IMAGEID[8]},${IMAGEID[9]},${IMAGEID[10]},${IMAGEID[11]},${IMAGEID[12]},${IMAGEID[13]},${IMAGEID[14]},${IMAGEID[15]},${IMAGEID[16]},${IMAGEID[17]},${IMAGEID[18]},${IMAGEID[19]},${IMAGEID[20]},${IMAGEID[21]},${IMAGEID[22]},${IMAGEID[23]},${IMAGEID[24]},${IMAGEID[25]},${IMAGEID[26]},${IMAGEID[27]},${IMAGEID[28]},${IMAGEID[29]},${IMAGEID[30]},${IMAGEID[31]},${IMAGEID[32]},${IMAGEID[33]},${IMAGEID[34]},${IMAGEID[35]},${IMAGEID[36]}"\
       ${HEADERSTRING} \
       ${FILE}

   BASE=`basename ${FILE} .fits`
   ${P_IMSTATS} -t 50000 100000 ${BASE}_*fits > \
       satlevels.txt_$$
   
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
                }' satlevels.txt_$$

   rm satlevels.txt_$$

   k=1
   while [ "${k}" -le "${NCHIPS}" ]
   do
       tmp=`get_key ${FILE} "DATE-OBS"`
       ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${tmp}" DUMMY1
       ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY2
       ${P_REPLACEKEY} ${BASE}_${k}.fits "COLOR1  = 0.0" DUMMY3
       ${P_REPLACEKEY} ${BASE}_${k}.fits "COLOR2  = 0.0" DUMMY4
       ${P_REPLACEKEY} ${BASE}_${k}.fits "COLOR3  = 0.0" DUMMY5
       ${P_REPLACEKEY} ${BASE}_${k}.fits "COLOR4  = 0.0" DUMMY6
       k=$(( $k + 1 ))
   done
   mv ${FILE} ORIGINALS/${BASE}${SUFFIX}  
done

if [ ! -d /$1/$2/SPLIT_IMAGES ]; then
    mkdir /$1/$2/SPLIT_IMAGES
fi

MAXX=$(( ${CUTX[${CHIP}]} + ${SIZEX[${CHIP}]} - 1 ))
MAXY=$(( ${CUTY[${CHIP}]} + ${SIZEY[${CHIP}]} - 1 ))

k=1

while [ "${k}" -le "${NCHIPS}" ]
do

MAXX=$(( ${CUTX[${k}]} + ${SIZEX[${k}]} - 1 ))
MAXY=$(( ${CUTY[${k}]} + ${SIZEY[${k}]} - 1 ))

${P_IMRED_ECL:?} `ls /$1/$2/*_${k}.fits` \
 	    -c ${CONF}/imred.conf \
 	    -MAXIMAGES ${NFRAMES} \
 	    -OVERSCAN N \
            -BIAS N\
 	    -COMBINE N \
 	    -OUTPUT Y \
 	    -OUTPUT_DIR /$1/$2/ \
 	    -OUTPUT_SUFFIX OFC.fits \
 	    -TRIM Y \
 	    -TRIM_REGION ${CUTX[${k}]},${MAXX},${CUTY[${k}]},${MAXY}

mv /$1/$2/*_${k}.fits /$1/$2/SPLIT_IMAGES
k=$(( $k + 1 ))
done
