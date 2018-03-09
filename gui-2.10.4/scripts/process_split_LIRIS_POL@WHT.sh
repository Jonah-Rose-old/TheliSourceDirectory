BASHPATH

# splits LIRIS images

# $1: main directory
# $2: science directory

# create image list: we assume that ONLY unsplit
# images are in the directory

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
  ra_off1=`get_key ${FILE} "XAPNOM"`
  ra_off2=`get_key ${FILE} "XAPOFF"`
  dec_off1=`get_key ${FILE} "YAPNOM"`
  dec_off2=`get_key ${FILE} "YAPOFF"`
  RA=`echo ${RA} ${ra_off1} ${ra_off2} | ${P_GAWK} '{print $1 + $2 + $3}'`
  DEC=`echo ${DEC} ${dec_off1} ${dec_off2} | ${P_GAWK} '{print $1 + $2 + $3}'`

  DIT=`get_key ${FILE} "EXPTIME"`
  NDIT=`get_key ${FILE} "COAVERAG"`
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

  FILT1=`get_key ${FILE} "LIRF1NAM" | ${P_CLEANSTRING}`
  FILT2=`get_key ${FILE} "LIRF2NAM" | ${P_CLEANSTRING}`
  FILTER=${FILT1}"+"${FILT2}
  if [ "${FILTER}" = "" ] || [ "${FILTER}" = "KEY_N/A" ]; then
      FILTER=UNKNOWN
  fi

  LST=`get_key ${FILE} "ST"`
  LST=`${P_DMSTODECIMAL} ${LST} | ${P_GAWK} '{print $1*3600}'`
  if [ "${LST}" = "" ] || [ "${LST}" = "KEY_N/A" ]; then
      LST=0.0
  fi

  AIRMASS=`get_key ${FILE} "AIRMASS"`
  if [ "${AIRMASS}" = "" ]; then
      AIRMASS=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 12:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  ROTSKYPA=`get_key ${FILE} "ROTSKYPA"`

  ${P_FITSSPLIT_ECL} -x 1 -s \
      -CRPIX1 "${REFPIXX[1]}" \
      -CRPIX2 "${REFPIXY[1]}" \
      -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
      -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
      -CD11 "${PIXSCX}" \
      -CD12 "0.0" \
      -CD21 "0.0" \
      -CD22 "${PIXSCY}" \
      -M11 "1" \
      -M12 "0" \
      -M21 "0" \
      -M22 "1" \
      ${FILE}

  ${P_REPLACEKEY} ${BASE}_1.fits "BSCALE  = 1.0" BSCALE
  ${P_REPLACEKEY} ${BASE}_1.fits "ROTSKYPA= ${ROTSKYPA}" DUMMY1
  ${P_REPLACEKEY} ${BASE}_1.fits "EXPT_DIT= ${EXPTIMEIND}" DUMMY2
  ${P_REPLACEKEY} ${BASE}_1.fits "MJD-OBS = ${MJD}" DUMMY3

  # scale the images such that they reflect the total exposure time.
  # otherwise we get problems with the weights when combining images
  # with different EXPTIME/COAVE settings
  if [ "${NDIT}" -gt "1" ]; then
      ${P_IC} '%1 '${NDIT}' *' ${BASE}_1.fits > ${BASE}_1.fits2
      mv ${BASE}_1.fits2 ${BASE}_1.fits
  fi

  mv ${FILE} ORIGINALS/${BASE}${SUFFIX}
done

# correct for the pixel mapping problem

ls -1 *.fits > ${TEMPDIR}/pixelmapimagelist_$$

cat ${TEMPDIR}/pixelmapimagelist_$$ |\
{
  while read FILE
  do
    BASE=`basename ${FILE} .fits`
    ${P_CORRLIRIS} ${FILE} ${BASE}_tmp.fits
    mv ${BASE}_tmp.fits ${FILE}
  done
}

# now split the images into the 4 polarimetry subsections

ls /$1/$2/*_1.fits > ${TEMPDIR}/polarimetrysplit_$$

# these are the sections we define
section1=`echo 0 | ${P_GAWK} '{print 43, 980, 734, 945}'`
section2=`echo 0 | ${P_GAWK} '{print 43, 980, 514, 712}'`
section3=`echo 0 | ${P_GAWK} '{print 43, 980, 285, 486}'`
section4=`echo 0 | ${P_GAWK} '{print 43, 980,  58, 269}'`

cat ${TEMPDIR}/polarimetrysplit_$$ |\
{
    while read FILE
    do
	BASE=`basename ${FILE} _1.fits`
	${P_REPLACEKEY} ${FILE} "CRPIX1  = 512" "CRPIX1"
	${P_REPLACEKEY} ${FILE} "CRPIX2  = 512" "CRPIX2"
	${P_FITSCUT} -i ${FILE} -o /$1/$2/${BASE}P_1.fits -x ${section1}
	${P_FITSCUT} -i ${FILE} -o /$1/$2/${BASE}P_2.fits -x ${section2}
	${P_FITSCUT} -i ${FILE} -o /$1/$2/${BASE}P_3.fits -x ${section3}
	${P_FITSCUT} -i ${FILE} -o /$1/$2/${BASE}P_4.fits -x ${section4}
	${P_REPLACEKEY} /$1/$2/${BASE}P_1.fits "CRPIX1  = 470" "CRPIX1"
	${P_REPLACEKEY} /$1/$2/${BASE}P_1.fits "CRPIX2  = 104" "CRPIX2"
	${P_REPLACEKEY} /$1/$2/${BASE}P_2.fits "CRPIX1  = 470" "CRPIX1"
	${P_REPLACEKEY} /$1/$2/${BASE}P_2.fits "CRPIX2  = 104" "CRPIX2"
	${P_REPLACEKEY} /$1/$2/${BASE}P_3.fits "CRPIX1  = 470" "CRPIX1"
	${P_REPLACEKEY} /$1/$2/${BASE}P_3.fits "CRPIX2  = 104" "CRPIX2"
	${P_REPLACEKEY} /$1/$2/${BASE}P_4.fits "CRPIX1  = 470" "CRPIX1"
	${P_REPLACEKEY} /$1/$2/${BASE}P_4.fits "CRPIX2  = 104" "CRPIX2"
	\rm ${FILE}
    done
}
