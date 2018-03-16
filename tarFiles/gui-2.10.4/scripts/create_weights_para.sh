BASHPATH -xv

# $1: main directory
# $2: science dir.
# $3: image extension (ext) on ..._iext.fits (i is the chip number)
# $4: chips to be processed


SCRIPTDIR=`pwd`

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash.include

V_GLOBW_DOFLAGS=TRUE

for CHIP in $4
do
  ${P_FIND} /$1/$2/ -maxdepth 1 -name \*_${CHIP}$3.fits \
            -print | ${P_SORT} > ${TEMPDIR}/crw_images_$$

  NUMTHREAD=`${SCRIPTDIR}/get_nthreads.sh ${NCHIPS} ${NPARA} ${CHIP}`

  FILE=`${P_GAWK} '(NR==1) {print $0}' ${TEMPDIR}/crw_images_$$`

  if [ -L ${FILE} ]; then
    LINK=`${P_READLINK} ${FILE}`
    RESULTDIR=`dirname ${LINK}`
  else
    RESULTDIR="$1/WEIGHTS"
  fi
  
  get_statminsection

  cat ${TEMPDIR}/crw_images_$$ |\
  {
    while read file
    do
      BASE=`basename ${file} $3.fits`

      # if the user only wants the global weight as a weight
      if [ "${V_COSMICSTHRESHOLD}" = "0" ] || [ "${V_COSMICSTHRESHOLD}_A" = "_A" ]; then
	  if [ "${V_WEIGHTLOWTHRESHOLD}_A" = "_A" ]; then
	      V_WEIGHTLOWTHRESHOLD="-1e9"
	  fi
	  if [ "${V_WEIGHTHIGHTHRESHOLD}_A" = "_A" ]; then
	      V_WEIGHTHIGHTHRESHOLD="1e9"
	  fi
	  INFLAGS="FLAG_NAMES \"\""
	  OUTFLAGS=""
	  if [ "${V_GLOBW_DOFLAGS}" = "TRUE" ]; then
              INFLAGS="-FLAG_NAMES ${RESULTDIR}/globalflag_${CHIP}.fits"
              OUTFLAGS="-OUTFLAG_NAME ${RESULTDIR}/${BASE}$3.flag.fits"
	  fi
	  if [ -r "/$1/$2/reg/${BASE}.reg" ]; then
	      ${P_WW} -c ${CONF}/weights.ww \
		  -WEIGHT_NAMES ${RESULTDIR}/globalweight_${CHIP}.fits,${file} \
		  ${INFLAGS} ${OUTFLAGS} \
		  -POLY_NAMES "/$1/$2/reg/${BASE}.reg" \
		  -OUTWEIGHT_NAME ${RESULTDIR}/${BASE}$3.weight.fits \
		  -WEIGHT_MIN "-1e9,${V_WEIGHTLOWTHRESHOLD}" \
		  -WEIGHT_MAX "1e9,${V_WEIGHTHIGHTHRESHOLD}" -POLY_INTERSECT Y
	  else
	      ${P_WW} -c ${CONF}/weights.ww \
		  -WEIGHT_NAMES ${RESULTDIR}/globalweight_${CHIP}.fits,${file} \
		  ${INFLAGS} ${OUTFLAGS} \
		  -OUTWEIGHT_NAME ${RESULTDIR}/${BASE}$3.weight.fits \
		  -WEIGHT_MIN "-1e9,${V_WEIGHTLOWTHRESHOLD}" \
		  -WEIGHT_MAX "1e9,${V_WEIGHTHIGHTHRESHOLD}" -POLY_INTERSECT Y
	  fi
      else

	  # if large parts of the image are empty or vignetted, we have to force sextractor
	  # to assume a fixed sky background, as otherwise the large intensity variations
	  # between good image areas and vignetted areas screw up the sextractor background 
	  # modelling

#	  get_region
	  backstring=""
	  if [ -f ${regionfile} ] && [ "${regionfile}_A" != "_A" ]; then
#	      region="-p ${INSTDIR}/${INSTRUMENT}.reg"
	      background=`${P_IMSTATS} ${file} \
		  -d 6 -t -1e8 1e8 \
		  -s ${statminsection_imstats} | \
		  ${P_GAWK} '($1 != "#") {print $2}'`
	      backstring="-BACK_TYPE MANUAL -BACK_VALUE ${background}"
	  fi

	  # this sextractor call does not produce the desired result if given a weight image;
	  # we have to force the background manually rather than downweighting pixels

          # first run sextractor to identify cosmic rays:
	  ${P_SEX} ${file} -c ${CONF}/cosmic.conf.sex -CHECKIMAGE_NAME \
	      ${RESULTDIR}/cosmic_${CHIP}_$$.fits \
	      -CATALOG_NAME ${TEMPDIR}/cosmic.cat \
	      -FILTER_THRESH 3.0 \
	      -DETECT_THRESH ${V_COSMICDT} \
	      -DETECT_MINAREA ${V_COSMICDMIN} \
	      -BACK_SIZE 120 \
	      -ANALYSIS_THRESH ${V_COSMICDT} \
	      -NTHREADS ${NUMTHREAD} \
	      ${backstring}
#	      -WEIGHT_TYPE MAP_WEIGHT \
#	      -WEIGHT_IMAGE ${RESULTDIR}/globalweight_${CHIP}.fits

          # then run weightwatcher
	  if [ "${V_WEIGHTLOWTHRESHOLD}_A" = "_A" ]; then
	      V_WEIGHTLOWTHRESHOLD="-1e9"
	  fi
	  if [ "${V_WEIGHTHIGHTHRESHOLD}_A" = "_A" ]; then
	      V_WEIGHTHIGHTHRESHOLD="1e9"
	  fi
	  INFLAGS="FLAG_NAMES \"\""
	  OUTFLAGS=""
	  if [ "${V_GLOBW_DOFLAGS}" = "TRUE" ]; then
              INFLAGS="-FLAG_NAMES ${RESULTDIR}/globalflag_${CHIP}.fits"
              OUTFLAGS="-OUTFLAG_NAME ${RESULTDIR}/${BASE}$3.flag.fits"
	  fi
	  if [ -r "/$1/$2/reg/${BASE}.reg" ]; then
	      ${P_WW} -c ${CONF}/weights.ww \
		  -WEIGHT_NAMES ${RESULTDIR}/globalweight_${CHIP}.fits,${RESULTDIR}/cosmic_${CHIP}_$$.fits,${file} \
		  ${INFLAGS} ${OUTFLAGS} \
		  -POLY_NAMES "/$1/$2/reg/${BASE}.reg" \
		  -OUTWEIGHT_NAME ${RESULTDIR}/${BASE}$3.weight.fits \
		  -WEIGHT_MIN "-1e9,-${V_COSMICSTHRESHOLD},${V_WEIGHTLOWTHRESHOLD}" \
		  -WEIGHT_MAX "1e9,${V_COSMICSTHRESHOLD},${V_WEIGHTHIGHTHRESHOLD}" -POLY_INTERSECT Y
	  else
	      ${P_WW} -c ${CONF}/weights.ww\
		  -WEIGHT_NAMES ${RESULTDIR}/globalweight_${CHIP}.fits,${RESULTDIR}/cosmic_${CHIP}_$$.fits,${file} \
		  ${INFLAGS} ${OUTFLAGS} \
		  -OUTWEIGHT_NAME ${RESULTDIR}/${BASE}$3.weight.fits \
		  -WEIGHT_MIN "-1e9,-${V_COSMICSTHRESHOLD},${V_WEIGHTLOWTHRESHOLD}" \
		  -WEIGHT_MAX "1e9,${V_COSMICSTHRESHOLD},${V_WEIGHTHIGHTHRESHOLD}" -POLY_INTERSECT Y
	  fi
	  rm ${RESULTDIR}/cosmic_${CHIP}_$$.fits

      fi

      # mask blooming spikes if requested
      if [ "${V_MASKBLOOMSPIKE}" = "1" ]; then
	  # detect spikes
	  ${P_FITSBLOOMDETECT} -i ${file} -o ${RESULTDIR}/${BASE}_bloom.fits \
	      -l ${V_BLOOMLOWLIMIT} -r ${V_BLOOMWIDTH}
	  # filter out small spikes
	  ${P_SEX} -c ${CONF}/seg.sex ${RESULTDIR}/${BASE}_bloom.fits \
	      -CHECKIMAGE_NAME ${RESULTDIR}/${BASE}_seg.fits \
	      -DETECT_MINAREA ${V_BLOOMMINAREA} -NTHREADS ${NUMTHREAD}
	  # set the corresponding pixels in the weight to zero
	  ${P_IC} '%1 0 %2 0.1 < ?' ${RESULTDIR}/${BASE}$3.weight.fits \
	      ${RESULTDIR}/${BASE}_seg.fits > ${RESULTDIR}/${BASE}$3.weight_tmp.fits 
	  mv ${RESULTDIR}/${BASE}$3.weight_tmp.fits ${RESULTDIR}/${BASE}$3.weight.fits 
	  rm ${RESULTDIR}/${BASE}_bloom.fits ${RESULTDIR}/${BASE}_seg.fits 
      fi

      if [ "${RESULTDIR}" != "$1/WEIGHTS" ]; then
          ln -s ${RESULTDIR}/${BASE}$3.weight.fits \ 
	  /$1/WEIGHTS/${BASE}$3.weight.fits
	  if [ "${V_GLOBW_DOFLAGS}" = "TRUE" ]; then
              ln -s ${RESULTDIR}/${BASE}$3.flag.fits \ 
	      /$1/WEIGHTS/${BASE}$3.flag.fits
	  fi
      fi
    done
  }
done
