#!/bin/bash -xv

# this script calculates color correction factors
# for red-, green- and blue-filtered images.

# $1: this directory
# $2: dir with the images
# $3: blue image
# $4: green image
# $5: red image
# $6: server

cd $1

DIR=$1

. progs.ini

. bash.include

${P_GAWK} '($1 !~ /BACK_TYPE/ && $1 !~ /BACK_VALUE/) {print}' \
    ${DATACONF}/postcoadd.conf.sex > ${TEMPDIR}/postcoadd.conf.sex_$$
POSTCOADD=${TEMPDIR}/postcoadd.conf.sex_$$

V_COLORCORR_B=$3
V_COLORCORR_G=$4
V_COLORCORR_R=$5
SERVER=$6

# get the NOMAD1/SDSS/UCAC4 catalogues
# (for amateur astronomy colour calibration)

cd11=`get_key $2/$3 CD1_1`
cd12=`get_key $2/$3 CD1_2`
if [ "${cd11}"_A = "_A" ]; then
    cd11=`${P_CDELT2CD} -i $2/$3 | ${P_GAWK} '{print $1}'`
    cd12=`${P_CDELT2CD} -i $2/$3 | ${P_GAWK} '{print $2}'`
fi
pixscale=`echo $cd11 $cd12 | ${P_GAWK} '{print sqrt($1*$1+$2*$2)}'`
naxis1=`${P_DFITS} $2/$3 | ${P_FITSORT} NAXIS1 | ${P_GAWK} '(NR!=1) {print $2}'`
naxis2=`${P_DFITS} $2/$3 | ${P_FITSORT} NAXIS2 | ${P_GAWK} '(NR!=1) {print $2}'`
radius=`echo $naxis1 $naxis2 $pixscale | ${P_GAWK} '{print sqrt($1*$1+$2*$2)*$3*60*0.5}'`
xcen=`echo $naxis1 | ${P_GAWK} '{print $1/2.}'`
ycen=`echo $naxis2 | ${P_GAWK} '{print $1/2.}'`
center_radec=`${P_XY2SKY} -d $2/$3 ${xcen} ${ycen} |\
              ${P_GAWK} '{if ($2<=0.) {print $1,$2} else {print $1, "+"$2}}'`

# The CALIBDIR below is created from within the GUI itself
CALIBDIR=$2/PHOTCAT_calibration

# delete old solutions
rm -f ${CALIBDIR}/PHOTCAT_calibration*.dat
rm -f ${CALIBDIR}/unconnected

# check if the connection can be established
${P_FINDNOMAD1} -c $center_radec -m 10 >& ${TEMPDIR}/testconnection
connection="_A"
connection=`${P_GAWK} '($0~/Network is unreachable/) {print "no"}' ${TEMPDIR}/testconnection`
if [ "${connection}" = "no" ]; then
    echo "unconnected" > ${CALIBDIR}/unconnected
    exit
fi

# retrieve the reference sources from all 3 catalogs:
ra=`echo ${center_radec} | ${P_GAWK} '{print $1}'`
dec=`echo ${center_radec} | ${P_GAWK} '{print $2}'`

# G2-type colors in ugriBVR filters from
# http://www.sdss.org/dr4/algorithms/sdssUBVRITransform.html#vega_sun_colors

UG=1.43  #(+/-0.08; )
GR=0.44  #(+/-0.05; )
RI=0.11  #(+/-0.05; )    these ranges roughly span G0V - G5V
BV=0.65  #(+/-0.05; )
VR=0.36  #(+/-0.05; )

xytol=0.05    # the initial tolerances (we iterate over increased values if no sources are found)
ugtol=0.08    # for u-g I chose a larger tolerance

# Somewhat bluer values are found by Covey et al. 2007
# Vizier J/AJ/134/2398/table3 from synthetic spectra:

#      u-g     g-r     r-i 
# G0V 1.30    0.38    0.14
# G2V 1.37    0.45    0.16
# G5V 1.49    0.49    0.16

for CAT in NOMAD SDSS UCAC4
do
    # some definitions
    if [ ${CAT} = "NOMAD" ]; then
	magstring="Bmag Vmag Rmag NOMAD1 Jmag"   # the last two are dummies needed for formatting reasons
    elif [ ${CAT} = "UCAC4" ]; then
	magstring="Bmag Vmag gmag rmag imag"
    else
	magstring="umag gmag rmag imag zmag"
    fi

    # download the catalogs
    ${P_VIZQUERY} -wget -site=${SERVER} -mime=csv \
	-out.max=1000000 -c.rm=${radius} -c="${ra} ${dec}" \
	-out=_RAJ -out=_DEJ -out="${magstring}" -source=${CAT} \
	> ${TEMPDIR}/colcorr.cat_$$
    ${P_GAWK} '($0 !~ /#/)' ${TEMPDIR}/colcorr.cat_$$ | \
	${P_GAWK} 'BEGIN{FS=";"} (NR>4) {print $1, $2, $3, $4, $5, $6, $7}' | \
	${P_GAWK} '(NF==7)' > ${TEMPDIR}/colorref_full_${CAT}_$$

    # keep only sources with matching G2-type colors
    # we iterate twice with extended tolerances if no sources are found
    for factor in 1.0 1.5 2.0
    do
	if [ "${CAT}" = "NOMAD" ]; then
	    COL1MIN=`echo ${BV} ${xytol} ${factor} | ${P_GAWK} '{print $1-$2*$3}'`
	    COL1MAX=`echo ${BV} ${xytol} ${factor} | ${P_GAWK} '{print $1+$2*$3}'`
	    COL2MIN=`echo ${VR} ${xytol} ${factor} | ${P_GAWK} '{print $1-$2*$3}'`
	    COL2MAX=`echo ${VR} ${xytol} ${factor} | ${P_GAWK} '{print $1+$2*$3}'`
	    ${P_GAWK} '{ if ($3-$4>="'${COL1MIN}'" && $3-$4<="'${COL1MAX}'" && 
                             $4-$5>="'${COL2MIN}'" && $4-$5<="'${COL2MAX}'") print}' \
	        ${TEMPDIR}/colorref_full_${CAT}_$$ > \
	        ${TEMPDIR}/colorref_filt1_${CAT}_$$
	elif [ "${CAT}" = "SDSS" ]; then
	    COL1MIN=`echo ${UG} ${ugtol} ${factor} | ${P_GAWK} '{print $1-$2*$3}'`
	    COL1MAX=`echo ${UG} ${ugtol} ${factor} | ${P_GAWK} '{print $1+$2*$3}'`
	    COL2MIN=`echo ${GR} ${xytol} ${factor} | ${P_GAWK} '{print $1-$2*$3}'`
	    COL2MAX=`echo ${GR} ${xytol} ${factor} | ${P_GAWK} '{print $1+$2*$3}'`
	    COL3MIN=`echo ${RI} ${xytol} ${factor} | ${P_GAWK} '{print $1-$2*$3}'`
	    COL3MAX=`echo ${RI} ${xytol} ${factor} | ${P_GAWK} '{print $1+$2*$3}'`
	    ${P_GAWK} '{ if ($3-$4>="'${COL1MIN}'" && $3-$4<="'${COL1MAX}'" && 
                             $4-$5>="'${COL2MIN}'" && $4-$5<="'${COL2MAX}'" &&
                             $5-$6>="'${COL3MIN}'" && $5-$6<="'${COL3MAX}'") print}' \
		${TEMPDIR}/colorref_full_${CAT}_$$ > \
		${TEMPDIR}/colorref_filt1_${CAT}_$$
	else # UCAC4
	    COL1MIN=`echo ${BV} ${xytol} ${factor} | ${P_GAWK} '{print $1-$2*$3}'`
	    COL1MAX=`echo ${BV} ${xytol} ${factor} | ${P_GAWK} '{print $1+$2*$3}'`
	    COL2MIN=`echo ${GR} ${xytol} ${factor} | ${P_GAWK} '{print $1-$2*$3}'`
	    COL2MAX=`echo ${GR} ${xytol} ${factor} | ${P_GAWK} '{print $1+$2*$3}'`
	    COL3MIN=`echo ${RI} ${xytol} ${factor} | ${P_GAWK} '{print $1-$2*$3}'`
	    COL3MAX=`echo ${RI} ${xytol} ${factor} | ${P_GAWK} '{print $1+$2*$3}'`
	    ${P_GAWK} '{ if ($3-$4>="'${COL1MIN}'" && $3-$4<="'${COL1MAX}'" && 
                             $5-$6>="'${COL2MIN}'" && $5-$6<="'${COL2MAX}'" &&
                             $6-$7>="'${COL3MIN}'" && $6-$7<="'${COL3MAX}'") print}' \
		${TEMPDIR}/colorref_full_${CAT}_$$ > \
		${TEMPDIR}/colorref_filt1_${CAT}_$$
	fi
	
	# we have to add line numbers for associate etc
	${P_GAWK} '{print NR, $0}' ${TEMPDIR}/colorref_filt1_${CAT}_$$ > ${TEMPDIR}/colorref_filt_${CAT}_$$

        # check how many sources we got, and break the loop if any were found;
	# using "indirect variable reference" to construct and assign the number_CAT variables
	nref=`wc ${TEMPDIR}/colorref_filt_${CAT}_$$ | ${P_GAWK} '{print $1}'`
	if [ ${CAT} = "NOMAD" ]; then
	    nref_NOMAD=${nref}
	elif [ ${CAT} = "SDSS" ]; then
	    nref_SDSS=${nref}
	else
	    nref_UCAC4=${nref}
	fi

	if [ $nref -gt 0 ]; then
	    break
	fi
    done

    # make an LDAC catalog
    if [ $nref -gt 0 ]; then
	${P_ASCTOLDAC} \
	    -a ${TEMPDIR}/colorref_filt_${CAT}_$$ \
	    -c ${DATACONF}/colcalib.refcat.asctoldac.conf \
	    -o /${CALIBDIR}/photref_${CAT}.cat \
	    -t OBJECTS
    else
	# For consistency, we need a result file
	echo 0           >  ${CALIBDIR}/PHOTCAT_calibration_${CAT}.dat
	echo 1.000 0.000 >> ${CALIBDIR}/PHOTCAT_calibration_${CAT}.dat
	echo 1.000 0.000 >> ${CALIBDIR}/PHOTCAT_calibration_${CAT}.dat
	echo 1.000 0.000 >> ${CALIBDIR}/PHOTCAT_calibration_${CAT}.dat
    fi
done


####################################################################################
# Now that we have dealt with the reference catalogs, let's look at the images
####################################################################################

# make a pan-chromatic detection image
${P_IC} '%1 %2 + %3 +' /$2/$3 /$2/$4 /$2/$5 > /${CALIBDIR}/detect_image.fits

# make the detection weight (if weights are present)
base=`basename ${V_COLORCORR_B} .fits`
BW=/$2/${base}.weight.fits
base=`basename ${V_COLORCORR_G} .fits`
GW=/$2/${base}.weight.fits
base=`basename ${V_COLORCORR_R} .fits`
RW=/$2/${base}.weight.fits
if [ -f ${BW} ] && [ -f ${GW} ] && [ -f ${RW} ]; then
    ${P_IC} '%1 %2 + %3 +' ${BW} ${GW} ${RW} > /${CALIBDIR}/detect_image.weight.fits
fi

# create object catalogues
images="${V_COLORCORR_B} ${V_COLORCORR_G} ${V_COLORCORR_R}" 

for image in ${images}
do
    base=`basename ${image} .fits`

    # skip object detection if the catalog exists already
    if [ -f "/${CALIBDIR}/${base}.cat" ]; then
	continue
    fi

    imagestring="/${CALIBDIR}/detect_image.fits,/$2/${image}"
    weightstring=""
    if [ -f /${CALIBDIR}/detect_image.weight.fits ]; then
	weightstring="-WEIGHT_IMAGE /${CALIBDIR}/detect_image.weight.fits,/$2/${base}.weight.fits"
        weightstring=${weightstring}" -WEIGHT_TYPE MAP_WEIGHT,MAP_WEIGHT"
    fi

    # saturation value = conservative 0.85*max(image)
    satlevel=`${P_STATS} < /$2/${image} | grep max | ${P_GAWK} '{print 0.85*$6}'`

    ${P_SEX} -c ${POSTCOADD} \
	/$2/${image} ${weightstring} \
	-CATALOG_NAME ${TEMPDIR}/${base}.cat \
	-CATALOG_TYPE FITS_LDAC \
	-FILTER Y \
	-DETECT_MINAREA 5 \
	-DETECT_THRESH 5 \
	-ANALYSIS_THRESH 5 \
	-MAG_ZEROPOINT 20 \
	-DEBLEND_MINCONT 0.0001 \
    	-NTHREADS ${NPARA} \
	-SATUR_LEVEL ${satlevel}

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
    
    ${P_ASCTOLDAC} -i ${TEMPDIR}/tmp.asc_$$ \
	-c ${DATACONF}/colcalib.imgcat.asctoldac.conf -t HFINDPEAKS \
	-o ${TEMPDIR}/hfind.cat_$$ -b 1 -n "KSB"
    
    # transfer the HFINDPEAKS table to the SEX catalog
    ${P_LDACADDTAB} -i ${TEMPDIR}/tmp1.cat_$$ \
	-o ${TEMPDIR}/tmp2.cat_$$ \
	-p ${TEMPDIR}/hfind.cat_$$ \
	-t HFINDPEAKS
    
    ${P_LDACFILTER} -i ${TEMPDIR}/tmp2.cat_$$ \
	-o /${CALIBDIR}/${base}.cat \
	-c "((((rg>0.0)AND(Flag=0))AND(MAG_AUTO<99))AND(MAGERR_AUTO<0.15));"

    ${P_LDACTOSKYCAT} -i /${CALIBDIR}/${base}.cat -t OBJECTS \
	-k SeqNr ALPHA_J2000 DELTA_J2000 MAG_AUTO \
	-l id_col SeqNr ra_col ALPHA_J2000 dec_col \
	DELTA_J2000 mag_col MAG_AUTO \
	-s > /${CALIBDIR}/${base}.skycat
done

# the individual catalogues
bcat=`basename ${V_COLORCORR_B} .fits`.cat
vcat=`basename ${V_COLORCORR_G} .fits`.cat
rcat=`basename ${V_COLORCORR_R} .fits`.cat

for CAT in NOMAD UCAC4 SDSS AVGWHITE
do
    if [ "${CAT}" != "AVGWHITE" ]; then
        # if no reference sources were found
	if [ "${CAT}" = "SDSS" ] && [ ${nref_SDSS} -eq 0 ]; then
	    continue
	fi
	if [ "${CAT}" = "UCAC4" ] && [ ${nref_UCAC4} -eq 0 ]; then
	    continue
	fi
	if [ "${CAT}" = "NOMAD" ] && [ ${nref_NOMAD} -eq 0 ]; then
	    continue
	fi

	# if reference sources were found
        # reformat the reference catalogues so that they run through associate
	${P_LDACDELTAB} -i /${CALIBDIR}/photref_${CAT}.cat -o ${TEMPDIR}/cc1_$$ -t FIELDS
	${P_LDACADDTAB} -i ${TEMPDIR}/cc1_$$ -o ${TEMPDIR}/cc2_$$ -p ${CALIBDIR}/${bcat} -t FIELDS
	${P_LDACADDKEY} -i ${TEMPDIR}/cc2_$$ -o ${TEMPDIR}/cc3_$$ -t OBJECTS -k Flag 0 SHORT ""
	${P_LDACADDKEY} -i ${TEMPDIR}/cc3_$$ -o ${TEMPDIR}/cc4_$$ -t OBJECTS -k A_WORLD 0.00027778 FLOAT ""
	${P_LDACADDKEY} -i ${TEMPDIR}/cc4_$$ -o ${TEMPDIR}/cc5_$$ -t OBJECTS -k B_WORLD 0.00027778 FLOAT ""
	${P_LDACADDKEY} -i ${TEMPDIR}/cc5_$$ -o ${TEMPDIR}/theli_colorref_full_${CAT}.cat_$$ \
	    -t OBJECTS -k THETA_J2000 0.0 FLOAT ""

	# some parameters for cleaner code
	EXTRAPARAM1_ASSOC=${TEMPDIR}/theli_colorref_full_${CAT}.cat_$$
	EXTRAPARAM2_ASSOC=${TEMPDIR}/colorref_tmp_$$
	MAKESSCCONF=${DATACONF}/colcorr_photcat_make_ssc.pairs
	REFMAG=REF_MAG_1
	LDACFILTER="((((MAGAUTO_B>0.)AND(MAGAUTO_G>0.))AND(MAGAUTO_R>0.))AND(REF_MAG_1>0.));"
    else 
	# some parameters for cleaner code
	EXTRAPARAM1_ASSOC=""
	EXTRAPARAM2_ASSOC=""
	MAKESSCCONF=${DATACONF}/colcorr_avgwhite_make_ssc.pairs
	REFMAG=""
	LDACFILTER="(((MAGAUTO_B>0.)AND(MAGAUTO_G>0.))AND(MAGAUTO_R>0.));"
    fi

    # associate and merge the object and the reference catalogues
    ${P_ASSOCIATE} \
	-i ${CALIBDIR}/${bcat} ${CALIBDIR}/${vcat} ${CALIBDIR}/${rcat} ${EXTRAPARAM1_ASSOC} \
	-o ${TEMPDIR}/B.cat_$$ ${TEMPDIR}/G.cat_$$ ${TEMPDIR}/R.cat_$$ ${EXTRAPARAM2_ASSOC} \
	-t OBJECTS -c ${DATACONF}/colcorr_assoc.conf -INTER_COLOR_TOL 0.5
    
    ${P_MAKESSC} \
	-i ${TEMPDIR}/B.cat_$$ ${TEMPDIR}/G.cat_$$ ${TEMPDIR}/R.cat_$$ ${EXTRAPARAM2_ASSOC} \
	-t OBJECTS -c ${MAKESSCCONF} \
	-o ${TEMPDIR}/RGBcolorref.cat_$$ 
    
    # count how many sources we have in common (before running ldacfilter which will just return an error)
    ${P_LDACTOASC} -i ${TEMPDIR}/RGBcolorref.cat_$$ -t PSSC -k MAGAUTO_B MAGAUTO_G MAGAUTO_R ${REFMAG} -b > \
	${TEMPDIR}/RGBcolorref.asc_$$ 

    if [ "${CAT}" != "AVGWHITE" ]; then
	ntot=`${P_GAWK} 'BEGIN{i=0} {if ($1>0 && $2>0 && $3>0 && $4>0) i=i+1} END {print i}' ${TEMPDIR}/RGBcolorref.asc_$$`
    else
	ntot=`${P_GAWK} 'BEGIN{i=0} {if ($1>0 && $2>0 && $3>0) i=i+1} END {print i}' ${TEMPDIR}/RGBcolorref.asc_$$`
    fi

    # how many sources are left after the matching?
    if [ ${CAT} = "NOMAD" ]; then
	nref_NOMAD=${ntot}
    elif [ ${CAT} = "SDSS" ]; then
	nref_SDSS=${ntot}
    elif [ ${CAT} = "UCAC4" ]; then
	nref_UCAC4=${ntot}
    else
	nref_AVGWHITE=${ntot}
    fi

    if [ ${ntot} -eq 0 ]; then
	# For consistency, we need a result file
	echo 0           >  ${CALIBDIR}/PHOTCAT_calibration_${CAT}.dat
	echo 1.000 0.000 >> ${CALIBDIR}/PHOTCAT_calibration_${CAT}.dat
	echo 1.000 0.000 >> ${CALIBDIR}/PHOTCAT_calibration_${CAT}.dat
	echo 1.000 0.000 >> ${CALIBDIR}/PHOTCAT_calibration_${CAT}.dat
    else
        # keep only sources that are found in both the image and the reference catalog
	${P_LDACFILTER} -i ${TEMPDIR}/RGBcolorref.cat_$$ -t PSSC -o ${TEMPDIR}/RGBcolorref_${CAT}.cat_$$ \
	    -c "${LDACFILTER}"

        # add the colors to the catalogs, just for reference
	if [ "${CAT}" = "NOMAD" ]; then
	    ${P_LDACCALC} -i ${TEMPDIR}/RGBcolorref_${CAT}.cat_$$ -t PSSC \
		-c "(REF_MAG_1-REF_MAG_2);" -n REF_B-V "" -k FLOAT \
		-c "(REF_MAG_2-REF_MAG_3);" -n REF_V-R "" -k FLOAT \
		-o ${CALIBDIR}/RGBcolorref_${CAT}.cat
	elif [ "${CAT}" = "SDSS" ]; then
	    ${P_LDACCALC} -i ${TEMPDIR}/RGBcolorref_${CAT}.cat_$$ -t PSSC \
		-c "(REF_MAG_1-REF_MAG_2);" -n REF_u-g "" -k FLOAT \
		-c "(REF_MAG_2-REF_MAG_3);" -n REF_g-r "" -k FLOAT \
		-c "(REF_MAG_3-REF_MAG_4);" -n REF_r-i "" -k FLOAT \
		-o ${CALIBDIR}/RGBcolorref_${CAT}.cat
	elif [ "${CAT}" = "UCAC4" ]; then
	    ${P_LDACCALC} -i ${TEMPDIR}/RGBcolorref_${CAT}.cat_$$ -t PSSC \
		-c "(REF_MAG_1-REF_MAG_2);" -n REF_B-V "" -k FLOAT \
		-c "(REF_MAG_3-REF_MAG_4);" -n REF_g-r "" -k FLOAT \
		-c "(REF_MAG_4-REF_MAG_5);" -n REF_r-i "" -k FLOAT \
		-o ${CALIBDIR}/RGBcolorref_${CAT}.cat
	else # AVGWHITE
	    ${P_LDACCALC} -i ${TEMPDIR}/RGBcolorref_${CAT}.cat_$$ -t PSSC \
		-c "(MAGAUTO_B-MAGAUTO_G);" -n B-G "" -k FLOAT \
		-c "(MAGAUTO_G-MAGAUTO_R);" -n G-R "" -k FLOAT \
		-o ${CALIBDIR}/RGBcolorref_${CAT}.cat
	fi
	
        # make a skycat catalog
	${P_LDACTOSKYCAT} -i ${CALIBDIR}/RGBcolorref_${CAT}.cat -t PSSC \
	    -k SeqNr Ra_G Dec_G MAGAUTO_G \
	    -l id_col SeqNr ra_col Ra_G dec_col Dec_G mag_col MAGAUTO_G \
	    -s > /${CALIBDIR}/PHOTCAT_sources_${CAT}.skycat

        # write out an ASCII catalog with the measured mags of the matched stars
	${P_LDACTOASC} -i ${CALIBDIR}/RGBcolorref_${CAT}.cat -t PSSC \
	    -k MAGAUTO_B MAGAUTO_G MAGAUTO_R -b > \
	    ${TEMPDIR}/colcorr_ascii_${CAT}.cat_$$
	
        # obtain the colors in units of fluxes
	${P_GAWK} '{print 10^(0.4*($1-$2)), 1.0, 10^(0.4*($3-$2))}' \
	    ${TEMPDIR}/colcorr_ascii_${CAT}.cat_$$ > ${TEMPDIR}/colcorr_ascii_${CAT}_fluxes.cat_tmp1_$$
	
	${P_SORT} -n -k 1 ${TEMPDIR}/colcorr_ascii_${CAT}_fluxes.cat_tmp1_$$ > \
	    ${TEMPDIR}/colcorr_ascii_${CAT}_fluxes.cat_$$
	
        # calculate the averages
	${P_GAWK} 'BEGIN {b=0;r=0} {b=b+$1; r=r+$3; barr[NR]=$1; rarr[NR]=$3} 
            END {meanb=b/NR
                 meanr=r/NR
                 rmsb=0
                 rmsr=0
                 for (i=1;i<=NR;i++) {
                     rmsb=rmsb+(barr[i]-meanb)^2
                     rmsr=rmsr+(rarr[i]-meanr)^2
                 }
                 if (NR>1) {
                   rmsb=sqrt(1/(NR-1)*rmsb)/sqrt(NR)
                   rmsr=sqrt(1/(NR-1)*rmsr)/sqrt(NR)
                 }
                 if (NR==1) {
                   rmsb=0.0
                   rmsr=0.0
                 }
                 print NR
                 printf "%2.3f %2.3f\n%2.3f %2.3f\n%2.3f %2.3f\n", meanb, rmsb, 1.0, 0.0, meanr, rmsr}' \
		     ${TEMPDIR}/colcorr_ascii_${CAT}_fluxes.cat_$$ >> \
		     ${CALIBDIR}/PHOTCAT_calibration_${CAT}.dat
    fi
done

# create a default result file (ignoring AVGWHITE result)
echo ${nref_SDSS}  SDSS  >  ${TEMPDIR}/PHOTCAT_calibration_selector_1.table_$$
echo ${nref_UCAC4} UCAC4 >> ${TEMPDIR}/PHOTCAT_calibration_selector_1.table_$$
echo ${nref_NOMAD} NOMAD >> ${TEMPDIR}/PHOTCAT_calibration_selector_1.table_$$

${P_SORT} -r -n -k 1 ${TEMPDIR}/PHOTCAT_calibration_selector_1.table_$$ > \
    ${TEMPDIR}/PHOTCAT_calibration_selector.table_$$

${P_GAWK} '(NR==1) {print $2}' ${TEMPDIR}/PHOTCAT_calibration_selector.table_$$ > ${CALIBDIR}/PHOTCAT_bestcat
