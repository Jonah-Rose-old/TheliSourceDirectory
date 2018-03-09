#!/bin/bash -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits FLAMINGOS2@GEMINI-SOUTH images

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

  RA=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CRVAL1 | ${P_GAWK} '(NR==2) {print $2}'`
  if [ "${RA}" = "" ] || [ "${RA}" = "KEY_N/A" ]; then
      RA=0.001
  fi

  DEC=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CRVAL2 | ${P_GAWK} '(NR==2) {print $2}'`
  if [ "${DEC}" = "" ] || [ "${DEC}" = "KEY_N/A" ]; then
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
  if [ "${EQUINOX}" = "" ] || [ "${EQUINOX}" = "0." ] ; then
      EQUINOX=2000.0
  fi

  DATEOBS1=`get_key ${FILE} "DATE"`
  DATEOBS2=`get_key ${FILE} "UT"`
  DATEOBS=${DATEOBS1}"T"${DATEOBS2}

  MJD=`${P_MJD} -f ${DATEOBS} | ${P_GAWK} '{print $7}'`

#  MJD=`get_key ${FILE} "MJD-OBS"`
#  if [ "${MJD}" = "" ] || [ "${MJD}" = "0." ] ; then
#      MJD=52000.0
#  fi

  FILTER1=`get_key ${FILE} "FILTER1" | ${P_CLEANSTRING}`
  FILTER2=`get_key ${FILE} "FILTER2" | ${P_CLEANSTRING}`
  if [ "${FILTER1}" = "Open" ]; then
      FILTER1=""
      FILTER=${FILTER2}
  fi
  if [ "${FILTER2}" = "Open" ]; then
      FILTER2=""
      FILTER=${FILTER1}
  fi
  if [ "${FILTER1}" != "Open" ] && [ "${FILTER2}" != "Open" ] && \
     [ "${FILTER1}_A" != "_A" ] && [ "${FILTER2}_A" != "_A" ]; then
      FILTER=${FILTER1}+${FILTER2}
  fi
  if [ "${FILTER1}" = "Open" ] && [ "${FILTER2}" = "Open" ]; then
      FILTER="Open"
  fi
  if [ "${FILTER}" = "" ]; then
      FILTER=UNKNOWN
  fi
  if [ "${FILTER2}" = "DK_G0807" ]; then
      FILTER="DARK"
  fi
  # rename filter
  if [ "${FILTER}" = "J_G0802" ]; then
      FILTER="J"
  fi
  if [ "${FILTER}" = "H_G0803" ]; then
      FILTER="H"
  fi
  if [ "${FILTER}" = "Ks_G0804" ]; then
      FILTER="K"
  fi
  if [ "${FILTER}" = "Y_G0811" ]; then
      FILTER="Y"
  fi
  if [ "${FILTER}" = "J-lo_G0801" ]; then
      FILTER="Jlo"
  fi
  if [ "${FILTER}" = "JH_G0809" ]; then
      FILTER="JH"
  fi
  if [ "${FILTER}" = "HK_G0806_good" ]; then
      FILTER="HK"
  fi

  LST=`get_key ${FILE} "ST"`
  if [ "${LST}" = "" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "0." ] || [ "${AIRMASS}" = "-9999." ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`
  
  if [ ${GABODSID} -lt 1 ]; then
      GABODSID=1
  fi


  cd11=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CD1_1 | ${P_GAWK} '(NR==2) {print $2}'`
  cd12=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CD1_2 | ${P_GAWK} '(NR==2) {print $2}'`
  cd21=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CD2_1 | ${P_GAWK} '(NR==2) {print $2}'`
  cd22=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CD2_2 | ${P_GAWK} '(NR==2) {print $2}'`
  cpx1=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CRPIX1 | ${P_GAWK} '(NR==2) {print $2}'`
  cpx2=`${P_DFITS} -x 1 ${FILE} | ${P_FITSORT} CRPIX2 | ${P_GAWK} '(NR==2) {print $2}'`

  # test if CD matrix is totally corrupted
  test=`echo "${cd11} ${cd12} ${cd21} ${cd22}" | \
      ${P_GAWK} '($1==0. && $2==0.&&$3==0.&&$4==0.) {print "bad"}'`
  if [ "${test}"_A = "bad_A" ]; then
      cd11=0.0
      cd22=0.0
      cd12=5.02e-5
      cd21=-5.02e-5
  fi

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${cpx1}" \
     -CRPIX2 "${cpx2}" \
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
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= '${DATEOBS}'" DUMMY1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "ORIGNAME= '${BASE}'" DUMMY2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY3
    ${P_DFITS} ${FILE} | ${P_FITSORT} LVDTVLTS LYOT ELEVATIO CRPA OBSTYPE GRISM MOSPOS TEMP_CH8 TEMP_CHB | \
	${P_GAWK} '(NR==2)' > ${TEMPDIR}/f2keys_$$
    ${P_REPLACEKEY} ${BASE}_${k}.fits "LVDTVLTS= `${P_GAWK} '{print $2}' ${TEMPDIR}/f2keys_$$`" DUMMY4
    ${P_REPLACEKEY} ${BASE}_${k}.fits "LYOT    = '`${P_GAWK} '{print $3}' ${TEMPDIR}/f2keys_$$`'" DUMMY5
    ${P_REPLACEKEY} ${BASE}_${k}.fits "ELEVATIO= `${P_GAWK} '{print $4}' ${TEMPDIR}/f2keys_$$`" DUMMY6
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CRPA    = `${P_GAWK} '{print $5}' ${TEMPDIR}/f2keys_$$`" DUMMY7
    ${P_REPLACEKEY} ${BASE}_${k}.fits "OBSTYPE = '`${P_GAWK} '{print $6}' ${TEMPDIR}/f2keys_$$`'" DUMMY8
    ${P_REPLACEKEY} ${BASE}_${k}.fits "GRISM   = '`${P_GAWK} '{print $7}' ${TEMPDIR}/f2keys_$$`'" DUMMY9
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MOSPOS  = '`${P_GAWK} '{print $8}' ${TEMPDIR}/f2keys_$$`'" DUMMY10
    ${P_REPLACEKEY} ${BASE}_${k}.fits "TEMP_CH8= '`${P_GAWK} '{print $9}' ${TEMPDIR}/f2keys_$$`'" DUMMY11
    ${P_REPLACEKEY} ${BASE}_${k}.fits "TEMP_CHB= '`${P_GAWK} '{print $10}' ${TEMPDIR}/f2keys_$$`'" DUMMY12
#    xoff=`${P_DFITS} ${FILE} | ${P_FITSORT} XOFFSET | ${P_GAWK} '(NR==2) {print $2}'`
#    yoff=`${P_DFITS} ${FILE} | ${P_FITSORT} YOFFSET | ${P_GAWK} '(NR==2) {print $2}'`
#    ${P_REPLACEKEY} ${BASE}_${k}.fits "XOFFSET = ${xoff}" DUMMY13
#    ${P_REPLACEKEY} ${BASE}_${k}.fits "YOFFSET = ${yoff}" DUMMY14
    mv ${BASE}_${k}.fits F2.${FILTER}.${DATEOBS}_${k}.fits
    k=$(( $k + 1 ))
  done

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}
done

rm ${TEMPDIR}/f2keys_$$
