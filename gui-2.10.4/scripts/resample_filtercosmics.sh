BASHPATH -xv

# $1: main dir
# $2: science dir

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

IDENT=${V_COADD_IDENT}_A
USERDIR=~/.theli/scripts/

if [ "${IDENT}" = "_A" ]; then
   IDENT=default
else
   IDENT=${V_COADD_IDENT}
fi

cd /$1/$2/coadd_${IDENT}/

# first, create the file information list

n=`ls *resamp.fits | wc | ${P_GAWK} '{print $1}'`
geometry=`${P_DFITS} ${USERDIR}/coadd.head | ${P_FITSORT} NAXIS1 NAXIS2 CRPIX1 CRPIX2 | ${P_GAWK} '(NR==2) {print $2, $3, $4, $5}'`

# the header line

echo $n ${geometry} > ${TEMPDIR}/cosmic.list_$$

# the body

ls *resamp.fits > ${TEMPDIR}/resamp.list_$$

cat ${TEMPDIR}/resamp.list_$$ |\
{
  while read file
  do
    BASE=`basename ${file} resamp.fits`
    image=${file}
    weight=${BASE}resamp.weight.fits
    geometry=`${P_DFITS} ${file} | ${P_FITSORT} NAXIS1 NAXIS2 CRPIX1 CRPIX2 FLXSCALE | ${P_GAWK} '(NR==2) {print $2, $3, $4, $5, $6}'`
    origbase=`basename ${file} .${IDENT}.resamp.fits`
    sky=`${P_DFITS} /$1/$2/${origbase}.fits | ${P_FITSORT} SKYVALUE | ${P_GAWK} '(NR==2) {print $2}'`
    if [ "${sky}"_A = "_A" ] || [ "${sky}" = "KEY_N/A" ]; then
	sky="0.0"
    fi
    echo ${image} ${weight} ${geometry} ${sky} >> ${TEMPDIR}/cosmic.list_$$
    NEXTDUMMY=`${P_DFITS} ${file} | grep DUMMY | \
            ${P_GAWK} 'BEGIN {nrmin=1000+0} {
            if (index($1,"DUMMY")==1) {
            nr=substr($1,6)+0; if(nr<nrmin) {nrmin=nr}}}
            END {if(NR>0) {print nrmin}}'`
    checksky=`${P_DFITS} /$1/$2/${origbase}.fits | ${P_FITSORT} SKYVALUE | ${P_GAWK} '(NR==2) {print $2}'`
    if [ ! "${checksky}" = "KEY_N/A" ] && [ "${NEXTDUMMY}_A" != "_A" ]; then
	${P_REPLACEKEY} ${file} "SKYVALUE= ${sky}" DUMMY${NEXTDUMMY}
    fi
  done
}

CLUSTERSIZE=""
if [ "${V_COADD_FILTERCLUSTERSIZE}"_A = "_A" ] || [ "${V_COADD_FILTERCLUSTERSIZE}" = "0" ]; then
    CLUSTERSIZE=""
else
    CLUSTERSIZE="-m $((${V_COADD_FILTERCLUSTERSIZE}+1))"
fi

BORDERWIDTH=""
if [ "${V_COADD_FILTERBORDERWIDTH}"_A = "_A" ]; then
    BORDERWIDTH=""
else
    BORDERWIDTH="-e ${V_COADD_FILTERBORDERWIDTH}"
fi

# determine how many lines we can read simultaneously without swapping
# (the scaling factor *100 is purely empirical)
RAM=`cat /proc/meminfo | gawk '($1 ~ /MemTotal/) {print $2*1024.}'`
DATASIZE=`ls -ltr *resamp.fits | gawk 'BEGIN{s=0} {s=s+$5} END {print s}'`
NUMLINES=`echo ${RAM} ${DATASIZE} | ${P_GAWK} '{printf "%d\n", $1/$2*50}'`
if [ ${NUMLINES} -eq 0 ]; then
    NUMLINES=1
fi
# Avoid problems with machines with lots of memory (sometimes leads to problems when the
# number of lines is >>1000
if [ ${NUMLINES} -gt 1000 ]; then
    NUMLINES=1000
fi

${P_SWARPFILTER} -i ${TEMPDIR}/cosmic.list_$$ -k ${V_COADD_FILTERTHRESHOLD} \
    ${CLUSTERSIZE} ${BORDERWIDTH} -n ${NUMLINES}
