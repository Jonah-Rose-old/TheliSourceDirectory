BASHPATH -xv

# takes a series of resampled images, splits it into four
# quadrants and makes a series of TIFF images that can be animated

# $1: script dir
# $2: coaddition directory
# $3: mask (mask or nomask)
# $4: chipid

DIR=`pwd`

cd $1

. progs.ini

cd $2/movie

# check if maximum overlapped cropped images exist

if [ ! -d "cropped_$4" ]; then

    mkdir cropped_$4
    cd cropped_$4

    # check the imageid
    ls /$2/*resamp.fits > ${TEMPDIR}/resamp.list_$$
    exec < ${TEMPDIR}/resamp.list_$$
    while read file
    do
	imageid=`basename ${file} resamp.fits | \
	    ${P_GAWK} 'BEGIN{FS="OFC"} {print $(NF-1)}' | \
	    ${P_GAWK} 'BEGIN{FS="_"} {print $NF}'`
	if [ "${imageid}" = "$4" ]; then
	    ln -s ${file} /$2/movie/cropped_$4
	fi
    done

    if [ "${3}" = "mask" ]; then
        # mask images with the weights
	ls *.fits > ${TEMPDIR}/list_$$
	cat ${TEMPDIR}/list_$$ |\
	    {
	    while read file
	      do
	      BASE=`basename ${file} .fits`
	      ${P_IC} '%1 0. %2 fabs 1.0e-06 > ?' ${file} ../../${BASE}.weight.fits > ${BASE}.fits2
	      mv ${BASE}.fits2 ${file}
	    done
	}
    fi

    # get image size limits
    ${P_DFITS} *.fits | ${P_FITSORT} CRPIX1 CRPIX2 NAXIS1 NAXIS2 \
	| ${P_GAWK} '($1 !~ /FILE/) {print $2, $3, $4, $5}' > ${TEMPDIR}/geom_$$
    
    xlow=`${P_GAWK} 'BEGIN {min = 1000000} {if ($1<min) min = $1} END {print (-1)*min}' ${TEMPDIR}/geom_$$`
    ylow=`${P_GAWK} 'BEGIN {min = 1000000} {if ($2<min) min = $2} END {print (-1)*min}' ${TEMPDIR}/geom_$$`
    xhigh=`${P_GAWK} 'BEGIN {min = 1000000} {if (($3-$1)<min) min = $3-$1} END {print min}' ${TEMPDIR}/geom_$$`
    yhigh=`${P_GAWK} 'BEGIN {min = 1000000} {if (($4-$2)<min) min = $4-$2} END {print min}' ${TEMPDIR}/geom_$$`
    
    # get the maximum overlap
    ls -1 *.fits > ${TEMPDIR}/maxoverlaplist_$$
    cat ${TEMPDIR}/maxoverlaplist_$$ |\
	{
	while read file
	  do
	  crpix1=`${P_DFITS} ${file} | ${P_FITSORT} CRPIX1 | ${P_GAWK} '($1 !~ /FILE/) {print $2}'`
	  crpix2=`${P_DFITS} ${file} | ${P_FITSORT} CRPIX2 | ${P_GAWK} '($1 !~ /FILE/) {print $2}'`
	  BASE=`basename ${file} .fits`
	  xlownew=`echo ${xlow} ${crpix1} | ${P_GAWK} '{print $2+$1+1}'`
	  ylownew=`echo ${ylow} ${crpix2} | ${P_GAWK} '{print $2+$1+1}'`
	  xhighnew=`echo ${xhigh} ${crpix1} | ${P_GAWK} '{print $2+$1}'`
	  yhighnew=`echo ${yhigh} ${crpix2} | ${P_GAWK} '{print $2+$1}'`
	  ${P_FITSCUT} -i ${file} -o ${BASE}_cropped.fits \
	      -x ${xlownew} ${xhighnew} ${ylownew} ${yhighnew}
	  crpix1new=`echo ${xlownew} ${crpix1} | ${P_GAWK} '{print $2-$1+1}'`
	  crpix2new=`echo ${ylownew} ${crpix2} | ${P_GAWK} '{print $2-$1+1}'`
	  ${P_REPLACEKEY} ${BASE}_cropped.fits "CRPIX1  = ${crpix1new}" CRPIX1
	  ${P_REPLACEKEY} ${BASE}_cropped.fits "CRPIX2  = ${crpix2new}" CRPIX2
	  rm ${file}
	done
    }
fi
