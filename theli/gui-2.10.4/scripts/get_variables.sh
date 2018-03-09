#!/bin/bash -xv

# $1: script dir
# $2: dir with resampled images
# $3: spatial tolerance in arcseconds
# $4: mag zeropoint
# $5: brightest magnitude for reference stars (to avoid saturation)
# $6: aperture window size in pixel
# $7: ALPHA variable (in degrees)
# $8: DELTA variable (in degrees)

dir=`pwd`

cd $1

. progs.ini

${P_GAWK} '($1 !~ /BACK_TYPE/ && $1 !~ /BACK_VALUE/) {print}' \
    ${DATACONF}/getvar.conf.sex > ${TEMPDIR}/postcoadd.conf.sex_$$
POSTCOADD=${TEMPDIR}/postcoadd.conf.sex_$$

cd $2

# *********************************************************************
# create object catalogues
# *********************************************************************

maglim=`echo $2 $3 | ${P_GAWK} '{print $1+$2}'`

catlist=""
assoclist=""
ldactoasc=""
mjdlist=""
numobjects=0

test ! -d photom && mkdir photom
test ! -d photom/cat && mkdir photom/cat
test -f ${TEMPDIR}/make_ssc_var.conf && rm ${TEMPDIR}/make_ssc_var.conf

i=0
for image in `ls -1 *resamp.fits`
do
  base=`basename ${image} .fits`
  if [ ! -f photom/cat/${base}.cat ]; then
    ${P_SEX} -c ${POSTCOADD} \
	/$2/${image} \
	-WEIGHT_IMAGE /$2/${base}.weight.fits \
	-WEIGHT_TYPE MAP_WEIGHT \
	-CATALOG_NAME ${TEMPDIR}/${base}.cat \
	-FILTER Y \
	-DETECT_MINAREA 10 \
	-DETECT_THRESH 20 \
	-ANALYSIS_THRESH 20 \
	-MAG_ZEROPOINT $4 \
	-DEBLEND_MINCONT 0.0001 \
	-PHOT_APERTURES $6 \
	-CATALOG_TYPE FITS_LDAC

    # we need to do lots of reformatting so that the stuff would run through
    # the "associate" and "make_ssc" steps

    ${P_LDACCONV} -i ${TEMPDIR}/${base}.cat \
	-o ${TEMPDIR}/${base}_ldac.cat -b 1 -c "sex" -f R
    
    ${P_LDACADDKEY} -i ${TEMPDIR}/${base}_ldac.cat \
	-o ${TEMPDIR}/${base}_ldac_tmp.cat \
	-t OBJECTS -k nu 1.0 FLOAT ""
    
    ${P_LDACCALC} -i ${TEMPDIR}/${base}_ldac_tmp.cat \
	-o ${TEMPDIR}/tmp1.cat_$$ -t OBJECTS \
	-c "(FLUX_RADIUS);" -n "rg" "" -k FLOAT \
	-c "(Xpos-1.0);" -n "x" "" -k LONG \
	-c "(Ypos-1.0);" -n "y" "" -k LONG \
	-c "(Xpos);" -n "xbad" "" -k FLOAT \
	-c "(Ypos);" -n "ybad" "" -k FLOAT
    
    ${P_LDACTOASC} -i ${TEMPDIR}/tmp1.cat_$$ -b -t FIELDS \
	-k FITSFILE SEXBKGND SEXBKDEV -s > ${TEMPDIR}/tmp.asc_$$
    
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
    
    ${P_ASCTOLDAC} -i ${TEMPDIR}/tmp.asc_$$ \
	-c ${TEMPDIR}/asctoldac_tmp.conf_$$ -t HFINDPEAKS \
	-o ${TEMPDIR}/hfind.cat_$$ -b 1 -n "KSB"
    
    # now transfer the HFINDPEAKS table to the SEX catalog
    ${P_LDACADDTAB} -i ${TEMPDIR}/tmp1.cat_$$ \
	-o ${TEMPDIR}/tmp2.cat_$$ \
	-p ${TEMPDIR}/hfind.cat_$$ \
	-t HFINDPEAKS

    # done with reformatting

    # filter crappy objects
    ${P_LDACFILTER} -i ${TEMPDIR}/tmp2.cat_$$ \
	-o photom/cat/${base}.cat \
	-c "(rg>0.0)AND(Flag=0);"

  fi

  # increment the arguments
  catlist="${catlist} cat/${base}.cat"
  assoclist="${assoclist} cat/${base}_assoc.cat"
  ldactoasc="${ldactoasc} Mag_${i}"
  magerrlist="${magerrlist} Magerr_${i}"
  
  # append the Ra, Dec, Mag and Mag errors to the make_ssc.conf  
  echo "# "                       >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_NAME  = Ra_${i}"      >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_INPUT = ALPHA_J2000"  >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_MERGE = AVERAGE"      >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_CHAN  = ${i}"         >> ${TEMPDIR}/make_ssc_var.conf
  echo "# "                       >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_NAME  = Dec_${i}"     >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_INPUT = DELTA_J2000"  >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_MERGE = AVERAGE"      >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_CHAN  = ${i}"         >> ${TEMPDIR}/make_ssc_var.conf
  echo "# "                       >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_NAME  = Mag_${i}"     >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_INPUT = MAG_APER"     >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_MERGE = AVERAGE"      >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_CHAN  = ${i}"         >> ${TEMPDIR}/make_ssc_var.conf
  echo "# "                       >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_NAME  = Magerr_${i}"  >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_INPUT = MAGERR_APER"  >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_MERGE = AVERAGE"      >> ${TEMPDIR}/make_ssc_var.conf
  echo "COL_CHAN  = ${i}"         >> ${TEMPDIR}/make_ssc_var.conf

  # get the MJD
  coaddid=`echo $2 | ${P_GAWK} 'BEGIN{FS="coadd_"} {print $NF}'`
  truebase=`basename ${image} .sub.${coaddid}.resamp.fits`
  dateobs=`${P_DFITS} ../${truebase}.fits | ${P_FITSORT} DATE-OBS | ${P_GAWK} '(NR==2) {print $2}'`
  exptime=`${P_DFITS} ../${truebase}.fits | ${P_FITSORT} EXPTIME | ${P_GAWK} '(NR==2) {print $2}'`
  mjd=`${P_MJD} -f ${dateobs} | ${P_GAWK} '{print $7}'`
  mjd=`echo ${mjd} ${exptime} | ${P_GAWK} '{printf "%lf\n", $1+$2/2./86400.}'`
  mjdlist="${mjdlist} ${mjd}"

  i=$((${i} + 1))
  numobjects=${i}
done

echo "MJD :" ${mjdlist} > ${TEMPDIR}/mjd_$$
echo "#" >> ${TEMPDIR}/mjd_$$


# *********************************************************************
# associate and merge the catalogues
# *********************************************************************

cd photom

${P_ASSOCIATE} -i ${catlist} -o ${assoclist} -t OBJECTS \
    -c ${DATACONF}/colcorr_assoc.conf -INTER_COLOR_TOL 0.5

${P_MAKESSC} -i ${assoclist} -o photom.cat -t OBJECTS -c ${TEMPDIR}/make_ssc_var.conf

\rm cat/*assoc.cat


# ***************************************************************************
# convert the stuff to ascii and keep only objects detected in all images
# ***************************************************************************

${P_LDACTOASC} -i photom.cat -t PSSC -k Ra_0 Dec_0 ${ldactoasc} -b |\
    ${P_GAWK} '($0 !~ /0.000/)' > ${TEMPDIR}/variable_fullascii_$$
${P_LDACTOASC} -i photom.cat -t PSSC -k ${magerrlist} -b |\
    ${P_GAWK} '($0 !~ /0.000/)' > ${TEMPDIR}/variable_fullascii_error_$$


# *********************************************************************
# identify the variable star
# *********************************************************************

objra=$7
objdec=$8
tol=`echo $3 | ${P_GAWK} '{print $1/3600}'`
ratol=`echo ${tol} ${dec} | ${P_GAWK} '{print $1/cos($2*3.14159/180)}'`
ramin=`echo ${objra} ${ratol} | ${P_GAWK} '{printf "%lf\n", $1-$2}'`
ramax=`echo ${objra} ${ratol} | ${P_GAWK} '{printf "%lf\n", $1+$2}'`
decmin=`echo ${objdec} ${tol} | ${P_GAWK} '{printf "%lf\n", $1-$2}'`
decmax=`echo ${objdec} ${tol} | ${P_GAWK} '{printf "%lf\n", $1+$2}'`

# find object(s) with similar coordinates
obj_row=`${P_GAWK} '{ if ($1>'${ramin}' && $1<'${ramax}' && 
                          $2>'${decmin}' && $2<'${decmax}') print NR}' \
                      ${TEMPDIR}/variable_fullascii_$$`

# exit if the variable star could not be identified unambiguously
nobj=`echo ${obj_row} | wc | ${P_GAWK} '{print $1}'`
if [ "${nobj}" -gt 1 ]; then
    echo "ERROR: variable source could not be identified unambiguously."
    echo "       Decrease the spatial tolerance."
    exit
fi


# *********************************************************************
# coordinate reformatting (dec -> hex)
# *********************************************************************

${P_GAWK} '{print $1, $2, "0"}' ${TEMPDIR}/variable_fullascii_$$ > ${TEMPDIR}/variable_coords_$$
${P_DECSEXCONV} -i ${TEMPDIR}/variable_coords_$$ -o ${TEMPDIR}/variable_coords_sex2_$$
${P_GAWK} '{print $1, $2}' ${TEMPDIR}/variable_coords_sex2_$$ > ${TEMPDIR}/variable_coords_sex

# separate all the non-coordinate stuff
${P_GAWK} '{split($0,a); for (j=3;j<=NF;j++) printf "%s ", a[j]; printf "\n"}' \
    ${TEMPDIR}/variable_fullascii_$$ > ${TEMPDIR}/variable_mags_$$

# replace the decimal coordinates and merge again with the non-coordinate stuff
${P_GAWK} '{line=$0; getline var <"'${TEMPDIR}'/variable_coords_sex"; print var, line}' \
    ${TEMPDIR}/variable_mags_$$ > ${TEMPDIR}/variable_fullascii_$$


# ****************************************************************************
# get the line corresponding to the variable star only
# ****************************************************************************

${P_GAWK} '(NR == '${obj_row}') {
           printf "%s\n%s\n", "#", "#Variable star:"
           print
           printf "%s\n%s\n", "#", "#Reference stars:"}' \
           ${TEMPDIR}/variable_fullascii_$$ > ${TEMPDIR}/photom_varstar.ascii


# ****************************************************************************************
# the reference light curve should be zero on average, i.e. we have to remove the mean
# ****************************************************************************************

# get the mean magnitude of the variable star
meanobjmag=`${P_GAWK} 'BEGIN{mean=0} {
                       split($0,a)
                       for (i=3;i<=NF;i++) mean=mean+a[i]/(NF-2)} 
                       END{print mean}' ${TEMPDIR}/photom_varstar.ascii`

# reject reference stars whose mean magnitude is less than "mean_mag(variable_star) + 0.1"
${P_GAWK} '{if (NR != '${obj_row}') {
               mean=0
               split($0,a)
               for (i=3;i<=NF;i++) mean=mean+a[i]/(NF-2)}
           if (mean<0.1+'${meanobjmag}' && mean>'$5') print $0}' \
           ${TEMPDIR}/variable_fullascii_$$ > ${TEMPDIR}/photom_refstar.ascii

# take out the average brightness from each reference lightcurve, 
# i.e. keep only the modulation

${P_GAWK} '{
     mean=0
     split($0,a)
     printf "%lf %lf ", $1, $2
     for (i=3;i<=NF;i++) mean=mean+a[i]
     for (i=3;i<=NF;i++) printf "%lf ", a[i]-mean/(NF-2)
     printf "\n"}' \
     ${TEMPDIR}/photom_refstar.ascii > ${TEMPDIR}/photom_tmp2_$$


# **************************************************************************
# calculate the average reference level and its rms
# **************************************************************************

${P_GAWK} '{
     split($0,a)
     for(j=1;j<=NF;j++) b[NR,j]=a[j]} END {
     printf "%s ", "Reference level:  "
     for (j=3;j<=NF;j++) {
         mean[j] = 0
         for (i=1;i<=NR;i++) {
             mean[j] = mean[j] + b[i,j]/NR
         }
         printf "%2.4f  ", mean[j]
     }
     printf "\n"
     printf "%s ", "Reference rms:    "
     for (j=3;j<=NF;j++) {
         rms[j] = 0
         for (i=1;i<=NR;i++) {
             rms[j] = rms[j] + (b[i,j] - mean[j])^2
         }
         printf "%2.4f  ", sqrt(rms[j]/(NR-1))
     }
     printf "\n"
     }' ${TEMPDIR}/photom_tmp2_$$ > ${TEMPDIR}/photom_reflevel.ascii


# *************************************************************************************
# now merge the MJD, the reference level, the variable star, and the reference stars
# *************************************************************************************

cat ${TEMPDIR}/mjd_$$ ${TEMPDIR}/photom_reflevel.ascii \
    ${TEMPDIR}/photom_varstar.ascii ${TEMPDIR}/photom_refstar.ascii > \
    photom_full.ascii

# ***********************************************************************************
# write only the interesting data for analysis, in normal and in transposed format
# ***********************************************************************************

# normal format
${P_GAWK} '($0!~/#/) {split($0,a); for (i=3;i<=NF;i++) printf "%s ", a[i]; printf "\n"}' \
    photom_full.ascii > photom_horizontal.ascii

# transpose
${P_GAWK} '{
     split($0,a); for(j=1;j<=NF;j++) b[NR,j]=a[j]} END {
     for (j=1;j<=NF;j++) {
         for (i=1;i<=NR;i++) {
             c[j,i]=b[i,j]
         }
     }
     for (i=1;i<=NR;i++) {
         for (j=1;j<=NR;j++) {
             printf "%s ", c[i,j]
         }
         printf "\n"
     }}' photom_horizontal.ascii > photom_vertical.ascii 
