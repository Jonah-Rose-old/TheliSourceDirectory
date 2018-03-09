#!/bin/bash -xv

# $1: main dir.
# $2: science dir
# $3: extension
# $4: chip

DIR=`pwd`

INSTDIR=instruments_professional
PREFIXDIR=${DIR}
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
    PREFIXDIR=${DIR}
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
    PREFIXDIR=""
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

USERDIR=~/.theli/instruments_user/

cd /$1/$2/

########################################################
# Use only those for which images are currently 
# present in the SCIENCE directory
########################################################

rm /$1/$2/astrom_photom_anet/cat/*
rm /$1/$2/astrom_photom_anet/headers/*

if [ "${NCHIPS}" -gt "1" ]; then
    ls -1 /$1/$2/cat/*_scamp.cat > ${TEMPDIR}/scampcatalogs_$$
    exec < ${TEMPDIR}/scampcatalogs_$$
    while read file
    do
	BASE=`basename $file _scamp.cat`
	images=`${P_FIND} /$1/$2/ -maxdepth 1 -name ${BASE}_\*$3.fits`
	if [ -n "${images}" ]; then
	    catname=`basename $file`
	    ln -s ${file} /$1/$2/astrom_photom_anet/cat/${catname}
	fi
    done
else
    ls -1 /$1/$2/cat/*$3.cat1 > ${TEMPDIR}/scampcatalogs_$$
    exec < ${TEMPDIR}/scampcatalogs_$$
    while read file
    do
	BASE=`basename $file _1$3.cat1`
	images=`${P_FIND} /$1/$2/ -maxdepth 1 -name ${BASE}_\*$3.fits`
	if [ -n "${images}" ]; then
	    ln -s ${file} /$1/$2/astrom_photom_anet/cat/${BASE}_scamp.cat
	fi
    done
fi
 
# a list of images
ls /$1/$2/headers_anet/*_1.head > ${TEMPDIR}/anet.list_$$

exec < ${TEMPDIR}/anet.list_$$
while read file
do
    base=`basename ${file} _1.head`
    rm /$1/$2/astrom_photom_anet/cat/${base}_scamp.head

    # concatenate the astrometry.net headers to correct the scamp catalogs
    i=1
    while [ ${i} -le ${NCHIPS} ]
    do
	cat /$1/$2/headers_anet/${base}_${i}.head >> \
	    /$1/$2/astrom_photom_anet/cat/${base}_scamp.ahead
	i=$((${i}+1))
    done
done

# now run scamp in photometry mode only

cd /$1/$2/astrom_photom_anet/cat

# check if any catalogs are present
ncat=`ls | grep scamp.cat | wc | ${P_GAWK} '{print $1}'`
if [ "${ncat}" = "0" ]; then
    ${P_ERRTEST} 0
    echo "No scamp catalogs found! Did you create the source catalogs?"
    exit
fi
# check if any headers are present
nhead=`ls | grep scamp.ahead | wc | ${P_GAWK} '{print $1}'`
if [ "${nhead}" = "0" ]; then
    ${P_ERRTEST} 0
    echo "No astrometry.net headers found! Did you run astrometry.net successfully?"
    exit
fi
# check if the number of catalogs and headers match
if [ "${ncat}" -ne "${nhead}" ]; then
    ${P_ERRTEST} 0
    echo "Not all catalogs have headers or vice versa!"
    exit
fi

${P_FIND} . -maxdepth 1 -name \*_scamp.cat > ${TEMPDIR}/scamp_cats_$$

if [ "${V_ANET_SCAMPDISTORT}" = "Y" ]; then
    scampdistort="-ASTRINSTRU_KEY ${V_SCAMP_ASTRINSTRUKEY} -SOLVE_ASTROM Y -STABILITY_TYPE ${V_SCAMP_STABILITY} -DISTORT_DEGREES ${V_SCAMP_POLYORDER}"
else
    scampdistort=""
fi

${P_SCAMP} @${TEMPDIR}/scamp_cats_$$ \
    -c ${DATACONF}/scamp.conf \
    -ASTREFCAT_NAME ~/.theli/scripts/theli_mystd_scamp.cat \
    -NTHREADS ${NPARA} \
    -PHOTINSTRU_KEY ${V_SCAMP_PHOTINSTRUKEY} \
    -SN_THRESHOLDS  ${V_SCAMP_SNLOW},${V_SCAMP_SNHIGH} \
    -MATCH N ${scampdistort} 
#    -CROSSID_RADIUS  ${V_SCAMP_CROSSIDRADIUS} \
#    -MOSAIC_TYPE     ${V_SCAMP_MOSAICTYPE}

rm *.ahead

# now get the relative magnitude offsets from the FLXSCALES
# estimated by scamp:
test -f ${TEMPDIR}/photdata.txt_$$ && rm ${TEMPDIR}/photdata.txt_$$

########################################################################
# Create a list of exposures that contains the root file name, only.
# (multi-chip cameras represented by one entry only)
########################################################################

test -f ${TEMPDIR}/allscampcats.txt_$$ && rm ${TEMPDIR}/allscampcats.txt_$$

${P_FIND} /$1/$2/ -maxdepth 1 -name \*$3.fits | ${P_GAWK} -F/ '{print $NF}' | ${P_SORT} > \
    ${TEMPDIR}/allscampimages.txt_$$

exec < ${TEMPDIR}/allscampimages.txt_$$
while read file
do
    BASE=`basename ${file} .fits`
    if [ -f /$1/$2/cat/"${BASE}".cat1 ]; then
	echo "${BASE}".cat1 >> ${TEMPDIR}/allscampcats.txt_$$
    fi
done

EXPOSURES=`cat ${TEMPDIR}/allscampcats.txt_$$ | \
    ${P_GAWK} '{ n = split($1, a, "_"); 
    name = ""; 
    for(i = 1; i < (n-1); i++) {
        name = name a[i] "_"
    } 
    name = name a[n-1]; 
    print name;}' | ${P_SORT} | uniq`


# Because the flux scales refer to an image normalised to one
# second we need to obtain the exposure times of all frames
# first. We also get the SCAMP flux scale and the photometric 
# instrument:
tmpdir=`pwd`
for EXPOSURE in ${EXPOSURES}
do
    EXPTIME=`${P_LDACTOASC} -i ../cat/${EXPOSURE}_scamp.cat \
	-t LDAC_IMHEAD -s | \
	fold | grep EXPTIME | \
	${P_GAWK} 'BEGIN{FS="="} {if ($1=="EXPTIME ") print $2}' | \
	${P_GAWK} '{print $1}'`
    FLXSCALE=`grep FLXSCALE ${EXPOSURE}_scamp.head | uniq | \
	${P_GAWK} '{if (NR==1) print $2}'`
    PHOTINST=`grep PHOTINST ${EXPOSURE}_scamp.head | uniq | \
	${P_GAWK} '{if (NR==1) print $2}'`
    
    echo ${EXPOSURE}" "${EXPTIME}" "${FLXSCALE}" "${PHOTINST} >> ${TEMPDIR}/photdata.txt_$$
done

#########################################################
# Calculate relative zeropoints and THELI fluxscales 
# for the different photometric contexts
#########################################################

${P_GAWK} 'BEGIN {maxphotinst = 1}
           { exposure[NR]       = $1 
             exptime[NR]        = $2 
             flxscale_scamp[NR] = $3
             photinst[NR]       = $4
             val[NR]            = -2.5 * log($3 * $2) / log(10.)
             m[$4]              = m[$4] + val[NR]
             nphotinst[$4]      = nphotinst[$4] + 1 
             if ($4 > maxphotinst) {maxphotinst = $4}} 
           END {
             for (i = 1; i <= maxphotinst; i++) {  
               m[i] = m[i] / nphotinst[i]
             } 
             for (i = 1; i <= NR; i++) {
               relzp[i] = val[i] - m[photinst[i]]   
               flxscale_theli[i] = (10**(-0.4 * relzp[i])) / exptime[i]
               printf("%s %f %e\n", exposure[i], relzp[i], flxscale_theli[i]);  
             }
           }' ${TEMPDIR}/photdata.txt_$$ > ${TEMPDIR}/photdata_relzp.txt_$$

######################################################################
# split the exposure catalogues for the individual chips
# and add the RZP and FLXSCALE header keywords. Put the headers 
# into appropriate headers_scamp directories
######################################################################

while read EXPOSURE RELZP FLXSCALE
do
  i=1
  j=1  # counts the actually available chips!
  while [ ${i} -le ${NCHIPS} ]
  do
    # we need to take care of catalogs that may not be
    # present (bad chips)!
    CATNAME=`grep -x ''${EXPOSURE}'_'${i}'[^0-9].*\.cat1' \
             ${TEMPDIR}/allscampcats.txt_$$`
    if [ -n "${CATNAME}" -a -f ../../cat/${CATNAME} ]
    then
      # first rename the SCAMP header keyword FLXSCALE
      # to FLSCALE. We need FLXSCALE for the THELI
      # flux scaling later:
      sed -e 's/FLXSCALE/FLSCALE /' ${EXPOSURE}_scamp.head |\
      ${P_GAWK} 'BEGIN {ext = '${j}'; nend = 0} 
                 {
                   if(nend < ext) {
                     if($1 == "END") {
                       nend++; 
                       next; 
                     } 
                     if(nend == (ext-1)) { print $0 } 
                   } 
                 }
                 END { printf("RZP     = %20f / THELI relative zeropoint\n", 
                              '${RELZP}');
                       printf("FLXSCALE= %20E / THELI relative flux scale\n", 
                              '${FLXSCALE}');
                       printf("END\n")
                 }' > ${EXPOSURE}_$i.head
      cp ${EXPOSURE}_$i.head /$1/$2/headers/
      cp ${EXPOSURE}_$i.head /$1/$2/headers_anet/
      cp ${EXPOSURE}_$i.head /$1/$2/astrom_photom_anet/backup/
      j=$(( $j + 1 ))
    fi
    i=$(( $i + 1 ))
  done
done < ${TEMPDIR}/photdata_relzp.txt_$$


cd /$1/$2/
cp .astrefcat headers/.astrefcat_used

rm -rf plots
mkdir plots
cp /$1/$2/astrom_photom_anet/cat/*.png plots/

cd ${DIR}

rm ${TEMPDIR}/*_$$

exit 0
