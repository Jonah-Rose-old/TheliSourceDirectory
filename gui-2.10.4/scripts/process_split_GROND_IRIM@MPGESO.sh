BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits GROND_IRIM@MPGESO images

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

# sub-directories for the various filters
mkdir -p /$1/$2_J /$1/$2_H /$1/$2_K

FILES=`ls -1l $1/$2/ | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

cd /$1/$2

mkdir ORIGINALS

for FILE in ${FILES}
do
    # extract the three filters
    ${P_SPLITGROND} -i ${FILE}
    
    # extract the GABODS and DATE-OBS keywords
    DATEOBS=`get_key ${FILE} "DATE-OBS"`
    MJD=`${P_MJD} -f ${DATEOBS} | ${P_GAWK} '{print $7}'`
    GABODSID=`${P_NIGHTID} -t 16:00:00 -d 31/12/1998 -m ${MJD} |\
      ${P_GAWK} ' ($1 ~ /Days/) {print $6}' | ${P_GAWK} 'BEGIN{ FS="."} {print $1}'`

    # we don't know the original file name, but we assume that the DATE-OBS keyword
    # is present in the raw data, and hence 'split_grond' works correctly, producing
    # the following file names
    for F in J H K
    do
	${P_REPLACEKEY} GROND.${F}.${DATEOBS}_1.fits "GABODSID= ${GABODSID}" DUMMY1
	${P_REPLACEKEY} GROND.${F}.${DATEOBS}_1.fits "MJD-OBS = ${MJD}" MJD-OBS
	mv GROND.${F}.${DATEOBS}_1.fits $1/$2_${F}/
    done
    mv ${FILE} ORIGINALS/
done
