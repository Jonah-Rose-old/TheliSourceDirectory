#!/bin/bash -xv

# splits WFI@INT images, correct rotations.

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
  RA=`${P_HMSTODECIMAL} ${RA}`
  RA1=`${P_DFITS} ${FILE} | ${P_FITSORT} "CRVAL1" | ${P_GAWK} '($1!="FILE") {print int($2+0.5)}'`
  if [ "${RA1}" -gt "361" ]; then
      RA=0.0
  fi

  DEC=`get_key ${FILE} "DEC"`
  DEC=`${P_DMSTODECIMAL} ${DEC}`
  DEC1=`${P_DFITS} ${FILE} | ${P_FITSORT} "CRVAL2" | ${P_GAWK} '($1!="FILE") {print int($2+0.5)}'`
  if [ "${DEC1}" -gt "90" ]; then
      DEC=0.0
  fi

  # deal with the funny aperture stuff of the WHT system:
  #  ra_off1=`get_key ${FILE} "XAPNOM"`
  #  ra_off2=`get_key ${FILE} "XAPOFF"`
  #  dec_off1=`get_key ${FILE} "YAPNOM"`
  #  dec_off2=`get_key ${FILE} "YAPOFF"`
  #  RA=`echo ${RA} ${ra_off1} ${ra_off2} | ${P_GAWK} '{x = $1 + $2 + $3; if (x>360 || x<0) print "0.0"; else print x}'`
  #  DEC=`echo ${DEC} ${dec_off1} ${dec_off2} | ${P_GAWK} '{x = $1 + $2 + $3; if (x>90 || x<-90) print "0.0"; else print x}'`

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

  FILTER=`get_key ${FILE} "WFFBAND" | ${P_CLEANSTRING}`
  if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "ST"`
  if [ "${LST}" = "" ] || [ "${LST}" = "KEY_N/A" ]; then
      LST=0.0
  else
      LST=`${P_DMSTODECIMAL} ${LST} | ${P_GAWK} '{print $1*3600.}'`
  fi

  MJD=`get_key ${FILE} "MJD-OBS"`
  if [ "${MJD}" = "" ] || [ "${MJD}" = "KEY_N/A" ]; then
      MJD=52000.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ] || [ "${AIRMASS}" = "KEY_N/A" ]; then
      AIRMASS=1.0
  fi
  AIRMASS=`echo ${AIRMASS} | ${P_GAWK} '{if ($1<1) {print 1.0} else {print $1}}'`

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  DATEOBS1=`get_key ${FILE} "DATE-OBS"`
  DATEOBS2=`get_key ${FILE} "UTSTART"`
  DATEOBS=${DATEOBS1}"T"${DATEOBS2}

  # the WCS information in these fits headers is totally screwed ...
  # transform the CCDs such that North is up and east is left, assuming the
  # INTs default rotator angle of 180 degrees (has been fixed around 2005).

  ${P_FITSSPLIT_ECL} \
      -CRPIX1 "1,1,1,1" \
      -CRPIX2 "1,1,1,1" \
      -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
      -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
      -CD11 "1.,1.,1.,1." \
      -CD12 "0.,0.,0.,0." \
      -CD21 "0.,0.,0.,0." \
      -CD22 "1.,1.,1.,1." \
      -M11 "0,-1,0,0" \
      -M12 "1,0,1,1" \
      -M21 "-1,0,-1,-1" \
      -M22 "0,-1,0,0" \
      ${FILE}

# force the CD matrix to fit the North-up and East-left orientation

  CD11=`echo ${PIXSCX} | ${P_GAWK} '{if ($1>0) print $1*(-1.); else print $1}'`
  CD22=`echo ${PIXSCY} | ${P_GAWK} '{if ($1<0) print $1*(-1.); else print $1}'`

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD1_1   = ${CD11}" CD1_1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD2_2   = ${CD22}" CD2_2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD1_2   = 0.0" CD1_2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CD2_1   = 0.0" CD2_1

    ranew=`echo ${RA} ${DEC} | ${P_GAWK} '{ \
	ra=$1-0.0733/cos($2*3.14159/180.); \
	if (ra>360) ra=ra-360.; \
	if (ra<0) ra=ra+360.} END {print ra}'`
    decnew=`echo ${DEC} | ${P_GAWK} '{print $1-0.02907}'` 

    ${P_REPLACEKEY} ${BASE}_${k}.fits "CRVAL1  = ${ranew}" CRVAL1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "CRVAL2  = ${decnew}" CRVAL2
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${DATEOBS}" DUMMY1
    ${P_REPLACEKEY} ${BASE}_${k}.fits "MJD-OBS = ${MJD}" DUMMY2
    k=$(( $k + 1 ))
  done

# fix the reference pixel
  ${P_REPLACEKEY} ${BASE}_1.fits "CRPIX1  = 1406" CRPIX1
  ${P_REPLACEKEY} ${BASE}_1.fits "CRPIX2  = 1406" CRPIX2
  ${P_REPLACEKEY} ${BASE}_2.fits "CRPIX1  = -690" CRPIX1
  ${P_REPLACEKEY} ${BASE}_2.fits "CRPIX2  = 1397" CRPIX2
  ${P_REPLACEKEY} ${BASE}_3.fits "CRPIX1  = 1380" CRPIX1
  ${P_REPLACEKEY} ${BASE}_3.fits "CRPIX2  = -710" CRPIX2
  ${P_REPLACEKEY} ${BASE}_4.fits "CRPIX1  = 1395" CRPIX1
  ${P_REPLACEKEY} ${BASE}_4.fits "CRPIX2  = 345" CRPIX2
  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}
done
