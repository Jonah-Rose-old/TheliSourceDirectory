#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits IMACS_F4@LCO images

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


# rename the images to the value of a FITS key (if requested)

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



for NUMBER in 1 2 3 4

do

  FILES=`ls -1l $1/$2/*c${NUMBER}* | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

  cd /$1/$2

  mkdir ORIGINALS

  for FILE in ${FILES}
    do
      SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
      BASE=`basename $FILE c${NUMBER}${SUFFIX}`
      mv ${FILE} ${BASE}tmp.fits
      FILE=${BASE}tmp.fits

      RA=`get_key ${FILE} "RA-D"`
      if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
         RA=0.0
      fi

      DEC=`get_key ${FILE} "DEC-D"`
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

      MJD=52000.0
      if [ "${MJD}" = "" ] || [ "${MJD}" = "KEY_N/A" ]; then
	  MJD=52000.0
      fi

      FILTER=`get_key ${FILE} "FILTER" | ${P_CLEANSTRING}`
      if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
	  FILTER=UNKNOWN
      fi

      LST=`get_key ${FILE} "ST"`
      if [ "${LST}" = "" ] || [ "${LST}" = "KEY_N/A" ]; then
	  LST=0.0
      fi
      
      AIRMASS=`get_key ${FILE} "AIRMASS"`
      if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
	  AIRMASS=1.0
      fi

      GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
        ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`
      
      ${P_FITSSPLIT_ECL} \
	  -CRPIX1 "${REFPIXX[$NUMBER]}" \
	  -CRPIX2 "${REFPIXY[$NUMBER]}" \
	  -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
	  -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
	  -CD11 "${PIXSCX}" \
	  -CD12 "0" \
	  -CD21 "0" \
	  -CD22 "${PIXSCY}" \
	  -M11 "0" \
	  -M12 "1" \
	  -M21 "-1" \
	  -M22 "0" \
	  ${FILE}
      
      mv ${BASE}tmp_1.fits ${BASE}_${NUMBER}.fits
      
      k=${NUMBER}
      CD11=`echo ${PIXSCX} | ${P_GAWK} '{if ($1>0) print $1*(-1.); else print $1}'`
      CD22=`echo ${PIXSCY} | ${P_GAWK} '{if ($1<0) print $1*(-1.); else print $1}'`
      ${P_REPLACEKEY} ${BASE}_${k}.fits "CD1_1   = ${CD11}" CD1_1
      ${P_REPLACEKEY} ${BASE}_${k}.fits "CD2_2   = ${CD22}" CD2_2
      ${P_REPLACEKEY} ${BASE}_${k}.fits "CD1_2   = 0.0" CD1_2
      ${P_REPLACEKEY} ${BASE}_${k}.fits "CD2_1   = 0.0" CD2_1
      tmp=`get_key ${FILE} "UT-DATE"`
      ${P_REPLACEKEY} ${BASE}_${k}.fits "UT-DATE = ${tmp}" DUMMY2
      tmp=`get_key ${FILE} "UT-TIME"`
      ${P_REPLACEKEY} ${BASE}_${k}.fits "UT-TIME = ${tmp}" DUMMY3
      ${P_REPLACEKEY} ${BASE}_${k}.fits "IMAGEID = ${NUMBER}" IMAGEID
      
      mv ${FILE} ORIGINALS/${BASE}c${NUMBER}${SUFFIX}
      
  done
done

for NUMBER in 5 6 7 8

do

  FILES=`ls -1l $1/$2/*c${NUMBER}* | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

  cd /$1/$2

  mkdir ORIGINALS

  for FILE in ${FILES}
    do
      SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
      BASE=`basename $FILE c${NUMBER}${SUFFIX}`
      mv ${FILE} ${BASE}tmp.fits
      FILE=${BASE}tmp.fits

      RA=`get_key ${FILE} "RA-D"`
      if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
         RA=0.0
      fi

      DEC=`get_key ${FILE} "DEC-D"`
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

      MJD=52000.0
      if [ "${MJD}" = "" ] || [ "${MJD}" = "KEY_N/A" ]; then
	  MJD=52000.0
      fi

      FILTER=`get_key ${FILE} "FILTER" | ${P_CLEANSTRING}`
      if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
	  FILTER=UNKNOWN
      fi

      LST=`get_key ${FILE} "ST"`
      if [ "${LST}" = "" ] || [ "${LST}" = "KEY_N/A" ]; then
	  LST=0.0
      fi
      
      AIRMASS=`get_key ${FILE} "AIRMASS"`
            if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
	  AIRMASS=1.0
      fi
      
      GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
        ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

      ${P_FITSSPLIT_ECL} \
	  -CRPIX1 "${REFPIXX[$NUMBER]}" \
	  -CRPIX2 "${REFPIXY[$NUMBER]}" \
	  -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
	  -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
	  -CD11 "${PIXSCX}" \
	  -CD12 "0" \
	  -CD21 "0" \
	  -CD22 "${PIXSCY}" \
	  -M11 "0" \
	  -M12 "-1" \
	  -M21 "1" \
	  -M22 "0" \
	  ${FILE}
      
      mv ${BASE}tmp_1.fits ${BASE}_${NUMBER}.fits
      
      k=${NUMBER}
      CD11=`echo ${PIXSCX} | ${P_GAWK} '{if ($1>0) print $1*(-1.); else print $1}'`
      CD22=`echo ${PIXSCY} | ${P_GAWK} '{if ($1<0) print $1*(-1.); else print $1}'`
      ${P_REPLACEKEY} ${BASE}_${k}.fits "CD1_1   = ${CD11}" CD1_1
      ${P_REPLACEKEY} ${BASE}_${k}.fits "CD2_2   = ${CD22}" CD2_2
      ${P_REPLACEKEY} ${BASE}_${k}.fits "CD1_2   = 0.0" CD1_2
      ${P_REPLACEKEY} ${BASE}_${k}.fits "CD2_1   = 0.0" CD2_1
      tmp=`get_key ${FILE} "UT-DATE"`
      ${P_REPLACEKEY} ${BASE}_${k}.fits "UT-DATE = ${tmp}" DUMMY2
      tmp=`get_key ${FILE} "UT-TIME"`
      ${P_REPLACEKEY} ${BASE}_${k}.fits "UT-TIME = ${tmp}" DUMMY3
      ${P_REPLACEKEY} ${BASE}_${k}.fits "IMAGEID = ${NUMBER}" IMAGEID
      
      mv ${FILE} ORIGINALS/${BASE}c${NUMBER}${SUFFIX}

  done
done
