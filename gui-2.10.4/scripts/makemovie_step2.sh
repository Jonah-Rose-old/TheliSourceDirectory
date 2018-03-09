BASHPATH

# determines the size of the cropped image

# $1: script dir
# $2: coaddition directory

DIR=`pwd`

cd $1

. progs.ini

cd $2

${P_DFITS} *.resamp_cropped.fits | ${P_FITSORT} CRPIX1 CRPIX2 NAXIS1 NAXIS2 \
    | ${P_GAWK} '($1 !~ /FILE/) {print $2, $3, $4, $5}' > ${TEMPDIR}/geom_$$

xlow=`${P_GAWK} 'BEGIN {min = 1000000} {if ($1<min) min = $1} END {print (-1)*min}' ${TEMPDIR}/geom_$$`
ylow=`${P_GAWK} 'BEGIN {min = 1000000} {if ($2<min) min = $2} END {print (-1)*min}' ${TEMPDIR}/geom_$$`
xhigh=`${P_GAWK} 'BEGIN {min = 1000000} {if (($3-$1)<min) min = $3-$1} END {print min}' ${TEMPDIR}/geom_$$`
yhigh=`${P_GAWK} 'BEGIN {min = 1000000} {if (($4-$2)<min) min = $4-$2} END {print min}' ${TEMPDIR}/geom_$$`

echo ${xhigh} ${xlow} ${yhigh} ${ylow} | ${P_GAWK} '{print $1-$2, $3-$4}'
