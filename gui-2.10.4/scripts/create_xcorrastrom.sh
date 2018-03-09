BASHPATH -xv

# calculates the shift between images

# $1: main dir
# $2: science dir
# $3: image extension

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash.include

DIR=`pwd`

# first create the xcorr filelist:
ls -1 /$1/$2/*$3.fits > ${TEMPDIR}/tmp.asc_$$

REFIMAGE=`cat ${TEMPDIR}/tmp.asc_$$ | ${P_GAWK} '(NR==1) {print $1}'`

# how many CPUs shall we use?
NUMTHREAD=`${DIR}/get_nthreads.sh ${NCHIPS} ${NPARA} 1`

test -e ${TEMPDIR}/xcorr.result_$$ && rm ${TEMPDIR}/xcorr.result_$$

cat ${TEMPDIR}/tmp.asc_$$ |\
{
  while read file
  do
    BASE=`basename ${file} .fits`
    WBASE=`basename ${file} $3.fits`

    WSTRING=""
    if [ -f /$1/WEIGHTS/${WBASE}$3.weight.fits ]; then
	WSTRING="-WEIGHT_IMAGE /$1/WEIGHTS/${WBASE}$3.weight.fits -WEIGHT_TYPE MAP_WEIGHT"
    fi

    # create image with objects only
    ${P_SEX} ${file} -c ${DATACONF}/image-objects.sex \
	-CATALOG_TYPE NONE  \
	-DETECT_MINAREA 5 \
	-DETECT_THRESH 1.5 \
	-ANALYSIS_THRESH 2 \
	-PARAMETERS_NAME ${CONF}/singleastrom.param.sex \
	-FILTER Y ${WSTRING} \
	-NTHREADS ${NUMTHREAD} \
	-VERBOSE_TYPE QUIET \
	-CHECKIMAGE_TYPE OBJECTS \
	-CHECKIMAGE_NAME ${TEMPDIR}/${BASE}.xcorrsex.fits
    
    # Multiply the images with the weight map to get rid off cosmics
    ${P_IC} '%1 %2 *' ${TEMPDIR}/${BASE}.xcorrsex.fits /$1/WEIGHTS/${WBASE}$3.weight.fits > \
	${TEMPDIR}/${TEMPDIR}/${BASE}.xcorrsex.fits_w
    mv ${TEMPDIR}/${TEMPDIR}/${BASE}.xcorrsex.fits_w ${TEMPDIR}/${TEMPDIR}/${BASE}.xcorrsex.fits

    # This looks weird, but it works, because the first file in this loop
    # IS the reference image, and therefore it gets copied just in time 
    # for the xcorr task
    if [ "${file}" = "${REFIMAGE}" ]; then
	cp ${TEMPDIR}/${BASE}.xcorrsex.fits ${TEMPDIR}/refimage.xcorrsex.fits
    fi

    peakpos=`${P_XCORR} \
	-r ${TEMPDIR}/refimage.xcorrsex.fits \
	-c ${TEMPDIR}/${BASE}.xcorrsex.fits \
	-o ${TEMPDIR}/${BASE}.xcorr.fits`

    nax1=`get_key ${TEMPDIR}/${BASE}.xcorrsex.fits NAXIS1`
    nax2=`get_key ${TEMPDIR}/${BASE}.xcorrsex.fits NAXIS2`

    xoffset=`echo ${peakpos} ${nax1} | ${P_GAWK} '{print $1-$3}'`
    yoffset=`echo ${peakpos} ${nax2} | ${P_GAWK} '{print $2-$3}'`

    echo ${file} ${xoffset} ${yoffset} >> ${TEMPDIR}/xcorr.result_$$

  done
}


# force the headers to be identical in CRVAL and CRPIX
# and make the actual header files

# create the header file
if [ ! -d $1/$2/headers ]; then
    mkdir $1/$2/headers
else 
    rm -rf $1/$2/headers
    mkdir $1/$2/headers
fi

i=0

cat ${TEMPDIR}/xcorr.result_$$ |\
{
  while read file xs ys
  do
    if [ "${i}" = "0" ]; then
	crpix1=`get_key $file CRPIX1`
	crpix2=`get_key $file CRPIX2`
	crval1=`get_key $file CRVAL1`
	crval2=`get_key $file CRVAL2`
    fi

    BASE=`basename $file $3.fits`

    touch $1/$2/headers/${BASE}.head

    # some more header stuff needed from the individual images
    exptime=`get_key $file EXPTIME`
    equinox=`get_key $file EQUINOX`
    airmass=`get_key $file AIRMASS`
    pv10=0.0
    pv20=0.0
    crpix1new=`echo ${crpix1} ${xs} | ${P_GAWK} '{print $1-$2}'`
    crpix2new=`echo ${crpix2} ${ys} | ${P_GAWK} '{print $1-$2}'`
    flxscale=`echo ${exptime} | ${P_GAWK} '{print 1./$1}'`
    {
    echo "CRPIX1  = ${crpix1new}"
    echo "CRPIX2  = ${crpix2new}"
    echo "CD1_1   = ${PIXSCX}"
    echo "CD2_2   = ${PIXSCY}"
    echo "CD1_2   = 0.0"
    echo "CD2_1   = 0.0"
    echo "CRVAL1  = ${crval1}"
    echo "CRVAL2  = ${crval2}"
    echo "EXPTIME = ${exptime}"
    echo "EQUINOX = ${equinox}"
    echo "LINXRMS = 0.0"
    echo "LINYRMS = 0.0"
    echo "ASTXRMS = 0.0"
    echo "ASTYRMS = 0.0"
    echo "LINTOL  = 5"
    echo "CTYPE1  = 'RA---TAN'"
    echo "CTYPE2  = 'DEC--TAN'"
    echo "PSORD   = 1"
    echo "PSTOL   = 5"
    echo "PV1_0   = 0.0"
    echo "PV1_1   = 1.0"
    echo "PV1_2   = 0.0"
    echo "PV2_0   = 0.0"
    echo "PV2_1   = 1.0"
    echo "PV2_2   = 0.0"
    echo "AIRMASS = ${airmass}"
    echo "ZP0     = 0."
    echo "ZP      = 0."
    echo "FLXSCALE= ${flxscale}"
    } >> $1/$2/headers/${BASE}.head

    i=$(( ${i} + 1 ))
  done
}

cd /$1/$2/headers

ls *.head > ${TEMPDIR}/headernames_$$
# calculate relative photometric zeropoints

if [ -f flxscale ]; then
    \rm flxscale
fi

cat ${TEMPDIR}/headernames_$$ |\
{
  while read file
  do
    BASE=`basename ${file} .head`
    EXPTIME=`${P_DFITS} ../${BASE}$3.fits | ${P_FITSORT} EXPTIME |\
	${P_GAWK} '(NR==2) {print $2}'`
    FLXSCALE=`grep FLXSCALE ${BASE}.head | ${P_GAWK} '{print $2}'`
    echo ${BASE}.head $FLXSCALE $EXPTIME >> flxscale
  done
}

${P_GAWK} '{flx[NR]=-2.5*log($2*$3)/log(10.); name[NR]=$1} END {
     i=1
     sum=0.0
     while (i<=NR) {
       sum=sum+flx[i]
       i++
     }
     sum=sum/NR
     i=1
     while (i<=NR) {
       print name[i], flx[i]-sum
       i++
     }
     }' flxscale > rzp

# add the relative zeropoints to the header files

cat rzp |\
{
  while read file zp
  do
    ${P_GAWK} 'BEGIN{rzp='$zp'}{print; if ($1~/FLXSCALE/) {print "RZP     =      "rzp"       / SCAMP relative zeropoint"}}' \
        $file > ${file}2
    mv ${file}2 ${file}
  done
}

\rm rzp flxscale

cd ${DIR}
