BASHPATH -xv

# the script creates normalised images.
# It puts the result into new directories ..._norm.
# The script assumes a threshold. Pixels under
# this threshhold will be assigned a value of threshold
# in the resulting images.
#
# 30.05.04:
# temporary files go to a TEMPDIR directory 
#
# 12.04.2005:
# - I rewrote the script so that it works with the
#   'imstats' program instead with the FLIPS based
#   'immode'.

# $1: main directory
# $2: directory from which normaliced images should be created 
# $3: chips to be processed

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash.include

# first calculate the modes:
test -e ${TEMPDIR}/immode.dat_$$ && rm ${TEMPDIR}/immode.dat_$$

for CHIP in $3
do
    get_statminsection
    ${P_IMSTATS} \
	-d 6 /$1/$2/$2_${CHIP}.fits \
	-s ${statminsection_imstats} \
	-t -1e8 1e8 \
	>> ${TEMPDIR}/immode.dat_$$
done

# clean the modes (in case of strong vignetting they might be underestimated)
# lower threshold: 25% of the initial mode estimate
${P_GAWK} '($1!="#") {print $2}' ${TEMPDIR}/immode.dat_$$  > \
    ${TEMPDIR}/immode_clean.dat_$$

k=1
for CHIP in $3
do
    get_statminsection
    thresh=`${P_GAWK} '(NR=='"${k}"') {print 0.25*$1}' ${TEMPDIR}/immode_clean.dat_$$`
    ${P_IMSTATS} \
	-d 6 /$1/$2/$2_${CHIP}.fits \
	-s ${statminsection_imstats} \
	-t ${thresh} 1e8 \
	>> ${TEMPDIR}/immode_new.dat_$$
    k=$((${k}+1))
done

# create the new directory for the normalized images if
# it does not exist already

if [ ! -d "/$1/$2_norm" ]; then
  mkdir "/$1/$2_norm"
fi

MODES=`${P_GAWK} '($1!="#") {printf ("%f ", $2)}' ${TEMPDIR}/immode_new.dat_$$`

# Set the threshold to 20% of the smallest mode value

THRESH=`${P_GAWK} '($1!="#") {print $2}' ${TEMPDIR}/immode_new.dat_$$ |\
        ${P_GAWK} 'BEGIN{min=1000000} {if ($1<min) min=$1} END {print 0.2*min}'`

i=1
for CHIP in $3
do
  ACTUMODE=`echo ${MODES} | ${P_GAWK} '{print $'${i}'}'`

  if [ -L "/$1/$2/$2_${CHIP}.fits" ]; then
    LINK=`${P_READLINK} /$1/$2/$2_${CHIP}.fits`
    RESULTDIR=`dirname ${LINK}`
    ln -s ${RESULTDIR}/$2_norm_${CHIP}.fits /$1/$2_norm/$2_norm_${CHIP}.fits
  else
    RESULTDIR=$1/$2_norm/
  fi

  # if a pixel is below the threshold, then set it to zero
  ${P_IC} -p -32 '%1 '${ACTUMODE}' / 0.0 %1 '${THRESH}' > ?' /$1/$2/$2_${CHIP}.fits > \
          ${RESULTDIR}/$2_norm_${CHIP}.fits

  i=$(( $i + 1 ))
done
