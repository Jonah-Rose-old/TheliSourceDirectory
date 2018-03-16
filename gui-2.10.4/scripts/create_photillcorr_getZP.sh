BASHPATH -uxv

# compares object fluxes in an image with those of a
# photometric reference catalogue, and calculates
# an absolute zeropoint

#$1: main directory
#$2: science dir. (the catalogues are assumed to be in $1/$2/cat)
#$3: image extension

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash_functions.include

DIR=`pwd`

CATS=`${P_FIND} /$1/$2/cat -maxdepth 1 -name \*_1$3.cat | ${P_SORT}`


# if the fitting is done individually for all chips
if [ "${V_ABSPHOT_FITTINGMETHODM2}" = "CHIP" ] && [ "${NCHIPS}" -gt 1 ]; then
  test -f ${TEMPDIR}/PIC_chipscalingfactors_$$ && rm ${TEMPDIR}/PIC_chipscalingfactors_$$
  test -f ${TEMPDIR}/PIC_chipscalingfactors_err_$$ && rm ${TEMPDIR}/PIC_chipscalingfactors_err_$$
  test -f $1/$2/PIC_chipscalingfactors && rm $1/$2/PIC_chipscalingfactors 
  test -f $1/$2/PIC_chipscalingfactors_err && rm $1/$2/PIC_chipscalingfactors_err 
  
  for CAT in ${CATS}
  do
    test -f ${TEMPDIR}/abszp0_$$ && rm ${TEMPDIR}/abszp0_$$
    CHIP=1
    # get the zeropoints for each chip of an exposure
    while [ "${CHIP}" -le "${NCHIPS}" ]
    do
	BASE=`basename ${CAT} _1$3.cat`
	FILE=$1/$2/${BASE}_${CHIP}$3.fits
	if [ -L ${FILE} ]; then
	    LINK=`${P_READLINK} ${FILE}`
	    RESULTDIR[${CHIP}]=`dirname ${LINK}`
	else
	    RESULTDIR[${CHIP}]="$1/$2"
	fi
	WBASE=`basename ${CAT} _1$3.cat`
	WFILE=$1/WEIGHTS/${BASE}_${CHIP}$3.weight.fits
	if [ -L ${WFILE} ]; then
	    LINK=`${P_READLINK} ${WFILE}`
	    RESULTDIRWEIGHT[${CHIP}]=`dirname ${LINK}`
	else
	    RESULTDIRWEIGHT[${CHIP}]="$1/WEIGHTS"
	fi
	if [ -f /$1/$2/cat/${BASE}_${CHIP}$3_merg.cat ]; then
	    ZPFLUX=`${P_FITSPIC} \
		-i /$1/$2/cat/${BASE}_${CHIP}$3_merg.cat \
		-a /$1/$2/${BASE}_${CHIP}$3.fits \
		-o /$1/$2/cat/${BASE}_${CHIP}$3_pic.cat`
	    ABSZP=`echo ${ZPFLUX} | ${P_GAWK} '{print $1}'`
	    echo ${ZPFLUX} >> ${TEMPDIR}/abszp0_$$
	else
	    ABSZP=-1.0
	    echo -1.0 0.0 0.0 >> ${TEMPDIR}/abszp0_$$
	fi
	CHIP=$((${CHIP}+1))
    done

    # convert the zeropoints into flux ratios
    ${P_GAWK} 'BEGIN{maxzp=0} {
             zp[NR] = $1
             if ($1>maxzp && $1 != -1.0 ) maxzp=$1
             } END {
             print maxzp
             for (i=1; i<=NR; i++) {
               if (zp[i] != -1.0) {
                 fluxcorrfac[i]=2.51189^(maxzp-zp[i])
                 printf("%.4f\n", fluxcorrfac[i])
               } else {
                 fluxcorrfac[i]=0.0
                 printf("%.4f\n", fluxcorrfac[i])
               }
            }
             }' ${TEMPDIR}/abszp0_$$ > ${TEMPDIR}/abszp1_$$     

    # the flux ratio uncertainties
    ${P_GAWK} 'BEGIN{maxzp=0} {
             zp[NR] = $1
             zperr[NR] = $2
             if ($1>maxzp && $1 != -1.0) maxzp=$1
             } END {
             for (i=1; i<=NR; i++) {
               if (zp[i] != -1.0) {           
                 err[i]=2.51189^(maxzp-zp[i])*log(2.51189)*zperr[i]
                 printf("%.4f\n", err[i])
               } else {
                 printf("%.4f\n", 0.0)
               }
             }
             }' ${TEMPDIR}/abszp0_$$ > ${TEMPDIR}/abszp1_err_$$     

    scales=`${P_GAWK} '(NR>1) {s[NR] = $1} END { 
	    for (i=2;i<=NR;i++) {
		printf("%s ",s[i])
            }
	    printf("\n")}' ${TEMPDIR}/abszp1_$$`

    echo ${BASE} ${scales} >> ${TEMPDIR}/PIC_chipscalingfactors_$$

    scales_err=`${P_GAWK} '{s[NR] = $1} END { 
	    for (i=1;i<=NR;i++) {
		printf("%s ",s[i])
            }
	    printf("\n")}' ${TEMPDIR}/abszp1_err_$$`

    echo ${BASE} ${scales_err} >> ${TEMPDIR}/PIC_chipscalingfactors_err_$$

    MAXZP=`${P_GAWK} '(NR==1)' ${TEMPDIR}/abszp1_$$`
    CHIP=1
    # do the flux correction
    # write the ZP (same for all chips)
    while [ "${CHIP}" -le "${NCHIPS}" ]
    do
	BASE=`basename ${CAT} _1$3.cat`
	CORRFAC=`${P_GAWK} '(NR=='${CHIP}'+1)' ${TEMPDIR}/abszp1_$$`
	# if the correction factor could not be determined, do nothing
	
	testval=""
	testval=`echo ${CORRFAC} | ${P_GAWK} '{if ($1 < 0.01) {print "UNKNOWN"}}'`

	test ! -d ${RESULTDIR[${CHIP}]}/$3_IMAGES && \
	    mkdir ${RESULTDIR[${CHIP}]}/$3_IMAGES

	if [ ! "${testval}" = "UNKNOWN" ]; then 
	    ${P_IC} '%1 '${CORRFAC}' *' /$1/$2/${BASE}_${CHIP}$3.fits > \
		${RESULTDIR[${CHIP}]}/${BASE}_${CHIP}$3P.fits
	else
	    cp /$1/$2/${BASE}_${CHIP}$3.fits \
		${RESULTDIR[${CHIP}]}/${BASE}_${CHIP}$3P.fits
	    CORRFAC="UNKNOWN"
	    MAXZP="-1"
	fi

	mv ${RESULTDIR[${CHIP}]}/${BASE}_${CHIP}$3.fits \
	    ${RESULTDIR[${CHIP}]}/$3_IMAGES/
	cp ${RESULTDIRWEIGHT[${CHIP}]}/${BASE}_${CHIP}$3.weight.fits \
	    ${RESULTDIRWEIGHT[${CHIP}]}/${BASE}_${CHIP}$3P.weight.fits
	cp ${RESULTDIRWEIGHT[${CHIP}]}/${BASE}_${CHIP}$3.flag.fits \
	    ${RESULTDIRWEIGHT[${CHIP}]}/${BASE}_${CHIP}$3P.flag.fits
	if [ "${RESULTDIR[${CHIP}]}" != "$1/$2" ]; then
	    ln -s ${RESULTDIR[${CHIP}]}/${BASE}_${CHIP}$3P.fits \
		/$1/$2/${BASE}_${CHIP}$3P.fits
	fi
	if [ "${RESULTDIRWEIGHT[${CHIP}]}" != "$1/$2" ]; then
	    ln -s ${RESULTDIRWEIGHT[${CHIP}]}/${BASE}_${CHIP}$3P.weight.fits \
		/$1/WEIGHTS/${BASE}_${CHIP}$3P.weight.fits
	    ln -s ${RESULTDIRWEIGHT[${CHIP}]}/${BASE}_${CHIP}$3P.flag.fits \
		/$1/WEIGHTS/${BASE}_${CHIP}$3P.flag.fits
	fi
	    
        # write
	value ${CORRFAC}
	writekey /$1/$2/${BASE}_${CHIP}$3P.fits FLUXCORR "${VALUE}" REPLACE
	value ${MAXZP}
	writekey /$1/$2/${BASE}_${CHIP}$3P.fits ZP "${VALUE}" REPLACE
	value ${V_ABSPHOT_STDCAT_DIRECT}
	writekey /$1/$2/${BASE}_${CHIP}$3P.fits MAGSYS "'${VALUE}'" REPLACE
 	# for compatibility we also need these:
	value 1
	writekey /$1/$2/${BASE}_${CHIP}$3P.fits ZPCHOICE "${VALUE}" REPLACE
	value 0.0
	writekey /$1/$2/${BASE}_${CHIP}$3P.fits COEFF "${VALUE}" REPLACE
	value ${ABSZP}
	writekey /$1/$2/${BASE}_${CHIP}$3P.fits ZP1 "${VALUE}" REPLACE
	value 0.0
	writekey /$1/$2/${BASE}_${CHIP}$3P.fits COEFF1 "${VALUE}" REPLACE
	value ${ABSZP}
	writekey /$1/$2/${BASE}_${CHIP}$3P.fits ZP2 "${VALUE}" REPLACE
	value 0.0
	writekey /$1/$2/${BASE}_${CHIP}$3P.fits COEFF2 "${VALUE}" REPLACE
	value ${ABSZP}
	writekey /$1/$2/${BASE}_${CHIP}$3P.fits ZP3 "${VALUE}" REPLACE
	value 0.0
	writekey /$1/$2/${BASE}_${CHIP}$3P.fits COEFF3 "${VALUE}" REPLACE

	CHIP=$((${CHIP}+1))
    done
  done
  cp ${TEMPDIR}/PIC_chipscalingfactors_$$ /$1/$2/PIC_chipscalingfactors
  cp ${TEMPDIR}/PIC_chipscalingfactors_err_$$ /$1/$2/PIC_chipscalingfactors_err
fi


# if the fitting is done for the entire mosaic
# (assuming that no chip differences exist)
if [ "${V_ABSPHOT_FITTINGMETHODM2}" = "MOSAIC" ] || [ "${NCHIPS}" = "1" ]; then
  for CAT in ${CATS}
  do
    BASE=`basename ${CAT} _1$3.cat`
    test -f ${TEMPDIR}/abszp0_$$ && rm ${TEMPDIR}/abszp0_$$
    k=1
    # merge all catalogs of one exposure
    # get the zeropoint
    CATLIST=""
    while [ "${k}" -le "${NCHIPS}" ]
    do
	if [ -f /$1/$2/cat/${BASE}_${k}$3_merg.cat ]; then
	    CATLIST="${CATLIST} /$1/$2/cat/${BASE}_${k}$3_merg.cat"
	fi
	k=$((${k}+1))
    done
    ${P_LDACPUTXY} -i ${CATLIST} -o /$1/$2/cat/${BASE}_ALL$3_merg.cat

    # the arguments -a and -o have no effect but we need them for
    # compatibility
    ZPFLUX=`${P_FITSPIC} -m \
	-i /$1/$2/cat/${BASE}_ALL$3_merg.cat \
	-a /$1/$2/${BASE}_1$3.fits \
	-o dummy`

    ABSZP=`echo ${ZPFLUX} | ${P_GAWK} '{print $1}'`
    ABSZPERR=`echo ${ZPFLUX} | ${P_GAWK} '{print $2}'`
    ABSZPRMS=`echo ${ZPFLUX} | ${P_GAWK} '{print $3}'`
    # write the ZP (same for all chips)
    k=1
    while [ "${k}" -le "${NCHIPS}" ]
    do
	value ${ABSZP}
	writekey /$1/$2/${BASE}_${k}$3.fits ZP "${VALUE}" REPLACE
	value ${ABSZPERR}
	writekey /$1/$2/${BASE}_${k}$3.fits ZP_ERR "${VALUE}" REPLACE
	value ${ABSZPRMS}
	writekey /$1/$2/${BASE}_${k}$3.fits ZP_RMS "${VALUE}" REPLACE
	value ${V_ABSPHOT_STDCAT_DIRECT}
	writekey /$1/$2/${BASE}_${k}$3.fits MAGSYS "'${VALUE}'" REPLACE
	# for compatibility we also need these:
	value 1
	writekey /$1/$2/${BASE}_${k}$3.fits ZPCHOICE "${VALUE}" REPLACE
	value 0.0
	writekey /$1/$2/${BASE}_${k}$3.fits COEFF "${VALUE}" REPLACE
	value ${ABSZP}
	writekey /$1/$2/${BASE}_${k}$3.fits ZP1 "${VALUE}" REPLACE
	value 0.0
	writekey /$1/$2/${BASE}_${k}$3.fits COEFF1 "${VALUE}" REPLACE
	value ${ABSZP}
	writekey /$1/$2/${BASE}_${k}$3.fits ZP2 "${VALUE}" REPLACE
	value 0.0
	writekey /$1/$2/${BASE}_${k}$3.fits COEFF2 "${VALUE}" REPLACE
	value ${ABSZP}
	writekey /$1/$2/${BASE}_${k}$3.fits ZP3 "${VALUE}" REPLACE
	value 0.0
	writekey /$1/$2/${BASE}_${k}$3.fits COEFF3 "${VALUE}" REPLACE
	k=$((${k}+1))
    done
  done
fi
