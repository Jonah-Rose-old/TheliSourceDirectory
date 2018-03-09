BASHPATH -xv

# the script runs analyseldac over the object catalogs

# $1: main directory
# $2: instrument
# $3: title
# $4: colorrange
# $5: units
# $6: scalemin
# $7: scalemax
# $8: scaling
# $9: smoothing
# $10: contour
# $11: grid ("grid" or "nogrid")
# $12: PSF ("data" or "polynomial")
# $13: chips to be processed

title=$3
colorrange=$4
units=$5
scalemin=$6
scalemax=$7
scaling=$8
smoothing=$9
contour=${10}
grid=${11}
psf=${12}

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/$2.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/$2.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/$2.ini

. bash.include

if [ ! -d /$1/imalyzer/cat/ ]; then
    echo "exit: directory with ksb catalogs not found"
    exit
fi

DIR=`pwd`

cd /$1/

# all images matching the filter
ls -1 /$1/${THELIFILTER} > ${TEMPDIR}/psfimages_step1_$$

test ! -d /$1/imalyzer/skycat && mkdir /$1/imalyzer/skycat 
test ! -d /$1/imalyzer/ds9cat && mkdir /$1/imalyzer/ds9cat

for CHIP in ${!#}
do

  # select matching chips only
  ${P_GAWK} 'BEGIN {FS="_"} {a="_"$NF; if (a~/_'${CHIP}'OFC[BCDHP]*\.fits/) print}' \
      ${TEMPDIR}/psfimages_step1_$$ > ${TEMPDIR}/psfimages_${CHIP}_$$

  # continue with next chip if there is nothing to do
  numimages=`wc ${TEMPDIR}/psfimages_${CHIP}_$$ | ${P_GAWK} '{print $1}'`
  if [ ${numimages} = 0 ]; then
      continue
  fi

  test -f ${TEMPDIR}/psfcats_${CHIP}_$$ && rm ${TEMPDIR}/psfcats_${CHIP}_$$

  while read file
  do
    BASE=`basename ${file} .fits`
    echo ${BASE}"_stars.asc" >> ${TEMPDIR}/psfcats_${CHIP}_$$
  done < ${TEMPDIR}/psfimages_${CHIP}_$$
    
  while read file
  do
    BASE=`basename ${file} _stars.asc`

    if [ ! -f /$1/${BASE}.fits ]; then
	continue
    fi

    # image geometry
    # calculate the pixel scale from the fits header
    # (the user might have selected an image from a different instrument
    # than the one currently loaded in THELI)
    nax1=`get_key /$1/${BASE}.fits NAXIS1`
    nax2=`get_key /$1/${BASE}.fits NAXIS2`
    cd11=`get_key /$1/${BASE}.fits CD1_1`
    cd12=`get_key /$1/${BASE}.fits CD1_2`
    pixelscale=`echo ${cd11} ${cd12} | ${P_GAWK} '{print sqrt($1*$1+$2*$2)*3600.}'`

    # filter the catalog
    ${P_LDACFILTER} \
	-i /$1/imalyzer/cat/${BASE}_stars.cat \
        -o /${TEMPDIR}/${BASE}_starsf.cat \
	-t OBJECTS \
	-c "(numax>70);"

    if [ "${psf}" = "data" ]; then 
	${P_LDACTOASC} \
	    -i /${TEMPDIR}/${BASE}_starsf.cat \
	    -t OBJECTS \
	    -k Xpos Ypos FWHM_IMAGE e1 e2 -b > \
	    /${TEMPDIR}/${BASE}_imalyzer.cat
    else
	${P_LDACTOASC} \
	    -i /${TEMPDIR}/${BASE}_starsf.cat \
	    -t OBJECTS \
	    -k Xpos Ypos FWHM_IMAGE e1corrpol e2corrpol -b > \
	    /${TEMPDIR}/${BASE}_imalyzer.cat
    fi

    # number of stars
    nstars=`wc /${TEMPDIR}/${BASE}_imalyzer.cat | ${P_GAWK} '{print $1}'`

    # smooth the data field, reject outliers
    if [ "${grid}" = "grid" ]; then
	${P_E2ELL} \
	    -i /${TEMPDIR}/${BASE}_imalyzer.cat \
	    -o /${TEMPDIR}/${BASE}_imalyzer_python.cat
#	${P_IMALYZER} \
#	    -i /${TEMPDIR}/${BASE}_imalyzer.cat \
#	    -o /${TEMPDIR}/${BASE}_imalyzer_python.cat \
#	    -x ${nax1} ${nax2}

        # user has (possibly) no write permission in the scriptdir
	cd /$1/imalyzer/
	${DIR}/imalyzer.py /${TEMPDIR}/${BASE}_imalyzer_python.cat \
	    ${BASE} ${pixelscale} ${nax1} ${nax2} \
	    ${colorrange} ${units} ${scalemin} \
	    ${scalemax} ${scaling} ${nstars} ${smoothing} ${contour}
    else
	cd /$1/imalyzer/
	${DIR}/imalyzer.py /${TEMPDIR}/${BASE}_imalyzer.cat \
	    ${BASE} ${pixelscale} ${nax1} ${nax2} \
	    ${colorrange} ${units} ${scalemin} \
	    ${scalemax} ${scaling} ${nstars} ${smoothing} ${contour}
    fi

    # make a skycat catalogue
    ${P_LDACTOSKYCAT} \
	-i /${TEMPDIR}/${BASE}_starsf.cat -t OBJECTS \
	-k SeqNr ALPHA_J2000 DELTA_J2000 MAG_AUTO \
	-l id_col SeqNr ra_col ALPHA_J2000 dec_col DELTA_J2000 mag_col MAG_AUTO \
	-s > /$1/imalyzer/skycat/${BASE}_stars.skycat

    # make a ds9 catalogue
    ${P_LDACTOASC} -i /${TEMPDIR}/${BASE}_starsf.cat -t OBJECTS \
	-k ALPHA_J2000 DELTA_J2000 -b > ${TEMPDIR}/ds9_tmp_$$
    echo global color=red select=0 highlite=1 wcs=wcs > \
	/$1/imalyzer/ds9cat/${BASE}_stars.reg
    ${P_GAWK} '{print "j2000;circle", $1, $2, "5i"}' ${TEMPDIR}/ds9_tmp_$$ >> \
	/$1/imalyzer/ds9cat/${BASE}_stars.reg

    rm ${TEMPDIR}/${BASE}_starsf.cat ${BASE}_imalyzer.cat ${BASE}_imalyzer_python.cat

  done < ${TEMPDIR}/psfcats_${CHIP}_$$
  
  rm ${TEMPDIR}/psfcats_${CHIP}_$$
done

cd ${DIR}
