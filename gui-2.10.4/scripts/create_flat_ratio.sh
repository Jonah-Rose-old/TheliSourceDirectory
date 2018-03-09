BASHPATH -xv

# this script determines the flux ratios between chips in a 
# flat-field image, to be fed into preprocess.

# $1: main directory
# $2: Flat directory

# preliminary work:
INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash.include

# setting up the (sorted) list of flats
rm ${TEMPDIR}/flatlist_imstats1_$$

CHIP=1
while [ "${CHIP}" -le "${NCHIPS}" ]
do
    echo /$1/$2/$2_${CHIP}.fits >> ${TEMPDIR}/flatlist_$$

    get_statminsection

    ${P_IMSTATS} /$1/$2/$2_${CHIP}.fits \
	-s ${statminsection_imstats} -t -1e8 1e8 | \
	${P_GAWK} '($1 !~ /#/) {print $2}' >> ${TEMPDIR}/flatlist_imstats1_$$

    CHIP=$((${CHIP}+1))
done

# extract the maximum
maxlevel=`${P_GETSTATS} -i ${TEMPDIR}/flatlist_imstats1_$$ -m max`

# normalise the fluxes with respect to the highest flux
${P_GAWK} '{printf "%.4f\n", $1/'"${maxlevel}"'}' ${TEMPDIR}/flatlist_imstats1_$$ > $1/$2//flatratios.dat

echo ${maxlevel} > $1/$2/.maxlevel.dat
