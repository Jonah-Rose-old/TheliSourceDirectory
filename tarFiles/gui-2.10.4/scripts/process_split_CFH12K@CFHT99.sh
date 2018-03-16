BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits CFH12K@CFHT99 images

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

for FILE in ${FILES}
do
  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  BASE=`basename $FILE ${SUFFIX}`
  mv ${FILE} ${BASE}.fits
  FILE=${BASE}.fits

  RA=`get_key ${FILE} "RA"`
  RA=`${P_HMSTODECIMAL} ${RA}`
  if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
      RA=0.001
  fi

  DEC=`get_key ${FILE} "DEC"`
  DEC=`${P_DMSTODECIMAL} ${DEC}`
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
  LST=`${P_HMSTODECIMAL} ${LST}`
  if [ "${LST}" = "" ] || [ "${LST}" = "KEY_N/A" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 22:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${REFPIXX[1]},${REFPIXX[2]},${REFPIXX[3]},${REFPIXX[4]},${REFPIXX[5]},${REFPIXX[6]},${REFPIXX[7]},${REFPIXX[8]},${REFPIXX[9]},${REFPIXX[10]},${REFPIXX[11]},${REFPIXX[12]}" \
     -CRPIX2 "${REFPIXY[1]},${REFPIXY[2]},${REFPIXY[3]},${REFPIXY[4]},${REFPIXY[5]},${REFPIXY[6]},${REFPIXY[7]},${REFPIXY[8]},${REFPIXY[9]},${REFPIXY[10]},${REFPIXY[11]},${REFPIXY[12]}" \
     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
     -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
     -CD11 "${PIXSCX},${PIXSCX},${PIXSCX},${PIXSCX},${PIXSCX},${PIXSCX},${PIXSCX},${PIXSCX},${PIXSCX},${PIXSCX},${PIXSCX},${PIXSCX}" \
     -CD12 "0,0,0,0,0,0,0,0,0,0,0,0" \
     -CD21 "0,0,0,0,0,0,0,0,0,0,0,0" \
     -CD22 "${PIXSCY},${PIXSCY},${PIXSCY},${PIXSCY},${PIXSCY},${PIXSCY},${PIXSCY},${PIXSCY},${PIXSCY},${PIXSCY},${PIXSCY},${PIXSCY}" \
     -M11 "-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,-1" \
     -M12 "0,0,0,0,0,0,0,0,0,0,0,0" \
     -M21 "0,0,0,0,0,0,0,0,0,0,0,0" \
     -M22 "-1,-1,1,1,1,-1,-1,1,1,-1,-1,1" \
     ${FILE}

  k=1
  CD11=`echo ${PIXSCX} | ${P_GAWK} '{if ($1>0) print $1*(-1.); else print $1}'`
  CD22=`echo ${PIXSCY} | ${P_GAWK} '{if ($1<0) print $1*(-1.); else print $1}'`
  while [ "${k}" -le "${NCHIPS}" ]
  do
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD1_1   = ${CD11}" CD1_1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD2_2   = ${CD22}" CD2_2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD1_2   = 0.0" CD1_2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD2_1   = 0.0" CD2_1
    k=$(( $k + 1 ))
  done

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    tmp=`get_key ${FILE} "DATE-OBS"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${tmp}" DUMMY1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY2
    k=$(( $k + 1 ))
  done

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}

done

