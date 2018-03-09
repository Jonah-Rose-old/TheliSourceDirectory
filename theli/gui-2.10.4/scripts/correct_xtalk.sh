#!/bin/bash -xv

# corrects inter- and intrachip crosstalk,
# both normal crosstalk and row crosstalk

# $1: main directory
# $2: science directory

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

dir=`pwd`

cd /$1/$2

for CHIP in $3
do
  FILES=`ls -1 *_${CHIP}.fits`

  for FILE in ${FILES}
  do
    # normal crosstalk correction
    if [ "${V_PRE_XTALK_NOR_CHECKED}" = "1" ]; then
	V_PRE_XTALK_NOR_BUTTONID_TMP=$((${V_PRE_XTALK_NOR_BUTTONID}+1))
	${P_CROSSTALK} -i ${FILE} -v ${V_PRE_XTALK_NOR_AMPLITUDE} \
	    -m ${V_PRE_XTALK_NOR_BUTTONID_TMP}
    fi
    # row crosstalk correction
    if [ "${V_PRE_XTALK_ROW_CHECKED}" = "1" ]; then
	V_PRE_XTALK_ROW_BUTTONID_TMP=$((${V_PRE_XTALK_ROW_BUTTONID}+4))
	${P_CROSSTALK} -i ${FILE} -v ${V_PRE_XTALK_ROW_AMPLITUDE} \
	    -m ${V_PRE_XTALK_ROW_BUTTONID_TMP}
    fi
    # multi crosstalk correction
    if [ "${V_PRE_XTALK_MULTI_CHECKED}" = "1" ]; then
	if [ "${V_PRE_XTALK_MULTI_BUTTONID}" = "0" ]; then
	    DIRECTION="x"
	fi
	if [ "${V_PRE_XTALK_MULTI_BUTTONID}" = "1" ]; then
	    DIRECTION="y"
	fi
	${P_CROSSTALK} -i ${FILE} -m 10 -h ${DIRECTION} ${V_PRE_XTALK_MULTI_NSECTION}
    fi
  done
done

cd ${dir}
