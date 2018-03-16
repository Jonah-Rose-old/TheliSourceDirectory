BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits EOS350D images

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

# convert the RAW images to FITS

for FILE in ${FILES}
do
  SUFFIX=`echo $FILE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  BASE=`basename $FILE ${SUFFIX}`

  ${P_TIFF2FITS} -i ${FILE} -o ${BASE}.fits

  mv ${FILE} ORIGINALS
  FILE=${BASE}.fits

  RA=0.001
  DEC=0.0
  EQUINOX=2000.0
  EXPTIME=300
  OBJECT=UNKNOWN
  MJD=52000.0
  FILTER=RGB
  LST=0.0
  AIRMASS=1.0

  EXPTIME=`get_key ${FILE} "EXPTIME"`
  if [ "${EXPTIME}" = "" ] || [ "${EXPTIME}" = "KEY_N/A" ]; then
      EXPTIME=1.0
  fi

  GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

  ${P_FITSSPLIT_ECL} \
     -CRPIX1 "${REFPIXX[1]}" \
     -CRPIX2 "${REFPIXY[1]}" \
     -CRVAL1 ${RA} -CRVAL2 ${DEC} -EXPTIME ${EXPTIME} -AIRMASS ${AIRMASS} \
     -GABODSID ${GABODSID} -FILTER ${FILTER} -EQUINOX ${EQUINOX} -OBJECT ${OBJECT} \
     -CD11 "${PIXSCX}" \
     -CD12 "0" \
     -CD21 "0" \
     -CD22 "${PIXSCY}" \
     -M11 "1" \
     -M12 "0" \
     -M21 "0" \
     -M22 "1" \
     ${FILE}

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    tmp=`get_key ${FILE} "DATE-OBS"`
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS= ${tmp}" DUMMY1
    k=$(( $k + 1 ))
  done

  ORIGFILE=`get_key ${FILE} "ORIGFILE"`
  ISOSPEED=`${P_DCRAW} -i -v ORIGINALS/${ORIGFILE} | \
            ${P_GAWK} 'BEGIN {FS=":"} {if ($1~/ISO/) print $2}'`
  BAYERPAT=`${P_DCRAW} -i -v ORIGINALS/${ORIGFILE} |
            ${P_GAWK} '($0~/Filter pattern/) {print $3}' | sed 's/\///g'`
  DATEOBS=`${P_DCRAW} -i -v ORIGINALS/${ORIGFILE} | ${P_GAWK} '($1~/Timestamp/) {
      if ($3=="Jan") month="01"
      if ($3=="Feb") month="02"
      if ($3=="Mar") month="03"
      if ($3=="Apr") month="04"
      if ($3=="May") month="05"
      if ($3=="Jun") month="06"
      if ($3=="Jul") month="07"
      if ($3=="Aug") month="08"
      if ($3=="Sep") month="09"
      if ($3=="Oct") month="10"
      if ($3=="Nov") month="11"
      if ($3=="Dec") month="12"
      print $6"-"month"-"$4"T"$5}'`

  k=1
  while [ "${k}" -le "${NCHIPS}" ]
  do
    ${P_REPLACEKEY} ${BASE}_${k}.fits "ISOSPEED=${ISOSPEED}" DUMMY9
    ${P_REPLACEKEY} ${BASE}_${k}.fits "DATE-OBS='${DATEOBS}'" DATE-OBS
    ${P_REPLACEKEY} ${BASE}_${k}.fits "BAYERPAT='${BAYERPAT}'" DUMMY10
    k=$(( $k + 1 ))
  done

  \rm ${FILE}

done

