#!/bin/bash

# $1: number of chips
# $2: number of processors
# $3: current chip

. progs.ini

NCHIPS=$1
NPARA=$2
CURRENTCHIP=$3

i=1
while [ "${i}" -le "${NCHIPS}" ]
do
  PARA[$i]=""
  i=$(( $i + 1 ))
done

# now divide the chips to the processors:

k=1
while [ "${k}" -le "${NPARA}" ]
do
  ACTUPROC=1
  while [ "${ACTUPROC}" -le "${NCHIPS}" ] && [ "${k}" -le "${NPARA}" ]
  do
    PARA[${ACTUPROC}]="${PARA[${ACTUPROC}]} ${k}"
    k=$(( $k + 1 ))
    ACTUPROC=$(( $ACTUPROC + 1 ))
  done
done

k=1
nthread=""
while [ "${k}" -le "${NCHIPS}" ]
do
    numthread=`echo ${PARA[${k}]} | ${P_GAWK} '{print NF}'`
    if [ "${numthread}" = "0" ]; then
	numthread=1
    fi
    nthread="${nthread} ${numthread}"
    k=$(( $k + 1 ))
done

final=`echo ${nthread} | ${P_GAWK} '{print $'"${CURRENTCHIP}"'}'`
echo ${final}
