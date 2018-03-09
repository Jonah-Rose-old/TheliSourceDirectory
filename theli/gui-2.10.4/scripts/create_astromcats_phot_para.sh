#!/bin/bash -xv

# the scripts creates object catalogs needed for astrometry.

#$1: main directory
#$2: science dir.
#$3: image extension (ext) on ..._iext.fits (i is the chip number)
#$4: chips to work on

SCRIPTDIR=`pwd`

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

mkdir -p /$1/$2/cat
mkdir -p /$1/$2/cat/skycat
mkdir -p /$1/$2/cat/ds9cat
mkdir -p /$1/$2/cat/lownum

. bash.include

SEXBACKSTRING=""
if [ "${V_SEXBACKLEVEL}_A" != ""_A ]; then
    SEXBACKSTRING="-BACK_TYPE MANUAL -BACK_VALUE ${V_SEXBACKLEVEL}"
fi

for CHIP in $4
do
  # do we have images in the 'lownum' directory from a previous run?
  # if yes, move them back
  if [ -d /$1/$2/lownum ]; then
      cd /$1/$2/lownum
      mv *${CHIP}$3.fits /$1/$2/
      cd ${SCRIPTDIR}
  fi

  NUMTHREAD=`${SCRIPTDIR}/get_nthreads.sh ${NCHIPS} ${NPARA} ${CHIP}`

  ls -1 /$1/$2/*_${CHIP}$3.fits > ${TEMPDIR}/astromimages_$$
  
  cat ${TEMPDIR}/astromimages_$$ |\
  {
    while read file
    do
      BASE=`basename ${file} .fits`
      WBASE=`basename ${file} $3.fits`

      FSTRING=""
      WSTRING=""

      SEXPARAMFILE=${CONF}/singleastrom.param.sex

      flagpresent="no"
      if [ -f /$1/WEIGHTS/${WBASE}$3.flag.fits ]; then
	  flagpresent="yes"
	  FSTRING="-FLAG_IMAGE /$1/WEIGHTS/${WBASE}$3.flag.fits -FLAG_TYPE MAX"
	  cp ${SEXPARAMFILE} ${TEMPDIR}/singleastrom.param.sex_$$
	  {
	      echo "IMAFLAGS_ISO"
	      echo "NIMAFLAGS_ISO"
	  } >> ${TEMPDIR}/singleastrom.param.sex_$$
	  SEXPARAMFILE=${TEMPDIR}/singleastrom.param.sex_$$
      fi
      if [ -f /$1/WEIGHTS/${WBASE}$3.weight.fits ]; then
	  WSTRING="-WEIGHT_IMAGE /$1/WEIGHTS/${WBASE}$3.weight.fits -WEIGHT_TYPE MAP_WEIGHT"
      fi

      saturation=""
      if [ "${V_SATURATION}"_A != "_A" ]; then
	  saturation="-SATUR_LEVEL ${V_SATURATION}"
      fi

      # extract objects
      ${P_SEX} ${file} -c ${DATACONF}/singleastrom.conf.sex \
	  -CATALOG_NAME /$1/$2/cat/${BASE}.cat0 \
	  -DETECT_MINAREA ${V_AP_DETECTMINAREA} \
	  -DETECT_THRESH ${V_AP_DETECTTHRESH} \
	  -DEBLEND_MINCONT ${V_DEBLENDMINCONT} \
	  -ANALYSIS_THRESH ${V_AP_DETECTTHRESH} \
	  -PARAMETERS_NAME ${SEXPARAMFILE} \
	  -PHOT_APERTURES ${V_ABSPHOT_APERTURE} \
	  -FILTER Y ${WSTRING} ${FSTRING} ${SEXBACKSTRING} \
	  -NTHREADS ${NUMTHREAD} ${saturation}

      CD11=`get_key ${file} CD1_1`
      CD21=`get_key ${file} CD2_1`
      PSCALE=`${P_GETPIXSCALE} -c ${CD11} ${CD21}`

      # check if the catalog contains any sources at all
      numobj=`${P_DFITS} -x 2 /$1/$2/cat/${BASE}.cat0 | ${P_FITSORT} NAXIS2 | \
	  ${P_GAWK} '(NR==2) {print $2}'`
      if [ "${numobj}" = "0" ]; then
	  rm /$1/$2/cat/${BASE}.cat0
	  continue
      fi

      # add IMAFLAGS_ISO and NIMAFLAGS_ISO should they be not present due to
      # missing flag images (needed for compatibility reasons)
      if [ "${flagpresent}" = "no" ]; then
	  ${P_LDACADDKEY} -i /$1/$2/cat/${BASE}.cat0 \
	      -o /$1/$2/cat/${BASE}.cat01 \
	      -t LDAC_OBJECTS -k IMAFLAGS_ISO 0 LONG ""
	  ${P_LDACADDKEY} -i /$1/$2/cat/${BASE}.cat01 \
	      -o /$1/$2/cat/${BASE}.cat02 \
	      -t LDAC_OBJECTS -k NIMAFLAGS_ISO 0 LONG ""
	  rm /$1/$2/cat/${BASE}.cat01
	  mv /$1/$2/cat/${BASE}.cat02 /$1/$2/cat/${BASE}.cat0
      fi

      # remove bad stuff
      minfluxrad=`echo ${V_SEXCATMINFWHM} | ${P_GAWK} '{print 0.5*$1}'`
      ${P_LDACFILTER} -i /$1/$2/cat/${BASE}.cat0 \
	  -o /$1/$2/cat/${BASE}.cat1 \
	  -t LDAC_OBJECTS -c "(((FLAGS<=${V_SEXCATFLAG})AND(FWHM_IMAGE>${V_SEXCATMINFWHM}))AND(FLUX_RADIUS>${minfluxrad}));" \
	  2> ${TEMPDIR}/ldacfilter_err.log_$$

      # Check if anything is left after filtering
      nooutput=`${P_GAWK} '{if ($0 ~/No output lines/) print "yes"}'  ${TEMPDIR}/ldacfilter_err.log_$$`
      if [ "${nooutput}"_A = "yes"_A ]; then
	  rm /$1/$2/cat/${BASE}.cat0
	  continue
      fi

      if [ "${V_AP_FILTER}" = "Y" ]; then
	  if [ "${FSTRING}"_A != "_A" ]; then
	      ${P_LDACFILTER} -i /$1/$2/cat/${BASE}.cat1 \
		  -o /$1/$2/cat/${BASE}.cat11 \
		  -t LDAC_OBJECTS -c "(((NIMAFLAGS_ISO<=2)AND(IMAFLAGS_ISO<=20))OR(ISOAREA_IMAGE>20));"
	      ${P_LDACFILTER} -i /$1/$2/cat/${BASE}.cat11 \
		  -o /$1/$2/cat/${BASE}.cat1 \
		  -t LDAC_OBJECTS -c "((ISOAREA_IMAGE>5)AND(B_IMAGE>1.0));"
	      rm /$1/$2/cat/${BASE}.cat11
	  else
	      ${P_LDACFILTER} -i /$1/$2/cat/${BASE}.cat1 \
		  -o /$1/$2/cat/${BASE}.cat2 \
		  -t LDAC_OBJECTS -c "((ISOAREA_IMAGE>5)AND(B_IMAGE>1.0));"
	      mv /$1/$2/cat/${BASE}.cat2 /$1/$2/cat/${BASE}.cat1
	  fi
      fi
      
      # convert the tables
      ${P_LDACCONV} -i /$1/$2/cat/${BASE}.cat1 \
	  -o /$1/$2/cat/${BASE}.cat2 \
          -b ${CHIP} -c ${INSTRUMENT} -f dummy 
 
      # the number of sources left in the catalog
      ntot=0
      ntot=`${P_DFITS} -x 1 /$1/$2/cat/${BASE}.cat2 | ${P_FITSORT} NAXIS2 | \
	  ${P_GAWK} '(NR==2) {print $2}'`
     
      # get the image seeing
      ${P_LDACFILTER} -i /$1/$2/cat/${BASE}.cat2 \
          -o ${TEMPDIR}/seeing_ldac_filt.cat_$$ -c "(Flag=0);"
      
      ngood=`${P_LDACTOASC} -i /$1/$2/cat/${BASE}.cat2 -t OBJECTS -k Flag -b | \
	  ${P_GAWK} '($1==0)' | wc | ${P_GAWK} '{print $1}'`

      if [ ${ngood} -gt 0 ]; then
	  ${P_LDACCALC} -i ${TEMPDIR}/seeing_ldac_filt.cat_$$ \
	      -o ${TEMPDIR}/seeing_ldac_filt.cat2_$$ \
	      -c "(1./MAGERR_AUTO);" -n "snratio" "" -k FLOAT 
	  
          # do it for FWHM_IMAGE
	  ${P_PREANISOTROPY} -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ \
	      -t OBJECTS -s 0.1 -c snratio 10.0 1000.0 -k FWHM_IMAGE MAG_AUTO >& \
	      ${TEMPDIR}/preaniso_$$
	  # expanding the limits by 0.01 to avoid possible rounding issues in ldacfilter
	  # occuring after the 4th digit
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
	     # calculate the median FWHM of all sources in the image
		  ${P_LDACTOASC} -b -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ -t OBJECTS \
		      -k FWHM_IMAGE > ${TEMPDIR}/seeing_$$.asc
	      fi
	  else
	  # calculate the median FWHM of all sources in the image
	      ${P_LDACTOASC} -b -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ -t OBJECTS \
		  -k FWHM_IMAGE > ${TEMPDIR}/seeing_$$.asc
	  fi
	  SEEING=`${P_GETSTATS} -i ${TEMPDIR}/seeing_$$.asc -m median`
	  FWHM_WORLD=`echo ${SEEING} | ${P_GAWK} '{printf("%.2f", $1*'${PSCALE}')}'`
	  FWHM_IMAGE=`echo ${SEEING} | ${P_GAWK} '{printf("%.2f", $1)}'`
	  ${P_LDACADDKEY} -i /$1/$2/cat/${BASE}.cat2 -o /$1/$2/cat/${BASE}.cat3 -t FIELDS \
	      -k FWHM_IMAGE ${FWHM_IMAGE} FLOAT "Median stellar branch FWHM_IMAGE"
	  ${P_LDACADDKEY} -i /$1/$2/cat/${BASE}.cat3 -o /$1/$2/cat/${BASE}.cat4 -t FIELDS \
	      -k FWHM_WORLD ${FWHM_WORLD} FLOAT "Median stellar branch FWHM_WORLD"
	  
          # do it for FLUX_RADIUS
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
	      # calculate the median HFD of all sources in the image
		  ${P_LDACTOASC} -b -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ -t OBJECTS \
		      -k FLUX_RADIUS > ${TEMPDIR}/seeing_$$.asc
	      fi
	  else
	  # calculate the median HFD of all sources in the image
	      ${P_LDACTOASC} -b -i ${TEMPDIR}/seeing_ldac_filt.cat2_$$ -t OBJECTS \
		  -k FLUX_RADIUS > ${TEMPDIR}/seeing_$$.asc
	  fi
	  SEEING=`${P_GETSTATS} -i ${TEMPDIR}/seeing_$$.asc -m median`
	  HFD_WORLD=`echo ${SEEING} | ${P_GAWK} '{printf("%.2f", 2.*$1*'${PSCALE}')}'`
	  HFD_IMAGE=`echo ${SEEING} | ${P_GAWK} '{printf("%.2f", 2.*$1)}'`
	  ${P_LDACADDKEY} -i /$1/$2/cat/${BASE}.cat4 -o /$1/$2/cat/${BASE}.cat5 -t FIELDS \
	      -k HFD_IMAGE ${HFD_IMAGE} FLOAT "Median HFD_IMAGE of all sources"
	  ${P_LDACADDKEY} -i /$1/$2/cat/${BASE}.cat5 -o /$1/$2/cat/${BASE}.cat -t FIELDS \
	      -k HFD_WORLD ${HFD_WORLD} FLOAT "Median HFD_WORLD of all sources"
      else
	  FWHM_WORLD=-1
	  FWHM_IMAGE=0
	  HFD_WORLD=-1
	  HFD_IMAGE=0	  
      fi

      ${P_LDACTOSKYCAT} -i /$1/$2/cat/${BASE}.cat -t OBJECTS \
	  -k SeqNr ALPHA_J2000 DELTA_J2000 MAG_AUTO \
	  -l id_col SeqNr ra_col ALPHA_J2000 dec_col \
	  DELTA_J2000 mag_col MAG_AUTO \
	  -s > /$1/$2/cat/skycat/${BASE}.skycat
      
      echo global color=green select=0 highlite=1 wcs=wcs \
	  > ${TEMPDIR}/${BASE}.ds9_head_$$

      ${P_LDACTOASC} -i /$1/$2/cat/${BASE}.cat -t OBJECTS \
	  -k ALPHA_J2000 DELTA_J2000 AWIN_WORLD BWIN_WORLD THETAWIN_J2000 -b \
	  -s > ${TEMPDIR}/${BASE}.ds9_body_$$

      ${P_GAWK} '{print "j2000;ellipse", $1, $2, $3*2., $4*2., $5}' \
	  ${TEMPDIR}/${BASE}.ds9_body_$$ > ${TEMPDIR}/${BASE}.ds9_body2_$$

      cat ${TEMPDIR}/${BASE}.ds9_head_$$ ${TEMPDIR}/${BASE}.ds9_body2_$$ > \
	  /$1/$2/cat/ds9cat/${BASE}.reg

      # if the number of objects is very small, move the catalog to the 'lownum' directory
      if [ ! -n ${V_AP_LOWNUM} ]; then
	  V_AP_LOWNUM=1
      fi
      if [ ${ntot} -lt ${V_AP_LOWNUM} ]; then
	mv /$1/$2/cat/${BASE}* /$1/$2/cat/lownum/
	mkdir -p /$1/$2/lownum
	mv /$1/$2/${BASE}*.fits /$1/$2/lownum
      fi

      \rm /$1/$2/cat/${BASE}.cat[0,2,3,4,5]
    done
  }
done

rm ${TEMPDIR}/seeing_ldac_filt.cat*_$$
rm ${TEMPDIR}/${BASE}.ds9*_$$

exit 0;
