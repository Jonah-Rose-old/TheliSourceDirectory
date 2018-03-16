BASHPATH -xv

# $1: main directory
# $2: science directory
# $3: extension
# $4: chips to be processed

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

for CHIP in $4
do
  RESULTDIR[${CHIP}]="$1/WEIGHTS"
done

for CHIP in $4
do
  ls -1 /$1/$2/*_${CHIP}$3.fits > ${TEMPDIR}/smoothedge_${CHIP}_$$

  FILE=`${P_GAWK} '(NR==1) {print $0}' ${TEMPDIR}/smoothedge_${CHIP}_$$`

  if [ -L ${FILE} ]; then
      LINK=`${P_READLINK} ${FILE}`
      RESULTDIR[${CHIP}]=`dirname ${LINK}`
  else
      RESULTDIR[${CHIP}]="$1/WEIGHTS"
  fi
  
  cat ${TEMPDIR}/smoothedge_${CHIP}_$$ |\
  {
    while read file
    do
      BASE=`basename ${file} .fits`
      
      ${P_FITSSMOOTHEDGE} -i $1/WEIGHTS/${BASE}.weight.fits \
	  -o ${RESULTDIR[${CHIP}]}/${BASE}.weightsmooth.fits \
	  -n ${V_COADD_SMOOTHEDGE}
      
      if [ "${RESULTDIR[${CHIP}]}" != "$1/WEIGHTS" ]; then
	  ln -s ${RESULTDIR[${CHIP}]}/${BASE}.weightsmooth.fits \
	      $1/WEIGHTS/${BASE}.weightsmooth.fits
      fi
    done
  }
done
