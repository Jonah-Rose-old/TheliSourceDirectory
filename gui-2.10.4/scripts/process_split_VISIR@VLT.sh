BASHPATH -xv

# ***************************************
# ***** INSTRUMENT splitting script *****
# *****      created by THELI       *****
# ***************************************

# splits VISIR@VLT images, returns the average of each chopping position

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



DIR=`pwd`

cd /$1/$2

mkdir ORIGINALS

CUBES=`ls -1l $1/$2/ | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

for CUBE in ${CUBES}
do
  SUFFIX=`echo $CUBE | ${P_GAWK} 'BEGIN {FS="."} {print "."$NF}'`
  BASE=`basename $CUBE $SUFFIX`
  BASE=`basename ${CUBE} .fits`
  nod=`get_key ${CUBE} "HIERARCH ESO SEQ NODPOS"`
  mv ${CUBE} ${BASE}_${nod}.fits
  pa=`get_key ${BASE}_${nod}.fits "HIERARCH ESO TEL CHOP POSANG"`
  cdmatrix=`${P_GETCDMATRIX} -s 0.075 -p ${pa} -n`
  cd11=`echo ${cdmatrix} | ${P_GAWK} '{print $1}'`
  cd12=`echo ${cdmatrix} | ${P_GAWK} '{print $2}'`
  cd21=`echo ${cdmatrix} | ${P_GAWK} '{print $3}'`
  cd22=`echo ${cdmatrix} | ${P_GAWK} '{print $4}'`
  ${DIR}/visir.py ${BASE}_${nod}.fits normal

  tmp=`get_key ${BASE}_${nod}.fits "HIERARCH ESO DET CHOP FREQ"`
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "CHOPFREQ= ${tmp}" DUMMY1
  tmp=`get_key ${BASE}_${nod}.fits "HIERARCH ESO DET CHOP NCYCLES"`
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "CNCYCLES= ${tmp}" DUMMY2
  tmp=`get_key ${BASE}_${nod}.fits "HIERARCH ESO DET NDIT"`
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "NDIT    = ${tmp}" DUMMY3
  tmp=`get_key ${BASE}_${nod}.fits "HIERARCH ESO INS EXTS ACT"`
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "SRC_TEMP= ${tmp}" DUMMY4
  tmp=`get_key ${BASE}_${nod}.fits "HIERARCH ESO INS TEMP1 VAL"`
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "DETTEMP1= ${tmp}" DUMMY5
  tmp=`get_key ${BASE}_${nod}.fits "HIERARCH ESO INS TEMP2 VAL"`
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "DETTEMP2= ${tmp}" DUMMY6
  tmp=`get_key ${BASE}_${nod}.fits "HIERARCH ESO SEQ CUMOFFSETX"`
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "CUMXOFF = ${tmp}" DUMMY7
  tmp=`get_key ${BASE}_${nod}.fits "HIERARCH ESO SEQ CUMOFFSETY"`
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "CUMYOFF = ${tmp}" DUMMY8
  tmp=`get_key ${BASE}_${nod}.fits "HIERARCH ESO SEQ JITTER WIDTH"`
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "JITTER  = ${tmp}" DUMMY9
  tmp=`get_key ${BASE}_${nod}.fits "HIERARCH ESO SEQ NODPOS"`
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "NODPOS  = ${tmp}" DUMMY10
  tmp=`get_key ${BASE}_${nod}.fits "HIERARCH ESO SEQ RELOFFSETX"`
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "RELXOFF = ${tmp}" DUMMY11
  tmp=`get_key ${BASE}_${nod}.fits "HIERARCH ESO SEQ RELOFFSETY"`
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "RELYOFF = ${tmp}" DUMMY12
  tmp=`get_key ${BASE}_${nod}.fits "DATE-OBS"`
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "DATE-OBS= ${tmp}" DUMMY13
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "MJD-OBS = ${MJD}" DUMMY14
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "CD1_1   = ${cd11}" DUMMY15
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "CD1_2   = ${cd12}" DUMMY16
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "CD2_1   = ${cd21}" DUMMY17
  ${P_REPLACEKEY} ${BASE}_${nod}_1.fits "CD2_2   = ${cd22}" DUMMY18

  mv ${BASE}_${nod}.fits ORIGINALS/${CUBE}
done
