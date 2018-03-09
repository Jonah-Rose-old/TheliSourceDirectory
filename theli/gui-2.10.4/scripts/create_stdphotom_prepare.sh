#!/bin/bash -uxv

# create catalogues suitable for absolute photometric calibration
# with 'create_abs_photo_info.sh'. The script takes astrometric
# information from a scamp processing and calculates 'correct' sky
# coordinates from objects catalogues (extracted from standard star
# fields). Afterwards the standard star object catalogues are
# merged with a reference standard star sources (e.g. from Landolt,
# Stetson, Sloan ....)

#$1: main directory
#$2: science dir. (the catalogues are assumed to be in $1/$2/cat)
#$3: image extension
#$4: chips

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

DIR=`pwd`

PHOTCAT=""
if [ "${V_ABSPHOT_STDCAT_INDIRECT}" = "LANDOLT_UBVRI" ]; then
    PHOTCAT=${DIR}"/../photstdcats/LANDOLT.cat"
    MAKESSCCONF="stdphotom_prepare_make_ssc_UBVRI.conf"
elif [ "${V_ABSPHOT_STDCAT_INDIRECT}" = "STETSON_UBVRI" ]; then
    PHOTCAT=${DIR}"/../photstdcats/STETSON.cat"
    MAKESSCCONF="stdphotom_prepare_make_ssc_UBVRI.conf"
elif [ "${V_ABSPHOT_STDCAT_INDIRECT}" = "STRIPE82_ugriz" ]; then
    PHOTCAT=${DIR}"/../photstdcats/STRIPE82.cat"
    MAKESSCCONF="stdphotom_prepare_make_ssc_ugriz.conf"
elif [ "${V_ABSPHOT_STDCAT_INDIRECT}" = "STRIPE82_ugriz_primed" ]; then
    PHOTCAT=${DIR}"/../photstdcats/STRIPE82_primed.cat"
    MAKESSCCONF="stdphotom_prepare_make_ssc_ugriz.conf"
elif [ "${V_ABSPHOT_STDCAT_INDIRECT}" = "SMITH_ugriz_primed" ]; then
    PHOTCAT=${DIR}"/../photstdcats/SMITH.cat"
    MAKESSCCONF="stdphotom_prepare_make_ssc_ugriz.conf"
elif [ "${V_ABSPHOT_STDCAT_INDIRECT}" = "MKO_JHK" ]; then
    PHOTCAT=${DIR}"/../photstdcats/MKO.cat"
    MAKESSCCONF="stdphotom_prepare_make_ssc_JHK.conf"
elif [ "${V_ABSPHOT_STDCAT_INDIRECT}" = "MKO_LM" ]; then
    PHOTCAT=${DIR}"/../photstdcats/MKO_LM.cat"
    MAKESSCCONF="stdphotom_prepare_make_ssc_LM.conf"
elif [ "${V_ABSPHOT_STDCAT_INDIRECT}" = "HUNT_JHK" ]; then
    PHOTCAT=${DIR}"/../photstdcats/HUNT.cat"
    MAKESSCCONF="stdphotom_prepare_make_ssc_JHK.conf"
elif [ "${V_ABSPHOT_STDCAT_INDIRECT}" = "2MASSfaint_JHK" ]; then
    PHOTCAT=${DIR}"/../photstdcats/2MASSfaint.cat"
    MAKESSCCONF="stdphotom_prepare_make_ssc_JHK.conf"
elif [ "${V_ABSPHOT_STDCAT_INDIRECT}" = "PERSSON_JHKKs" ]; then
    PHOTCAT=${DIR}"/../photstdcats/PERSSON.cat"
    MAKESSCCONF="stdphotom_prepare_make_ssc_JHKKs.conf"
elif [ "${V_ABSPHOT_STDCAT_INDIRECT}" = "JAC_YJHKLM" ]; then
    PHOTCAT=${DIR}"/../photstdcats/JAC.cat"
    MAKESSCCONF="stdphotom_prepare_make_ssc_YJHKLM.conf"
elif [ "${V_ABSPHOT_STDCAT_INDIRECT}" = "PERSSON_RED_JHKKs" ]; then
    PHOTCAT=${DIR}"/../photstdcats/PERSSON_RED.cat"
    MAKESSCCONF="stdphotom_prepare_make_ssc_JHKKs.conf"
elif [ "${V_ABSPHOT_STDCAT_INDIRECT}" = "WASHINGTON" ]; then
    PHOTCAT=${DIR}"/../photstdcats/WASHINGTON.cat"
    MAKESSCCONF="stdphotom_prepare_make_ssc_CMT1T2.conf"
else
    echo `${P_ERRTEST} 0` Invalid standard star catalog: ${PHOTCAT}
    exit
fi

# The PHOTCAT catalog has to be adjusted to accomodate for the user defined WCS uncertainty 
# (in case no WCS solution is made); I could also do this in the object catalog, but I find it
# easier to do it here (as we have to do it only once as compared to n times for n exposures)

if [ "${V_ABSPHOT_WCSERRCHECKBOX}" = "N" ]; then
    wcsuncertainty=`echo ${V_ABSPHOT_WCSERR} | ${P_GAWK} '{print $1/3600.}'`
    ${P_LDACDELKEY} -i ${PHOTCAT} -t STDTAB -k A_WCS B_WCS -o ${TEMPDIR}/stdphotcat_tmp0.cat_$$
    ${P_LDACADDKEY} -i ${TEMPDIR}/stdphotcat_tmp0.cat_$$ -o ${TEMPDIR}/stdphotcat_tmp1.cat_$$ \
	-t STDTAB -k A_WCS ${wcsuncertainty} Float ""
    ${P_LDACADDKEY} -i ${TEMPDIR}/stdphotcat_tmp1.cat_$$ -o ${TEMPDIR}/stdphotcat_tmp2.cat_$$ \
	-t STDTAB -k B_WCS ${wcsuncertainty} Float ""
    mv ${TEMPDIR}/stdphotcat_tmp2.cat_$$ ${TEMPDIR}/stdphotcat_tmp.cat_$$
else
    cp ${PHOTCAT} ${TEMPDIR}/stdphotcat_tmp.cat_$$
fi
PHOTCAT=${TEMPDIR}/stdphotcat_tmp.cat_$$

nobj_total=0

for CHIP in ${!#}
do
  CATS=`${P_FIND} ${MD}/${SD}/cat -maxdepth 1 -name \*_${CHIP}${EXTENSION}.cat`

  nobj_chip=0

  for CAT in ${CATS}
  do
    BASE=`basename ${CAT} .cat`
    HEADBASE=`basename ${CAT} ${EXTENSION}.cat`

    # get 'correct' astrometric scamp information to the catalogues:    

    # Astrometry SHOULD be done
    if [ "${V_ABSPHOT_WCSERRCHECKBOX}" = "Y" ]; then
	${S_APLASTROMSCAMP} -i ${MD}/${SD}/cat/${BASE}.cat \
            -o ${MD}/${SD}/cat/${BASE}_corr.cat \
            -s ${MD}/${SD}/headers/${HEADBASE}.head \
            -t OBJECTS -p XWIN_IMAGE YWIN_IMAGE -r Ra Dec
	
	${P_MAKEJOIN} -i /${MD}/${SD}/cat/${BASE}_corr.cat \
            -o ${TEMPDIR}/tmp.cat0_$$ \
	    -c ${DATACONF}/stdphotom_prepare_make_join.conf

        # calculate object magnitudes with a magzeropoint of 0 and
        # an exposure time normalisation of 1s (1.08574 is 2.5 / log(10)):
	${P_LDACCALC} -i ${TEMPDIR}/tmp.cat0_$$ \
            -o ${TEMPDIR}/tmp.cat00_$$ \
            -t OBJECTS -c "(-1.08574*log(FLUX_APER/EXPTIME));" \
            -n MAG_corr "exposure time corr. MAG_APER" -k FLOAT

	${P_LDACCALC} -i ${TEMPDIR}/tmp.cat00_$$ \
            -o ${TEMPDIR}/tmp.cat01_$$ \
            -t OBJECTS -c "(1.08574*log(1.+FLUXERR_APER/FLUX_APER));" \
            -n MAGERR_corr "exposure time corr. MAGERR_APER" -k FLOAT

	${P_LDACRENTAB} -i ${TEMPDIR}/tmp.cat01_$$ \
            -o ${TEMPDIR}/tmp.cat1_$$ -t OBJECTS STDTAB

	${P_LDACRENKEY} -i ${TEMPDIR}/tmp.cat1_$$ -o ${TEMPDIR}/tmp.cat2_$$ \
            -t STDTAB -k AWIN_WORLD A_WCS BWIN_WORLD B_WCS \
            THETAWIN_J2000 THETAWCS

	${P_ASSOCIATE} -i ${TEMPDIR}/tmp.cat2_$$ ${PHOTCAT} \
	    -o ${TEMPDIR}/obj_tmp.cat3_$$ ${TEMPDIR}/ref_tmp.cat4_$$ \
            -t STDTAB -c ${DATACONF}/stdphotom_prepare_associate.conf

    # Astrometry SHOULD NOT be done
    else
	${P_MAKEJOIN} -i ${MD}/${SD}/cat/${BASE}.cat \
            -o ${TEMPDIR}/tmp.cat0_$$ \
	    -c ${DATACONF}/stdphotom_prepare_make_join.conf

        # calculate object magnitudes with a magzeropoint of 0 and
        # an exposure time normalisation of 1s (1.08574 is 2.5 / log(10)):
	${P_LDACCALC} -i ${TEMPDIR}/tmp.cat0_$$ \
            -o ${TEMPDIR}/tmp.cat00_$$ \
            -t OBJECTS -c "(-1.08574*log(FLUX_APER/EXPTIME));" \
            -n MAG_corr "exposure time corr. MAG_APER" -k FLOAT

	${P_LDACCALC} -i ${TEMPDIR}/tmp.cat00_$$ \
            -o ${TEMPDIR}/tmp.cat01_$$ \
            -t OBJECTS -c "(1.08574*log(1.+FLUXERR_APER/FLUX_APER));" \
            -n MAGERR_corr "exposure time corr. MAGERR_APER" -k FLOAT

	${P_LDACRENTAB} -i ${TEMPDIR}/tmp.cat01_$$ \
            -o ${TEMPDIR}/tmp.cat1_$$ -t OBJECTS STDTAB

	${P_LDACRENKEY} -i ${TEMPDIR}/tmp.cat1_$$ -o ${TEMPDIR}/tmp.cat2_$$ \
            -t STDTAB -k AWIN_WORLD A_WCS BWIN_WORLD B_WCS \
            THETAWIN_J2000 THETAWCS ALPHA_J2000 Ra DELTA_J2000 Dec

	${P_ASSOCIATE} -i ${TEMPDIR}/tmp.cat2_$$ ${PHOTCAT} \
	    -o ${TEMPDIR}/obj_tmp.cat3_$$ ${TEMPDIR}/ref_tmp.cat4_$$ \
            -t STDTAB -c ${DATACONF}/stdphotom_prepare_associate.conf
#	    -INTER_COLOR_TOL ${V_ABSPHOT_WCSERR} \
#	    -ISO_COLOR_TOL ${V_ABSPHOT_WCSERR}
    fi

    # if no objects found, continue with next catalog
    nobj=`${P_LDACTOASC} -i ${TEMPDIR}/obj_tmp.cat3_$$ -t STDTAB -k Pair_1 -b | \
	${P_GAWK} 'BEGIN {s=0} {if ($1>0) s++} END {print s}'`
    if [ "${nobj}" = "0" ]; then
	continue
    fi

    ${P_LDACFILTER} -i ${TEMPDIR}/obj_tmp.cat3_$$ -o ${TEMPDIR}/obj_tmp.cat5_$$ \
                    -c "(Pair_1>0);" -t STDTAB

    if [ "$?" -eq "0" ]; then
        # if no objects found, continue with next catalog
	nobj=`${P_LDACTOASC} -i ${TEMPDIR}/ref_tmp.cat4_$$ -t STDTAB -k Pair_0 -b | \
	    ${P_GAWK} 'BEGIN {s=0} {if ($1>0) s++} END {print s}'`
	if [ "${nobj}" = "0" ]; then
	    continue
	fi
	${P_LDACFILTER} -i ${TEMPDIR}/ref_tmp.cat4_$$ \
            -o ${TEMPDIR}/ref_tmp.cat6_$$ -c "(Pair_0>0);" -t STDTAB
	${P_ASSOCIATE} -i ${TEMPDIR}/obj_tmp.cat5_$$ ${TEMPDIR}/ref_tmp.cat6_$$ \
            -o ${TEMPDIR}/obj_tmp.cat7_$$ ${TEMPDIR}/ref_tmp.cat8_$$ \
      	    -t STDTAB -c ${DATACONF}/stdphotom_prepare_associate.conf
	${P_MAKESSC} -i ${TEMPDIR}/obj_tmp.cat7_$$ ${TEMPDIR}/ref_tmp.cat8_$$ \
            -o /${MD}/${SD}/cat/${BASE}_merg.cat \
            -t STDTAB \
            -c ${DATACONF}/${MAKESSCCONF}

	nobj_total=$((${nobj_total} + ${nobj}))
	nobj_chip=$((${nobj_chip} + ${nobj}))
    fi
  done
  ${P_LDACPASTE} -i /${MD}/${SD}/cat/*_${CHIP}${EXTENSION}_merg.cat \
                 -o /${MD}/${SD}/cat/chip_${CHIP}_merg.cat -t PSSC

  if [ "${nobj_chip}" = "0" ] && [ ${NCHIPS} -gt 1 ]; then
      echo "WARNING: Chip ${CHIP} does not overlap with sources from ${V_ABSPHOT_STDCAT_INDIRECT}."
  fi

done

# if no overlapping sources were found:

if [ "${nobj_total}" = "0" ] && [ ${NCHIPS} == 1 ]; then
    echo `${P_ERRTEST} 0` Your standard star field does not overlap with ${V_ABSPHOT_STDCAT_INDIRECT}, or the standard stars could not be identified/matched.
    exit 1;
fi

# clean up
rm  ${TEMPDIR}/*_$$
