#!/bin/bash -xv

#$1: main directory (filter)
#$2: Bias directory
#$3: Flat directory
#$4: Science directory
#$5: chips to be processed

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

# the resultdir is where the output coadded images
# will go. If ONE image of the corresponding chip
# is a link the image will go to THIS directory
for CHIP in $5
do
  RESULTDIR[${CHIP}]="$1/$4"
done


# perform preprocessing (overscan correction,
# BIAS subtraction, first flatfield pass)
for CHIP in $5
do

    # check whether the superflat exists.
    # if yes, then delete it.
    test -f $1/$4/$4_${CHIP}.fits && rm $1/$4/$4_${CHIP}.fits
    test -f $1/$4/$4_${CHIP}_illum.fits && rm $1/$4/$4_${CHIP}_illum.fits
    test -f $1/$4/$4_${CHIP}_fringe.fits && rm $1/$4/$4_${CHIP}_fringe.fits

    FILES=`ls $1/$4/*_${CHIP}.fits`
    
    for FILE in ${FILES}
    do
	BAYERPATTERN=`get_key ${FILE} BAYERPAT`
	if [ -L ${FILE} ]; then
	    LINK=`${P_READLINK} ${FILE}`
 	    BASE=`basename ${LINK} .fits`
 	    DIR=`dirname ${LINK}`
 	    ln -s ${DIR}/${BASE}OFC.fits $1/$4/${BASE}OFC.fits
 	    RESULTDIR[${CHIP}]=`dirname ${LINK}`    
	fi
    done 
    
    MAXX=$(( ${CUTX[${CHIP}]} + ${SIZEX[${CHIP}]} - 1 ))
    MAXY=$(( ${CUTY[${CHIP}]} + ${SIZEY[${CHIP}]} - 1 ))
    
    do_overscan=Y
    if [ "${OVSCANX1[${CHIP}]}" = "0" ] && \
	[ "${OVSCANX2[${CHIP}]}" = "0" ]; then
	do_overscan=N
    fi
    
    # flat scaling
    if [ "${V_DO_FLAT}" = "Y" ]; then
	FLATSCALEVALUE=`cat /$1/$3/.maxlevel.dat`
	FLATFLAG="-FLAT_SCALE ${V_DO_FLAT} -FLAT_SCALEVALUE ${FLATSCALEVALUE}" 
	if [ "${FLATSCALEVALUE}_A" = "_A" ]; then
	    echo `${P_ERRTEST} 0`": Could not find /$1/$3/.maxlevel.dat. Recreate flats!"
	    exit
	fi
    else
	FLATFLAG="-FLAT_SCALE ${V_DO_FLAT}"
    fi

    get_statminsection

    # check if a dark is subtracted, and if yes,
    # then if it needs to be rescaled
    # we subtract the mode of the dark to account for the bias level (on zero order)

    BIASDARKIMAGE=/$1/$2/$2_${CHIP}.fits

    if [ "${V_CAL_USEDARK}" = "TRUE" ] && [ "${V_DO_BIAS}" = "Y" ]; then
	DARKTIME=`get_key /$1/$2/$2_${CHIP}.fits EXPTIME`
	if [ "${DARKTIME}"_A != "_A" ]; then
	    echo "ok, dark exposure time present; rescaling"
	    testimage=`ls /$1/$4/*_${CHIP}.fits | ${P_GAWK} '(NR==1)'`
	    EXPTIME=`get_key ${testimage} EXPTIME`
	    MODE=`${P_IMSTATS} -d 6 /$1/$2/$2_${CHIP}.fits \
		-s ${statminsection_imstats} | ${P_GAWK} '($0!~/#/) {print $2}'`
	    RESCALE=`echo ${DARKTIME} ${EXPTIME} | \
		${P_GAWK} '{x=1.-$1/$2; if (x<0.01 && x> -0.01) print "NO"; else print $2/$1}'`
	    if [ "${RESCALE}" != "NO" ]; then
		${P_IC} '%1 '${MODE}' - '${RESCALE}' *' \
		    /$1/$2/$2_${CHIP}.fits > \
		    /$1/$2/$2_${CHIP}rescaled.fits
		BIASDARKIMAGE=/$1/$2/$2_${CHIP}rescaled.fits
	    fi
	fi
    fi
    
    # health checks
    if [ "${V_DO_BIAS}" = "Y" ] && [ ! -f ${BIASDARKIMAGE} ]; then
	echo `${P_ERRTEST} 0`": Could not find master BIAS for chip ${CHIP}"
	continue
    fi
    if [ "${V_DO_FLAT}" = "Y" ] && [ ! -f /$1/$3/$3_${CHIP}.fits ]; then
	echo `${P_ERRTEST} 0`": Could not find master FLAT for chip ${CHIP}"
	continue
    fi

    # without nonlinearity correction
    if [ "${V_NONLINCORR}" = "N" ] || [ ! -f ${INSTDIR}/${INSTRUMENT}".nldat" ]; then
        # overscan correct, bias subtract and flatfield
        # science images:

	# enforce an upper limit to maximages (required by preprocess)
	if [ ${NFRAMES} -gt 1000 ]; then
	    NFRAMES=1000
	fi

	${P_IMRED_ECL:?} `ls /$1/$4/*_${CHIP}.fits` \
	    -c ${CONF}/imred.conf \
	    -MAXIMAGES ${NFRAMES} \
	    -OVERSCAN ${do_overscan} \
	    -OVERSCAN_REGION ${OVSCANX1[${CHIP}]},${OVSCANX2[${CHIP}]} \
	    -OVERSCAN_REJECT ${V_CAL_OVSCANNLOW},${V_CAL_OVSCANNHIGH} \
	    -BIAS ${V_DO_BIAS} \
	    -BIAS_IMAGE ${BIASDARKIMAGE} \
	    -FLAT ${V_DO_FLAT} \
	    -FLAT_IMAGE /$1/$3/$3_${CHIP}.fits \
	    -COMBINE N \
	    -OUTPUT Y \
	    -OUTPUT_DIR /$1/$4/ \
	    -OUTPUT_SUFFIX OFC.fits \
	    -TRIM Y \
	    -FLAT_THRESHHOLD 0.1 \
	    -TRIM_REGION ${CUTX[${CHIP}]},${MAXX},${CUTY[${CHIP}]},${MAXY} \
	    -STATSSEC ${statminsection} ${FLATFLAG}
    fi

    # with nonlinearity correction
    if [ "${V_NONLINCORR}" = "Y" ] && [ -f ${INSTDIR}/${INSTRUMENT}".nldat" ]; then
        # overscan correct and bias subtract science images:
	${P_IMRED_ECL:?} `ls /$1/$4/*_${CHIP}.fits` \
	    -c ${CONF}/imred.conf \
	    -MAXIMAGES ${NFRAMES} \
	    -OVERSCAN ${do_overscan} \
	    -OVERSCAN_REGION ${OVSCANX1[${CHIP}]},${OVSCANX2[${CHIP}]} \
	    -OVERSCAN_REJECT ${V_CAL_OVSCANNLOW},${V_CAL_OVSCANNHIGH} \
	    -BIAS ${V_DO_BIAS} \
	    -BIAS_IMAGE ${BIASDARKIMAGE} \
	    -COMBINE N \
	    -OUTPUT Y \
	    -OUTPUT_DIR /$1/$4/ \
	    -OUTPUT_SUFFIX O.fits \
	    -TRIM Y \
	    -TRIM_REGION ${CUTX[${CHIP}]},${MAXX},${CUTY[${CHIP}]},${MAXY} \
	    -STATSSEC ${statminsection} ${FLATFLAG}

	# do the nonlinearity correction
	ls /$1/$4/*_${CHIP}O.fits > ${TEMPDIR}/nonlinlist_$$
	cat ${TEMPDIR}/nonlinlist_$$ |\
          {
	    while read file
	    do
		BASE=`basename ${file} _${CHIP}O.fits`
		${P_FITSNONLINEARITY} -i ${file} -o ${RESULTDIR[${CHIP}]}/${BASE}_${CHIP}ON.fits \
		    -c ${CHIP} -f ${INSTDIR}/${INSTRUMENT}".nldat"
		mv ${RESULTDIR[${CHIP}]}/${BASE}_${CHIP}ON.fits ${RESULTDIR[${CHIP}]}/${BASE}_${CHIP}O.fits 
	    done
	}

	# flatfield the images
	${P_IMRED_ECL:?} `ls /$1/$4/*_${CHIP}O.fits` \
	    -c ${CONF}/imred.conf \
	    -MAXIMAGES ${NFRAMES} \
	    -FLAT ${V_DO_FLAT} \
	    -FLAT_IMAGE /$1/$3/$3_${CHIP}.fits \
	    -COMBINE N \
	    -OUTPUT Y \
	    -OUTPUT_DIR /$1/$4/ \
	    -OUTPUT_SUFFIX FC.fits \
	    -FLAT_THRESHHOLD 0.1 \
	    -STATSSEC ${statminsection} ${FLATFLAG}

	rm /$1/$4/*_${CHIP}O.fits
    fi
    
    # do the demosaicing if requested
    if [ ! "${BAYERPATTERN}_A" = "_A" ]; then
	BAYERFILES=`ls $1/$4/*_${CHIP}OFC.fits`
	for FILE in ${BAYERFILES}
	do
	    BAYERPATTERN=`get_key ${FILE} BAYERPAT`

	    BASE=`basename ${FILE} _${CHIP}OFC.fits`
	    ${P_FITSDEMOSAICBAYER} -i ${FILE} -p ${BAYERPATTERN} -q 2
	    
	    ${P_REPLACEKEY} $1/$4/${BASE}_${CHIP}OFC.R.fits "FILTER  = 'Red'" FILTER
	    ${P_REPLACEKEY} $1/$4/${BASE}_${CHIP}OFC.G.fits "FILTER  = 'Green'" FILTER
	    ${P_REPLACEKEY} $1/$4/${BASE}_${CHIP}OFC.B.fits "FILTER  = 'Blue'" FILTER

       	    mv $1/$4/${BASE}_${CHIP}OFC.R.fits $1/$4/${BASE}_R_${CHIP}OFC.fits
	    mv $1/$4/${BASE}_${CHIP}OFC.G.fits $1/$4/${BASE}_G_${CHIP}OFC.fits
	    mv $1/$4/${BASE}_${CHIP}OFC.B.fits $1/$4/${BASE}_B_${CHIP}OFC.fits
	    rm ${FILE}
	done
    fi

    if [ ! -d /$1/$4/SPLIT_IMAGES ]; then
 	mkdir /$1/$4/SPLIT_IMAGES
    fi
    mv /$1/$4/*_${CHIP}.fits /$1/$4/SPLIT_IMAGES
done
