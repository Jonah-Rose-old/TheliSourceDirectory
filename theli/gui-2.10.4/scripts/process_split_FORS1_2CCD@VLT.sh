#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# prepares FORS1_2CCD@VLT images

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


FILES=`ls -1l $1/$2/ | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

cd /$1/$2

mkdir ORIGINALS

#rename the images to a unique filename
for FILE in ${FILES}
do
  uid=`get_key ${FILE} "HIERARCH ESO DET EXP NO"`
  chip_id=`get_key ${FILE} "ORIGFILE" | ${P_GAWK} 'BEGIN{FS="CHIP"} {print $2}'`
  # chip_id already contains a .fits suffix
  mv ${FILE} FORS1.${uid}_${chip_id}
done

FILES=`ls -1l $1/$2/ | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

for FILE in ${FILES}
do
  uid=`get_key ${FILE} "HIERARCH ESO DET EXP NO"`
  chip_id=`get_key ${FILE} "ORIGFILE" | ${P_GAWK} 'BEGIN{FS="CHIP"} {print $2}'`

  arcfile=`get_key ${FILE} "ARCFILE"`

  RA=`get_key ${FILE} "CRVAL1"`
  if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
      RA=`get_key ${FILE} "RA"`
      if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
	  RA=0.0
      fi
  fi

  DEC=`get_key ${FILE} "CRVAL2"`
  if [ "${DEC}" = "" ] || [ "${DEC}" = "KEY_N/A" ]; then
      DEC=`get_key ${FILE} "DEC"`
      if [ "${DEC}" = "" ] || [ "${DEC}" = "KEY_N/A" ]; then
	  DEC=0.0
      fi
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

  FILTER=`get_key ${FILE} "FILTER1" | ${P_CLEANSTRING}`
  if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
      FILTER=`get_key ${FILE} "HIERARCH ESO INS FILT1 NAME" | ${P_CLEANSTRING}`
      if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
	  FILTER=UNKNOWN
      fi
  fi

  LST=`get_key ${FILE} "LST"`
  if [ "${LST}" = "" ] || [ "${LST}" = "KEY_N/A" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
      AIRMASS=`get_key ${FILE} "HIERARCH ESO TEL AIRM START"`
      if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
	  AIRMASS=1.0
      fi
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  imageid=2

  BASE=`basename ${FILE} .fits`
  if [ "${chip_id}" = "1.fits" ]; then
      dateobs=`get_key ${FILE} "DATE-OBS"`
      imageid=1
  fi

  cd11=`get_key ${FILE} "CD1_1"`
  cd12=`get_key ${FILE} "CD1_2"`
  cd21=`get_key ${FILE} "CD2_1"`
  cd22=`get_key ${FILE} "CD2_2"`
  crpix1=`get_key ${FILE} "CRPIX1"`
  crpix2=`get_key ${FILE} "CRPIX2"`

  if [ "${cd11}" = "" ]; then
      cd11=`get_key ${FILE} "CDELT1"`
  fi
  if [ "${cd22}" = "" ]; then
      cd22=`get_key ${FILE} "CDELT2"`
  fi
  if [ "${cd12}" = "" ]; then
      cd12=0.0
  fi
  if [ "${cd21}" = "" ]; then
      cd21=0.0
  fi

  cd11=`echo ${cd11} | ${P_GAWK} '{ if ($1>=1.) print "'-${PIXSCALE}'"; else print $1}'`
  cd22=`echo ${cd22} | ${P_GAWK} '{ if ($1>=1.) print "'-${PIXSCALE}'"; else print $1}'`

  ${P_FITSSPLIT_ECL} \
      -CRPIX1 "${crpix1}" \
      -CRPIX2 "${crpix2}" \
      -CD11 "${cd11}" \
      -CD12 "${cd12}" \
      -CD21 "${cd21}" \
      -CD22 "${cd22}" \
      -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
      -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
      -M11 "1" \
      -M12 "0" \
      -M21 "0" \
      -M22 "1" \
      ${FILE}

  mv ${FILE} ORIGINALS/${arcfile}

  tmp=`get_key ${FILE} "DATE-OBS"`
  ${P_REPLACEKEY} ${BASE}_1.fits "DATE-OBS= ${tmp}" DUMMY1
  ${P_REPLACEKEY} ${BASE}_1.fits "IMAGEID = ${imageid}" IMAGEID
  ${P_REPLACEKEY} ${BASE}_1.fits "MJD-OBS = ${MJD}" DUMMY2
  ${P_REPLACEKEY} ${BASE}_2.fits "MJD-OBS = ${MJD}" DUMMY2

  mv ${BASE}_1.fits FORS1.${dateobs}_${chip_id}

done
