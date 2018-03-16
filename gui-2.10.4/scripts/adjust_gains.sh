BASHPATH -xv

# adjusts different gains

# $1: main dir
# $2: science dir
# $3: "OFCS" etc...

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash.include

cd /$1/$2

ls *_1$3.fits > ${TEMPDIR}/gain.list_$$

cat ${TEMPDIR}/gain.list_$$ |\
{
  while read file
  do
    BASE=`basename ${file} _1$3.fits`
    ${P_IMSTATS} -d 6 -t -1e8 1e8 ${BASE}_*$3.fits | ${P_GAWK} '($1 != "#") {print $1, $2}' |\ 
    ${P_GAWK} 'BEGIN {max=0} {name[NR]=$1; mode[NR]=$2; if ($2>max) max=$2} END {i=0; while (i<NR) {i++; print name[i], mode[i]/max}}' > ${TEMPDIR}/gaincorr.list_$$

    cat ${TEMPDIR}/gaincorr.list_$$ |\
    {
      while read file factor
      do
        BASE1=`basename ${file} .fits`
	${P_IC} '%1 '"${factor}"' /' ${file} > ${BASE1}_mod.fits
        mv ${BASE1}_mod.fits ${file}
      done
    }
  done
}
