#!/bin/bash -xv

# $1: main dir (filter)
# $2: Science directory
# $3: chips to be processed

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

SCRIPTDIR=`pwd`

for CHIP in $3
do
  # delete a globalweight_dummy file (region masking) if existing in the WEIGHTS directory
  # It might be for a different instrument, or the polygons might have changed
  test -e /$1/WEIGHTS/globalweight_dummy_${CHIP}.fits && rm /$1/WEIGHTS/globalweight_dummy_${CHIP}.fits

  NUMTHREAD=`${SCRIPTDIR}/get_nthreads.sh ${NCHIPS} ${NPARA} ${CHIP}`

  FILE=`ls /$1/$2/$2_${CHIP}.fits`

  if [ -L ${FILE} ]; then
      LINK=`${P_READLINK} ${FILE}`
      DIR=`dirname ${LINK}`
      RESULTDIR[${CHIP}]=`dirname ${LINK}`    
  else
      RESULTDIR[${CHIP}]="$1/$2"
  fi

  # check if there is a region file, and prep sextractor for using it
  get_region
  prepare_sex_for_region $1

  # Should the background model be smoothed?
  if [ "${V_BACK_ILLUMSMOOTH}"_A != "_A" ]; then
      # Use sextractor for large kernels
      if [ ${V_BACK_ILLUMSMOOTH} -gt 5 ]; then
	  ${P_SEX} /$1/$2/$2_${CHIP}.fits \
	      -c ${CONF}/illumfringe.sex \
	      -CHECKIMAGE_NAME ${RESULTDIR[${CHIP}]}/$2_${CHIP}_illum.fits \
	      -CHECKIMAGE_TYPE BACKGROUND \
	      -BACK_SIZE ${V_BACK_ILLUMSMOOTH} \
	      -NTHREADS ${NUMTHREAD} \
	      ${weightstring}
	  # commented out because prepare_sex_for_region forces constant background,
	  # which is NOT wanted here (we want a model)
	  # ${backstring} \
      else
	  # use a median filter for very small kernels
	  ${P_FITSMEDIAN} \
	      -i $1/$2/$2_${CHIP}.fits \
	      -o ${RESULTDIR[${CHIP}]}/$2_${CHIP}_illum.fits \
	      -s ${V_BACK_ILLUMSMOOTH}
      fi
      # make sure pixels in mask area remain constant
      ${P_FITSPOLYGON} -i ${RESULTDIR[${CHIP}]}/$2_${CHIP}_illum.fits \
	  -p $regionfile \
	  -v ${background} \
	  -o ${TEMPDIR}/${CHIP}_tmp.fits
      mv ${TEMPDIR}/${CHIP}_tmp.fits ${RESULTDIR[${CHIP}]}/$2_${CHIP}_illum.fits
  fi

  # Do we need a fringing model?
  if [ "${V_BACK_APPLYMODE}" = "2" ] || [ "${V_BACK_APPLYMODE}" = "3" ]; then
      ${P_SEX} /$1/$2/$2_${CHIP}.fits \
	  -c ${CONF}/illumfringe.sex \
	  -CHECKIMAGE_NAME ${RESULTDIR[${CHIP}]}/$2_${CHIP}_fringe.fits \
	  -CHECKIMAGE_TYPE -BACKGROUND \
	  -BACK_SIZE ${V_BACK_ILLUMSMOOTH} \
	  -NTHREADS ${NUMTHREAD} \
	  ${weightstring}
	  # commented out because prepare_sex_for_region forces constant background,
	  # which is NOT wanted here (we want a model)
	  # ${backstring} \

      # Should it be smoothed?
      if [ "${V_BACK_FRINGESMOOTH}"_A != "_A" ]; then
	  ${P_FITSMEDIAN} \
	      -i ${RESULTDIR[${CHIP}]}/$2_${CHIP}_fringe.fits \
	      -o ${RESULTDIR[${CHIP}]}/$2_${CHIP}_fringe_smooth.fits \
	      -s ${V_BACK_FRINGESMOOTH}
	  # make sure pixels in mask area remain constant
	  ${P_FITSPOLYGON} -i ${RESULTDIR[${CHIP}]}/$2_${CHIP}_fringe_smooth.fits \
	      -p $regionfile \
	      -v ${background} \
	      -o ${TEMPDIR}/${CHIP}_tmp.fits
	  mv ${TEMPDIR}/${CHIP}_tmp.fits ${RESULTDIR[${CHIP}]}/$2_${CHIP}_fringe_smooth.fits
      fi
      # make sure pixels in mask area remain constant
      ${P_FITSPOLYGON} -i ${RESULTDIR[${CHIP}]}/$2_${CHIP}_fringe.fits \
	  -p $regionfile \
	  -v ${background} \
	  -o ${TEMPDIR}/${CHIP}_tmp.fits
      mv ${TEMPDIR}/${CHIP}_tmp.fits ${RESULTDIR[${CHIP}]}/$2_${CHIP}_fringe.fits
  fi

  if [ "${RESULTDIR[${CHIP}]}" != "$1/$2" ]; then
      test -e ${RESULTDIR[${CHIP}]}/$2_${CHIP}_illum.fits && \
	  ln -s ${RESULTDIR[${CHIP}]}/$2_${CHIP}_illum.fits \
	  $1/$2/$2_${CHIP}_illum.fits
      test -e ${RESULTDIR[${CHIP}]}/$2_${CHIP}_fringe.fits && \
	  ln -s ${RESULTDIR[${CHIP}]}/$2_${CHIP}_fringe.fits \
	  $1/$2/$2_${CHIP}_fringe.fits
      test -e ${RESULTDIR[${CHIP}]}/$2_${CHIP}_fringe_smooth.fits && \
	  ln -s ${RESULTDIR[${CHIP}]}/$2_${CHIP}_fringe_smooth.fits \
	  $1/$2/$2_${CHIP}_fringe_smooth.fits
  fi
done
