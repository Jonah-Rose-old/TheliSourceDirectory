BASHPATH -xv

# This script will check for the presence of bright objects in the 
# images based on source catalogs

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

test -f /$1/$2/background_exclusion && rm /$1/$2/background_exclusion

dir=`pwd`

safety_distance=`echo ${V_BACK_DISTANCE} ${PIXSCALE} | ${P_GAWK} '{print $1*60/$2}'`

# Get the geometries
${dir}/get_refcat_geometry.sh $1 $2 OFC
# Fetch the results
ra=`${P_GAWK} '{print $1}' ${TEMPDIR}/auto_ra_dec.dat`
dec=`${P_GAWK} '{print $2}' ${TEMPDIR}/auto_ra_dec.dat`
radius=`${P_GAWK} '{print $3}' ${TEMPDIR}/auto_ra_dec.dat`

VIZOPTION="-wget -mime=csv -site=${V_BACK_SERVER} -out.max=100000000 -c.rm=${radius} -out=_RAJ -out=_DEJ"

# obtain data from UCAC4
${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=UCAC4 \
    -out="f.mag" f.mag=0..${V_BACK_MAGLIMIT} | \
    ${P_GAWK} '($0 !~/#/)' | ${P_GAWK} '(NR>4&&NF>0)' | \
    ${P_GAWK} 'BEGIN{FS=";"} {
             printf ("%s %s %f\n", $1, $2, $3)
             }' > ${TEMPDIR}/brightsourcecat.dat_$$

# EXIT if no sources left
numstars=`wc ${TEMPDIR}/brightsourcecat.dat_$$ | ${P_GAWK} '{print $1}'`
if [ "${numstars}" = "0" ]; then
    exit 1
fi

# Now check all images for bright stars
# Uses the output of get_refcat_geometry.sh!
cd /$1/$2/
exec < ${TEMPDIR}/imdata_geometry.dat
while read file ra dec n m
do
    # Check for bright stars
    ${P_SKY2XY} -j $file @${TEMPDIR}/brightsourcecat.dat_$$ > ${TEMPDIR}/bs_filt_$$

    # Split the result in on-chip and off-chip data
    ${P_GAWK} '($0 !~/off image/)' ${TEMPDIR}/bs_filt_$$ > ${TEMPDIR}/bs_onchip_$$
    ${P_GAWK} '($0 ~/off image/)'  ${TEMPDIR}/bs_filt_$$ > ${TEMPDIR}/bs_offchip_$$
    n_on=`wc ${TEMPDIR}/bs_onchip_$$ | ${P_GAWK} '{print $1}'`
    n_off=`wc ${TEMPDIR}/bs_offchip_$$ | ${P_GAWK} '{print $1}'`

    # First the on-chip images
    if [ ${n_on} -gt 0 ]; then
	echo ${file} >> /$1/$2/background_exclusion
	continue
    fi

    # Check for off-chip stars (within a certain distance of the chip)
    if [ ${n_off} -gt 0 ]; then
	test=`${P_GAWK} 'BEGIN {
            d="'${safety_distance}'"
            n="'${n}'"
            m="'${m}'"
            test=0} {
            xstar=$5
            ystar=$6
            if (xstar > -1*d && xstar < n+d && ystar > -1*d && ystar < m+d) {test=1}} END {print test}'	${TEMPDIR}/bs_offchip_$$`
	if [ ${test} -eq 1 ]; then
	    echo ${file} >> /$1/$2/background_exclusion
	fi
    fi
done

# merge with manual background exclusion file:

cat /$1/$2/background_exclusion  /$1/$2/background_exclusion_manual | uniq > ${TEMPDIR}/background_exclusion_$$
mv ${TEMPDIR}/background_exclusion_$$ /$1/$2/background_exclusion
