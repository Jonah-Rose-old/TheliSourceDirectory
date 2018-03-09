#!/bin/bash -xv

# $1: main dir.
# $2: science dir
# $3: extension
# $4: chip

DIR=`pwd`

INSTDIR=instruments_professional
PREFIXDIR=${DIR}
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
    PREFIXDIR=${DIR}
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
    PREFIXDIR=""
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

USERDIR=~/.theli/instruments_user/

#############################################################
# Delete old solutions and re-create the directories
#############################################################

cd /$1/$2/

mkdir -p astrom_photom_anet

cd astrom_photom_anet
test -d cat && rm -rf cat
test -d headers && rm -rf headers
mkdir -p cat
mkdir -p backup
mkdir -p headers
cd ..

test -d headers_anet && rm -rf headers_anet
test -d headers && rm -rf headers
test -d plots && rm plots/*
mkdir headers_anet headers

########################################################
# Link the previously created composite catalogs.
# Use only those for which images are currently 
# present in the SCIENCE directory
########################################################

for CHIP in $4
do
    ls -1 /$1/$2/cat/*_${CHIP}$3.anet > ${TEMPDIR}/anetcatalogs_$$
    exec < ${TEMPDIR}/anetcatalogs_$$
    while read file
    do
	BASE=`basename $file _${CHIP}$3.anet`
	image=`${P_FIND} /$1/$2/ -maxdepth 1 -name ${BASE}_${CHIP}$3.fits`
	if [ -n "${image}" ]; then
	    ln -s ${file} /$1/$2/astrom_photom_anet/cat/${BASE}_${CHIP}$3.anet
	fi
    done
done

cd astrom_photom_anet/cat

# check if any catalogs are present
ncat=`ls | grep "\.anet" | wc | ${P_GAWK} '{print $1}'`
if [ "${ncat}" = "0" ]; then
    ${P_ERRTEST} 0
    echo "No astrometry.net catalogs found! Did you create the source catalogs?"
    exit
fi


#***************************************
# now run astrometry.net
#***************************************

for CHIP in $4
do
    ls -1 *_${CHIP}$3.anet > ${TEMPDIR}/anet.catlist_$$
    sed 's/\.anet/\.wcs/g' ${TEMPDIR}/anet.catlist_$$ > ${TEMPDIR}/anet.wcslist_$$

    filesin=`${P_GAWK} '{a[NR]=$1} 
              END {for (i=1;i<=NR;i=i+1) printf "%s ",a[i]; printf "\n"}' \
		  ${TEMPDIR}/anet.catlist_$$`
    filesout=`${P_GAWK} '{a[NR]=$1} 
              END {for (i=1;i<=NR;i=i+1) printf "%s ",a[i]; printf "\n"}' \
		  ${TEMPDIR}/anet.wcslist_$$`

    pixscalemin=`echo ${PIXSCALE} ${V_ANET_MAXSCALE} | ${P_GAWK} '{print $1/$2}'`
    pixscalemax=`echo ${PIXSCALE} ${V_ANET_MAXSCALE} | ${P_GAWK} '{print $1*$2}'`

    exec < ${TEMPDIR}/anet.catlist_$$
    while read file
    do
	base=`basename ${file} $3.anet`
	${P_SOLVEFIELD} \
	    -w ${SIZEX[${CHIP}]} \
	    -e ${SIZEY[${CHIP}]} \
	    -O -p -u app \
	    -L ${pixscalemin} \
	    -H ${pixscalemax} \
	    -l 5 \
	    -2 \
	    -T \
	    -s MAG \
	    -a \
	    -b ${DATACONF}/anet.backend.cfg \
	    -R none -M none -B none -S none \
	    -W ${TEMPDIR}/${base}.wcs \
	    ${file}
	    
	${P_DFITS} ${TEMPDIR}/${base}.wcs > ${TEMPDIR}/${base}.head

        # keep only scamp-style header entries
	${P_GAWK} '($1!~/SIMPLE/ && $1!~/BITPIX/ && $1!~/NAXIS/ && $1!~/EXTEND/ && 
                $1!~/WCSAXES/ && $1!~/LONPOLE/ && $1!~/LATPOLE/ && 
                $1!~/IMAGEW/ && $1!~/IMAGEH/ && $1!~/A_/ && $1!~/B_/ && 
                $1!~/AP_/ && $1!~/BP_/ && $1!~/HISTORY/ && $1!~/DATE/ && 
                $1!~/COMMENT/ && $1!="END")' \
		${TEMPDIR}/${base}.head > \
		${TEMPDIR}/${base}.head2
	sed 's/-SIP/    /g' ${TEMPDIR}/${base}.head2 > \
	    ${TEMPDIR}/${base}.head3
	sed 's/ + SIP distortions//g' ${TEMPDIR}/${base}.head3 > \
	    ${TEMPDIR}/${base}.head4

	# add some missing parameters
	{
	    echo "PV1_0   = 0.0                 / Astrometric system"
	    echo "PV2_0   = 0.0                 / Astrometric system"
	    echo "PV1_1   = 1.0                 / Astrometric system"
	    echo "PV2_1   = 1.0                 / Astrometric system"
	    echo "END     "
	} >> ${TEMPDIR}/${base}.head4

	mv ${TEMPDIR}/${base}.head4 /$1/$2/headers_anet/${base}.head

	rm ${TEMPDIR}/${base}.head[2,3]
    done
 
   rm *${CHIP}$3.axy *${CHIP}$3.xyls
done
