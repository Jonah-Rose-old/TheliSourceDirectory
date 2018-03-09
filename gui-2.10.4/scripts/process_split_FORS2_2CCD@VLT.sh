BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# prepares FORS2_2CCD@VLT images

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
  mv ${FILE} FORS2.${uid}_${chip_id}
done

FILES=`ls -1l $1/$2/ | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

for FILE in ${FILES}
do
  uid=`get_key ${FILE} "HIERARCH ESO DET EXP NO"`
  chip_id=`get_key ${FILE} "ORIGFILE" | ${P_GAWK} 'BEGIN{FS="CHIP"} {print $2}'`

  arcfile=`get_key ${FILE} "ARCFILE"`

  RA=`get_key ${FILE} "RA"`
  if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
      RA=`get_key ${FILE} "RA"`
      if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
	  RA=0.0
      fi
  fi

  DEC=`get_key ${FILE} "DEC"`
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

  ${P_CDELT2CD} -i ${FILE} -s ${PIXSCALE} > ${TEMPDIR}/cdelt2cd_$$
  cd11=`${P_GAWK} '{print $1}' ${TEMPDIR}/cdelt2cd_$$`
  cd12=`${P_GAWK} '{print $2}' ${TEMPDIR}/cdelt2cd_$$`
  cd21=`${P_GAWK} '{print $3}' ${TEMPDIR}/cdelt2cd_$$`
  cd22=`${P_GAWK} '{print $4}' ${TEMPDIR}/cdelt2cd_$$`
  crpix1=`get_key ${FILE} "CRPIX1"`
  crpix2=`get_key ${FILE} "CRPIX2"`

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

  ${P_REPLACEKEY} ${BASE}_1.fits "DATE-OBS= ${dateobs}" DUMMY1
  ${P_REPLACEKEY} ${BASE}_1.fits "IMAGEID = ${imageid}" IMAGEID
  ${P_REPLACEKEY} ${BASE}_1.fits "MJD-OBS = ${MJD}" DUMMY2

  mv ${BASE}_1.fits FORS2.${dateobs}_${chip_id}

done
