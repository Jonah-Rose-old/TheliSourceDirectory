#!/bin/bash -xv

# this script creates a standard star catalogue from an image

# $1: image name including full path
# $2: detection threshold
# $3: detection minarea
# $4: catpath

. progs.ini

${P_GAWK} '($1 !~ /BACK_TYPE/ && $1 !~ /BACK_VALUE/) {print}' \
    ${DATACONF}/postcoadd.conf.sex > ${TEMPDIR}/postcoadd.conf.sex_$$
POSTCOADD=${TEMPDIR}/postcoadd.conf.sex_$$

BASE=`basename $1`
BASE2=`basename $1 .fits`
DIR=`dirname $1`

if [ ! -f ${DIR}/${BASE2}.weight.fits ]; then
    ${P_SEX} -c ${POSTCOADD} $1 \
	-CATALOG_NAME ${TEMPDIR}/${BASE}_std.cat_$$ \
	-DETECT_MINAREA $3 \
	-DETECT_THRESH $2 \
	-ANALYSIS_THRESH $2 \
	-FILTER Y \
	-CATALOG_TYPE FITS_LDAC \
	-NTHREADS ${NPARA}
else
    ${P_SEX} -c ${POSTCOADD} $1 \
	-CATALOG_NAME ${TEMPDIR}/${BASE}_std.cat_$$ \
	-WEIGHT_IMAGE ${DIR}/${BASE2}.weight.fits \
	-WEIGHT_TYPE MAP_WEIGHT \
	-DETECT_MINAREA $3 \
	-DETECT_THRESH $2 \
	-ANALYSIS_THRESH $2 \
	-FILTER Y \
	-CATALOG_TYPE FITS_LDAC \
	-NTHREADS ${NPARA}
fi

${P_LDACCONV} -i ${TEMPDIR}/${BASE}_std.cat_$$ \
    -o ${TEMPDIR}/${BASE}_stdldac.cat_$$ -b 1 -c "sex" -f R

${P_LDACADDKEY} -i ${TEMPDIR}/${BASE}_stdldac.cat_$$ \
    -o ${TEMPDIR}/${BASE}_tmp.cat_$$ \
    -t OBJECTS -k nu 1.0 FLOAT ""

${P_LDACCALC} -i  ${TEMPDIR}/${BASE}_tmp.cat_$$ \
    -o  ${TEMPDIR}/${BASE}_tmp.cat1_$$ -t OBJECTS \
    -c "(FLUX_RADIUS);" -n "rg" "" -k FLOAT \
    -c "(Xpos-1.0);" -n "x" "" -k LONG \
    -c "(Ypos-1.0);" -n "y" "" -k LONG \
    -c "(Xpos);" -n "xbad" "" -k FLOAT \
    -c "(Ypos);" -n "ybad" "" -k FLOAT

cp ${TEMPDIR}/${BASE}_tmp.cat1_$$ ~/.theli/scripts/theli_PIC.cat

${P_LDACTOASC} -i ${TEMPDIR}/${BASE}_tmp.cat1_$$ -b -t FIELDS \
    -k FITSFILE SEXBKGND SEXBKDEV -s \
    > ${TEMPDIR}/${BASE}_tmp.asc_$$

# create a config file for asctoldac on the fly

echo "VERBOSE = DEBUG"       >  ${TEMPDIR}/asctoldac_tmp.conf_$$
echo "COL_NAME  = IMAGE"     >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo "COL_TTYPE = STRING"    >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo "COL_HTYPE = STRING"    >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo 'COL_COMM = ""'         >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo 'COL_UNIT = ""'         >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo 'COL_DEPTH = 128'       >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo "COL_NAME  = SKY_MODE"  >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo "COL_TTYPE = FLOAT"     >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo "COL_HTYPE = FLOAT"     >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo 'COL_COMM = ""'         >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo 'COL_UNIT = ""'         >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo 'COL_DEPTH = 1'         >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo "COL_NAME  = SKY_SIGMA" >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo "COL_TTYPE = FLOAT"     >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo "COL_HTYPE = FLOAT"     >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo 'COL_COMM = ""'         >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo 'COL_UNIT = ""'         >> ${TEMPDIR}/asctoldac_tmp.conf_$$
echo 'COL_DEPTH = 1'         >> ${TEMPDIR}/asctoldac_tmp.conf_$$

${P_ASCTOLDAC} -i ${TEMPDIR}/${BASE}_tmp.asc_$$ \
    -c ${TEMPDIR}/asctoldac_tmp.conf_$$ -t HFINDPEAKS \
    -o ${TEMPDIR}/${BASE}_hfind.cat_$$ -b 1 -n "KSB"

# now transfer the HFINDPEAKS table to the SEX catalog
${P_LDACADDTAB} -i ${TEMPDIR}/${BASE}_tmp.cat1_$$ \
    -o ${TEMPDIR}/${BASE}_tmp.cat2_$$ \
    -p ${TEMPDIR}/${BASE}_hfind.cat_$$ \
    -t HFINDPEAKS

${P_LDACFILTER} -i ${TEMPDIR}/${BASE}_tmp.cat2_$$ \
    -o ${TEMPDIR}/${BASE}_coadd.cat_$$ \
    -c "(rg>0.0);"

# make a skycat catalogue
${P_LDACTOSKYCAT} -i ${TEMPDIR}/${BASE}_coadd.cat_$$ -t OBJECTS \
    -k SeqNr ALPHA_J2000 \
    DELTA_J2000 MAG_AUTO -s -l id_col SeqNr ra_col \
    ALPHA_J2000 dec_col DELTA_J2000 mag_col MAG_AUTO \
    > ~/.theli/scripts/theli_mystd.skycat
numstars=`${P_GAWK} 'END {if ($1~/---/ || $1~/ /) print 0; else print $1}' ~/.theli/scripts/theli_mystd.skycat`

if [ "${numstars}" = "0" ]; then
    exit 1
fi

${P_LDACTOASC} -i ${TEMPDIR}/${BASE}_coadd.cat_$$ -t OBJECTS \
    -k ALPHA_J2000 DELTA_J2000 MAG_AUTO -b \
    > ${TEMPDIR}/${BASE}_coadd.asciicat_$$

${P_LDACTOASC} -i ${TEMPDIR}/${BASE}_coadd.cat_$$ -t OBJECTS \
    -k ALPHA_J2000 DELTA_J2000 A_WORLD B_WORLD THETA_J2000 -b \
    > ${TEMPDIR}/${BASE}_coadd.ds9cat_$$

# make a ds9 catalogue
echo global color=red select=0 highlite=1 wcs=wcs > \
    ~/.theli/scripts/theli_mystd.reg
echo "fk5" >> ~/.theli/scripts/theli_mystd.reg

${P_GAWK} '{print "ellipse("$1","$2","2.*$3","2.*$4","$5")"}' \
    ${TEMPDIR}/${BASE}_coadd.ds9cat_$$ >> ~/.theli/scripts/theli_mystd.reg

${P_GAWK} '{print "1 ", NR, $1, $2, $3}' \
    ${TEMPDIR}/${BASE}_coadd.asciicat_$$ > \
    ${TEMPDIR}/${BASE}_coadd.asciicat2_$$

\rm ${TEMPDIR}/postcoadd.conf.sex_$$

# make a scamp catalogue and config file
echo VERBOSE = DEBUG         >  ${TEMPDIR}/makescamp.conf_$$
echo COL_NAME  = X_WORLD     >> ${TEMPDIR}/makescamp.conf_$$
echo COL_TTYPE = DOUBLE      >> ${TEMPDIR}/makescamp.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makescamp.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makescamp.conf_$$
echo COL_NAME  = Y_WORLD     >> ${TEMPDIR}/makescamp.conf_$$
echo COL_TTYPE = DOUBLE      >> ${TEMPDIR}/makescamp.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makescamp.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makescamp.conf_$$
echo COL_NAME  = MAG         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_TTYPE = FLOAT       >> ${TEMPDIR}/makescamp.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makescamp.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makescamp.conf_$$
echo COL_NAME  = MAGERR      >> ${TEMPDIR}/makescamp.conf_$$
echo COL_TTYPE = FLOAT       >> ${TEMPDIR}/makescamp.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makescamp.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makescamp.conf_$$
echo COL_NAME  = ERRA_WORLD  >> ${TEMPDIR}/makescamp.conf_$$
echo COL_TTYPE = FLOAT       >> ${TEMPDIR}/makescamp.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makescamp.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makescamp.conf_$$
echo COL_NAME  = ERRB_WORLD  >> ${TEMPDIR}/makescamp.conf_$$
echo COL_TTYPE = FLOAT       >> ${TEMPDIR}/makescamp.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makescamp.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makescamp.conf_$$
echo COL_NAME  = ERRTHETA_WORLD  >> ${TEMPDIR}/makescamp.conf_$$
echo COL_TTYPE = FLOAT       >> ${TEMPDIR}/makescamp.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makescamp.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makescamp.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makescamp.conf_$$

${P_LDACTOASC} -i ${TEMPDIR}/${BASE}_coadd.cat_$$ -t OBJECTS \
    -k ALPHA_J2000 DELTA_J2000 MAG_AUTO MAGERR_AUTO ERRA_WORLD \
    ERRB_WORLD ERRTHETA_WORLD -b > ${TEMPDIR}/${BASE}_coadd.asciicat_$$

${P_GAWK} '{print $0}' ${TEMPDIR}/${BASE}_coadd.asciicat_$$ > \
    ${TEMPDIR}/tmp_scamp_$$

${P_ASCTOLDAC} -a ${TEMPDIR}/tmp_scamp_$$ \
    -o ${TEMPDIR}/tmp2_scamp0_$$ \
    -t OBJECTS -c ${TEMPDIR}/makescamp.conf_$$

${P_LDACRENTAB} -i ${TEMPDIR}/tmp2_scamp0_$$ \
    -o ${TEMPDIR}/tmp2_scamp1_$$ \
    -t OBJECTS LDAC_OBJECTS 
${P_LDACRENTAB} -i ${TEMPDIR}/tmp2_scamp1_$$ \
    -o ~/.theli/scripts/theli_mystd_scamp.cat \
    -t FIELDS LDAC_IMHEAD

catdir=`dirname $1`
cd ${catdir}
cd ../cat

mkdir -p /$4/ds9cat
mkdir -p /$4/skycat

cp ~/.theli/scripts/theli_mystd.skycat /$4/skycat
cp ~/.theli/scripts/theli_mystd.reg /$4/ds9cat

# rm ${TEMPDIR}/*_$$
