#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits VIRCAM@VISTA images

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
  if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
      RA=0.001
  fi

  DEC=`get_key ${FILE} "DEC"`
  if [ "${DEC}" = "" ] || [ "${DEC}" = "KEY_N/A" ]; then
      DEC=0.0
  fi

  # VIRCAM stores the SUM image, not the average. No rescaling is necessary.
  DIT=`get_key ${FILE} "HIERARCH ESO DET DIT"`
  NDIT=`get_key ${FILE} "HIERARCH ESO DET NDIT"`
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

  FILTER=`get_key ${FILE} "HIERARCH ESO INS FILT1 NAME" | ${P_CLEANSTRING}`
  if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "LST"`
  if [ "${LST}" = "" ] || [ "${LST}" = "KEY_N/A" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "HIERARCH ESO TEL AIRM START"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

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

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${cpx1}" \
     -CRPIX2 "${cpx2}" \
     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
     -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
     -CD11 "${cd11}" \
     -CD12 "${cd12}" \
     -CD21 "${cd21}" \
     -CD22 "${cd22}" \
     -M11 "1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1" \
     -M12 "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0" \
     -M21 "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0" \
     -M22 "1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1" \
     ${FILE}

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    tmp=`get_key ${FILE} "DATE-OBS"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${tmp}" DUMMY1
    seeing1=`get_key ${FILE} "HIERARCH ESO TEL AMBI FWHM START"`
    seeing2=`get_key ${FILE} "HIERARCH ESO TEL AMBI FWHM END"`
    seeing=`echo ${seeing1} ${seeing2} | ${P_GAWK} '{print ($1 + $2 ) / 2.}'`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "SEEINGMN= ${seeing}" DUMMY2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "EXPT_DIT= ${DIT}" DUMMY3
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY4

    # no rescaling necessary for VIRCAM
    k=$(( $k + 1 ))
  done

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}

done
