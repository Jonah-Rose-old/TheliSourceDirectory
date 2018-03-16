BASHPATH -xv

# calculates the shift between images

# $1: main dir
# $2: science dir
# $3: image extension
# $4: "int" or "float"

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

DIR=`pwd`

# first create the zeroorder filelist:
ls -1 /$1/$2/*$3.fits > ${TEMPDIR}/tmp.asc_$$

# now create the zeroorder catalog list, and include 
# only those cats where we have images in $2

if [ -f "${TEMPDIR}/zeroorder_catlist_$$" ]; then
    rm ${TEMPDIR}/zeroorder_catlist_$$
fi

cat ${TEMPDIR}/tmp.asc_$$ |\
{
  while read file
  do
    BASE=`basename $file .fits`
    DIRNAM=`dirname $file`
    echo ${DIRNAM}"/cat/"${BASE}".cat" >> ${TEMPDIR}/zeroorder_catlist_$$
  done
}

ncat=`wc ${TEMPDIR}/zeroorder_catlist_$$ | ${P_GAWK} '{print $1}'`

echo ${ncat} > ${TEMPDIR}/zeroorder_catlist_1_$$

xmax=0
ymax=0

cat ${TEMPDIR}/zeroorder_catlist_$$ |\
{
  while read file
  do
    BASECAT=`basename ${file} .cat`
    xmax=`${P_DFITS} $1/$2/${BASECAT}.fits | ${P_FITSORT} NAXIS1 | ${P_GAWK} '(NR==2) {printf "%d\n", $2/10}'`
    ymax=`${P_DFITS} $1/$2/${BASECAT}.fits | ${P_FITSORT} NAXIS2 | ${P_GAWK} '(NR==2) {printf "%d\n", $2/10}'`
    nobj=`${P_LDACDESC} -i ${file} -t OBJECTS -e -q | ${P_GAWK} '{print $5}'`
    echo ${nobj} > $1/$2/cat/${BASECAT}_zero.cat
    ${P_LDACTOASC} -i ${file} -t OBJECTS -k Xpos Ypos MAG_AUTO -b -q \
    >> $1/$2/cat/${BASECAT}_zero.cat
    echo $1/$2/cat/${BASECAT}_zero.cat >> ${TEMPDIR}/zeroorder_catlist_1_$$
  done

  max=${xmax}
  if [ "${ymax}" -gt "${xmax}" ]; then
      max=${ymax}
  fi

  ${P_GETSHIFT} -i ${TEMPDIR}/zeroorder_catlist_1_$$ -s ${max} \
      -o 0.1 -p -m 4 -f 3 > ${TEMPDIR}/zeroshift_result0_$$

  touch ${TEMPDIR}/zeroshift_result1_$$

  cat ${TEMPDIR}/zeroshift_result0_$$ |\
  {
    while read file xs ys zp xrms yrms zrms nobj size
    do
      BASE=`basename ${file} _zero.cat`
      echo "$1/$2/${BASE}.fits $xs $ys $zp $xrms $yrms $zrms $nobj $size" >> \
	  ${TEMPDIR}/zeroshift_result1_$$
    done
  }
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

cat ${TEMPDIR}/zeroshift_result1_$$ |\
{
  while read file xs ys zp xrms yrms zrms nobj size
  do
    if [ "${V_AP_ZEROSHIFT}" = "INTSHIFT" ]; then
	xs=`echo ${xs} | ${P_GAWK} '{if (($1-int($1))<0.5) print int($1); else print int($1)+1}'`
	ys=`echo ${ys} | ${P_GAWK} '{if (($1-int($1))<0.5) print int($1); else print int($1)+1}'`
    fi
    if [ "${i}" = "0" ]; then
	crpix1=`${P_DFITS} $file | ${P_FITSORT} CRPIX1 | ${P_GAWK} '(NR==2) {print $2}'`
	crpix2=`${P_DFITS} $file | ${P_FITSORT} CRPIX2 | ${P_GAWK} '(NR==2) {print $2}'`
	crval1=`${P_DFITS} $file | ${P_FITSORT} CRVAL1 | ${P_GAWK} '(NR==2) {print $2}'`
	crval2=`${P_DFITS} $file | ${P_FITSORT} CRVAL2 | ${P_GAWK} '(NR==2) {print $2}'`
    fi

    BASE=`basename $file $3.fits`

    touch $1/$2/headers/${BASE}.head

    # some more header stuff needed from the individual images
    exptime=`${P_DFITS} $file | ${P_FITSORT} EXPTIME | ${P_GAWK} '(NR==2) {print $2}'`
    equinox=`${P_DFITS} $file | ${P_FITSORT} EQUINOX | ${P_GAWK} '(NR==2) {print $2}'`
    airmass=`${P_DFITS} $file | ${P_FITSORT} AIRMASS | ${P_GAWK} '(NR==2) {print $2}'`
    pv10=`echo ${xrms} ${PIXSCX} | ${P_GAWK} '{print $1*$2*1000.}'`
    pv20=`echo ${yrms} ${PIXSCY} | ${P_GAWK} '{print $1*$2*1000.}'`
    crpix1new=`echo ${crpix1} ${xs} | ${P_GAWK} '{print $1-$2}'`
    crpix2new=`echo ${crpix2} ${ys} | ${P_GAWK} '{print $1-$2}'`
    flxscale=`echo ${zp} ${exptime} | ${P_GAWK} '{print 10^(-0.4*$1)/$2}'`
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
    echo "LINXRMS = ${xrms}"
    echo "LINYRMS = ${yrms}"
    echo "ASTXRMS = ${xrms}"
    echo "ASTYRMS = ${yrms}"
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
#    EXPTIME=`${P_LDACTOASC} -i ../cat/${BASE}$3.cat1 -t LDAC_IMHEAD -s |\
#	fold | grep EXPTIME | ${P_GAWK} '{print $3}'`
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
