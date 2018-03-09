#!/bin/bash -uxv

# corrects the coordinates in the object catalogs according
# to the scamp solution

#$1: main directory
#$2: science dir. (the catalogues are assumed to be in $1/$2/cat)
#$3: image extension

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

PHOTCAT=~/.theli/scripts/theli_PIC.cat

DIR=`pwd`

#################################################################
#
# correct the sky coordinates in the object catalogs
#
#################################################################

for CHIP in ${!#}
do
  IMAGES=`find ${MD}/${SD}/ -maxdepth 1 -name \*_${CHIP}${EXTENSION}.fits | ${P_SORT}`

  for IMAGE in ${IMAGES}
  do
    BASE=`basename ${IMAGE} .fits`
    HEADBASE=`basename ${IMAGE} ${EXTENSION}.fits`

    # get 'correct' astrometric scamp information to the catalogues:    
    ${S_APLASTROMSCAMP} -i ${MD}/${SD}/cat/${BASE}.cat \
        -o ${TEMPDIR}/${BASE}_tmp.cat0_$$ \
        -s ${MD}/${SD}/headers/${HEADBASE}.head \
        -t OBJECTS -p Xpos Ypos -r Ra Dec

    ${P_MAKEJOIN} -i ${TEMPDIR}/${BASE}_tmp.cat0_$$ \
                  -o ${TEMPDIR}/${BASE}_tmp.cat1_$$ \
		  -c ${DATACONF}/stdphotom_prepare_make_join.conf
    # calculate object magnitudes with a magzeropoint of 0 and
    # an exposure time normalisation of 1s (1.08574 is 2.5 / log(10)):
    ${P_LDACCALC} -i ${TEMPDIR}/${BASE}_tmp.cat1_$$ \
                  -o ${MD}/${SD}/cat/${BASE}_corr.cat \
                  -t OBJECTS -c "(-1.08574*log(FLUX_AUTO/EXPTIME));" \
                  -n MAG_corr "exposure time corr. MAG_AUTO" -k FLOAT
    ${P_ASSOCIATE} -i ${MD}/${SD}/cat/${BASE}_corr.cat ${PHOTCAT} \
	-o ${TEMPDIR}/obj_assoc.cat_$$ ${TEMPDIR}/ref_assoc.cat_$$ \
        -t OBJECTS -c ${DATACONF}/PIC_associate.conf
    ${P_MAKESSC} -i ${TEMPDIR}/obj_assoc.cat_$$ ${TEMPDIR}/ref_assoc.cat_$$ \
        -o ${TEMPDIR}/${BASE}_PIC_merged.cat_$$ \
        -t OBJECTS -c ${DATACONF}/PIC_make_ssc.conf

    # make ascii catalog to check if anything would be left. if yes, do ldacfilter

    num=1
    ${P_LDACTOASC} -i ${TEMPDIR}/${BASE}_PIC_merged.cat_$$ -t PSSC \
	-b -k Mag_REF MagErr_OBJ Flag > ${TEMPDIR}/${BASE}_testcat0_$$
    num=`${P_GAWK} '($1>1 && $2<'${V_ABSPHOT_MAXPHOTERR}' && $2>0 && $3==0)' ${TEMPDIR}/${BASE}_testcat0_$$ |\
             ${P_GAWK} 'END {print NR}'`
    
    if [ "${num}" -gt 0 ]; then
	${P_LDACFILTER} -i ${TEMPDIR}/${BASE}_PIC_merged.cat_$$ -t PSSC \
            -o /${MD}/${SD}/cat/${BASE}_merg.cat \
	    -c "((((Mag_REF>0)AND(MagErr_OBJ<${V_ABSPHOT_MAXPHOTERR}))AND(MagErr_OBJ>0))AND(Flag=0));"
    fi
  done
  ${P_LDACPASTE} -i /${MD}/${SD}/cat/*_${CHIP}${EXTENSION}_merg.cat \
      -o /${MD}/${SD}/cat/chip_${CHIP}_merg.cat -t PSSC
done

# clean up
\rm ${TEMPDIR}/*_$$
