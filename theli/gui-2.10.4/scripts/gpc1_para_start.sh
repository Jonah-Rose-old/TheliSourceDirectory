#!/bin/bash -xv

# $1: dir with GPC1 raw chip images

INSTDIR=instruments_professional
. ${INSTDIR}/GPC1@PS1.ini >& /dev/null

# do some initialisations

NPARA=4

dir=`pwd`

cd $1

test -f rawdata && rm rawdata

${P_FIND} . -maxdepth 1 -name \*.XY??.ch.fits | ${P_SORT} > rawdata

i=1
while [ "${i}" -le "${NPARA}" ]
do
    test -f rawdata_set${i} && rm rawdata_set${i}
    ${P_GAWK} 'BEGIN {
          npara="'${NPARA}'"
          nthread="'${i}'"-1} 
          {name[NR] = $1} END {
           s=int(NR/npara)+1; 
           smin=s*nthread; 
           smax=s*(nthread+1); 
           for (i=1;i<=NR;i++) {if (i>=smin && i<smax) print name[i]} }' \
	 rawdata > rawdata_set${i}
    i=$(( $i + 1 ))
done

cd ${dir}

# START THE SCRIPTS

SCRIPT=process_split_GPC1_ROE.sh

k=1
while [ "${k}" -le "${NPARA}" ]
do
  ( ./${SCRIPT} $1 rawdata_set${k} ) &
  k=$(( $k + 1 ))
done

# wait until all subprocesses have finished
wait

exit 0;
