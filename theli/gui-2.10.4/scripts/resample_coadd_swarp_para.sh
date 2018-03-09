#!/bin/bash -xv

# this script performs resampling of data with swarp
# preparing the final coaddition. It can perform
# its task in parallel mode.

#$1: main dir.
#$2: science dir.
#$3: image extension (ext) on ..._iext.fits (i is the chip number)

# preliminary work:
INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

PSCALE=${PIXSCALE}
if [ ! "${V_COADD_PIXSCALE}_A" = "_A" ]; then
    PSCALE=${V_COADD_PIXSCALE}
fi

EXTEN=$3

SCRIPTDIR=`pwd`

# first of all, check if sky subtracted images are present.
# if not, strip the ".sub" string from the extension
ls /$1/$2/*${EXTEN}.fits > ${TEMPDIR}/skysubresamplist_$$
numimages=`${P_GAWK} '{print $NR}' ${TEMPDIR}/skysubresamplist_$$`
if [ "${numimages}_A" = "_A" ]; then
    EXTEN=`basename ${EXTEN} .sub`
fi

IDENT=${V_COADD_IDENT}_A

if [ "${IDENT}" = "_A" ]; then
   IDENT=default
else
   IDENT=${V_COADD_IDENT}
fi

# which chips to coadd
CHIPS=$4
if [ ! "${V_COADD_CHIPS}_A" = "_A" ]; then
    CHIPS=""
    for EXTCHIP in $4
    do
	for INTCHIP in ${V_COADD_CHIPS}
	do
	    if [ "${INTCHIP}" = "${EXTCHIP}" ]; then
		CHIPS="${CHIPS} ${INTCHIP}"
	    fi
	done
    done
fi

if [ "${CHIPS}_A" = "_A" ]; then
    # nothing to do for this thread
    exit
fi

for CHIP in $CHIPS
do
  RESULTDIR[${CHIP}]="$1/$2/coadd_${IDENT}"
done

for CHIP in $CHIPS
do
  FILES=`ls $1/$2/coadd_${IDENT}/*_${CHIP}${EXTEN}.fits`

  # how many CPUs shall we use?
  cd ${SCRIPTDIR}
  NUMTHREAD=`${SCRIPTDIR}/get_nthreads.sh ${NCHIPS} ${NPARA} ${CHIP}`

  # NUMTHREAD=8

  # if the user removed a few chips, then continue
  # (will produce a mmep runtime e-r-r-o-r otherwise)
  if [ "${FILES}_A" = "_A" ]; then
      continue
  fi

  for FILE in ${FILES}
  do
     LINK=`${P_READLINK} ${FILE}`
     if [ -L ${LINK} ]; then
	LINKREAL=`${P_READLINK} ${LINK}` 
        BASE=`basename ${LINKREAL} .fits`
        DIR=`dirname ${LINKREAL}`
        ln -s ${DIR}/${BASE}.${IDENT}.resamp.fits \
	    $1/$2/coadd_${IDENT}/${BASE}.${IDENT}.resamp.fits
        ln -s ${DIR}/${BASE}.${IDENT}.resamp.weight.fits \
	    $1/$2/coadd_${IDENT}/${BASE}.${IDENT}.resamp.weight.fits
        RESULTDIR[${CHIP}]=`dirname ${LINKREAL}`    
     fi
  done 

  cd ${RESULTDIR[${CHIP}]}

  USERDIR=~/.theli/scripts/

#  if [ ! -f coadd.head ]; then
#      cp ${USERDIR}/coadd.head .
#  fi

  SWARPCONF=${DATACONF}/create_coadd_swarp2.swarp

  ${P_SWARP} -c ${SWARPCONF} \
      ${FILES} -RESAMPLE Y -COMBINE N \
      -RESAMPLE_SUFFIX .${IDENT}.resamp.fits \
      -RESAMPLE_DIR . \
      -PIXEL_SCALE ${PSCALE} \
      -NTHREADS ${NUMTHREAD} \
      -VERBOSE_TYPE QUIET \
      -WEIGHT_TYPE MAP_WEIGHT \
      -CELESTIAL_TYPE ${V_COADD_CELESTIALTYPE} \
      -RESAMPLING_TYPE ${V_COADD_KERNEL} \
      -COMBINE_TYPE ${V_COADD_COMBINETYPE} \
      -PROJECTION_TYPE ${V_COADD_PROJECTION} \
      -RESCALE_WEIGHTS ${V_COADD_RESCALEWEIGHTS} \
      -COPY_KEYWORDS "DATE-OBS"

  #  test -f coadd.head && rm coadd.head

  cd ${DIR}
done  
