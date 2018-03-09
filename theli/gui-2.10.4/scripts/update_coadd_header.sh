#!/bin/bash -xv

#$1: main dir
#$2: science dir
#$3: image extension

SCRIPTDIR=`pwd`

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash_functions.include

. bash.include

# exit if the chips.cat5 catalog does not exist (for whatever reasons)
if [ ! -f /$1/$2/cat/chips.cat5 ]; then
    exit
fi


# the list with images that went into the coaddition
DIR=`pwd`

EXT=$3

cd /$1/$2/coadd_${V_COADD_IDENT}/

# the list of stacked images
${P_FIND} . -maxdepth 1 -name \*.resamp.fits > ${TEMPDIR}/coaddupdatelist_$$

# strip the chips, we just want the exposure names
# the first GAWK is needed as V_COADD_IDENT could contain a "_"

# BUGGY! awk apparently cannot use a string as FS if it contains the string 'x+y'
# where x and y are arbitrary characters. 'x+' and '+y' work, though.
# replaced it with 'sed'

#${P_GAWK} 'BEGIN{FS="'${V_COADD_IDENT}'.resamp.fits"} {print $1}' \
#    ${TEMPDIR}/coaddupdatelist_$$ | \

sed 's/'${V_COADD_IDENT}'.resamp.fits//g' ${TEMPDIR}/coaddupdatelist_$$ | \
    ${P_GAWK} '{n=split($1,s,"_"); 
           for (i=1;i<n-1;i++) {printf("%s_",s[i])}; 
           printf("%s\n"),s[n-1]}' | \
	       ${P_SORT} | uniq > ${TEMPDIR}/coaddupdatelist1_$$ 

# the list of catalogs

cd /$1/$2/cat/

# the list of stacked images
${P_FIND} . -maxdepth 1 -name \*${EXT}.cat > ${TEMPDIR}/allcats_$$

# strip the chips, we just want the exposure names
${P_GAWK} '{n=split($1,s,"_"); 
           for (i=1;i<n-1;i++) {printf("%s_",s[i])}; 
           printf("%s\n"),s[n-1]}' \
    ${TEMPDIR}/allcats_$$ | ${P_SORT} | uniq > \
    ${TEMPDIR}/allcats1_$$ 

test -f ${TEMPDIR}/protocat_$$ && rm ${TEMPDIR}/protocat_$$

exec < ${TEMPDIR}/allcats1_$$
while read file
do
  isstacked=`${P_GAWK} 'BEGIN{isstacked=0} {
             if ($1=="'${file}'") isstacked=1} 
             END {print isstacked}' ${TEMPDIR}/coaddupdatelist1_$$`
  echo `basename ${file}` ${isstacked} >> ${TEMPDIR}/protocat_$$
done

# match the protocat entries with those in chips.cat5
# (i am not assuming that files are in the same order)

${P_LDACTOASC} -i /$1/$2/cat/chips.cat5 -t STATS -s -k IMAGENAME -b > \
    ${TEMPDIR}/chips5protocat_$$

${P_GAWK} 'BEGIN {i=1
                  while (getline var[i] <"'${TEMPDIR}/protocat_$$'" > 0) {
                         split(var[i],a," ")
                         name[i]=a[1]
                         isstacked[i]=a[2]
                         i++
                  }
                  n=i} 
           {
           for (i=0; i<=n; i++) {
               if ($1==name[i]) {
                  print isstacked[i]
               }
           }}' ${TEMPDIR}/chips5protocat_$$ > ${TEMPDIR}/protocat1_$$

# convert the protocat into LDAC format and add it to chips.cat5

{
echo COL_NAME  = P_"${V_COADD_IDENT}"
echo COL_TTYPE = SHORT
echo COL_HTYPE = INT
echo COL_COMM  = "If the image is in the stack"
echo COL_UNIT  = 1
} > ${TEMPDIR}/proto_asctoldac.conf_$$

${P_ASCTOLDAC} -a ${TEMPDIR}/protocat1_$$ \
    -c ${TEMPDIR}/proto_asctoldac.conf_$$ \
    -o ${TEMPDIR}/protocat1.ldac_$$ \
    -t STATS -b 1 -n "ISSTACKED"

${P_LDACJOINKEY} -i /$1/$2/cat/chips.cat5 \
    -p ${TEMPDIR}/protocat1.ldac_$$ \
    -k P_${V_COADD_IDENT} -t STATS \
    -o /$1/$2/cat/${V_COADD_IDENT}.cat


#########################################################
#
# finished with preparations
#
#########################################################


# set default values if the last command line arguments are not provided:
if [ "${V_COADD_SEEING}" != "" ] && [ "${V_COADD_RZP}" = "" ]; then
    COADDCONDITION="(SEEING<${V_COADD_SEEING});"
fi
if [ "${V_COADD_SEEING}" = "" ] && [ "${V_COADD_RZP}" != "" ]; then
    COADDCONDITION="(RZP>${V_COADD_RZP});"
fi
if [ "${V_COADD_SEEING}" != "" ] && [ "${V_COADD_RZP}" != "" ]; then
    COADDCONDITION="((SEEING<${V_COADD_SEEING})AND(RZP>${V_COADD_RZP}));"
fi
if [ "${COADDCONDITION}"_A = "_A" ]; then
    COADDCONDITION="NONE"
fi

# get EXPTIME and hence effective gain from the coaddition catalog.
EXPTIME=0.0
GAINEFF=0.0
NCOMBINE=-1
FILTER="'NOT SPECIFIED'"
OBJECT="'NOT SPECIFIED'"
DATEST="'NOT SPECIFIED'"
DATEEND="'NOT SPECIFIED'"
GABODSST=-1
GABODSEN=-1
if [ -f /$1/$2/cat/${V_COADD_IDENT}.cat ]; then
  ${P_LDACFILTER} -i /$1/$2/cat/${V_COADD_IDENT}.cat -t STATS \
      -o ${TEMPDIR}/tmp_$$.cat -c "(P_${V_COADD_IDENT}=1);"

  FILTER="'`${P_LDACTOASC} -s -b -i ${TEMPDIR}/tmp_$$.cat \
          -t STATS -k FILTER | ${P_GAWK} '(NR==1)'`'"

  if [ ! "${FILTER}" = "${V_COADD_FILTER}" ]; then
      FILTER="${V_COADD_FILTER}"
  fi

  OBJECT="'`${P_LDACTOASC} -s -b -i ${TEMPDIR}/tmp_$$.cat \
          -t STATS -k OBJECT | ${P_SORT} | uniq -c | ${P_SORT} -nr | \
          ${P_GAWK} '(NR==1) {print $2}'`'"

  NCOMBINE=`${P_LDACTOASC} -b -i ${TEMPDIR}/tmp_$$.cat -t STATS -k EXPTIME\
            | wc -l`

  EXPTIME=`${P_LDACTOASC} -b -i ${TEMPDIR}/tmp_$$.cat -t STATS -k EXPTIME |\
           ${P_GAWK} 'BEGIN {time = 0} {
                        time = time+$1
                      } END { printf("%.2f", time) }'`

  GAINEFF=`${P_GAWK} 'BEGIN {printf("%.2f", '${GAIN}'*'${EXPTIME}')}'`

  GABODSST=`${P_LDACTOASC} -b -i ${TEMPDIR}/tmp_$$.cat -t STATS -k GABODSID |\
            ${P_SORT} -n | ${P_GAWK} '(NR == 1) { print $1 }'`

  GABODSEN=`${P_LDACTOASC} -b -i ${TEMPDIR}/tmp_$$.cat -t STATS -k GABODSID |\
            ${P_SORT} -nr | ${P_GAWK} '(NR == 1) { print $1 }'`

  if [ "${GABODSST}_A" != "_A" ]; then
    DATEST=`${P_CALDATE} -d 31/12/1998 -i ${GABODSST} | ${P_GAWK} '{print $3}'`
  fi

  if [ "${GABODSEN}_A" != "_A" ]; then
    DATEEND=`${P_CALDATE} -d 31/12/1998 -i ${GABODSEN} | ${P_GAWK} '{print $3}'`
  fi

  rm ${TEMPDIR}/tmp_$$.cat
fi

#
# determine the seeing in the coadded image:
#
echo "NUMBER"      >  ${TEMPDIR}/seeing_sexparam.asc_$$
echo "FWHM_IMAGE"  >> ${TEMPDIR}/seeing_sexparam.asc_$$
echo "FLUX_RADIUS" >> ${TEMPDIR}/seeing_sexparam.asc_$$
echo "MAG_AUTO"    >> ${TEMPDIR}/seeing_sexparam.asc_$$
echo "MAGERR_AUTO" >> ${TEMPDIR}/seeing_sexparam.asc_$$
echo "FLAGS"       >> ${TEMPDIR}/seeing_sexparam.asc_$$

${P_SEX} /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits -c ${CONF}/postcoadd.conf.sex \
    -CATALOG_NAME ${TEMPDIR}/seeing_$$.cat \
    -CATALOG_TYPE FITS_LDAC \
    -FILTER_NAME ${DATACONF}/default.conv \
    -WEIGHT_IMAGE /$1/$2/coadd_${V_COADD_IDENT}/coadd.weight.fits \
    -WEIGHT_TYPE MAP_WEIGHT \
    -DETECT_THRESH 10 -DETECT_MINAREA 5 \
    -ANALYSIS_THRESH 10 \
    -MAG_ZEROPOINT 20 \
    -PARAMETERS_NAME ${TEMPDIR}/seeing_sexparam.asc_$$ \
    -NTHREADS ${NPARA}

${P_LDACCONV}  -i ${TEMPDIR}/seeing_$$.cat -o ${TEMPDIR}/seeing_ldac.cat_$$ \
               -b 1 -c "sex" -f R

ngood=`${P_LDACTOASC} -i ${TEMPDIR}/seeing_ldac.cat_$$ -t OBJECTS -k Flag -b | \
      ${P_GAWK} '($1==0)' | wc | ${P_GAWK} '{print $1}'`

if [ ${ngood} -gt 0 ]; then
    ${P_LDACFILTER} -i ${TEMPDIR}/seeing_ldac.cat_$$ \
        -o ${TEMPDIR}/seeing_ldac_filt.cat_$$ -c "(Flag=0);"

    ${P_LDACCALC} -i ${TEMPDIR}/seeing_ldac_filt.cat_$$ \
	-o ${TEMPDIR}/seeing_ldac_filt.cat2_$$ \
	-c "(1./MAGERR_AUTO);" -n "snratio" "" -k FLOAT 
    
    CD11=`get_key /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits CD1_1`
    CD21=`get_key /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits CD2_1`
    PSCALE=`${P_GETPIXSCALE} -c ${CD11} ${CD21}`

    # get FWHM_IMAGE
    ${P_PREANISOTROPY} -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ \
	-t OBJECTS -s 0.1 -c snratio 10.0 1000.0 -k FWHM_IMAGE MAG_AUTO >& \
	${TEMPDIR}/preaniso_$$
    # expanding limits to avoid rounding issues in ldacfilter occuring after the 4th digit
    SMIN=`${P_GAWK} '($0 ~ /propose/) {print $8-0.01}'  ${TEMPDIR}/preaniso_$$`
    SMAX=`${P_GAWK} '($0 ~ /propose/) {print $12+0.01}' ${TEMPDIR}/preaniso_$$`
    MMIN=`${P_GAWK} '($0 ~ /propose/) {print $14-0.01}' ${TEMPDIR}/preaniso_$$`
    MMAX=`${P_GAWK} '($0 ~ /propose/) {print $18+0.01}' ${TEMPDIR}/preaniso_$$`
    nobj=0
    TEST="FALSE"
    TEST=`echo ${SMIN} ${SMAX} ${MMIN} ${MMAX} | \
	${P_GAWK} '{if ($1<$2 && $3<$4) print "TRUE"}'`

    if [ "${SMIN}"_A != ""_A ] && [ "${SMAX}"_A != ""_A ] && 
	[ "${MMIN}"_A != ""_A ] && [ "${MMAX}"_A != ""_A ] &&
	[ "${TEST}" = "TRUE" ]; then
	${P_LDACTOASC} -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ \
	    -t OBJECTS -k FWHM_IMAGE MAG_AUTO -b > ${TEMPDIR}/seeing_asciifilt.cat_$$
	nobj=`${P_GAWK} 'BEGIN{nobj=0}
           { if ($1>'${SMIN}' && $1<'${SMAX}' && $2>'${MMIN}' && $2<'${MMAX}' ) nobj++}
           END {print nobj}' ${TEMPDIR}/seeing_asciifilt.cat_$$`
	
	if [ ${nobj} -gt 5 ]; then
	    ${P_LDACFILTER} -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ \
		-o ${TEMPDIR}/seeing_ldac_filt.cat3_$$ \
		-c "((((FWHM_IMAGE>${SMIN})AND(FWHM_IMAGE<${SMAX}))AND(MAG_AUTO>${MMIN}))AND(MAG_AUTO<${MMAX}));"
	    ${P_LDACTOASC} -b -i ${TEMPDIR}/seeing_ldac_filt.cat3_$$ -t OBJECTS \
		-k FWHM_IMAGE > ${TEMPDIR}/seeing_$$.asc
	else
	    ${P_LDACTOASC} -b -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ -t OBJECTS \
		-k FWHM_IMAGE > ${TEMPDIR}/seeing_$$.asc
	fi
    else
	${P_LDACTOASC} -b -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ -t OBJECTS \
	    -k FWHM_IMAGE > ${TEMPDIR}/seeing_$$.asc
    fi
    SEEING=`${P_GETSTATS} -i ${TEMPDIR}/seeing_$$.asc -m median`
    SEEINGERR=`${P_GETSTATS} -i ${TEMPDIR}/seeing_$$.asc -m sigma`
    FWHM_WORLD=`echo ${SEEING} | ${P_GAWK} '{printf("%.2f", $1*'${PSCALE}')}'`
    FWHM_ERR=`echo ${SEEINGERR} | ${P_GAWK} '{printf("%.2f", $1*'${PSCALE}')}'`
    FWHM_IMAGE=`echo ${SEEING} | ${P_GAWK} '{printf("%.2f", $1)}'`
    
    
    # get FLUX_RADIUS
    ${P_PREANISOTROPY} -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ \
	-t OBJECTS -s 0.1 -c snratio 10.0 1000.0 -k FLUX_RADIUS MAG_AUTO >& \
	${TEMPDIR}/preaniso_$$
    SMIN=`${P_GAWK} '($0 ~ /propose/) {print $8-0.01}'  ${TEMPDIR}/preaniso_$$`
    SMAX=`${P_GAWK} '($0 ~ /propose/) {print $12+0.01}' ${TEMPDIR}/preaniso_$$`
    MMIN=`${P_GAWK} '($0 ~ /propose/) {print $14-0.01}' ${TEMPDIR}/preaniso_$$`
    MMAX=`${P_GAWK} '($0 ~ /propose/) {print $18+0.01}' ${TEMPDIR}/preaniso_$$`
    nobj=0
    TEST="FALSE"
    TEST=`echo ${SMIN} ${SMAX} ${MMIN} ${MMAX} | \
	${P_GAWK} '{if ($1<$2 && $3<$4) print "TRUE"}'`

    if [ "${SMIN}"_A != ""_A ] && [ "${SMAX}"_A != ""_A ] && 
	[ "${MMIN}"_A != ""_A ] && [ "${MMAX}"_A != ""_A ] &&
	[ "${TEST}" = "TRUE" ]; then
	${P_LDACTOASC} -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ \
	    -t OBJECTS -k FLUX_RADIUS MAG_AUTO -b > ${TEMPDIR}/seeing_asciifilt.cat_$$
	nobj=`${P_GAWK} 'BEGIN{nobj=0}
           { if ($1>'${SMIN}' && $1<'${SMAX}' && $2>'${MMIN}' && $2<'${MMAX}' ) nobj++}
           END {print nobj}' ${TEMPDIR}/seeing_asciifilt.cat_$$`
	if [ ${nobj} -gt 5 ]; then
	    ${P_LDACFILTER} -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ \
		-o ${TEMPDIR}/seeing_ldac_filt.cat3_$$ \
		-c "((((FLUX_RADIUS>${SMIN})AND(FLUX_RADIUS<${SMAX}))AND(MAG_AUTO>${MMIN}))AND(MAG_AUTO<${MMAX}));"
	    ${P_LDACTOASC} -b -i ${TEMPDIR}/seeing_ldac_filt.cat3_$$ -t OBJECTS \
		-k FLUX_RADIUS > ${TEMPDIR}/seeing_$$.asc
	else
	    ${P_LDACTOASC} -b -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ -t OBJECTS \
		-k FLUX_RADIUS > ${TEMPDIR}/seeing_$$.asc
	fi
    else
	${P_LDACTOASC} -b -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ -t OBJECTS \
	    -k FLUX_RADIUS > ${TEMPDIR}/seeing_$$.asc
    fi
    SEEING=`${P_GETSTATS} -i ${TEMPDIR}/seeing_$$.asc -m median`
    SEEINGERR=`${P_GETSTATS} -i ${TEMPDIR}/seeing_$$.asc -m sigma`
    HFD_WORLD=`echo ${SEEING} | ${P_GAWK} '{printf("%.2f", 2.*$1*'${PSCALE}')}'`
    HFD_ERR=`echo ${SEEINGERR} | ${P_GAWK} '{printf("%.2f", 2.*$1*'${PSCALE}')}'`
    HFD_IMAGE=`echo ${SEEING} | ${P_GAWK} '{printf("%.2f", 2.*$1)}'`
else
    FWHM_WORLD=-1
    FWHM_ERR=0
    FWHM_IMAGE=0
    HFD_WORLD=-1
    HFD_ERR=0
    HFD_IMAGE=0
fi
    
rm ${TEMPDIR}/seeing_sexparam.asc_$$
rm ${TEMPDIR}/seeing_$$.cat
rm ${TEMPDIR}/seeing_$$.asc
rm ${TEMPDIR}/seeing_ldac.cat_$$
rm ${TEMPDIR}/seeing_ldac_filt.cat_$$
rm ${TEMPDIR}/seeing_ldac_filt.cat2_$$

# remove some interfering keywords

test=`get_key /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits EXPTIME`
if [ ! "${test}"_A = "_A" ]; then
    ${P_REPLACEKEY} /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits \
	"DUMMY5  = 0" EXPTIME
fi

test=`get_key /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits GAIN`
if [ ! "${test}"_A = "_A" ]; then
    ${P_REPLACEKEY} /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits \
	"DUMMY52 = 0" GAIN
fi

test=`get_key /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits SATURATE`
if [ ! "${test}"_A = "_A" ]; then
    ${P_REPLACEKEY} /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits \
	"DUMMY53 = 0" SATURATE
fi

test=`get_key /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits OBJECT`
if [ ! "${test}"_A = "_A" ]; then
    ${P_REPLACEKEY} /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits \
	"DUMMY54 = 0" OBJECT
fi

# write keywords in header

# TEXPTIME key (total integrated exposure time)
value ${EXPTIME}
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits \
    TEXPTIME "${VALUE} / total Exposure Time" REPLACE

# EXPTIME keyword (effective exposure time in the co-added images;
# up to now the scaling is always for 1s!
value "1.00"
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits EXPTIME \
      "${VALUE} / effective Exposure Time (s)" REPLACE

# the following expression for the GAIN is only valid if the unit of the 
# co-added image is ADU/sec and if a mean (weighted mean) co-addition 
# was performed.
value ${GAINEFF}
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  GAIN \
         "${VALUE} / effective GAIN for SExtractor" REPLACE

# INSTRUME keyword
value "'${INSTRUMENT}'"
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  INST_TEL \
         "${VALUE} / Instrument and Telescope" REPLACE

# FILTER key
value "'${FILTER}'"
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  FILTER \
         "${VALUE} / Filter Name" REPLACE

# OBJECT key
value "'${OBJECT}'"
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  OBJECT \
         "${VALUE} / Target Name" REPLACE

# ASTROMREFCAT key
ASTROMREFCAT=`cat /$1/$2/headers/.astrefcat_used`
if [ "${ASTROMREFCAT}"_A = "_A" ]; then
    ASTROMREFCAT="'UNKNOWN'"
fi
value "'${ASTROMREFCAT}'"
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  ASTRCAT \
         "${VALUE} / Astrometric reference catalogue" REPLACE

# ASTRERR keyword (fixed for all catalogues for the moment)
#value "0.30"
#writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  ASTRERR \
#         "${VALUE} / estimated absolute astrometric uncertainty(arcsec)" REPLACE

# truncate the MAGZP after three decimal digits:
if [ -f /$1/$2/cat/chips_phot.cat5 ]; then
    ${P_LDACTESTEXIST} -i /$1/$2/cat/chips_phot.cat5 -t ABSPHOTOM -k COADDZP
    if [ "$?" -gt "0" ]; then
	MAGZP="-1.0"
    else
	${P_LDACTOASC} -i /$1/$2/cat/chips_phot.cat5 -t ABSPHOTOM -k COADDZP -b > \
	    ${TEMPDIR}/zp_$$
	${P_GAWK} '($1>0)' ${TEMPDIR}/zp_$$ > ${TEMPDIR}/zp2_$$
	MAGZP=`${P_GETSTATS} -i ${TEMPDIR}/zp2_$$ -m mean | \
	    ${P_GAWK} '{printf("%.3f", $1)}'`
    fi
    value ${MAGZP}
    writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  ZP \
	"${VALUE} / Magnitude Zeropoint (-1.0 if unknown)" REPLACE
    
    # photomeric zeropoint uncertainty (fixed for the moment)
    MAGZPERR=`${P_GAWK} 'BEGIN { if('${MAGZP}' > 0) {
                   printf("%.3f", 0.10)
               } else {
                   printf("%.3f", -1.0)
                   }}'`
    value ${MAGZPERR}
    writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  ZP_ERROR \
	"${VALUE} / Zeropoint uncertainty; currently fixed to 0.1" REPLACE
    
    # MAGSYS keyword (magnitude system: AB, Vega, 2MASS or SDSS)
    if [ ! "${MAGZP}" = "-1.00" ]; then 
	value "'${V_ABSPHOT_STDCAT}'"
	writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  MAGSYS \
	    "${VALUE} / Magnitude System" REPLACE
    else
	value "'undetermined'"
	writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  MAGSYS \
	    "${VALUE} / Magnitude System" REPLACE
    fi
fi

#
# BUNIT keyword
value "'ADU/s'"
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  BUNIT \
         "${VALUE} / Unit of pixel values" REPLACE

# SEEING keywords
value ${FWHM_WORLD}
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  FWHM \
    "${VALUE} / Image seeing (FWHM, arcsec)" REPLACE

value ${FWHM_ERR}
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  FWHM_ERR \
    "${VALUE} / Image seeing error (FWHM, arcsec)" REPLACE

value ${FWHM_IMAGE}
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  FWHM_IMG \
    "${VALUE} / Image seeing (FWHM, pixel)" REPLACE

value ${HFD_WORLD}
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  HFD \
    "${VALUE} / Image seeing (half flux diameter, arcsec)" REPLACE

value ${HFD_ERR}
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  HFD_ERR \
    "${VALUE} / Image seeing error (half flux diameter, arcsec)" REPLACE

value ${HFD_IMAGE}
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  HFD_IMG \
    "${VALUE} / Image seeing (half flux diameter, pixel)" REPLACE

# NCOMBINE key
value ${NCOMBINE}
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  NCOMBINE \
         "${VALUE} / number of individual frames in this stack" REPLACE

# GABODSST key
value ${GABODSST}
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  GABODSST \
         "${VALUE} / first GABODSID of images in this stack" REPLACE

# GABODSEN key
value ${GABODSEN}
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  GABODSEN \
         "${VALUE} / last GABODSID of images in this stack" REPLACE

# DATE-ST key
value "'${DATEST}'"
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  DATE-ST \
         "${VALUE} / first date of stack data (local time at tel.)" REPLACE

# DATE-END key
value "'${DATEEND}'"
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  DATE-END \
         "${VALUE} / last date of stack data (local time at tel.)" REPLACE

# PROJECT key
value "'${PROJECTNAME}'"
writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  PROJECT \
         "${VALUE} / The PROJECT name used in THELI GUI" REPLACE

#
# add information on the condition on the input images
# to enter the stacking process.
writecommhis /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  COMMENT ""
writecommhis /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  COMMENT "Conditions on the input images:"

#
# split the coaddition condition in strings of length 20 if necessary
echo ${COADDCONDITION} | ${P_GAWK} 'BEGIN {len=66} 
                      {
                        if((n=length($1)/len)-(int(length($1)/len))>0.001) { 
                          n=n+1; 
                        }
                        pos=1; 
                        for(i=1; i<=n; i++) {
                          print substr($1,pos,len); 
                          pos+=len;
                        }
                      }' > ${TEMPDIR}/coaddcondition_$$.txt

j=1
while read string
do
  writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  COND${j} "'${string}'" REPLACE
  j=$(( $j + 1 ))
done < ${TEMPDIR}/coaddcondition_$$.txt

rm ${TEMPDIR}/coaddcondition_$$.txt
 
# add information about the photometric solutions entering the
# coadded image
if [ -f /$1/$2/cat/chips_phot.cat5 ]; then
    ${P_LDACTESTEXIST} -i /$1/$2/cat/chips_phot.cat5 -t SOLPHOTOM
    if [ "$?" -eq "0" ]; then
	${P_LDACTOASC} -i /$1/$2/cat/chips_phot.cat5 -b -t SOLPHOTOM | \
	    ${P_GAWK} '{printf $0"; "}' | \
	    ${P_GAWK} 'BEGIN {len=66} 
             {
               if ((n = length($0) / len) - (int(length($0)/len)) > 0.001) { 
                 n = n + 1; 
               } 
               pos = 1; 
               for (i = 1; i <= n; i++) {
                 print substr($0,pos,len); 
                 pos+=len;
               }
             }' > ${TEMPDIR}/nights_$$.txt
	j=1
	while read string
	do
	    writekey /$1/$2/coadd_${V_COADD_IDENT}/coadd.fits  SOLPH${j} "'${string}'" REPLACE
	    j=$(( $j + 1 ))
	done < ${TEMPDIR}/nights_$$.txt
	
	rm ${TEMPDIR}/nights_$$.txt
    fi
fi

rm ${TEMPDIR}/seeing_ldac_filt.cat*_$$
