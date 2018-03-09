#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits GSAOI@GEMINI-SOUTH images

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

  RA=`get_keyext ${FILE} "CRVAL1"`
  if [ "${RA}" = "" ]; then
      RA=0.001
  fi

  DEC=`get_keyext ${FILE} "CRVAL2"`
  if [ "${DEC}" = "" ]; then
      DEC=0.0
  fi

  EXPTIME=`get_key ${FILE} "EXPTIME"`
  if [ "${EXPTIME}" = "" ]; then
      EXPTIME=`get_key ${FILE} "EXPOSURE"`
  fi
  if [ "${EXPTIME}" = "" ]; then
      EXPTIME=1.0
  fi

  OBJECT=`get_key ${FILE} "OBJECT"`
  if [ "${OBJECT}" = "" ]; then
      OBJECT=UNKNOWN
  fi

  EQUINOX=`get_key ${FILE} "EQUINOX" | sed 's/[^0-9.]//g'`
  if [ "${EQUINOX}" = "" ] || [ "${EQUINOX}" = "0." ]; then
      EQUINOX=2000.0
  fi

  DATEOBS1=`get_key ${FILE} "DATE"`
  DATEOBS2=`get_key ${FILE} "UT"`
  DATEOBS=${DATEOBS1}"T"${DATEOBS2}

  MJD=`${P_MJD} -f ${DATEOBS} | ${P_GAWK} '{print $7}'`

  FILTER1=`get_key ${FILE} "FILTER1" | ${P_CLEANSTRING}`
  FILTER2=`get_key ${FILE} "FILTER2" | ${P_CLEANSTRING}`
  if [ "${FILTER1}" = "Clear" ]; then
      FILTER1=""
      FILTER=${FILTER2}
  fi
  if [ "${FILTER2}" = "Clear" ]; then
      FILTER2=""
      FILTER=${FILTER1}
  fi
  if [ "${FILTER1}" != "Clear" ] && [ "${FILTER2}" != "Clear" ] && \
     [ "${FILTER1}_A" != "_A" ] && [ "${FILTER2}_A" != "_A" ]; then
      FILTER=${FILTER1}+${FILTER2}
  fi
  if [ "${FILTER1}" = "Clear" ] && [ "${FILTER2}" = "Clear" ]; then
      FILTER="Clear"
  fi
  if [ "${FILTER}" = "" ]; then
      FILTER=UNKNOWN
  fi
  if [ "${FILTER2}" = "Blocked1_G1114" ] || [ "${FILTER2}" = "Blocked2_G1128" ]; then
      FILTER="DARK"
  fi
  # rename filter
  if [ "${FILTER}" = "Z_G1101" ]; then
  	FILTER="Z"
  fi
  if [ "${FILTER}" = "J_G1102" ]; then
  	FILTER="J"
  fi
  if [ "${FILTER}" = "H_G1103" ]; then
  	FILTER="H"
  fi
  if [ "${FILTER}" = "Kprime_G1104" ]; then
  	FILTER="Kp"
  fi
  if [ "${FILTER}" = "Kshort_G1105" ]; then
  	FILTER="Ks"
  fi
  if [ "${FILTER}" = "K_G1106" ]; then
  	FILTER="K"
  fi
  if [ "${FILTER}" = "Jcont_G1107" ]; then
  	FILTER="Jc"
  fi
  if [ "${FILTER}" = "Hcont_G1108" ]; then
  	FILTER="Hc"
  fi
  if [ "${FILTER}" = "CH4short_G1109" ]; then
  	FILTER="CH4s"
  fi
  if [ "${FILTER}" = "CH4long_G1110" ]; then
  	FILTER="CH4l"
  fi
  if [ "${FILTER}" = "Kcntshrt_G1111" ]; then
  	FILTER="Kcs"
  fi
  if [ "${FILTER}" = "Kcntlong_G1112" ]; then
  	FILTER="Kcl"
  fi
  if [ "${FILTER}" = "HeI1083_G1115" ]; then
  	FILTER="HeI1083"
  fi
  if [ "${FILTER}" = "PaG_G1116" ]; then
  	FILTER="PaG"
  fi
  if [ "${FILTER}" = "PaB_G1117" ]; then
  	FILTER="PaB"
  fi
  if [ "${FILTER}" = "FeII_G1118" ]; then
  	FILTER="FeII"
  fi
  if [ "${FILTER}" = "H2(1-0)_G1121" ]; then
  	FILTER="H210"
  fi
  if [ "${FILTER}" = "BrG_G1122" ]; then
  	FILTER="BrG"
  fi
  if [ "${FILTER}" = "H2(2-1)_G1123" ]; then
  	FILTER="H221"
  fi
  if [ "${FILTER}" = "CO2360_G1124" ]; then
  	FILTER="CO2360"
  fi

  LST=`get_key ${FILE} "ST"`
  if [ "${LST}" = "" ]; then
      LST=0.0
  else
      LST=`${P_DMSTODECIMAL} ${LST} | ${P_GAWK} '{print $1*3600.}'`
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "0." ] || [ "${AIRMASS}" = "-9999." ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  get_wcs

  # override the reference pixel
  # cpx1="-110,2090,2090,-98"
  # cpx2="2121,2121,-74,-73"

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${cpx1}" \
     -CRPIX2 "${cpx2}" \
     -CD11 "${cd11}" \
     -CD12 "${cd12}" \
     -CD21 "${cd21}" \
     -CD22 "${cd22}" \
     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
     -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
     -M11 "1,1,1,1" \
     -M12 "0,0,0,0" \
     -M21 "0,0,0,0" \
     -M22 "1,1,1,1" \
     ${FILE}

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${DATEOBS}" DUMMY1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "ORIGNAME= '${BASE}'" DUMMY2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY3
    mv ${BASE}_${k}.fits GSAOI.${FILTER}.${DATEOBS}_${k}.fits
    k=$(( $k + 1 ))
  done

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}
done
