BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits PISCES@KPNO images

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
  mkdir ORIGINALS


  p_ls > ${TEMPDIR}/fitskey_$$

  cat ${TEMPDIR}/fitskey_$$ |\
  {
    while read FILE
    do
      newname=`get_key ${FILE} ${V_RENAME_FITSKEY}`
      if [ "${newname}" != "" ]; then
         mv ${FILE} ${newname}
      else
         echo ERROR: Could not find ${V_RENAME_FITSKEY} in the FITS header
         mv ${FILE} ORIGINALS/
      fi
    done
  }
fi


cd /$1/$2

FILES=`p_ls`

mkdir ORIGINALS

for FILE in ${FILES}
do
  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  BASE=`basename $FILE ${SUFFIX}`
  mv ${FILE} ${BASE}.fits
  FILE=${BASE}.fits

  RA=`get_key ${FILE} "RA"`
  if [ "${RA}" != "" ]; then
      RA=`${P_HMSTODECIMAL} ${RA}`
  fi
  if [ "${RA}" = "" ]; then
      RA=0.001
  fi

  DEC=`get_key ${FILE} "DEC"`
  if [ "${DEC}" != "" ]; then
      DEC=`${P_DMSTODECIMAL} ${DEC}`
  fi
  if [ "${DEC}" = "" ]; then
      DEC=0.0
  fi

  DIT=`get_key ${FILE} "EXPTIME"`
  NDIT=`get_key ${FILE} "COADDS"`
  EXPTIME=`echo ${DIT} ${NDIT} | ${P_GAWK} '{print $1*$2}'`
  if [ "${EXPTIME}" = "" ]; then
      EXPTIME=1.0
  fi

  OBJECT=`get_key ${FILE} "OBJECT"`
  if [ "${OBJECT}" = "" ]; then
      OBJECT=UNKNOWN
  fi

  EQUINOX=`get_key ${FILE} "EPOCH" | sed 's/[^0-9.]//g'`
  if [ "${EQUINOX}" = "" ]; then
      EQUINOX=2000.0
  fi

  MJD=`get_key ${FILE} "MJD"`
  if [ "${MJD}" = "" ]; then
      MJD=52000.0
  fi

  FILTER=`get_key ${FILE} "FILTER" | ${P_CLEANSTRING}`
  if [ "${FILTER}" = "" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "SD_TIME"`
  if [ "${LST}" = "" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 19:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  POSANGLE=`get_key ${FILE} "POSANGLE"`
  pscale=`get_key ${FILE} "SCALE"`
  cd11=`${P_GETCDMATRIX} -s ${SCALE} -p ${pscale} -n -f | ${P_GAWK} '{print $1}'`
  cd12=`${P_GETCDMATRIX} -s ${SCALE} -p ${pscale} -n -f | ${P_GAWK} '{print $2}'`
  cd21=`${P_GETCDMATRIX} -s ${SCALE} -p ${pscale} -n -f | ${P_GAWK} '{print $3}'`
  cd22=`${P_GETCDMATRIX} -s ${SCALE} -p ${pscale} -n -f | ${P_GAWK} '{print $4}'`

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${REFPIXX[1]}" \
     -CRPIX2 "${REFPIXY[1]}" \
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

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    tmp1=`get_key ${FILE} "DATE-OBS"`
    tmp2=`get_key ${FILE} "TIME-OBS"`
    dateobs=${tmp1}T${tmp2}
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${dateobs}" DUMMY1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY2
    k=$(( $k + 1 ))
  done

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}

done
