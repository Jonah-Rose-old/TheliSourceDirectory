BASHPATH -xv

# for a A1-B1-B2-A2 chop-nod pattern, this script will
# subtract images pair-wise, in a user-supplied manner
# $1: main directory (filter)
# $2: science directory
# $3: fits extension
# $4: manner
# $5: invert
# $6: chips to be processed

# preliminary work:
INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

for CHIP in $6
do
  RESULTDIR[${CHIP}]="$1/$2"
done

for CHIP in $6
do
  FILES=`ls $1/$2/*_${CHIP}$3.fits`
  ls $1/$2/*_${CHIP}$3.fits > list_${CHIP}_$$

  for FILE in ${FILES}
  do
    if [ -L ${FILE} ]; then
	 LINK=`${P_READLINK} ${FILE}`
	 BASE=`basename ${LINK} .fits`
	 DIR=`dirname ${LINK}`
	 ln -s ${DIR}/${BASE}H.fits $1/$2/${BASE}H.fits
	 RESULTDIR[${CHIP}]=`dirname ${LINK}`  
    fi  
  done 

  ${P_GAWK} 'BEGIN{k=1;i=1} {x[NR]=$1} END {
       while (i<=NR) {
         if (k<=4) {
            printf "%s ", x[i]
            k++
         }
         if (k>4) {printf "\n"; k=1}
            i++
         }
       }' list_${CHIP}_$$ >& ${TEMPDIR}/list1234_$$

  if [ ! -d /$1/$2/$3_IMAGES ]; then
      mkdir /$1/$2/$3_IMAGES
  fi

  CODE=$4
  invert=$5
  invindex=0
  out=0

  cat ${TEMPDIR}/list1234_$$ |\
  {
    while read i1 i2 i3 i4
    do
      f1=`filename ${i1}`
      f2=`filename ${i2}`
      f3=`filename ${i3}`
      f4=`filename ${i4}`
      out=0
      if [ "$CODE" = "P0110" ] && [ "${out}" = "0" ]; then
	  BASE1=`basename ${i2} .fits`
	  BASE2=`basename ${i3} .fits`
	  ${P_IC} '%1 %2 -' ${i2} ${i1} > ${RESULTDIR[${CHIP}]}/${BASE1}H.fits
	  ${P_IC} '%1 %2 -' ${i3} ${i4} > ${RESULTDIR[${CHIP}]}/${BASE2}H.fits
#	  ${P_IC} '%1 %2 - 1000. +' ${i2} ${i1} > ${RESULTDIR[${CHIP}]}/${BASE1}H.fits
#	  ${P_IC} '%1 %2 - 1000. +' ${i3} ${i4} > ${RESULTDIR[${CHIP}]}/${BASE2}H.fits
	  if [ "${invert}" = "1" ]; then
	      CODE="P1001"
	  fi
	  out=1
      fi
      if [ "$CODE" = "P1001" ] && [ "${out}" = "0" ]; then
	  BASE1=`basename ${i2} .fits`
	  BASE2=`basename ${i3} .fits`
	  ${P_IC} '%1 %2 -' ${i1} ${i2} > ${RESULTDIR[${CHIP}]}/${BASE1}H.fits
	  ${P_IC} '%1 %2 -' ${i4} ${i3} > ${RESULTDIR[${CHIP}]}/${BASE2}H.fits
#	  ${P_IC} '%1 %2 - 1000. +' ${i1} ${i2} > ${RESULTDIR[${CHIP}]}/${BASE1}H.fits
#	  ${P_IC} '%1 %2 - 1000. +' ${i4} ${i3} > ${RESULTDIR[${CHIP}]}/${BASE2}H.fits
	  if [ "${invert}" = "1" ]; then
	      CODE="P0110"
	  fi
	  out=1
      fi
      if [ "$CODE" = "P0101" ] && [ "${out}" = "0" ]; then
	  BASE1=`basename ${i2} .fits`
	  BASE2=`basename ${i4} .fits`
	  ${P_IC} '%1 %2 -' ${i2} ${i1} > ${RESULTDIR[${CHIP}]}/${BASE1}H.fits
	  ${P_IC} '%1 %2 -' ${i4} ${i3} > ${RESULTDIR[${CHIP}]}/${BASE2}H.fits
#	  ${P_IC} '%1 %2 - 1000. +' ${i2} ${i1} > ${RESULTDIR[${CHIP}]}/${BASE1}H.fits
#	  ${P_IC} '%1 %2 - 1000. +' ${i4} ${i3} > ${RESULTDIR[${CHIP}]}/${BASE2}H.fits
	  if [ "${invert}" = "1" ]; then
	      CODE="P1010"
	  fi
	  out=1
      fi
      if [ "$CODE" = "P1010" ] && [ "${out}" = "0" ]; then
	  BASE1=`basename ${i2} .fits`
	  BASE2=`basename ${i3} .fits`
	  ${P_IC} '%1 %2 -' ${i1} ${i2} > ${RESULTDIR[${CHIP}]}/${BASE1}H.fits
	  ${P_IC} '%1 %2 -' ${i3} ${i4} > ${RESULTDIR[${CHIP}]}/${BASE2}H.fits
#	  ${P_IC} '%1 %2 - 1000. +' ${i1} ${i2} > ${RESULTDIR[${CHIP}]}/${BASE1}H.fits
#	  ${P_IC} '%1 %2 - 1000. +' ${i3} ${i4} > ${RESULTDIR[${CHIP}]}/${BASE2}H.fits
	  if [ "${invert}" = "1" ]; then
	      CODE="P0101"
	  fi
	  out=1
      fi

      if [ "${RESULTDIR[${CHIP}]}" != "$1/$2" ]; then
	  ln -s ${RESULTDIR[${CHIP}]}/${BASE1}H.fits \
	      /$1/$2/${BASE1}H.fits
	  ln -s ${RESULTDIR[${CHIP}]}/${BASE2}H.fits \
	      /$1/$2/${BASE2}H.fits
      fi
      mv $1/$2/${f1} $1/$2/${f2} $1/$2/${f3} $1/$2/${f4} /$1/$2/$3_IMAGES
    done
  } 
done
