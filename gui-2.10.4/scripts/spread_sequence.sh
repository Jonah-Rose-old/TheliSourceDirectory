BASHPATH -xv

# this script selects the first $3-1 images of a sequence of $4 images
# and puts them in separate directories for processing

# $1: main dir
# $2: science dir
# $3: image extension (OFC)
# $4: number of groups
# $5: length of sequence

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

# make direcories where the data goes

i=1
while [ "${i}" -le "$4" ]
do
    mkdir /$1/$2_S${i}
    i=$(( $i + 1 ))
done

# identify the data corresponding to the groups


cd /$1/$2

CHIP=1
while [ "${CHIP}" -le "${NCHIPS}" ]
do
  echo $4 $5 > ${TEMPDIR}/imagelist_$$
  ls -1 *_${CHIP}$3.fits >> ${TEMPDIR}/imagelist_$$

  echo "BASHPATH" > ${TEMPDIR}/bla_$$
  ${P_GAWK} '{
     if (NR==1) { ngroups = $1; lseries = $2; nrow = 1; i = 1 }
     if (NR > 1) {
       if (i < ngroups) {
          print "mv " $1, "../'"$2_"'S"i
	  nrow++
	  i++
	  first = 1
       }
       if (i == ngroups) {
          if (first == 0) {
             print "mv " $1, "../'"$2_"'S"i
             nrow++
	  }
	  if (first == 1) first = 0;
          if (nrow == lseries+1) {
	     nrow = 1
	     i = 1
          }
       }
     }
     }' ${TEMPDIR}/imagelist_$$ >> ${TEMPDIR}/bla_$$

  chmod +x ${TEMPDIR}/bla_$$
  ${TEMPDIR}/bla_$$
  \rm ${TEMPDIR}/bla_$$ ${TEMPDIR}/imagelist_$$

  CHIP=$(( ${CHIP} + 1 ))
done
