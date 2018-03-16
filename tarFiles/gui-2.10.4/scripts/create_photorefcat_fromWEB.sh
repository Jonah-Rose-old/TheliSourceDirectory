BASHPATH -uxv

# compares object fluxes in an image with those of a
# photometric reference catalogue, and calculates
# a correction image.

# $1: main directory
# $2: science dir. (the catalogues are assumed to be in $1/$2/cat)
# $3: image extension
# $4: server

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

# give meaningful names to command line arguments:
MD=$1
SD=$2
EXTENSION=$3
SERVER=$4
#SERVER=${V_AP_SERVER}
ra=${V_AP_REFRA}
dec=${V_AP_REFDEC}
SIZE=${V_AP_RADIUS}

DIR=`pwd`

###############################################################
#
# download SDSS or 2MASS catalogue
#
###############################################################

# get the field coordinates

# if the coordinates are determined from the images

if [ "${ra}" = "header" ] || [ "${dec}" = "header" ]; then
    cd ${MD}/${SD}/
    ls *${EXTENSION}.fits > ${TEMPDIR}/imcoord.list_$$
    if [ -f ${TEMPDIR}/imcoord.coords_$$ ]; then
	rm ${TEMPDIR}/imcoord.coords_$$
    fi
    
    # obtain the sky coordinates for all images in the set.
    # add 1000 to right ascension in order to avoid problems with
    # values around 0 (360) degrees.
    
    exec < ${TEMPDIR}/imcoord.list_$$
    while read file
    do
	crval1=`${P_DFITS} ${file} | ${P_FITSORT} CRVAL1 |\
                ${P_GAWK} '(NR!=1) {printf "%4.6f\n", $2 + 1000}'`
	crval2=`${P_DFITS} ${file} | ${P_FITSORT} CRVAL2 |\
                ${P_GAWK} '(NR!=1) {printf "%4.6f\n", $2}'`
	echo ${crval1} ${crval2} >> ${TEMPDIR}/imcoord.coords_$$
    done
    
   # calculate the mean position and the extent in alpha and delta;
   # subtract 1000 from the right ascension
    
    ra=`${P_GAWK} 'BEGIN {mean_alpha = 0; i = 0} {
       mean_alpha = mean_alpha + $1
       i = i + 1 } 
   END { printf "%4.6f\n", mean_alpha / i - 1000}' ${TEMPDIR}/imcoord.coords_$$`
    delta=`${P_GAWK} 'BEGIN {mean_delta = 0; i = 0} {
       mean_delta = mean_delta + $2
       i = i + 1 } 
   END { printf "%4.6f\n", mean_delta / i }' ${TEMPDIR}/imcoord.coords_$$`
    dec=`echo $delta | ${P_GAWK} '{if ($1>0) print "+"$1; else print $1}'`
    cd $DIR
fi


VIZOPTION="-mime=csv -site=${SERVER} -out.max=10000000 -c.rm=${SIZE} -out=_RAJ -out=_DEJ"

# obtain data from 2MASS
if [ "${V_ABSPHOT_STDCAT_DIRECT}" = "2MASS" ]; then
    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=2MASS-PSC \
	-out="${V_ABSPHOT_STDFILTERM2}mag" | ${P_GAWK} '($0 !~/#/)' | \
	${P_GAWK} '(NR>4)' | sed 's/;/ /g' | \
	${P_GAWK} '(NF>2) {printf ("%s %s %f\n", $1, $2, $3)}' \
    > ${TEMPDIR}/tmp1_$$
fi

# obtain data from SDSS
if [ "${V_ABSPHOT_STDCAT_DIRECT}" = "SDSS" ]; then
#    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=SDSS-DR7 \
#	-out="${V_ABSPHOT_STDFILTERM2}mag" | ${P_GAWK} '($0 !~/#/)' | \
#	${P_GAWK} '(NR>4)' | sed 's/;/ /g' | \
#	${P_GAWK} '(NF>2) {printf ("%s %s %f\n", $1, $2, $3)}' \
#    > ${TEMPDIR}/tmp1_$$

    ramin=`echo ${ra} ${SIZE} ${dec} | \
	${P_GAWK} '{ramin=$1-$2/60./cos($3*3.14159/180.); if (ramin<0) {ramin=ramin+360.}} END {print ramin}'`
    ramax=`echo ${ra} ${SIZE} ${dec} | \
	${P_GAWK} '{ramax=$1+$2/60./cos($3*3.14159/180.); if (ramax>360) {ramax=ramax-360.}} END {print ramax}'`
    decmin=`echo ${dec} ${SIZE} | ${P_GAWK} '{decmin=$1-$2/60.; if (decmin<-90) {decmin=-90}} END {print decmin}'`
    decmax=`echo ${dec} ${SIZE} | ${P_GAWK} '{decmax=$1+$2/60.; if (decmax>90) {decmax=90}} END {print decmax}'`

    python ${DIR}/sqlcl.py -l -f csv -q "SELECT ra,dec,${V_ABSPHOT_STDFILTERM2} FROM PhotoObj WHERE ra BETWEEN ${ramin} AND ${ramax} AND dec BETWEEN ${decmin} AND ${decmax} AND ${V_ABSPHOT_STDFILTERM2} > 0 AND clean = 1" | sed 's/,/ /g' > ${TEMPDIR}/tmp1_$$

fi

# EXIT if no sources were retrieved

numobj=`wc ${TEMPDIR}/tmp1_$$ | ${P_GAWK} '{print $1}'`

if [ "${numobj}" = "0" ]; then
    echo "No photometric reference sources retrieved!"
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
echo COL_NAME  = A_WORLD     >> ${TEMPDIR}/makePIC.conf_$$
echo COL_TTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makePIC.conf_$$
echo COL_NAME  = B_WORLD     >> ${TEMPDIR}/makePIC.conf_$$
echo COL_TTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makePIC.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makePIC.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makePIC.conf_$$
echo COL_NAME  = THETA_J2000 >> ${TEMPDIR}/makePIC.conf_$$
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

ls -1 /${MD}/${SD}/cat/*_1${EXTENSION}.cat > ${TEMPDIR}/fieldstab.list_$$
fieldscat=`${P_GAWK} '(NR==1)' ${TEMPDIR}/fieldstab.list_$$`

${P_LDACADDTAB} -i ${TEMPDIR}/tmp2_PIC_$$ \
    -o ~/.theli/scripts/theli_PIC.cat \
    -p ${fieldscat} -t FIELDS

# clean up
rm  ${TEMPDIR}/tmp*_PIC* ${TEMPDIR}/makePIC.conf_$$ ${TEMPDIR}/tmp1_$$
