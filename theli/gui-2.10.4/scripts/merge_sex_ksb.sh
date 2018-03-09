#!/bin/bash

# the script merges info from the singleastrom
# and the check_science_PSF step. It overtakes
# e1, e2 from the KSB cats to the SExtractor cats
# giving the "new" catalogs the same .cat as the old
# ones. Hence these catalogs can be used in the
# further processing having KSB info. Also this step
# is not absolutely necssary in the processing and
# can be omitted if this PSF information is not 
# important.
#
# 30.05.04:
# tempaorary files go to a TEMPDIR directory 

#$1: main dir.
#$2: science dir.
#$3: image extension (ext) on ..._iext.fits (i is the chip number)

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

# first make a security copy if it does not yet exist
if [ ! -d /$1/$2/cat/copy ]; then
  mkdir /$1/$2/cat/copy
  cp /$1/$2/cat/*$3.cat /$1/$2/cat/copy
fi

# now overtake KSB info
ls -1 /$1/$2/cat/copy/*$3.cat > ${TEMPDIR}/mergecats

# test if all KSB cats are there; if not
# do nothing !!!

if [ -f ${TEMPDIR}/fail ]; then
  rm ${TEMPDIR}/fail
fi

cat ${TEMPDIR}/mergecats |\
while read CAT
do
  BASE=`basename ${CAT} .cat`
  if [ ! -f "/$1/$2/cat/${BASE}_ksb_tmp.cat2" ]; then
    echo "/$1/$2/cat/${BASE}_ksb_tmp.cat2 is not there" >> fail
  fi
done  

if [ -f ${TEMPDIR}/fail ]; then
  echo "script stopped due to missing KSB files"
  exit 1
fi

cat ${TEMPDIR}/mergecats |\
{
  while read CAT
  do
    BASE=`basename ${CAT} .cat`
    ${P_LDACRENTAB} -i /$1/$2/cat/${BASE}_ksb_tmp.cat2 \
		     -o ${TEMPDIR}/tmp.cat -t OBJECTS LDAC_OBJECTS
    ${P_LDACFILTER} -i ${CAT} -o ${TEMPDIR}/tmp1.cat -t LDAC_OBJECTS\
		     -c "(FLUX_RADIUS>0.0)AND(FLUX_RADIUS<10.0);"
    ${P_LDACJOINKEY} -i ${TEMPDIR}/tmp1.cat -o /$1/$2/cat/${BASE}.cat \
		      -p ${TEMPDIR}/tmp.cat \
		      -t LDAC_OBJECTS -k e1 e2 rh cl   
    rm ${TEMPDIR}/tmp.cat ${TEMPDIR}/tmp1.cat		     
  done
}
