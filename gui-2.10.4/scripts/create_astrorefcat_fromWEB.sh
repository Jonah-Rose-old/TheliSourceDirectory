BASHPATH -xv

# this script retrieves and reformats a 
# standard star catalogue from the CDS server

# $1: main dir
# $2: science dir
# $3: image extension (OFC etc.)
# $4: the catalogue queried
# $5: server
# $6: (standard) (optional)

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

maindir=$1/$2/
imext=$3
catid=$4
server=$5
#server=${V_AP_SERVER}
ra=${V_AP_REFRA}
dec=${V_AP_REFDEC}
maglim=${V_AP_MAGLIM}
standard=$6

###if [ "${standard}"_A = "_A" ]; then
    stdcat_anetindex=theli_mystd_anet.index
    stdcat_anetname=theli_mystd_anet.cat
    stdcat_scampname=theli_mystd_scamp.cat
    stdcat_skycatname=theli_mystd.skycat
    stdcat_ds9name=theli_mystd.reg
#else
#    stdcat_anetindex=theli_mystd_anet_std.index
#    stdcat_anetname=theli_mystd_anet_std.cat
#    stdcat_scampname=theli_mystd_scamp_std.cat
#    stdcat_skycatname=theli_mystd_std.skycat
#    stdcat_ds9name=theli_mystd_std.reg
####fi

echo $catid > ${maindir}/.astrefcat

dir=`pwd`

# if the coordinates are determined from the images
if [ "$1" != "_" ] && [ "$2" != "_" ]; then 
    if [ "${ra}" = "header" ] || [ "${dec}" = "header" ]; then
	# A script that determines the pointing center and the optimal search radius
	${dir}/get_refcat_geometry.sh $1 $2 $3
	# Fetch the results
	ra=`${P_GAWK} '{print $1}' ${TEMPDIR}/auto_ra_dec.dat`
	dec=`${P_GAWK} '{print $2}' ${TEMPDIR}/auto_ra_dec.dat`
	radius=`${P_GAWK} '{print $3}' ${TEMPDIR}/auto_ra_dec.dat`
    fi
fi

# the CDS client wants an explicit "+" if dec is positive
dec=`echo ${dec} | ${P_GAWK} 'BEGIN {FS=":"} {if ($1>0 && $1!~/+/) print "+"$0; else print $0}'`

# the radius within which we retrieve the catalog
if [ "${V_AP_RADIUS}_A" = "_A" ]; then
    size=${radius}
else
    size=${V_AP_RADIUS}
fi

VIZOPTION="-wget -mime=csv -site=${server} -out.max=100000000 -c.rm=${size} -out=_RAJ -out=_DEJ"

#obtain data from the allsky catalog
if [ "${catid}" = "ALLSKY" ]; then
   ${P_GAWK} '($3<'${maglim}')' ../astrstdcats/gsc1_allsky.cat > ${TEMPDIR}/tmp1_$$
fi

# obtain data from the PPMXL
if [ "${catid}" = "PPMXL" ]; then
    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=I/317 \
	-out=b2mag -out=r2mag -out=imag | \
	${P_GAWK} '($0 !~/#/)' | ${P_GAWK} '(NR>4&&NF>0)' | \
	${P_GAWK} 'BEGIN{FS=";"} {
             av=0
             sum=0
             if ($3 > 0.) {av+=$3; sum++}
             if ($4 > 0.) {av+=$4; sum++}
             if ($5 > 0.) {av+=$5; sum++}
             if (sum !=0) {printf ("%s %s %f\n", $1, $2, av/sum)}
             }' > ${TEMPDIR}/tmp1_$$
fi

#obtain data from TYC
if [ "${catid}" = "TYC" ]; then
    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=TYC/tyc_main \
	Vmag=0..${maglim} \
	-out=Vmag | \
	${P_GAWK} '($0 !~/#/)' | ${P_GAWK} '(NR>4&&NF>0)' | sed 's/;/ /g' \
	> ${TEMPDIR}/tmp1_$$
fi

# obtain data from the GSC2
if [ "${catid}" = "GSC-2.3" ]; then
    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=GSC2.3 \
	Fmag=0..${maglim} \
	-out="Fmag" | \
	${P_GAWK} '($0 !~/#/)' | ${P_GAWK} '(NR>4&&NF>0)' | sed 's/;/ /g' \
        > ${TEMPDIR}/tmp1_$$
fi

# obtain data from UCAC4
if [ "${catid}" = "UCAC4" ]; then
    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=UCAC4 \
	-out="f.mag" f.mag=0..${maglim} | \
	${P_GAWK} '($0 !~/#/)' | ${P_GAWK} '(NR>4&&NF>0)' | \
	${P_GAWK} 'BEGIN{FS=";"} {
             printf ("%s %s %f\n", $1, $2, $3)
             }' > ${TEMPDIR}/tmp1_$$
fi


# obtain data from URAT1
if [ "${catid}" = "URAT1" ]; then
    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=URAT1 \
	-out="f.mag" f.mag=0..${maglim} | \
	${P_GAWK} '($0 !~/#/)' | ${P_GAWK} '(NR>4&&NF>0)' | \
	${P_GAWK} 'BEGIN{FS=";"} {
             printf ("%s %s %f\n", $1, $2, $3)
             }' > ${TEMPDIR}/tmp1_$$
fi


# obtain data from SPM4
if [ "${catid}" = "SPM4" ]; then
    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=SPM4 \
	-out="Vmag" Vmag=0..${maglim} | \
	${P_GAWK} '($0 !~/#/)' | ${P_GAWK} '(NR>4&&NF>0)' | \
	${P_GAWK} 'BEGIN{FS=";"} {
             printf ("%s %s %f\n", $1, $2, $3)
             }' > ${TEMPDIR}/tmp1_$$
fi


# obtain data from IGSL
if [ "${catid}" = "IGSL" ]; then
    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=IGSL \
	-out="magG" magG=0..${maglim} | \
	${P_GAWK} '($0 !~/#/)' | ${P_GAWK} '(NR>4&&NF>0)' | \
	${P_GAWK} 'BEGIN{FS=";"} {
             printf ("%s %s %f\n", $1, $2, $3)
             }' > ${TEMPDIR}/tmp1_$$
fi


# obtain data from GAIA-DR1
if [ "${catid}" = "GAIA-DR1" ]; then
    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=I/337/gaia \
	-out="<Gmag>" | \
	${P_GAWK} '($0 !~/#/)' | ${P_GAWK} '(NR>4&&NF>0)' | \
	${P_GAWK} 'BEGIN{FS=";"} {
             if ($3<='${maglim}') printf ("%s %s %f\n", $1, $2, $3)
             }' > ${TEMPDIR}/tmp1_$$
fi

# obtain data from the USNO-B1
if [ "${catid}" = "USNO-B1" ]; then
    ${P_VIZQUERY} ${VIZOPTION} -c="${ra} ${dec}" -source=USNOB1 \
	-out="B1mag B2mag R1mag R2mag Imag" | \
	${P_GAWK} '($0 !~/#/)' | ${P_GAWK} '(NR>4&&NF>0)' | \
	${P_GAWK} 'BEGIN{FS=";"} {
             av=0
             sum=0
             if ($3 > 0.) {av+=$3; sum++}
             if ($4 > 0.) {av+=$4; sum++}
             if ($5 > 0.) {av+=$5; sum++}
             if ($6 > 0.) {av+=$6; sum++}
             if ($7 > 0.) {av+=$7; sum++}
             if (sum !=0) {printf ("%s %s %f\n", $1, $2, av/sum)}
             }' > ${TEMPDIR}/tmp1_$$
fi

# obtain data from the 2MASS
if [ "${catid}" = "2MASS" ]; then
    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=2MASS-PSC \
	-out="Jmag Hmag Kmag" | \
	${P_GAWK} '($0 !~/#/)' | ${P_GAWK} '(NR>4&&NF>0)' | \
	${P_GAWK} 'BEGIN{FS=";"} {
            av=0
            sum=0
            if ($3 > 0.) {av+=$3; sum++}
            if ($4 > 0.) {av+=$4; sum++}
            if ($5 > 0.) {av+=$5; sum++}
            if (sum !=0) {printf ("%s %s %f\n", $1, $2, av/sum)}
            }' > ${TEMPDIR}/tmp1_$$
fi

# obtain data from the SDSS-DR7
#if [ "${catid}" = "SDSS-DR7" ]; then
#    ${P_VIZQUERY} ${VIZOPTION} -c="$ra $dec" -source=SDSS-DR7 \
#	-out="gmag rmag imag" | \
#	${P_GAWK} '($0 !~/#/)' | ${P_GAWK} '(NR>4&&NF>0)' | \
#	${P_GAWK} 'BEGIN{FS=";"} {
#          av=0
#         sum=0
#         if ($3 > 0.) {av+=$3; sum++}
#        if ($4 > 0.) {av+=$4; sum++}
#       if ($5 > 0.) {av+=$5; sum++}
#      if (sum !=0) {printf ("%s %s %f\n", $1, $2, av/sum)}
#     }' > ${TEMPDIR}/tmp1_$$
#fi

# obtain data from the SDSS-DR9
if [ "${catid}" = "SDSS-DR9" ]; then
    # check the coordinate format; the python script below needs decimal values
    ra_format=`echo ${ra} | ${P_GAWK} '{if ($1 ~ ":") print "hhmmss"; else print "decimal"}'`
    dec_format=`echo ${dec} | ${P_GAWK} '{if ($1 ~ ":") print "ddmmss"; else print "decimal"}'`
    if [ "${ra_format}" != "decimal" ]; then
	ra=`${P_HMSTODECIMAL} ${ra}` 
    fi
    if [ "${dec_format}" != "decimal" ]; then
	dec=`${P_DMSTODECIMAL} ${dec}` 
    fi

    ramin=`echo ${ra} ${size} ${dec} | \
	${P_GAWK} '{ramin=$1-$2/60./cos($3*3.14159/180.); if (ramin<0) {ramin=ramin+360.}} END {print ramin}'`
    ramax=`echo ${ra} ${size} ${dec} | \
	${P_GAWK} '{ramax=$1+$2/60./cos($3*3.14159/180.); if (ramax>360) {ramax=ramax-360.}} END {print ramax}'`
    decmin=`echo ${dec} ${size} | ${P_GAWK} '{decmin=$1-$2/60.; if (decmin<-90) {decmin=-90}} END {print decmin}'`
    decmax=`echo ${dec} ${size} | ${P_GAWK} '{decmax=$1+$2/60.; if (decmax>90) {decmax=90}} END {print decmax}'`

    python ${dir}/sqlcl.py -l -f csv -q "SELECT ra,dec,r FROM PhotoObj WHERE ra BETWEEN ${ramin} AND ${ramax} AND dec BETWEEN ${decmin} AND ${decmax} AND r < ${maglim} AND r > 10 AND clean = 1" | sed 's/,/ /g' > ${TEMPDIR}/tmp1_$$
fi

# filter the magnitudes

if [ "${maglim}_A" != "_A" ]; then
    ${P_GAWK} '{if ($3<'${maglim}') printf ("%s %s %f\n", $1, $2, $3)}' \
	${TEMPDIR}/tmp1_$$ > ${TEMPDIR}/tmp1filt_$$
    mv ${TEMPDIR}/tmp1filt_$$ ${TEMPDIR}/tmp1_$$
fi

# EXIT if no sources left

numstars=`wc ${TEMPDIR}/tmp1_$$ | ${P_GAWK} '{print $1}'`

if [ "${numstars}" = "0" ]; then
    test -f ~/.theli/scripts/${stdcat_skycatname} && rm ~/.theli/scripts/${stdcat_skycatname}
    touch ~/.theli/scripts/${stdcat_skycatname}
    exit 1
fi


# make a scamp catalogue

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

${P_GAWK} '{print $0, "0.1", "0.00003", "0.00003", "0.1"}' \
    ${TEMPDIR}/tmp1_$$ > ${TEMPDIR}/tmp_scamp_$$

${P_ASCTOLDAC} -a ${TEMPDIR}/tmp_scamp_$$ \
               -o ${TEMPDIR}/tmp2_scamp0_$$ \
               -t OBJECTS -c ${TEMPDIR}/makescamp.conf_$$

${P_LDACRENTAB} -i ${TEMPDIR}/tmp2_scamp0_$$ \
    -o ${TEMPDIR}/tmp2_scamp1_$$ -t OBJECTS LDAC_OBJECTS 
${P_LDACRENTAB} -i ${TEMPDIR}/tmp2_scamp1_$$ \
    -o ~/.theli/scripts/${stdcat_scampname} -t FIELDS LDAC_IMHEAD

#******************************************
# make an astrometry.net index
#******************************************

${P_LDACTOASC} -i ~/.theli/scripts/${stdcat_scampname} \
    -t LDAC_OBJECTS -k X_WORLD Y_WORLD MAG -b > ${TEMPDIR}/anet.cat
${P_ASCTOLDAC} -a ${TEMPDIR}/anet.cat -o ${TEMPDIR}/${stdcat_anetname} \
    -t OBJECTS -c ${DATACONF}/anet.asctoldac_ref.conf

# the diameter of one chip in arcminutes
radius=`echo ${SIZEX[1]} ${SIZEY[1]} ${PIXSCALE} | ${P_GAWK} '{print sqrt($1^2+$2^2)*$3/60.}'`

scale=`echo ${radius} | \
    ${P_GAWK} '{x = 2. / log(2.) * log($1/6)
	        if (x-int(x) < 0.5) y = int(x)
		else y = int(x)+1
		if (y<0) y=0
                if (y>19) y=19
		print y}'`

${P_BUILDINDEX} -i ${TEMPDIR}/${stdcat_anetname} \
    -o ~/.theli/scripts/${stdcat_anetindex} -P ${scale} -E -M -S MAG

# make a ds9 catalogue
echo global color=red select=0 highlite=1 wcs=wcs > \
    ~/.theli/scripts/${stdcat_ds9name}
echo "fk5" >> ~/.theli/scripts/${stdcat_ds9name}

${P_GAWK} '{print "circle("$1","$2",5\")"}' ${TEMPDIR}/tmp1_$$ >> \
    ~/.theli/scripts/${stdcat_ds9name}


# make a skycat catalogue
${P_GAWK} '{print NR, $1, $2, $3}' ${TEMPDIR}/tmp1_$$ > ${TEMPDIR}/skc_tmp2_$$

# make a skycat config file
echo VERBOSE = DEBUG         >  ${TEMPDIR}/makeskycat.conf_$$
echo COL_NAME  = SeqNr       >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_TTYPE = LONG        >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_HTYPE = INT         >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_NAME  = ALPHA_J2000 >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_TTYPE = DOUBLE      >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_UNIT = "deg"        >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_NAME  = DELTA_J2000 >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_TTYPE = DOUBLE      >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_UNIT = "deg"        >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_NAME  = MAG_AUTO    >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_TTYPE = FLOAT       >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_HTYPE = FLOAT       >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_COMM = '""'         >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_UNIT = '""'         >> ${TEMPDIR}/makeskycat.conf_$$
echo COL_DEPTH = 1           >> ${TEMPDIR}/makeskycat.conf_$$

${P_ASCTOLDAC} -a ${TEMPDIR}/skc_tmp2_$$ \
    -o ${TEMPDIR}/skc_tmp3_$$ \
    -t OBJECTS -c ${TEMPDIR}/makeskycat.conf_$$

${P_LDACTOSKYCAT} -i ${TEMPDIR}/skc_tmp3_$$ -t OBJECTS \
    -k SeqNr ALPHA_J2000 DELTA_J2000 MAG_AUTO \
    -l id_col SeqNr ra_col ALPHA_J2000 dec_col \
    DELTA_J2000 mag_col MAG_AUTO \
    -s > ~/.theli/scripts/${stdcat_skycatname}

mkdir -p /$1/$2/cat/ds9cat
mkdir -p /$1/$2/cat/skycat

cp ~/.theli/scripts/theli_mystd.skycat /$1/$2/cat/skycat
cp ~/.theli/scripts/theli_mystd.reg /$1/$2/cat/ds9cat

rm ${TEMPDIR}/skc_tmp[2,3]_$$ ${TEMPDIR}/tmp_scamp_$$ ${TEMPDIR}/tmp2_scamp0_$$
rm ${TEMPDIR}/tmp2_scamp1_$$
