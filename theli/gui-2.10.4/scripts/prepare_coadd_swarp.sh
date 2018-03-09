#!/bin/bash -xv

# preliminary work:
INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash.include

# default values:

CHIPS=""

i=1
while [ "${i}" -le "${NCHIPS}" ]
do
  CHIPS="${CHIPS} ${i}"
  i=$(( $i + 1 ))
done

MAIN=$1
SCIENCE=$2
EXTEN=$3
WBASE=".sub"
SCRIPT="default"
CONDITION=""
HEADER=""
RA=${V_COADD_REFRA}
DEC=${V_COADD_REFDEC}
WEIGHTDIR="WEIGHTS"
PSCALE=${PIXSCALE}

# first of all, check if sky subtracted images are present
# if not, strip the ".sub" string from the extension
ls /${MAIN}/${SCIENCE}/*${EXTEN}.fits > ${TEMPDIR}/skysublist_$$
numimages=`${P_GAWK} 'END {print NR}' ${TEMPDIR}/skysublist_$$`
if [ "${numimages}" = "0" ]; then
    EXTEN=`basename ${EXTEN} .sub`
    WBASE=""
fi

if [ ! "${V_COADD_IDENT}_A" = "_A" ]; then
    SCRIPT=${V_COADD_IDENT}
fi

if [ ! "${V_COADD_CHIPS}_A" = "_A" ]; then
    CHIPS=${V_COADD_CHIPS}
fi

if [ ! "${V_COADD_PIXSCALE}_A" = "_A" ]; then
    PSCALE=${V_COADD_PIXSCALE}
fi

# weight images are assumed to reside in 
# /${MAIN}/${WEIGHTDIR}

# create links of the science and weight frames:
DIR=`pwd`
USERDIR=~/.theli/scripts/

cd /${MAIN}/${SCIENCE}/

# create coadd directory if it does not yet exist
if [ ! -d coadd_${SCRIPT} ]; then
  mkdir coadd_${SCRIPT}
else
  # in case that the coaddition directory already
  # exists we assume that we want to REDO it.
  # Hence all old files are deleted.
  \rm -f ./coadd_${SCRIPT}/*
fi

test -f ${USERDIR}/coaddimages && \rm ${USERDIR}/coaddimages
test -f ${TEMPDIR}/coaddimages_$$ && \rm ${TEMPDIR}/coaddimages_$$

if [ "${V_COADD_SEEING}" != "" ] && [ "${V_COADD_RZP}" = "" ]; then
    CONDITION="(SEEING<${V_COADD_SEEING});"
fi
if [ "${V_COADD_SEEING}" = "" ] && [ "${V_COADD_RZP}" != "" ]; then
    CONDITION="(RZP>${V_COADD_RZP});"
fi
if [ "${V_COADD_SEEING}" != "" ] && [ "${V_COADD_RZP}" != "" ]; then
    CONDITION="((SEEING<${V_COADD_SEEING})AND(RZP>${V_COADD_RZP}));"
fi

if [ "${CONDITION}_A" = "_A" ]; then
    for chip in ${CHIPS}
    do
	ls -1 /${MAIN}/${SCIENCE}/*_${chip}${EXTEN}.fits >> ${TEMPDIR}/coaddimages_$$
    done
else
    ${P_LDACFILTER} -i /${MAIN}/${SCIENCE}/cat/chips.cat5 -t STATS -c "${CONDITION}" \
        -o /${MAIN}/${SCIENCE}/cat/${SCRIPT}.cat -m ${SCRIPT}
    
    if [ "$?" -gt "0" ]; then
	echo "Error in filtering list catalog; exiting"
	exit 1;
    fi
    
    ${P_LDACFILTER} -i /${MAIN}/${SCIENCE}/cat/${SCRIPT}.cat -t STATS \
        -c "(${SCRIPT}=1);" -o ${TEMPDIR}/tmp.cat_$$
    ${P_LDACTOASC} -i ${TEMPDIR}/tmp.cat_$$ -t STATS -b -s \
        -k IMAGENAME > ${TEMPDIR}/tmp.asc_$$
    
    cat ${TEMPDIR}/tmp.asc_$$ |\
    {
      while read FILE
      do
        for chip in ${CHIPS}
        do
	  # only use the image if it is also present in the SCIENCE directory
	  if [ -f "/${MAIN}/${SCIENCE}/${FILE}_${chip}${EXTEN}.fits" ]; then
	      echo "/${MAIN}/${SCIENCE}/${FILE}_${chip}${EXTEN}.fits" >> \
		  ${TEMPDIR}/coaddimages_$$
	  fi
        done
      done
    }
    rm ${TEMPDIR}/tmp.asc_$$ ${TEMPDIR}/tmp.cat_$$
fi

# stack images with a certain FILTER keyword only
exec < ${TEMPDIR}/coaddimages_$$
while read file
do
    FILTER=`get_key ${file} FILTER | \
        sed -e 's/\]//g' -e 's/\[//g' | \
	sed 's/[*# ;()\$\^\/]//g'`
    if [ ! "${V_COADD_FILTER}" = "All" ]; then
	if [ "${FILTER}" = "${V_COADD_FILTER}" ]; then
	    echo ${file} >> ${USERDIR}/coaddimages
	fi
    else
	echo ${file} >> ${USERDIR}/coaddimages
    fi
done

test -f ${TEMPDIR}/coaddimages_$$ && rm ${TEMPDIR}/coaddimages_$$ 

# get median RA and DEC for the coaddition centre
# if RA and DEC are not provided

if [ "${RA}_A" = "_A" ] || [ "${DEC}_A" = "_A" ]; then
  test -f ${TEMPDIR}/coaddcoord1_$$ && rm ${TEMPDIR}/coaddcoord1_$$
  test -f ${TEMPDIR}/coaddcoord2_$$ && rm ${TEMPDIR}/coaddcoord2_$$
  exec < ${USERDIR}/coaddimages
  while read file
  do
    dir=`dirname ${file}`"/headers/"
    base=`basename ${file} ${EXTEN}.fits`
    # the centre of the image
    n1=`get_key ${file} NAXIS1 | ${P_GAWK} '{print 0.5*$1}'`
    n2=`get_key ${file} NAXIS2 | ${P_GAWK} '{print 0.5*$1}'`
    # astrometric keywords
    crval1=`grep CRVAL1 ${dir}/${base}.head | ${P_GAWK} '{print $3}'`
    crval2=`grep CRVAL2 ${dir}/${base}.head | ${P_GAWK} '{print $3}'`
    crpix1=`grep CRPIX1 ${dir}/${base}.head | ${P_GAWK} '{print $3}'`
    crpix2=`grep CRPIX2 ${dir}/${base}.head | ${P_GAWK} '{print $3}'`
    cd11=`grep CD1_1 ${dir}/${base}.head | ${P_GAWK} '{print $3}'`
    cd12=`grep CD1_2 ${dir}/${base}.head | ${P_GAWK} '{print $3}'`
    cd21=`grep CD2_1 ${dir}/${base}.head | ${P_GAWK} '{print $3}'`
    cd22=`grep CD2_2 ${dir}/${base}.head | ${P_GAWK} '{print $3}'`
    # calculate zero order reference coordinate for image centers
    ra=`echo ${crval1} ${n1} ${crpix1} ${cd11} ${n2} ${crpix2} ${cd12} | \
	${P_GAWK} '{print $1 + ($2-$3)*$4 + ($5-$6)*$7}'`
    dec=`echo ${crval2} ${n1} ${crpix1} ${cd21} ${n2} ${crpix2} ${cd22} | \
	${P_GAWK} '{print $1 + ($2-$3)*$4 + ($5-$6)*$7}'`
    echo ${ra} >> ${TEMPDIR}/coaddcoord1_$$
    echo ${dec} >> ${TEMPDIR}/coaddcoord2_$$
  done
  # calculate median RA and DEC
  ramedian=`${P_GETSTATS} -i ${TEMPDIR}/coaddcoord1_$$ -m median`
  decmedian=`${P_GETSTATS} -i ${TEMPDIR}/coaddcoord2_$$ -m median`
  RA=`${P_DECIMALTOHMS} ${ramedian}`
  DEC=`${P_DECIMALTODMS} ${decmedian}`
fi

cat ${USERDIR}/coaddimages |\
{
  while read file
  do
    BASE=`basename ${file}`
    BASEWEIGHT=`basename ${file} ${WBASE}.fits`

    ln -s ${file} ./coadd_${SCRIPT}/${BASE}

    # stack using smoothed or unsmoothed weights
    if [ ${V_COADD_SMOOTHEDGE}_A = "_A" ]; then
	ln -s /${MAIN}/${WEIGHTDIR}/${BASEWEIGHT}.weight.fits \
	    ./coadd_${SCRIPT}/${BASEWEIGHT}${WBASE}.weight.fits
    else
	ln -s /${MAIN}/${WEIGHTDIR}/${BASEWEIGHT}.weightsmooth.fits \
	    ./coadd_${SCRIPT}/${BASEWEIGHT}${WBASE}.weight.fits
    fi
    HEADBASE=`basename ${file} ${EXTEN}.fits`
    if [ -f /${MAIN}/${SCIENCE}/headers/${HEADBASE}.head ]; then

	# if proper motion is not requested
	if [ "${V_COADD_PROPMOTIONRA}" = "" ] && [ "${V_COADD_PROPMOTIONDEC}" = "" ]; then 
	    cp /${MAIN}/${SCIENCE}/headers/${HEADBASE}.head \
		./coadd_${SCRIPT}/${HEADBASE}${EXTEN}.head
	fi
	# if proper motion is requested
	if [ "${V_COADD_PROPMOTIONRA}" != "" ] && [ "${V_COADD_PROPMOTIONDEC}" != "" ]; then 
	    # calculate the speed in CRVAL1 and CRVAL2 (per second)
	    RA4prop=`${P_HMSTODECIMAL} ${RA}`
	    DEC4prop=`${P_DMSTODECIMAL} ${DEC}`
	    crval1corr=`echo ${V_COADD_PROPMOTIONRA} ${DEC4prop} |\
                            ${P_GAWK} '{printf "%.9f\n", $1/3600/cos($2*3.14159265/180.)}'`
	    crval2corr=`echo ${V_COADD_PROPMOTIONDEC} |\
                            ${P_GAWK} '{printf "%.9f\n",  $1/3600}'`
	    
	    # starting time (first image in the list)
	    firstimage=`${P_GAWK} '(NR==1)' ${USERDIR}/coaddimages`
	    timezero=`${P_DFITS} ${firstimage} | ${P_FITSORT} -d MJD-OBS | ${P_GAWK} '{print $2}'`
	    timenow=`${P_DFITS} ${file} | ${P_FITSORT} -d MJD-OBS | ${P_GAWK} '{print $2}'`

	    if [ "${timezero}" = "KEY_N/A" ] || [ "${timenow}" = "KEY_N/A" ]; then
		echo `${P_ERRTEST} 0` "Could not find MJD-OBS keyword"
		exit
	    fi

	    timediff=`echo ${timezero} ${timenow} | ${P_GAWK} '{printf "%.9f\n", ($2-$1)*1440}'`
	    
	    # angular offset
	    delta_crval1=`echo ${timediff} ${crval1corr} | ${P_GAWK} '{printf "%.9f\n", $1*$2}'`
	    delta_crval2=`echo ${timediff} ${crval2corr} | ${P_GAWK} '{printf "%.9f\n", $1*$2}'`
	    
	    # correct header
	    # DO NOT wrap around the following two lines with "\" (fails!)
	    echo ${delta_crval1} ${delta_crval2} > ./coadd_${SCRIPT}/${HEADBASE}${EXTEN}.head_tmp
	    cat /${MAIN}/${SCIENCE}/headers/${HEADBASE}.head >> \
		./coadd_${SCRIPT}/${HEADBASE}${EXTEN}.head_tmp
	    ${P_GAWK} '{
                   if (NR==1) {delta_crval1 = $1; delta_crval2 = $2}
                   if (NR>1 && $1 != "CRVAL1" && $1 != "CRVAL2") {print}
                   if ($1 == "CRVAL1") {printf "%s%.9e\n", "CRVAL1  = ",$3+delta_crval1}
                   if ($1 == "CRVAL2") {printf "%s%.9e\n", "CRVAL2  = ",$3+delta_crval2}
                }' ./coadd_${SCRIPT}/${HEADBASE}${EXTEN}.head_tmp \
		    > ./coadd_${SCRIPT}/${HEADBASE}${EXTEN}.head
	    \rm ./coadd_${SCRIPT}/${HEADBASE}${EXTEN}.head_tmp
	fi
    else
	echo "/${MAIN}/${SCIENCE}/headers/${HEADBASE}.head not present; Aborting"
	exit 1;
    fi
  done
}

#
cd ./coadd_${SCRIPT}

SWARPCONF=${DATACONF}/create_coadd_swarp.swarp

#SWARPCONF=${DATACONF}/create_coadd_swarp2.swarp
#P_SWARP=${BIN}/swarp_theli2

# run swarp to get output header

COADDSIZE=""
if [ "${V_COADD_SIZEX}_A" != "_A" ] && [ "${V_COADD_SIZEY}_A" != "_A" ]; then
    COADDSIZE="-IMAGE_SIZE ${V_COADD_SIZEX},${V_COADD_SIZEY}"
fi

${P_SWARP} -c ${SWARPCONF} \
    *${EXTEN}.fits \
    -CENTER ${RA},${DEC} \
    -CENTER_TYPE MANUAL -HEADER_ONLY Y -PIXELSCALE_TYPE MANUAL \
    -PIXEL_SCALE ${PSCALE} \
    -PROJECTION_TYPE ${V_COADD_PROJECTION} \
    -CELESTIAL_TYPE ${V_COADD_CELESTIALTYPE} \
    -RESAMPLING_TYPE ${V_COADD_KERNEL} ${COADDSIZE}

fold coadd.fits | grep -v "^PV" > ${USERDIR}/coadd.head

rm coadd.fits

# multiply a rotation matrix onto the CD matrix if requested by the user

if [ ! "${V_COADD_SKYPOSANGLE}_A" = "_A" ]; then

    # get the initial rotation angle
    k=0
    exec < ${USERDIR}/coaddimages
    while read file
    do
	dir=`dirname ${file}`"/headers/"
	base=`basename ${file} ${EXTEN}.fits`
	cd11init=`grep CD1_1 ${dir}/${base}.head | ${P_GAWK} '{print $3}'`
	cd12init=`grep CD1_2 ${dir}/${base}.head | ${P_GAWK} '{print $3}'`
	cd21init=`grep CD2_1 ${dir}/${base}.head | ${P_GAWK} '{print $3}'`
	cd22init=`grep CD2_2 ${dir}/${base}.head | ${P_GAWK} '{print $3}'`
	V_COADD_SKYPOSANGLE_INIT=`${P_GETPOSANGLE} -c ${cd11init} ${cd12init} ${cd21init} ${cd22init}`
	V_COADD_SKYPOSANGLE_INIT=`echo ${V_COADD_SKYPOSANGLE_INIT} | gawk '{print -1.*$1}'`
	# break after the first image
	if [ "${k}" = "0" ]; then
	    break
	fi
    done
    cd11=`grep CD1_1 ${USERDIR}/coadd.head | ${P_GAWK} '{print $3}'`
    cd22=`grep CD2_2 ${USERDIR}/coadd.head | ${P_GAWK} '{print $3}'`
    cd12=0.0
    cd21=0.0
    nax1=`grep NAXIS1 ${USERDIR}/coadd.head | ${P_GAWK} '{print $3}'`
    nax2=`grep NAXIS2 ${USERDIR}/coadd.head | ${P_GAWK} '{print $3}'`

    axes=`${P_GET_ROTIMSIZE} -g ${nax1} ${nax2} -i ${V_COADD_SKYPOSANGLE_INIT} -s ${V_COADD_SKYPOSANGLE}`

    newmatrix=`${P_ROTATECDMATRIX} -c ${cd11} ${cd12} ${cd21} ${cd22} -l -p ${V_COADD_SKYPOSANGLE}`
    echo ${newmatrix} ${axes} > ${TEMPDIR}/coadd2.head_$$
    cat ${USERDIR}/coadd.head >> ${TEMPDIR}/coadd2.head_$$
    
    ${P_GAWK} '{if (NR==1) {cd11=$1;cd12=$2;cd21=$3;cd22=$4;nax1=$5;nax2=$6}
                if (NR>1) {
                  if ($1!~/CD1_1/ && $1!~/CD1_2/ && $1!~/CD2_1/ && $1!~/CD2_2/ && $1!~/NAXIS/) {print}
                  if ($1 == "NAXIS1") print "NAXIS1  = "nax1"                                                                                "
                  if ($1 == "NAXIS2") print "NAXIS2  = "nax2"                                                                                "
                  if ($1 == "CRPIX1") print "CRPIX1  = "int(nax1/2)"                                                                                "
                  if ($1 == "CRPIX2") print "CRPIX2  = "int(nax2/2)"                                                                                "
                  if ($1 == "CD1_1") print "CD1_1   = "cd11"                                                                                "
                  if ($1 == "CD1_2") print "CD1_2   = "cd12"                                                                                "
                  if ($1 == "CD2_1") print "CD2_1   = "cd21"                                                                                "
                  if ($1 == "CD2_2") print "CD2_2   = "cd22"                                                                                "
                }}' ${TEMPDIR}/coadd2.head_$$ > ${TEMPDIR}/coadd3.head_$$

    # truncate the new header file so that it has not more than 80 characters per line

    ${P_GAWK} '{print substr($0,1,80)}' ${TEMPDIR}/coadd3.head_$$ > ${USERDIR}/coadd.head
fi

cp ${USERDIR}/coadd.head .
