BASHPATH -xv

# $1: main dir.
# $2: science dir
# $3: extension

. progs.ini

. bash_functions.include

. bash.include

# exit if no restored headers exist
if [ ! -d /$1/$2/.origheader_backup ]; then
    echo "No backup headers exist. Exiting."
    exit
fi

cd /$1/$2/.origheader_backup
ls *.head > ${TEMPDIR}/headerlist_$$

exec < ${TEMPDIR}/headerlist_$$
while read header
do
    BASE=`basename ${header} .origastrom.head`
    IMAGE=$1/$2/${BASE}.fits

    # skip the process if the image does not exist
    if [ ! -f ${IMAGE} ]; then
	continue
    fi

    # restore the header only if it has been modified previously;
    # otherwise jump to the next image
    updated=`get_key ${IMAGE} ZEROHEAD`
    if [ "${updated}" != "YES" ]; then
	continue
    fi

    # update the header
    crval1=`grep CRVAL1 ${header} | ${P_GAWK} '{print $3}'`
    crval2=`grep CRVAL2 ${header} | ${P_GAWK} '{print $3}'`
    crpix1=`grep CRPIX1 ${header} | ${P_GAWK} '{print $3}'`
    crpix2=`grep CRPIX2 ${header} | ${P_GAWK} '{print $3}'`
    cd11=`grep CD1_1 ${header} | ${P_GAWK} '{print $3}'`
    cd12=`grep CD1_2 ${header} | ${P_GAWK} '{print $3}'`
    cd21=`grep CD2_1 ${header} | ${P_GAWK} '{print $3}'`
    cd22=`grep CD2_2 ${header} | ${P_GAWK} '{print $3}'`
    ${P_REPLACEKEY} ${IMAGE} "CRVAL1  = ${crval1}" CRVAL1
    ${P_REPLACEKEY} ${IMAGE} "CRVAL2  = ${crval2}" CRVAL2
    ${P_REPLACEKEY} ${IMAGE} "CRPIX1  = ${crpix1}" CRPIX1
    ${P_REPLACEKEY} ${IMAGE} "CRPIX2  = ${crpix2}" CRPIX2
    ${P_REPLACEKEY} ${IMAGE} "CD1_1   = ${cd11}" CD1_1
    ${P_REPLACEKEY} ${IMAGE} "CD1_2   = ${cd12}" CD1_2
    ${P_REPLACEKEY} ${IMAGE} "CD2_1   = ${cd21}" CD2_1
    ${P_REPLACEKEY} ${IMAGE} "CD2_2   = ${cd22}" CD2_2

    # indicate that the header has not been updated
    value "'NO'"
    writekey ${IMAGE} \
	ZEROHEAD "${VALUE} / Astrometric header update" REPLACE
done
