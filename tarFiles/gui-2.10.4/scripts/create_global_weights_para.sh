BASHPATH -xv

# the script creates global weights

#$1: main directory
#$2: norm flat directory
#$3: science directory
#$4: chips to be processed

. bash.include

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

SCRIPTDIR=`pwd`

MAIN=$1
FLATDIR=$2
FLATMIN=${V_GLOBWFLATMIN}
FLATMAX=${V_GLOBWFLATMAX}
DARKDIR=${V_GLOBWDARKDIR}
DARKMIN=${V_GLOBWDARKMIN}
DARKMAX=${V_GLOBWDARKMAX}
SUPERFLATDIR=""
SUPERFLATMIN=""
SUPERFLATMAX=""
V_GLOBW_DOFLAGS=TRUE
flag_sf=0

if [ ! -d /${MAIN}/WEIGHTS ]; then
  mkdir /${MAIN}/WEIGHTS
fi

# Delete old global weight maps
# (could be from a different intsrument!)
for CHIP in $4
do
    rm /${MAIN}/WEIGHTS/global*_${CHIP}.fits
done


# if the globalweight is requested to be constant 1 everywhere:
if [ "${V_GLOBW_UNIFORMWEIGHT}" = "TRUE" ]; then
    for CHIP in $4
    do
      ${P_IC} -p -32 -c ${SIZEX[${CHIP}]} ${SIZEY[${CHIP}]} 'x 0 * 1 +' > \
	  /${MAIN}/WEIGHTS/globalweight_${CHIP}.fits
      ${P_IC} -p 8 -c ${SIZEX[${CHIP}]} ${SIZEY[${CHIP}]} 'x 0 * 0 +' > \
	  /${MAIN}/WEIGHTS/globalflag_${CHIP}.fits

      get_region
      
      # region file
      if [ -f "${regionfile}" ] && [ "${regionfile}_A" != "_A" ]; then
  	  # the weight image
	  ${P_FITSPOLYGON} \
	      -i ${MAIN}/WEIGHTS/globalweight_${CHIP}.fits \
	      -p ${regionfile} \
	      -o ${MAIN}/WEIGHTS/globalweight_${CHIP}.fits2
	  mv ${MAIN}/WEIGHTS/globalweight_${CHIP}.fits2 ${MAIN}/WEIGHTS/globalweight_${CHIP}.fits
	  
	  # the flag image
	  ${P_FITSPOLYGON} \
	      -i /${MAIN}/WEIGHTS/globalflag_${CHIP}.fits \
	      -p ${regionfile} \
	      -o /${MAIN}/WEIGHTS/globalflag_${CHIP}.fits2 \
	      -v 1
	  mv /${MAIN}/WEIGHTS/globalflag_${CHIP}.fits2 /${MAIN}/WEIGHTS/globalflag_${CHIP}.fits 
      fi
    done

    # exit the script, no defect detection
    exit
fi


# the following is only done if no constant weight is assumed

for CHIP in $4
do
  NUMTHREAD=`${SCRIPTDIR}/get_nthreads.sh ${NCHIPS} ${NPARA} ${CHIP}`

  WNAMES=""
  WMIN=""
  WMAX=""
  WFLAG=""
  ACTUFLAG=2

  WNAMES="/${MAIN}/${FLATDIR}/${FLATDIR}_${CHIP}.fits"
  FLATBASE=`basename ${FLATDIR} _norm`

  WMIN="${FLATMIN}"
  WMAX="${FLATMAX}"
  WFLAG="${ACTUFLAG}"

  # if an optional DARK directory is given
  if [ "${V_GLOBWDARKDIR}_A" != "_A" ]; then
    ACTUFLAG=$(( ${ACTUFLAG} * 2 ))
    WNAMES="${WNAMES},/${MAIN}/${DARKDIR}/${DARKDIR}_${CHIP}.fits"
    WMIN="${WMIN},${DARKMIN}"
    WMAX="${WMAX},${DARKMAX}"      
    WFLAG="${WFLAG},${ACTUFLAG}"      
  fi

  # if an optional SUPERFLAT directory is given
  if [ "${flag_sf}" = "1" ]; then
    ACTUFLAG=$(( ${ACTUFLAG} * 2 ))
    WNAMES="${WNAMES},/${MAIN}/${SUPERFLATDIR}/${SUPERFLATDIR}_${CHIP}.fits"
    WMIN="${WMIN},${SUPERFLATMIN}"
    WMAX="${WMAX},${SUPERFLATMAX}"      
    WFLAG="${WFLAG},${ACTUFLAG}"      
  fi

  # create link if necessary:
  if [ -L "/${MAIN}/${FLATDIR}/${FLATDIR}_${CHIP}.fits" ]; then
    LINK=`${P_READLINK} /${MAIN}/${FLATDIR}/${FLATDIR}_${CHIP}.fits`
    RESULTDIR=`dirname ${LINK}`
    ln -s ${RESULTDIR}/globalweight_${CHIP}.fits \
	/${MAIN}/WEIGHTS/globalweight_${CHIP}.fits
    if [ "${V_GLOBW_DOFLAGS}" = "TRUE" ]; then
        # create dummy flag
	${P_IC} -c ${SIZEX[${CHIP}]} ${SIZEY[${CHIP}]} -p 8 '0' \
	    > ${RESULTDIR}/flag_${CHIP}_$$.fits 
	ln -s ${RESULTDIR}/globalflag_${CHIP}.fits \
	    /${MAIN}/WEIGHTS/globalflag_${CHIP}.fits
    fi
  else
    RESULTDIR="/${MAIN}/WEIGHTS/"
  fi   

  # create dummy flag
  if [ "${V_GLOBW_DOFLAGS}" = "TRUE" ]; then
      naxis1=`get_key /${MAIN}/${FLATDIR}/${FLATDIR}_${CHIP}.fits NAXIS1`
      naxis2=`get_key /${MAIN}/${FLATDIR}/${FLATDIR}_${CHIP}.fits NAXIS2`
      ${P_IC} -c ${naxis1} ${naxis2} -p 8 '0' \
	  > ${RESULTDIR}/flag_${CHIP}_$$.fits 
  fi

  # write config file for ww
  echo "WEIGHT_NAMES ${WNAMES}"   >  ${TEMPDIR}/${CHIP}.ww_$$
  echo "WEIGHT_MIN ${WMIN}"       >> ${TEMPDIR}/${CHIP}.ww_$$
  echo "WEIGHT_MAX ${WMAX}"       >> ${TEMPDIR}/${CHIP}.ww_$$
  echo "WEIGHT_OUTFLAGS ${WFLAG}" >> ${TEMPDIR}/${CHIP}.ww_$$
  #
  if [ "${V_GLOBW_DOFLAGS}" = "FALSE" ]; then
      echo 'FLAG_NAMES ""'     >> ${TEMPDIR}/${CHIP}.ww_$$
      echo 'FLAG_MASKS ""'     >> ${TEMPDIR}/${CHIP}.ww_$$
      echo 'FLAG_WMASKS ""'    >> ${TEMPDIR}/${CHIP}.ww_$$
      echo 'FLAG_OUTFLAGS ""'  >> ${TEMPDIR}/${CHIP}.ww_$$
      echo 'OUTFLAG_NAME ""'   >> ${TEMPDIR}/${CHIP}.ww_$$
  else
      echo "FLAG_NAMES ${RESULTDIR}/flag_${CHIP}_$$.fits" >> ${TEMPDIR}/${CHIP}.ww_$$
      echo 'FLAG_MASKS "0x0"'  >> ${TEMPDIR}/${CHIP}.ww_$$
      echo 'FLAG_WMASKS "0x0"' >> ${TEMPDIR}/${CHIP}.ww_$$
      echo 'FLAG_OUTFLAGS "0"' >> ${TEMPDIR}/${CHIP}.ww_$$
      echo "OUTFLAG_NAME ${RESULTDIR}/globalflag_${CHIP}.fits" >> ${TEMPDIR}/${CHIP}.ww_$$
  fi
  #

  # we deal with the regions separately. WW has bad memory management not allowing for large polygon masks
  echo 'POLY_NAMES ""'            >> ${TEMPDIR}/${CHIP}.ww_$$
  echo 'POLY_OUTFLAGS ""'         >> ${TEMPDIR}/${CHIP}.ww_$$
  echo "OUTWEIGHT_NAME ${RESULTDIR}/globalweight_${CHIP}.fits" >> ${TEMPDIR}/${CHIP}.ww_$$

  ${P_WW} -c ${TEMPDIR}/${CHIP}.ww_$$

  # now deal with regions
  get_region

  if [ -f "${regionfile}" ] && [ "${regionfile}_A" != "_A" ]; then
      # the weight image
      ${P_FITSPOLYGON} \
	  -i ${MAIN}/WEIGHTS/globalweight_${CHIP}.fits \
	  -p ${regionfile} \
	  -o ${MAIN}/WEIGHTS/globalweight_${CHIP}.fits2
      mv ${MAIN}/WEIGHTS/globalweight_${CHIP}.fits2 ${MAIN}/WEIGHTS/globalweight_${CHIP}.fits
      
      # the flag image
      ${P_FITSPOLYGON} \
	  -i /${MAIN}/WEIGHTS/globalflag_${CHIP}.fits \
	  -p ${regionfile} \
	  -o /${MAIN}/WEIGHTS/globalflag_${CHIP}.fits2 \
	  -v 1
      mv /${MAIN}/WEIGHTS/globalflag_${CHIP}.fits2 /${MAIN}/WEIGHTS/globalflag_${CHIP}.fits 
  fi

  rm ${RESULTDIR}/flag_${CHIP}_$$.fits

  #############################################################################
  # automatic detection of pixel defects
  #
  
  ls -1 /$MAIN/WEIGHTS/globalweight_${CHIP}.fits > ${TEMPDIR}/globw_${CHIP}_$$

  FILE=`${P_GAWK} '(NR==1) {print $0}' ${TEMPDIR}/globw_${CHIP}_$$`

  if [ -L ${FILE} ]; then
      LINK=`${P_READLINK} ${FILE}`
      RESULTDIR[${CHIP}]=`dirname ${LINK}`
  else
      RESULTDIR[${CHIP}]="/$MAIN/WEIGHTS/"
  fi  
  
  # detect bad pixels based on a FLAT

  cat ${TEMPDIR}/globw_${CHIP}_$$ |\
  {
  while read file
    do
      BASE=`basename ${file} .fits`

      #
      # now run sextractor to obtain a smooth background model
      
      if [ "${V_DEFECT_KERNELSIZE}" != "" ]; then

	  ${P_SEX} ${file} -c ${DATACONF}/image-objects.sex \
	      -DETECT_THRESH 3 \
	      -DETECT_MINAREA 5 \
	      -BACK_SIZE ${V_DEFECT_KERNELSIZE} \
	      -CHECKIMAGE_TYPE BACKGROUND \
	      -CHECKIMAGE_NAME ${RESULTDIR[${CHIP}]}/${BASE}"_smooth.fits" \
	      -NTHREADS ${NUMTHREAD} \
	      -WEIGHT_TYPE MAP_WEIGHT \
	      -WEIGHT_IMAGE ${RESULTDIR}/globalweight_${CHIP}.fits

          # normalise flat by the smoothed flat
	  ${P_IC} '%1 %2 /' ${RESULTDIR[${CHIP}]}/${BASE}".fits" \
	      ${RESULTDIR[${CHIP}]}/${BASE}"_smooth.fits" \
	      > ${RESULTDIR[${CHIP}]}/${BASE}".m.fits"

	  rm ${RESULTDIR[${CHIP}]}/${BASE}_smooth.fits
	  if [ "${RESULTDIR[${CHIP}]}" != "/$MAIN/WEIGHTS/" ]; then
	      rm /$MAIN/WEIGHTS/${BASE}"_smooth.fits"
	  fi
      fi

      # remove existing old results
      test -f ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits" && rm ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits"
      test -f ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits" && rm ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits"
      test -f ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits" && rm ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits"

      # collapse the image to detect bad rows and/or columns
      if [ "${V_DEFECT_KERNELSIZE}" != "" ] && [ "${V_DEFECT_ROWTOL}" != "" ]; then
	  ${P_FITSCOLLAPSE} -i ${RESULTDIR[${CHIP}]}/${BASE}".m.fits" \
	      -o ${RESULTDIR[${CHIP}]}/${BASE}".cx.fits" -c x -t ${FLATMIN}
	  low=`echo ${V_DEFECT_ROWTOL} | ${P_GAWK} '{print 1.0-$1}'`
	  high=`echo ${V_DEFECT_ROWTOL} | ${P_GAWK} '{print 1.0+$1}'`
	  ${P_FITSMASK} -i ${RESULTDIR[${CHIP}]}/${BASE}".cx.fits" -l ${low} -h ${high} -n \
	      -o ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits"
	  mv ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits" ${RESULTDIR[${CHIP}]}/${BASE}".cx.fits"
      fi

      if [ "${V_DEFECT_KERNELSIZE}" != "" ] && [ "${V_DEFECT_COLTOL}" != "" ]; then
	  ${P_FITSCOLLAPSE} -i ${RESULTDIR[${CHIP}]}/${BASE}".m.fits" \
	      -o ${RESULTDIR[${CHIP}]}/${BASE}".cy.fits" -c y -t ${FLATMIN}
	  low=`echo ${V_DEFECT_COLTOL} | ${P_GAWK} '{print 1.0-$1}'`
	  high=`echo ${V_DEFECT_COLTOL} | ${P_GAWK} '{print 1.0+$1}'`
	  ${P_FITSMASK} -i ${RESULTDIR[${CHIP}]}/${BASE}".cy.fits" -l ${low} -h ${high} -n \
	      -o ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits"
	  mv ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits" ${RESULTDIR[${CHIP}]}/${BASE}".cy.fits"
      fi

      # detect bad pixels
      if [ "${V_DEFECT_KERNELSIZE}" != "" ] && [ "${V_DEFECT_CLUSTOL}" != "" ]; then
	  low=`echo ${V_DEFECT_CLUSTOL} | ${P_GAWK} '{print 1.0-$1}'`
	  high=`echo ${V_DEFECT_CLUSTOL} | ${P_GAWK} '{print 1.0+$1}'`
	  ${P_FITSMASK} -i ${RESULTDIR[${CHIP}]}/${BASE}".m.fits" -l ${low} -h ${high} -n \
	      -o ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits"
      fi

      # provide dummy files if the real files don't exist
#      test ! -f ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits" && \
#	  ${P_IC} '%1 0. * 1. +' ${file} > ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits" 
#      test ! -f ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits" && \
#	  ${P_IC} '%1 0. * 1. +' ${file} > ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits" 
#      test ! -f ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits" && \
#	  ${P_IC} '%1 0. * 1. +' ${file} > ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits" 

      test ! -f ${RESULTDIR[${CHIP}]}/${BASE}".cx.fits" && \
	  ${P_IC} '%1 0. * 1. +' ${file} > ${RESULTDIR[${CHIP}]}/${BASE}".cx.fits" 
      test ! -f ${RESULTDIR[${CHIP}]}/${BASE}".cy.fits" && \
	  ${P_IC} '%1 0. * 1. +' ${file} > ${RESULTDIR[${CHIP}]}/${BASE}".cy.fits" 
      test ! -f ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits" && \
	  ${P_IC} '%1 0. * 1. +' ${file} > ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits" 

      # make the masks
#      ${P_IC} '%1 %2 * %3 * %4 *' ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits" \
#	  ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits" \
#	  ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits" \
#	  ${RESULTDIR}/globalweight_${CHIP}.fits > \
#         ${RESULTDIR[${CHIP}]}/${BASE}".mask.fits" 

      ${P_IC} '%1 %2 * %3 * %4 *' ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits" \
	  ${RESULTDIR[${CHIP}]}/${BASE}".cx.fits" \
	  ${RESULTDIR[${CHIP}]}/${BASE}".cy.fits" \
	  ${RESULTDIR}/globalweight_${CHIP}.fits > \
          ${RESULTDIR[${CHIP}]}/${BASE}".mask.fits" 

      mv ${RESULTDIR[${CHIP}]}/${BASE}.mask.fits  ${RESULTDIR}/globalweight_${CHIP}.fits

#      rm ${RESULTDIR[${CHIP}]}/${BASE}".cx.fits" ${RESULTDIR[${CHIP}]}/${BASE}".cy.fits" \
#          ${RESULTDIR[${CHIP}]}/${BASE}".m.fits" ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits" \
#	  ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits" ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits"

      if [ "${RESULTDIR[${CHIP}]}" != "/$MAIN/WEIGHTS/" ]; then
        ln -s ${RESULTDIR[${CHIP}]}/globalweight_${CHIP}.fits /$MAIN/WEIGHTS/globalweight_${CHIP}.fits
        ln -s ${RESULTDIR[${CHIP}]}/globalflag_${CHIP}.fits /$MAIN/WEIGHTS/globalflag_${CHIP}.fits
      fi

      i=$(( ${i} + 1 ))
    done
  }


  # detect bad pixels based on a SUPERFLAT

  if [ "${V_DEFECT_KERNELSIZE_SF}" = "" ]; then
      continue
  fi

  if [ "${V_DEFECT_ROWTOL_SF}" = "" ] && [ "${V_DEFECT_COLTOL_SF}" = "" ] && [ "${V_DEFECT_CLUSTOL_SF}" = "" ]; then
      continue
  fi

  ls -1 /$MAIN/$3/$3_${CHIP}.fits > ${TEMPDIR}/supf_${CHIP}_$$

  nsf=`wc ${TEMPDIR}/supf_${CHIP}_$$ | ${P_GAWK} '{print $1}'`
  
  if [ "${nsf}" = "0" ]; then
      continue
  fi

  FILE=`${P_GAWK} '(NR==1) {print $0}' ${TEMPDIR}/supf_${CHIP}_$$`
  
  cat ${TEMPDIR}/supf_${CHIP}_$$ |\
  {
  while read file
    do
      BASE=`basename ${file} .fits`

      #
      # now run sextractor to obtain a smooth background model
      
      if [ "${V_DEFECT_KERNELSIZE_SF}" != "" ]; then

	  ${P_SEX} ${file} -c ${DATACONF}/image-objects.sex \
	      -DETECT_THRESH 3 \
	      -DETECT_MINAREA 5 \
	      -BACK_SIZE ${V_DEFECT_KERNELSIZE_SF} \
	      -CHECKIMAGE_TYPE BACKGROUND \
	      -CHECKIMAGE_NAME ${RESULTDIR[${CHIP}]}/${BASE}"_smooth.fits" \
	      -NTHREADS ${NUMTHREAD} \
	      -WEIGHT_TYPE MAP_WEIGHT \
	      -WEIGHT_IMAGE ${RESULTDIR}/globalweight_${CHIP}.fits

          # normalise flat by the smoothed flat
	  ${P_IC} '%1 %2 /' ${file} \
	      ${RESULTDIR[${CHIP}]}/${BASE}"_smooth.fits" \
	      > ${RESULTDIR[${CHIP}]}/${BASE}".m.fits"

	  rm ${RESULTDIR[${CHIP}]}/${BASE}_smooth.fits
	  if [ "${RESULTDIR[${CHIP}]}" != "/$MAIN/WEIGHTS/" ]; then
	      rm /$MAIN/WEIGHTS/${BASE}"_smooth.fits"
	  fi
      fi

      # remove existing old results
      test -f ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits" && rm ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits"
      test -f ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits" && rm ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits"
      test -f ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits" && rm ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits"

      # collapse the image to detect bad rows and/or columns
      if [ "${V_DEFECT_KERNELSIZE_SF}" != "" ] && [ "${V_DEFECT_ROWTOL_SF}" != "" ]; then
	  ${P_FITSCOLLAPSE} -i ${RESULTDIR[${CHIP}]}/${BASE}".m.fits" \
	      -o ${RESULTDIR[${CHIP}]}/${BASE}".cx.fits" -c x -t ${FLATMIN}
	  low=`echo ${V_DEFECT_ROWTOL_SF} | ${P_GAWK} '{print 1.0-$1}'`
	  high=`echo ${V_DEFECT_ROWTOL_SF} | ${P_GAWK} '{print 1.0+$1}'`
	  ${P_FITSMASK} -i ${RESULTDIR[${CHIP}]}/${BASE}".cx.fits" -l ${low} -h ${high} -n \
	      -o ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits"
#	  mv ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits" ${RESULTDIR[${CHIP}]}/${BASE}".cx.fits"
      fi

      if [ "${V_DEFECT_KERNELSIZE_SF}" != "" ] && [ "${V_DEFECT_COLTOL_SF}" != "" ]; then
	  ${P_FITSCOLLAPSE} -i ${RESULTDIR[${CHIP}]}/${BASE}".m.fits" \
	      -o ${RESULTDIR[${CHIP}]}/${BASE}".cy.fits" -c y -t ${FLATMIN}
	  low=`echo ${V_DEFECT_COLTOL_SF} | ${P_GAWK} '{print 1.0-$1}'`
	  high=`echo ${V_DEFECT_COLTOL_SF} | ${P_GAWK} '{print 1.0+$1}'`
	  ${P_FITSMASK} -i ${RESULTDIR[${CHIP}]}/${BASE}".cy.fits" -l ${low} -h ${high} -n \
	      -o ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits"
#	  mv ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits" ${RESULTDIR[${CHIP}]}/${BASE}".cy.fits"
      fi

      # detect bad pixels
      if [ "${V_DEFECT_KERNELSIZE_SF}" != "" ] && [ "${V_DEFECT_CLUSTOL_SF}" != "" ]; then
	  low=`echo ${V_DEFECT_CLUSTOL_SF} | ${P_GAWK} '{print 1.0-$1}'`
	  high=`echo ${V_DEFECT_CLUSTOL_SF} | ${P_GAWK} '{print 1.0+$1}'`
	  ${P_FITSMASK} -i ${RESULTDIR[${CHIP}]}/${BASE}".m.fits" -l ${low} -h ${high} -n \
	      -o ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits"
      fi

      # provide dummy files if the real files don't exist
      test ! -f ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits" && \
	  ${P_IC} '%1 0. * 1. +' ${file} > ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits" 
      test ! -f ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits" && \
	  ${P_IC} '%1 0. * 1. +' ${file} > ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits" 
      test ! -f ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits" && \
	  ${P_IC} '%1 0. * 1. +' ${file} > ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits" 

      # make the masks
      ${P_IC} '%1 %2 * %3 * %4 *' ${RESULTDIR[${CHIP}]}/${BASE}".mc.fits" \
	  ${RESULTDIR[${CHIP}]}/${BASE}".cx1.fits" \
	  ${RESULTDIR[${CHIP}]}/${BASE}".cy1.fits" \
	  ${RESULTDIR}/globalweight_${CHIP}.fits > \
          ${RESULTDIR[${CHIP}]}/${BASE}".mask.fits" 

      mv ${RESULTDIR[${CHIP}]}/${BASE}.mask.fits  ${RESULTDIR}/globalweight_${CHIP}.fits

      rm ${RESULTDIR[${CHIP}]}/${BASE}".cx.fits" ${RESULTDIR[${CHIP}]}/${BASE}".cy.fits" \
          ${RESULTDIR[${CHIP}]}/${BASE}".m.fits" 

      if [ "${RESULTDIR[${CHIP}]}" != "/$MAIN/WEIGHTS/" ]; then
        ln -s ${RESULTDIR[${CHIP}]}/globalweight_${CHIP}.fits /$MAIN/WEIGHTS/globalweight_${CHIP}.fits
        ln -s ${RESULTDIR[${CHIP}]}/globalflag_${CHIP}.fits /$MAIN/WEIGHTS/globalflag_${CHIP}.fits
      fi

      i=$(( ${i} + 1 ))
    done
  }
done
