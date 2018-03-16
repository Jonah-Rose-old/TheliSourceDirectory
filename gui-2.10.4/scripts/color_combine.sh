BASHPATH -vx

# this script combines a narrow and a broad-band image without changing stellar colours

# $1: directory
# $2: broad-band image
# $3: narrow-band image
# $4: broad-band width
# $5: narrow-band width
# $6: weight

. progs.ini 

# The combined image is calculated as follows:
# RHa = (1 - w * B(Ha) / B(R)) * R + w * Ha

bfac=`echo $4 $5 $6 | ${P_GAWK} '{print 1.-$3*$2/$1}'`

FILTER1=`echo $2 | sed 's/_cropped\.fits//g'`
FILTER2=`echo $3 | sed 's/_cropped\.fits//g'`

${P_IC} '%1 '${bfac}' *' $1/$2 > $1/broad_scaled.fits
${P_IC} '%1 '$6' * %2 +' $1/$3 $1/broad_scaled.fits > /$1/${FILTER1}+${FILTER2}_cropped.fits

# create fake weights, just so that we have them (e.g. for cat creation during colour calibration)
cp /$1/${FILTER1}_cropped.weight.fits /$1/${FILTER1}+${FILTER2}_cropped.weight.fits

rm $1/broad_scaled.fits
