BASHPATH -xv

# $1: main directory
# $2: Science directory
# $3: Sky directory ('noskydir' if not present)
# $4: chips to be processed

#######################################################################################
# How this works:
#
# This script creates masked OFC images. If a two-pass is requested, it will
# automatically apply the background correction and then creates refined 
# masked OFC images
#
# -- NO SKY directory:
#
#    -- one-pass: with or without sextractor masking
#    -- two-pass: first pass without sextractor masking, second pass with sextractor 
#                 masking (if requested); script will loop once
# 
# -- WITH SKY directory:
#
#    -- same as above, but create the masked images based on the SKY directory
#
#######################################################################################


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

##################################################
# BEGIN MASKING SPECIFIC STUFF
#

# Do we work on SKY or on SCIENCE images?
MASKDATADIR=$2
if [ "$3" != "noskydir" ]; then
    MASKDATADIR=$3
fi

#
# END MASKING SPECIFIC STUFF
##################################################

for CHIP in $4; do
  RESULTDIR[${CHIP}]="$1/${MASKDATADIR}"
done

# Delete old data and recreate directory structures
if [ "$3" = "noskydir" ]; then
    rm -rf /$1/$2/MASK_IMAGES /$1/$2/BACKGROUND
    mkdir -p /$1/$2/OFC_IMAGES
    mkdir -p /$1/$2/MASK_IMAGES
else
    rm -rf /$1/$3/MASK_IMAGES /$1/$3/BACKGROUND
    mkdir -p /$1/$2/OFC_IMAGES
    mkdir -p /$1/$3/OFC_IMAGES
    mkdir -p /$1/$3/MASK_IMAGES
fi


##################################################################
# A LOOP that runs once or twice depending on one-pass or two-pass
##################################################################

loop=1
while [ ${loop} -le 2 ]; do

    # A suffix, depending on the loop we are in
    suffix="OFC"
    if [ ${loop} -eq 2 ] && [ ${V_BACK_TWOPASS} = "Y" ]; then
	suffix="OFCb"
    fi

    ######################################################
    # No SExtractor masking if:
    # -- in two-pass mode and this is the first pass
    # -- SExtractor masking has been deactivated
    ######################################################

    if [[ "${V_BACK_TWOPASS}" = "Y" && ${loop} -eq 1 || \
	 "${V_BACK_DETECTMINAREA}"_A == "_A" || \
	 "${V_BACK_DETECTTHRESH}"_A == "_A" ]]; then

	# Just link the unmasked images and call them .mask.fits
	for CHIP in $4; do
	    ls -1 /${RESULTDIR[${CHIP}]}/*_${CHIP}${suffix}.fits > ${TEMPDIR}/backmasks_$$
	    cat ${TEMPDIR}/backmasks_$$ |\
            {
		while read FILE; do
		    BASE=`basename ${FILE} ${suffix}.fits`
		    ln -sf ${FILE} /${RESULTDIR[${CHIP}]}/${BASE}OFC.mask.fits
		    # Establish the link structure
		    if [ "${RESULTDIR[${CHIP}]}" != "$1/${MASKDATADIR}" ]; then
			ln -s ${RESULTDIR[${CHIP}]}/${BASE}OFC.mask.fits \
			    $1/${MASKDATADIR}/${BASE}OFC.mask.fits
		    fi
		done
	    }
	done
    else


    ######################################################
    # SExtractor masking in all other cases
    ######################################################

	for CHIP in $4;	do
	    # delete globalweight_dummy (region masking) if it exists in the WEIGHTS directory;
	    # It might be for a different instrument, or the polygons might have changed
	    test -e /$1/WEIGHTS/globalweight_dummy_${CHIP}.fits && \
		 rm /$1/WEIGHTS/globalweight_dummy_${CHIP}.fits
	    
	    ls -1 /${RESULTDIR[${CHIP}]}/*_${CHIP}${suffix}.fits > ${TEMPDIR}/backmasks_$$
	    
	    NUMTHREAD=`${SCRIPTDIR}/get_nthreads.sh ${NCHIPS} ${NPARA} ${CHIP}`
	    
	    FILE=`${P_GAWK} '(NR==1) {print $0}' ${TEMPDIR}/backmasks_$$`
	    
	    # remove existing mask files
	    rm ${RESULTDIR[${CHIP}]}/*_${CHIP}OFC.mask.fits
	    
	    cat ${TEMPDIR}/backmasks_$$ |\
            {
		while read FILE; do
		    BASE=`basename ${FILE} ${suffix}.fits`
		    
		    # check if there is a region file, and prep sextractor for using it
		    get_region
		    prepare_sex_for_region $1
		    
		    # run sextractor to mask objects
		    ${P_SEX} ${FILE} -c ${DATACONF}/background_masks.sex \
			-CATALOG_NAME ${RESULTDIR[${CHIP}]}/${BASE}OFC.mask.cat \
			-CHECKIMAGE_NAME ${RESULTDIR[${CHIP}]}/${BASE}OFC.mask.fits \
			-CHECKIMAGE_TYPE SEGMENTATION \
			-DETECT_MINAREA ${V_BACK_DETECTMINAREA} \
			-DETECT_THRESH ${V_BACK_DETECTTHRESH} \
			-ANALYSIS_THRESH ${V_BACK_DETECTTHRESH} \
			-BACK_SIZE 256 \
			-FILTER ${V_BACK_SEXFILTER} \
			-NTHREADS ${NUMTHREAD} \
			${backstring} ${weightstring} \
			-VERBOSE_TYPE NORMAL
		    
		    # optionally, expand the SExtractor mask and overwrite the input image
		    if [ "${V_BACK_MASKEXPAND}_A" != "_A" ]; then
			wstring="-w /$1/WEIGHTS/globalweight_dummy_${CHIP}.fits"
			if [ ! -e /$1/WEIGHTS/globalweight_dummy_${CHIP}.fits ]; then
			    mkdir -p /$1/WEIGHTS/
			    ${P_IC} -p -32 -c ${SIZEX[${CHIP}]} ${SIZEY[${CHIP}]} '1' > \
				/$1/WEIGHTS/globalweight_dummy_${CHIP}.fits
			fi
			
			${P_EXPANDSEXMASK} \
			    -i ${RESULTDIR[${CHIP}]}/${BASE}OFC.mask.fits \
			    -o ${RESULTDIR[${CHIP}]}/${BASE}OFC.mask.fits \
			    -c ${RESULTDIR[${CHIP}]}/${BASE}OFC.mask.cat \
			    -r ${V_BACK_MASKEXPAND} ${wstring}
		    fi
		    
		    rm ${RESULTDIR[${CHIP}]}/${BASE}OFC.mask.cat
			
		    # set pixels in the input image to a very large negative value
		    # if they are (a) part of an object, or (b) masked by a ds9 region file;
		    # we can safely overwrite the comparison image (-t) with the result (-o)
		    
		    if [ "${regionfile}"_A = "_A" ]; then
			regionstring=""
		    else
			regionstring="-p ${regionfile}"
		    fi
		    
		    # Finally, we ALWAYS mask the OFC image as these are the data from which the
		    # final background model has to be calculated. 
		    ${P_FITSPOLYGON} \
			-i $1/${MASKDATADIR}/${BASE}OFC.fits \
			-t ${RESULTDIR[${CHIP}]}/${BASE}OFC.mask.fits \
			-o ${RESULTDIR[${CHIP}]}/${BASE}OFC.mask.fits \
			-w 0. \
			-v -1.0e9 \
			${regionstring}
		    
		    # Establish the link structure
		    if [ "${RESULTDIR[${CHIP}]}" != "$1/${MASKDATADIR}" ]; then
			ln -s ${RESULTDIR[${CHIP}]}/${BASE}OFC.mask.fits \
			    $1/${MASKDATADIR}/${BASE}OFC.mask.fits
		    fi
		done
	    }
	done
    fi


    # Current state:
    # The masked images have been created and reside in the SCIENCE or SKY directories.

    ######################################################
    # CREATE THE BACKGROUND MODEL AND SUBTRACT IT
    ######################################################

    # Run if
    # 1: we don't have SKY images
    # 2: we do have SKY images and are in the final loop, which may be loop 1 or 2 depending on 1-pass or 2-pass

    if [[ "$3" = "noskydir" || \
	"$3" != "noskydir" && ${loop} -eq 2 && "${V_BACK_TWOPASS}" = "Y" || \
	"$3" != "noskydir" && ${loop} -eq 1 && "${V_BACK_TWOPASS}" = "N" ]]; then
	./process_background_assistant_para.sh $1 $2 $1/${MASKDATADIR} "$4"
    fi

    for CHIP in $4; do
	if [ "${V_BACK_TWOPASS}" = "N" ] || [ ${loop} -eq 2 ]; then
	    ${P_RENAME} OFCb OFCB /$1/$2/*_${CHIP}OFCb.fits
	fi
    done

    # If the background model is calculated from sky exposures, 
    # then we fully correct them as well (either for 2-pass mode, or at least for reference)
    if [ "$3" != "noskydir" ]; then
	./process_background_assistant_para.sh $1 $3 $1/${MASKDATADIR} "$4"
	for CHIP in $4; do
	    if [ "${V_BACK_TWOPASS}" = "N" ] || [ ${loop} -eq 2 ]; then
		${P_RENAME} OFCb OFCB /$1/$3/*_${CHIP}OFCb.fits
	    fi
	done
    fi

    # Exit the loop if in one-pass mode
    if [ "${V_BACK_TWOPASS}" = "N" ]; then

	for CHIP in $4; do
	    mv /$1/$2/*_${CHIP}OFC.fits /$1/$2/OFC_IMAGES
	    if [ "$3" = "noskydir" ]; then
		mv /$1/$2/*_${CHIP}OFC.mask.fits /$1/$2/MASK_IMAGES
	    else
		mv /$1/$3/*_${CHIP}OFC.fits /$1/$3/OFC_IMAGES
		mv /$1/$3/*_${CHIP}OFC.mask.fits /$1/$3/MASK_IMAGES
	    fi
	done

	break
    fi

    loop=$((${loop}+1))
done

if [ "${V_BACK_TWOPASS}" = "Y" ]; then

    for CHIP in $4; do
	mv /$1/$2/*_${CHIP}OFC.fits /$1/$2/OFC_IMAGES
	if [ "$3" = "noskydir" ]; then
	    mv /$1/$2/*_${CHIP}OFC.mask.fits /$1/$2/MASK_IMAGES
	else
	    mv /$1/$3/*_${CHIP}OFC.fits /$1/$3/OFC_IMAGES
	    mv /$1/$3/*_${CHIP}OFC.mask.fits /$1/$3/MASK_IMAGES
	fi
    done

fi
