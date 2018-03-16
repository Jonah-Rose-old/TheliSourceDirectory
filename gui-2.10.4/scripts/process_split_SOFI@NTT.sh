BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits SOFI@NTT images

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
  BASE=`basename $FILE $SUFFIX`
  mv ${FILE} ${BASE}.fits
  FILE=${BASE}.fits

  RA=`get_key ${FILE} "RA"`
  if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
      RA=0.0
  fi

  DEC=`get_key ${FILE} "DEC"`
  if [ "${DEC}" = "" ] || [ "${DEC}" = "KEY_N/A" ]; then
      DEC=0.0
  fi

  DIT=`get_key ${FILE} "HIERARCH ESO DET DIT"`
  NDIT=`get_key ${FILE} "HIERARCH ESO DET NDIT"`
  EXPTIME=`echo ${DIT} ${NDIT} | ${P_GAWK} '{print $1*$2}'`
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

  FILT1=`get_key ${FILE} "FILTER1" | ${P_CLEANSTRING}`
  FILT2=`get_key ${FILE} "FILTER2" | ${P_CLEANSTRING}`
  FILTER=${FILT1}"+"${FILT2}
  if [ "${FILTER}" = "+" ]; then
      FILT1=`get_key ${FILE} "HIERARCH ESO INS FILT1 NAME" | ${P_CLEANSTRING}`
      FILT2=`get_key ${FILE} "HIERARCH ESO INS FILT2 NAME" | ${P_CLEANSTRING}`
      FILTER=${FILT1}"+"${FILT2}
      FILTER=`echo ${FILTER} | sed 's/+open//g'`
  else
      FILTER="UNKNOWN"
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

  cd11=`get_key ${FILE} "CD1_1"`                                                                        
  cd12=`get_key ${FILE} "CD1_2"`                                                                        
  cd21=`get_key ${FILE} "CD2_1"`                                                                        
  cd22=`get_key ${FILE} "CD2_2"`                                                                        
  crpix1=`get_key ${FILE} "CRPIX1"`                                                                     
  crpix2=`get_key ${FILE} "CRPIX2"`

  # if CD matrix is not present:
  if [ "${cd11}"_A = "_A" ]; then
      cdelt1=`get_key ${FILE} "CDELT1"`
      cdelt2=`get_key ${FILE} "CDELT2"`
      pc11=`get_key ${FILE} "PC1_1"`
      pc12=`get_key ${FILE} "PC1_2"`
      pc21=`get_key ${FILE} "PC2_1"`
      pc22=`get_key ${FILE} "PC2_2"`
      if [ "${pc11}"_A = "_A" ]; then
	  pc11=`get_key ${FILE} "PC001001"`
	  pc12=`get_key ${FILE} "PC001002"`
	  pc21=`get_key ${FILE} "PC002001"`
	  pc22=`get_key ${FILE} "PC002002"`
      fi

      cd11=`echo ${cdelt1} ${pc11} | ${P_GAWK} '{print $1*$2}'`
      cd12=`echo ${cdelt1} ${pc12} | ${P_GAWK} '{print $1*$2}'`
      cd21=`echo ${cdelt2} ${pc21} | ${P_GAWK} '{print $1*$2}'`
      cd22=`echo ${cdelt2} ${pc22} | ${P_GAWK} '{print $1*$2}'`
  fi

  if [ "${crpix1}"_A = "_A" ]; then
      crpix1=512.0
      crpix2=512.0
  fi

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

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    dateobs=`get_key ${FILE} "DATE-OBS"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${dateobs}" DUMMY1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "EXPT_DIT= ${DIT}" DUMMY3

    # scale the images such that they reflect the total exposure time.
    # otherwise we get problems with the weights when combining images
    # with different EXPTIME/NDIT settings
    if [ "${NDIT}" -gt "1" ]; then
	${P_IC} '%1 '${NDIT}' *' ${BASE}_${k}.fits > SOFI.${FILTER}.${dateobs}_${k}.fits
	rm ${BASE}_${k}.fits
    fi
    k=$(( $k + 1 ))
  done

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}

done
