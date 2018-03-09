#!/bin/bash -vx

# this script convolves all images to the same seeing

#$1: directory
#$2: image list (one file per line)

. progs.ini 

${P_GAWK} '($1 !~ /BACK_TYPE/ && $1 !~ /BACK_VALUE/) {print}' \
    ${DATACONF}/postcoadd.conf.sex > ${TEMPDIR}/postcoadd.conf.sex_$$
POSTCOADD=${TEMPDIR}/postcoadd.conf.sex_$$

dir=$1


cd ${dir}

test ! -d seeing && mkdir seeing
test -f seeing/seeing.dat && rm seeing/seeing.dat

echo "NUMBER"      >  ${TEMPDIR}/seeing_sexparam.asc_$$
echo "FWHM_IMAGE"  >> ${TEMPDIR}/seeing_sexparam.asc_$$
echo "MAG_AUTO"    >> ${TEMPDIR}/seeing_sexparam.asc_$$
echo "MAGERR_AUTO" >> ${TEMPDIR}/seeing_sexparam.asc_$$
echo "FLAGS"       >> ${TEMPDIR}/seeing_sexparam.asc_$$

cat $2 |\
{
  while read file
  do
    BASE=`basename ${file} .fits`
    
    weightstring=""
    if [ -f /${dir}/${BASE}.weight.fits ]; then
	weightstring="-WEIGHT_IMAGE /${dir}/${BASE}.weight.fits -WEIGHT_TYPE MAP_WEIGHT"
    fi

    ${P_SEX} -c ${POSTCOADD} \
	/${dir}/${file} ${weightstring} \
	-CATALOG_NAME ${TEMPDIR}/${BASE}_seeing.cat_$$ \
	-CATALOG_TYPE FITS_LDAC \
	-FILTER Y \
	-DETECT_MINAREA 5 \
	-DETECT_THRESH 10 \
	-ANALYSIS_THRESH 10 \
	-MAG_ZEROPOINT 20 \
	-DEBLEND_MINCONT 0.0001 \
	-PARAMETERS_NAME ${TEMPDIR}/seeing_sexparam.asc_$$ \
	-NTHREADS ${NPARA}
    

    ${P_LDACCONV}  -i ${TEMPDIR}/${BASE}_seeing.cat_$$ \
	-o ${TEMPDIR}/seeing_ldac.cat_$$ \
        -b 1 -c "sex" -f R
    
    ${P_LDACFILTER} -i ${TEMPDIR}/seeing_ldac.cat_$$ \
        -o ${TEMPDIR}/seeing_ldac_filt.cat_$$ -c "(Flag=0);"
    
    ${P_LDACCALC} -i ${TEMPDIR}/seeing_ldac_filt.cat_$$ \
	-o ${TEMPDIR}/seeing_ldac_filt.cat2_$$ \
	-c "(1./MAGERR_AUTO);" -n "snratio" "" -k FLOAT
    
    ${P_PREANISOTROPY} -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ \
	-t OBJECTS -s 0.1 -c snratio 10.0 1000.0 -k FWHM_IMAGE MAG_AUTO >& \
	${TEMPDIR}/preaniso_$$

    SMIN=`${P_GAWK} '($0 ~ /propose/) {print $8}'  ${TEMPDIR}/preaniso_$$`
    SMAX=`${P_GAWK} '($0 ~ /propose/) {print $12}' ${TEMPDIR}/preaniso_$$`
    MMIN=`${P_GAWK} '($0 ~ /propose/) {print $14}' ${TEMPDIR}/preaniso_$$`
    MMAX=`${P_GAWK} '($0 ~ /propose/) {print $18}' ${TEMPDIR}/preaniso_$$`

    ${P_LDACFILTER} -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ \
	-o ${TEMPDIR}/seeing_ldac_filt.cat3_$$ \
	-c "((((FWHM_IMAGE>${SMIN})AND(FWHM_IMAGE<${SMAX}))AND(MAG_AUTO>${MMIN}))AND(MAG_AUTO<${MMAX}));"

    ${P_LDACTOASC} -b -i ${TEMPDIR}/seeing_ldac_filt.cat3_$$ -t OBJECTS \
        -k FWHM_IMAGE > ${TEMPDIR}/seeing.asc_$$

    seeing=`${P_GETSTATS} -i ${TEMPDIR}/seeing.asc_$$ -m median`

    echo ${file} $seeing >> ${dir}/seeing/seeing.dat
  done
}

rm $1/$2
rm ${TEMPDIR}/seeing_ldac_filt.cat*_$$
rm ${TEMPDIR}/*_$$
