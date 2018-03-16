BASHPATH -xv

# this script downloads a photometric reference catalogue (SDSS or 2MASS)
# creates an object catalog for an astrometrically calibrated image,
# and calculates an average photometric zeropoint that is written into
# the images header

# $1: full image path
# $2: catalog (SDSS or 2MASS) 
# $3: filter (u,g,r,i,z,J,H,Ks)
# $4: max phot error
# $5: saturation level
# $6: magtype
# $7: aperture
# $8: download server address

server=$8

. progs.ini
. bash_functions.include
. bash.include

DIR=`pwd`

#dir=`dirname $1`
image=$1
filter=$3

###############################################################
#
# create object catalog
#
###############################################################

BASE=`basename ${image} .fits`
WSTRING=""

if [ -f /$1/${BASE}.weight.fits ]; then
    WSTRING="-WEIGHT_IMAGE /$1/${BASE}.weight.fits -WEIGHT_TYPE MAP_WEIGHT"
fi

# extract objects
${P_SEX} ${image} -c ${DATACONF}/singleastrom.conf.sex \
    -PARAMETERS_NAME ${DATACONF}/absphotcoadd.param.sex \
    -CATALOG_NAME ${TEMPDIR}/${BASE}.cat1_$$ \
    -DETECT_MINAREA 10 \
    -DETECT_THRESH 10 \
    -ANALYSIS_THRESH 10 \
    -FILTER Y ${WSTRING} ${FSTRING} \
    -PHOT_APERTURES $7 \
    -SATUR_LEVEL $5

# remove bad stuff
${P_LDACFILTER} -i ${TEMPDIR}/${BASE}.cat1_$$ \
    -o ${TEMPDIR}/${BASE}.cat2_$$ -t LDAC_OBJECTS -c "(FLAGS=0);"

# convert the tables
${P_LDACCONV} -i ${TEMPDIR}/${BASE}.cat2_$$ \
    -o ${TEMPDIR}/${BASE}.objcat_$$ -b 1 -c CAMERA -f DUMMY

###############################################################
#
# download SDSS or 2MASS catalogue
#
###############################################################

# get the ra dec of the image centre (not necessarily the CRPIX!)

xcen=`get_key ${image} NAXIS1 | ${P_GAWK} '{print 0.5*$1}'`
ycen=`get_key ${image} NAXIS2 | ${P_GAWK} '{print 0.5*$1}'`

ra=`${P_XY2SKY}  -d ${image} ${xcen} ${ycen} | ${P_GAWK} '{print $1}'`
dec=`${P_XY2SKY} -d ${image} ${xcen} ${ycen} | ${P_GAWK} '{print $2}'`

# getting the image size depends on which keywords are present
# in the FITS header; if nothing is found, use a radius of 30 arcmin
cd11=`get_key ${image} CD1_1`
cd12=`get_key ${image} CD1_2`
pixscale=""
if [ "${cd11}_A" != "A" ] && [ "${cd12}_A" != "A" ]; then
    pixscale=`echo ${cd11} cd{12} | ${P_GAWK} '{print sqrt($1*$1+$2*$2)}'`
else
    cdelt1=`get_key ${image} CDELT1`
    if [ "${cdelt1}_A" != "A" ]; then
	pixscale=`echo ${cdelt1} | ${P_GAWK} '{if ($1<0) print -1*$1; else print $1}'`
    fi
fi
if [ "${pixscale}_A" != "A" ]; then
    SIZE=`echo ${pixscale} ${xcen} ${ycen} | ${P_GAWK} '{print sqrt($2*$2+$3*$3)*$1*60.}'`
else
    SIZE="30."
fi

EXPTIME=`get_key ${image} EXPTIME`
# if the exptime keyword does not exist, then assume it is 1.0 seconds
# (fine for older reductions with theli)
if [ "${EXPTIME}"_A = "_A" ]; then
    EXPTIME=1.0
fi

VIZOPTION="-wget -mime=csv -site=${server} -out.max=1000000 -c.rm=${SIZE} -out=_RAJ -out=_DEJ"

# obtain data from 2MASS
if [ "$2" = "2MASS" ]; then
    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=2MASS-PSC \
	-out="${filter}mag" | ${P_GAWK} '($0 !~/#/)' | \
	${P_GAWK} '(NR>4)' | sed 's/;/ /g' | \
	${P_GAWK} '(NF>2) {printf ("%s %s %f\n", $1, $2, $3)}' \
    > ${TEMPDIR}/tmp1_$$
fi

# obtain data from SDSS
if [ "$2" = "SDSS" ]; then
    ramin=`echo ${ra} ${SIZE} ${dec} | \
	${P_GAWK} '{ramin=$1-$2/60./cos($3*3.14159/180.); if (ramin<0) {ramin=ramin+360.}} END {print ramin}'`
    ramax=`echo ${ra} ${SIZE} ${dec} | \
	${P_GAWK} '{ramax=$1+$2/60./cos($3*3.14159/180.); if (ramax>360) {ramax=ramax-360.}} END {print ramax}'`
    decmin=`echo ${dec} ${SIZE} | ${P_GAWK} '{decmin=$1-$2/60.; if (decmin<-90) {decmin=-90}} END {print decmin}'`
    decmax=`echo ${dec} ${SIZE} | ${P_GAWK} '{decmax=$1+$2/60.; if (decmax>90) {decmax=90}} END {print decmax}'`

    python ${DIR}/sqlcl.py -l -f csv -q "SELECT ra,dec,${filter} FROM PhotoObj WHERE ra BETWEEN ${ramin} AND ${ramax} AND dec BETWEEN ${decmin} AND ${decmax} AND ${filter} > 10 AND clean = 1" | sed 's/,/ /g' > ${TEMPDIR}/tmp1_$$

#    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=SDSS-DR7 \
#	-out="${filter}mag" | ${P_GAWK} '($0 !~/#/)' | \
#	${P_GAWK} '(NR>4)' | sed 's/;/ /g' | \
#	${P_GAWK} '(NF>2) {printf ("%s %s %f\n", $1, $2, $3)}' \
#   > ${TEMPDIR}/tmp1_$$
fi


# EXIT if no sources were retrieved

numobj=`wc ${TEMPDIR}/tmp1_$$ | ${P_GAWK} '{print $1}'`

ERR1="No photometric"
ERR2="reference sources retrieved!"

if [ "${numobj}" = "0" ]; then
    echo ${ERR1} ${ERR2}
    exit 1
fi

# make a LDAC catalogue

echo VERBOSE = DEBUG         >  ${TEMPDIR}/makePIC.conf_$$
echo COL_NAME  = SeqNr       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_TTYPE = LONG        >> ${TEMPDIR}/makePIC.conf_$$
echo COL_HTYPE = INT         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makePIC.conf_$$
echo COL_NAME  = Ra          >> ${TEMPDIR}/makePIC.conf_$$
echo COL_TTYPE = DOUBLE      >> ${TEMPDIR}/makePIC.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makePIC.conf_$$
echo COL_NAME  = Dec         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_TTYPE = DOUBLE      >> ${TEMPDIR}/makePIC.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makePIC.conf_$$
echo COL_NAME  = MAG         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_TTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makePIC.conf_$$
echo COL_NAME  = AWIN_WORLD  >> ${TEMPDIR}/makePIC.conf_$$
echo COL_TTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makePIC.conf_$$
echo COL_NAME  = BWIN_WORLD  >> ${TEMPDIR}/makePIC.conf_$$
echo COL_TTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makePIC.conf_$$
echo COL_NAME  = THETAWIN_J2000 >> ${TEMPDIR}/makePIC.conf_$$
echo COL_TTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makePIC.conf_$$
echo COL_NAME  = Flag        >> ${TEMPDIR}/makePIC.conf_$$
echo COL_TTYPE = SHORT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_HTYPE = INT         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makePIC.conf_$$

${P_GAWK} '{print NR, $0, "0.0003", "0.0003", "0.0", "0"}' \
    ${TEMPDIR}/tmp1_$$ > ${TEMPDIR}/tmp_PIC_$$

${P_ASCTOLDAC} -a ${TEMPDIR}/tmp_PIC_$$ \
    -o ${TEMPDIR}/tmp1_PIC_$$ \
    -t OBJECTS -c ${TEMPDIR}/makePIC.conf_$$

${P_LDACDELTAB} -i ${TEMPDIR}/tmp1_PIC_$$ -o ${TEMPDIR}/tmp2_PIC_$$ \
    -t FIELDS

# write an arbitrary but valid FIELDS table
${P_LDACADDTAB} -i ${TEMPDIR}/tmp2_PIC_$$ \
    -o ${TEMPDIR}/${BASE}.refcat_$$ \
    -p ${TEMPDIR}/${BASE}.objcat_$$ -t FIELDS

# clean up
rm ${TEMPDIR}/tmp*_PIC* ${TEMPDIR}/makePIC.conf_$$ ${TEMPDIR}/tmp1_$$
rm ${TEMPDIR}/${BASE}.cat[1,2]_$$

###############################################################
#
# merge the object and the reference catalogs
#
###############################################################

# calculate object magnitudes with a magzeropoint of 0 and
# an exposure time normalisation of 1s (1.08574 is 2.5 / log(10)):

if [ "$6" = "MAG_AUTO" ]; then
    ${P_LDACCALC} -i ${TEMPDIR}/${BASE}.objcat_$$ \
	-o ${TEMPDIR}/${BASE}.corrcat1_$$ \
	-t OBJECTS -c "(-1.08574*log(FLUX_AUTO/${EXPTIME}));" \
	-n MAG_corr "exposure time corr. MAG_AUTO" -k FLOAT
fi

if [ "$6" = "MAG_APER" ]; then
    ${P_LDACCALC} -i ${TEMPDIR}/${BASE}.objcat_$$ \
	-o ${TEMPDIR}/${BASE}.corrcat1_$$ \
	-t OBJECTS -c "(-1.08574*log(FLUX_APER/${EXPTIME}));" \
	-n MAG_corr "exposure time corr. MAG_APER" -k FLOAT
fi

${P_LDACRENKEY} -i ${TEMPDIR}/${BASE}.corrcat1_$$ \
    -o ${TEMPDIR}/${BASE}.corrcat_$$ -t OBJECTS \
    -k ALPHA_J2000 Ra DELTA_J2000 Dec
${P_ASSOCIATE} -i ${TEMPDIR}/${BASE}.corrcat_$$ ${TEMPDIR}/${BASE}.refcat_$$ \
    -o ${TEMPDIR}/obj_assoc.cat_$$ ${TEMPDIR}/ref_assoc.cat_$$ \
    -t OBJECTS -c ${DATACONF}/PIC_associate.conf
${P_MAKESSC} -i ${TEMPDIR}/obj_assoc.cat_$$ ${TEMPDIR}/ref_assoc.cat_$$ \
    -o ${TEMPDIR}/PIC_merged.cat_$$ \
    -t OBJECTS -c ${DATACONF}/absphotcoadd_make_ssc.conf
${P_LDACFILTER} -i ${TEMPDIR}/PIC_merged.cat_$$ -t PSSC \
    -o ${TEMPDIR}/${BASE}_merged.cat_$$ \
    -c "(((Mag_REF>0)AND(MagErr_OBJ<$4))AND(MagErr_OBJ>0));"

nobj=`${P_LDACTOASC} -b -i ${TEMPDIR}/${BASE}_merged.cat_$$ -t PSSC -k Mag_REF | \
    wc | ${P_GAWK} '{print $1}'`

###############################################################
#
# get the ZP
#
###############################################################

ZPFLUX=`${P_FITSPIC} -m \
    -i ${TEMPDIR}/${BASE}_merged.cat_$$ \
    -a ${image} -o dummy`

ABSZP=`echo ${ZPFLUX} | ${P_GAWK} '{printf("%.3f", $1)}'`
ABSZPERR=`echo ${ZPFLUX} | ${P_GAWK} '{printf("%.3f", $2)}'`
ABSZPRMS=`echo ${ZPFLUX} | ${P_GAWK} '{printf("%.3f", $3)}'`
# write the ZP
value ${ABSZP}
writekey ${image} ZP_D "${VALUE} / Mag ZP from direct comp. with ZP_D_SYS" REPLACE
value ${ABSZPERR}
writekey ${image} ZP_D_ERR "${VALUE} / Mag ZP ERR from direct comp. with ZP_D_SYS" REPLACE
value ${ABSZPRMS}
writekey ${image} ZP_D_RMS "${VALUE} / RMS of (MAGref - MAGimg; sources used for ZP_D)" REPLACE
value ${nobj}
writekey ${image} ZP_D_NUM "${VALUE} / Number of sources used to determine ZP_D" REPLACE
value $2
writekey ${image} ZP_D_SYS "'${VALUE}'" REPLACE

# rm ${TEMPDIR}/${BASE}*.cat*_$$
