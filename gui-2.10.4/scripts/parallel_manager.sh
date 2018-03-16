BASHPATH

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

# do some initialisations

if [ ${NPARA} -gt ${NCHIPS} ]; then
  NPARA=${NCHIPS}
fi

i=1
while [ "${i}" -le "${NPARA}" ]
do
  PARA[$i]=""
  i=$(( $i + 1 ))
done

# now divide the chips to the processors:
k=1
while [ ${k} -le ${NCHIPS} ]
do
  ACTUPROC=1
  while [ ${ACTUPROC} -le ${NPARA} ]
  do
    if [ ${k} -le ${NCHIPS} ]; then
      PARA[${ACTUPROC}]="${PARA[${ACTUPROC}]} ${k}"
    fi

    k=$(( $k + 1 ))
    ACTUPROC=$(( $ACTUPROC + 1 ))
  done
done


# finally start all the processes

SCRIPT=$1
shift
ARGS=$*

k=1
while [ "${k}" -le "${NPARA}" ]
do
  ( ./${SCRIPT} ${ARGS} "${PARA[${k}]}" ) &
  k=$(( $k + 1 ))
done

# wait until all subprocesses have finished
wait

exit 0;
