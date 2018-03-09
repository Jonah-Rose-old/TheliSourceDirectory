#!/bin/bash -xv

# $1: main dir.
# $2: science dir
# $3: extension
# $4: scamp mode (leave either empty or set to 'photom')


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

V_SCAMP_MODE=$4


# extend the distortion parameters if necessary
if [ "${V_SCAMP_POLYORDER2}"_A != "_A" ]; then
    V_SCAMP_POLYORDER="${V_SCAMP_POLYORDER},${V_SCAMP_POLYORDER2}"
fi

if [ "${V_SCAMP_DISTORTGROUPS}"_A != "_A" ]; then
    V_SCAMP_DISTORTGROUPS="1,1,${V_SCAMP_DISTORTGROUPS}"
else
    V_SCAMP_DISTORTGROUPS="1,1"
fi

if [ "${V_SCAMP_DISTORTKEYS}"_A != "_A" ]; then
    V_SCAMP_DISTORTKEYS="XWIN_IMAGE,YWIN_IMAGE,${V_SCAMP_DISTORTKEYS}"
else
    V_SCAMP_DISTORTKEYS="XWIN_IMAGE,YWIN_IMAGE"
fi

#############################################################
# Delete old solutions and re-create the directories
#############################################################

cd /$1/$2/

mkdir -p astrom_photom_scamp

cd astrom_photom_scamp
test -d cat && rm -rf cat
test -d headers && rm -rf headers
test -d plots && rm -rf plots
mkdir cat
mkdir headers
mkdir plots
mkdir -p backup
cd ..

test -d headers_scamp && rm -rf headers_scamp
test -d headers && rm -rf headers
test -d plots && rm plots/*
mkdir headers_scamp headers

test -f ${TEMPDIR}/scamp_global.ahead && rm ${TEMPDIR}/scamp_global.ahead


########################################################
# Link the previously created composite catalogs.
# Use only those for which images are currently 
# present in the SCIENCE directory
########################################################

if [ "${NCHIPS}" -gt "1" ]; then
    ls -1 /$1/$2/cat/*_scamp.cat > ${TEMPDIR}/scampcatalogs_$$
    exec < ${TEMPDIR}/scampcatalogs_$$
    while read file
    do
	BASE=`basename $file _scamp.cat`
	images=`${P_FIND} /$1/$2/ -maxdepth 1 -name ${BASE}_\*$3.fits`
	if [ -n "${images}" ]; then
	    catname=`basename $file`
	    ln -s ${file} /$1/$2/astrom_photom_scamp/cat/${catname}
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
	    ln -s ${file} /$1/$2/astrom_photom_scamp/cat/${BASE}_scamp.cat
	fi
    done
fi
 

cd astrom_photom_scamp/cat

# check if any catalogs are present
ncat=`ls | grep scamp.cat | wc | ${P_GAWK} '{print $1}'`
if [ "${ncat}" = "0" ]; then
    ${P_ERRTEST} 0
    echo "No scamp catalogs found! Did you create the source catalogs?"
    exit
fi


##########################################################
# If a global .ahead file does not exist, then 
# create one based on the first exposure in the list.
# Only needed for multi-chip cameras, and only if
# existing headers from a previous scamp run don't exist
##########################################################

if [ "${NCHIPS}" -gt "1" ]; then
    if [ "${V_SCAMP_FOCALPLANE}" = "NEW" ] || \
       [ ! -f ${PREFIXDIR}/${INSTDIR}/${INSTRUMENT:?}.ahead ]; then

        # We need to delete all *.head and *.ahead files
	rm -f *.head *.ahead

        # for some reason scamp gives better results if the
        # 'ordering' of files is shuffled in time! Do not ask!

	${P_FIND} ../cat/ -maxdepth 1 -name \*scamp.cat > ${TEMPDIR}/scamp_cats_$$

	# ${P_FIND} ../cat/ -maxdepth 1 -name \*scamp.cat | \
	#    ${P_GAWK} '{line[NR] = $0} END { srand();
        #   for (i = 1; i <= NR; i++) { 
        #     n = int(rand() * NR) + 1; 
        #     temp = line[i]; 
        #     line[i] = line[n];
        #     line[n] = temp;
        #   }
        #   for (i = 1; i <= NR; i++) {
        #     print line[i];
        #   }}' > ${TEMPDIR}/scamp_cats_$$

	globalheadercat=`${P_GAWK} '(NR==1)' ${TEMPDIR}/scamp_cats_$$`
	globalheaderbase=`basename ${globalheadercat} _scamp.cat`
	
	if [ "${V_SCAMP_MODE}" = "photom" ]; then
	    V_SCAMP_POLYORDER=2
	    if [ "${NCHIPS}" = "1" ]; then
		V_SCAMP_POLYORDER=1
	    fi
	    V_SCAMP_MOSAICTYPE=LOOSE
	    V_SCAMP_STABILITY=EXPOSURE
            # do not use saturated sources
	    FLAGS="-FLAGS_MASK 244"
	    
            # do not use manually downloaded reference catalogue
            # as it wouldn't match if different standard star fields
            # are loaded at the same time
	    ${P_SCAMP} @${TEMPDIR}/scamp_cats_$$ \
		-c ${DATACONF}/scamp.conf \
		-ASTREF_CATALOG PPMX \
		-CDSCLIENT_EXEC aclient \
		-SAVE_REFCATALOG N \
		-REF_SERVER cocat1.u-strasbg.fr \
		-NTHREADS ${NPARA} \
		-DISTORT_DEGREES ${V_SCAMP_POLYORDER} \
		-DISTORT_GROUPS  ${V_SCAMP_DISTORTGROUPS} \
		-POSITION_MAXERR ${V_SCAMP_MAXOFFSET} \
		-POSANGLE_MAXERR ${V_SCAMP_MAXPOSANGLE} \
		-PIXSCALE_MAXERR ${V_SCAMP_MAXSCALE} \
		-FGROUP_RADIUS   ${V_SCAMP_FGROUPRADIUS} \
		-CROSSID_RADIUS  ${V_SCAMP_CROSSIDRADIUS} \
		-ASTRINSTRU_KEY  ${V_SCAMP_ASTRINSTRUKEY} \
		-PHOTINSTRU_KEY  ${V_SCAMP_PHOTINSTRUKEY} \
		-SN_THRESHOLDS   ${V_SCAMP_SNLOW},${V_SCAMP_SNHIGH} \
		-MOSAIC_TYPE     ${V_SCAMP_MOSAICTYPE} \
		-STABILITY_TYPE  ${V_SCAMP_STABILITY} \
		-MATCH_FLIPPED   ${V_SCAMP_MATCHFLIPPED} \
		-MATCH           ${V_SCAMP_MATCHWCS} \
		-ASTREF_WEIGHT   ${V_SCAMP_ASTREFWEIGHT} \
		${FLAGS}
	else
#	    -ASTREFCAT_NAME ~/.theli/scripts/theli_mystd_scamp_std.cat \
	    ${P_SCAMP} @${TEMPDIR}/scamp_cats_$$ \
		-c ${DATACONF}/scamp.conf \
		-CDSCLIENT_EXEC aclient \
		-ASTREFCAT_NAME ~/.theli/scripts/theli_mystd_scamp.cat \
		-NTHREADS ${NPARA} \
		-MOSAIC_TYPE LOOSE \
		-DISTORT_DEGREES ${V_SCAMP_POLYORDER} \
		-DISTORT_GROUPS  ${V_SCAMP_DISTORTGROUPS} \
		-POSITION_MAXERR ${V_SCAMP_MAXOFFSET} \
		-POSANGLE_MAXERR ${V_SCAMP_MAXPOSANGLE} \
		-PIXSCALE_MAXERR ${V_SCAMP_MAXSCALE} \
		-FGROUP_RADIUS   ${V_SCAMP_FGROUPRADIUS} \
		-CROSSID_RADIUS  ${V_SCAMP_CROSSIDRADIUS} \
		-ASTRINSTRU_KEY  ${V_SCAMP_ASTRINSTRUKEY} \
		-PHOTINSTRU_KEY  ${V_SCAMP_PHOTINSTRUKEY} \
		-MATCH_FLIPPED   ${V_SCAMP_MATCHFLIPPED} \
		-MATCH           ${V_SCAMP_MATCHWCS} \
		-SN_THRESHOLDS   ${V_SCAMP_SNLOW},${V_SCAMP_SNHIGH} \
		-STABILITY_TYPE  ${V_SCAMP_STABILITY} \
		-ASTREF_WEIGHT   ${V_SCAMP_ASTREFWEIGHT}
	fi
	
	# keep only CRPIX1/2 and the CD-matrix
	${P_GAWK} '($1~/CRPIX/ || $1=="CD1_1" || $1=="CD1_2" || $1=="CD2_1" || $1=="CD2_2" || $1=="END")' \
	    ${globalheaderbase}_scamp.head > ${TEMPDIR}/scamp_global.ahead

        # Check if a default global header file exists
        # for this instrument. If not, create it.
	if [ ! -f "${USERDIR}/${INSTRUMENT}.ahead" ]; then
	    cp ${TEMPDIR}/scamp_global.ahead ${USERDIR}/${INSTRUMENT}.ahead
	fi
    fi
    
    # Check if the default global header file shall be used
    # or the one determined from this particular data set.
    if [ "${V_SCAMP_FOCALPLANE}" = "DEFAULT" ]; then   
	if [ -f "${DIR}/${INSTDIR}/${INSTRUMENT}.ahead" ]; then
	    aheader_global_tmp=${DIR}/${INSTDIR}/${INSTRUMENT}.ahead
	else 
	    aheader_global_tmp=/${USERDIR}/${INSTRUMENT}.ahead
	fi
	# get the position angle of the current data set,
	# and rotate the default .ahead file accordingly
	tmpdir=`pwd`
	cd /$1/$2/astrom_photom_scamp/cat/
	ls *_scamp.cat > ${TEMPDIR}/pa_list_$$
	pa_cat=`${P_GAWK} '(NR==1) {print $1}' ${TEMPDIR}/pa_list_$$`
	pa_image=`basename ${pa_cat} _scamp.cat`"_1"$3.fits
	pa=`${P_GETPOSANGLE} -f $1/$2/${pa_image}`
	# the position angle of the current aheader file
	cd11=`grep CD1_1 ${aheader_global_tmp} | awk '(NR==1) {print $3}'`
	cd12=`grep CD1_2 ${aheader_global_tmp} | awk '(NR==1) {print $3}'`
	cd21=`grep CD2_1 ${aheader_global_tmp} | awk '(NR==1) {print $3}'`
	cd22=`grep CD2_2 ${aheader_global_tmp} | awk '(NR==1) {print $3}'`
	paahead=`${P_GETPOSANGLE} -c ${cd11} ${cd12} ${cd21} ${cd22}`
	padiff=`echo ${pa} ${paahead} | ${P_GAWK} '{if ($1-$2<0.) print $2-$1; else print $1-$2}'`

	# increase POSANGLE_MAXERR if the difference between nominal FP angle and actual PA is larger than
	# the user provided value
	newpa=`echo ${V_SCAMP_MAXPOSANGLE} ${padiff} | ${P_GAWK} '{if ($2>$1) a=int($2+1.); else a=0} END {if (a>180) a=180; if (a>0) print a}'`
	if [ "${newpa}_A" != "_A" ]; then
	    V_SCAMP_MAXPOSANGLE=${newpa}
	fi

	rotate=`echo ${pa} ${paahead} | ${P_GAWK} '{a=$1-$2; if (a<0) a=$2-$1} END {if (a>1.5 && a<358.5 || a<-1.5 && a>-358.5) print "Y"; else print "N"}'`
	# we rotate the aheader file if the difference is larger than 1.5 degrees
#	if [ "${rotate}" = "Y" ]; then
#	    ${P_ROTATECDMATRIX} \
#		-i ${aheader_global_tmp} \
#		-o ${TEMPDIR}/scamp_global.ahead \
#		-p ${pa}
#	fi
#	aheader_global=${TEMPDIR}/scamp_global.ahead


	# comment this line if you uncomment the section above
	aheader_global=${aheader_global_tmp}

	cd ${tmpdir}
    fi
    if [ "${V_SCAMP_FOCALPLANE}" = "NEW" ]; then
	aheader_global=${TEMPDIR}/scamp_global.ahead
    fi
fi

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


################################################################################
# If present, split the instrument ahead file in parts for every chip.
# We need to put together a separate one for each file (missing chips etc.).
# Only if no old solution is going to be used as a starting point.
################################################################################

AHEAD=0
if [ "${NCHIPS}" -gt "1" ] && [ -f ${aheader_global} ]; then
    ${P_GAWK} 'BEGIN {
               ext = "1"; 
               filename = "'${TEMPDIR}'/'${INSTRUMENT}'_" ext ".ahead_" '$$'; 
             } 
             { if($1 != "END") {
                 print $0 >> filename
               } else {
                 ext++;
                 filename = "'${TEMPDIR}'/'${INSTRUMENT}'_" ext ".ahead_" '$$'; 
               } 
             }' ${aheader_global}
    AHEAD=1
fi


##########################################################################
# Identify missing chips and create new .ahead files for each exposure
##########################################################################

if [ "${NCHIPS}" -gt "1" ]; then
    for EXPOSURE in ${EXPOSURES}
    do
        # MISSCHIP contains the missing chips in the form of a pasted string.
        # If e. g. chips 19 and 25 are bad the variable would contain 
        # 01925 (read 0_19_25; '0' is always at the beginning)
	MISSCHIP=0     
	
        # identify missing chips
	i=1
	while [ ${i} -le ${NCHIPS} ]
	do
	    CATNAME=`grep -x ''${EXPOSURE}'_'${i}'[^0-9].*\.cat1' \
		${TEMPDIR}/allscampcats.txt_$$`
	    
	    if [ ! -n "${CATNAME}" ]; then
		MISSCHIP=${MISSCHIP}${i}
	    fi
	    i=$(( ${i} + 1 ))
	done
	
        # Reconstruct the individual .ahead file 
	test -f ./${EXPOSURE}_scamp.ahead && rm ./${EXPOSURE}_scamp.ahead

	i=1
	while [ ${i} -le ${NCHIPS} ]
	do
	    CATNAME=`grep -x ''${EXPOSURE}'_'${i}'[^0-9].*\.cat1' \
		${TEMPDIR}/allscampcats.txt_$$`
	    
	    if [ -n "${CATNAME}" ] && [ -f "$1/$2/cat/${CATNAME}" ]; then
		if [ ${AHEAD} -eq 1 ]; then
		    cat  ${TEMPDIR}/${INSTRUMENT}_${i}.ahead_$$ 
		fi
		echo "MISSCHIP= '${MISSCHIP}'" 
		echo "END      "               
	    fi
	    i=$(( ${i} + 1 ))
	done > ./${EXPOSURE}_scamp.ahead
    done
fi


############################################
# scamp
############################################

cd ../headers

# for some reason scamp gives better results if the
# 'ordering' of files is shuffled in time! Do not ask!

${P_FIND} ../cat/ -maxdepth 1 -name \*scamp.cat > ${TEMPDIR}/scamp_cats_$$

#${P_FIND} ../cat/ -maxdepth 1 -name \*scamp.cat | \
#    ${P_GAWK} '{line[NR] = $0} END { srand();
#           for (i = 1; i <= NR; i++) { 
#             n = int(rand() * NR) + 1; 
#             temp = line[i]; 
#             line[i] = line[n];
#             line[n] = temp;
#           }
#           for (i = 1; i <= NR; i++) {
#             print line[i];
#           }}' > ${TEMPDIR}/scamp_cats_$$


if [ "${V_SCAMP_FOCALPLANE}" = "NONE" ] || \
    [ "${NCHIPS}" = "1" ]; then
    aheaderstring=""
else
    aheaderstring="-AHEADER_GLOBAL ${aheader_global}"
fi


# in photometry mode we don't need high astrometric precision,
# and it is possibly difficult to achieve such due to the short
# exposure times. Hence, keep it simple!
if [ "${V_SCAMP_MODE}" = "photom" ]; then
    V_SCAMP_POLYORDER=2
    if [ "${NCHIPS}" = "1" ]; then
	V_SCAMP_POLYORDER=1
    fi
    V_SCAMP_MOSAICTYPE=LOOSE
    V_SCAMP_STABILITY=EXPOSURE
    V_SCAMP_MOSAICTYPE=SAME_CRVAL
    V_SCAMP_STABILITY=INSTRUMENT
    # do not use saturated sources
    FLAGS="-FLAGS_MASK 244"

    # do not use manually downloaded reference catalogue
    # as it wouldn't match if different standard star fields
    # are loaded at the same time
    ${P_SCAMP} @${TEMPDIR}/scamp_cats_$$ \
	-c ${DATACONF}/scamp.conf \
	-ASTREF_CATALOG PPMX \
	-CDSCLIENT_EXEC aclient \
	-SAVE_REFCATALOG N \
	-REF_SERVER cocat1.u-strasbg.fr \
	-NTHREADS ${NPARA} \
	${aheaderstring} \
	${FLAGS} \
	-DISTORT_DEGREES ${V_SCAMP_POLYORDER} \
	-DISTORT_GROUPS  ${V_SCAMP_DISTORTGROUPS} \
	-POSITION_MAXERR ${V_SCAMP_MAXOFFSET} \
	-POSANGLE_MAXERR ${V_SCAMP_MAXPOSANGLE} \
	-PIXSCALE_MAXERR ${V_SCAMP_MAXSCALE} \
	-FGROUP_RADIUS   ${V_SCAMP_FGROUPRADIUS} \
	-CROSSID_RADIUS  ${V_SCAMP_CROSSIDRADIUS} \
	-ASTRINSTRU_KEY  ${V_SCAMP_ASTRINSTRUKEY} \
	-PHOTINSTRU_KEY  ${V_SCAMP_PHOTINSTRUKEY} \
	-SN_THRESHOLDS   ${V_SCAMP_SNLOW},${V_SCAMP_SNHIGH} \
	-MOSAIC_TYPE     ${V_SCAMP_MOSAICTYPE} \
	-STABILITY_TYPE  ${V_SCAMP_STABILITY} \
	-MATCH_FLIPPED   ${V_SCAMP_MATCHFLIPPED} \
	-MATCH           ${V_SCAMP_MATCHWCS} \
	-ASTREF_WEIGHT   ${V_SCAMP_ASTREFWEIGHT} \
	-ASTREFMAG_LIMITS 0.0,${V_AP_MAGLIM}
else
    # saturated sources are ok
    FLAGS="-FLAGS_MASK 240"    

#   -ASTREFCAT_NAME ~/.theli/scripts/theli_mystd_scamp_std.cat \
    ${P_SCAMP} @${TEMPDIR}/scamp_cats_$$ \
	-c ${DATACONF}/scamp.conf \
	-ASTREFCAT_NAME ~/.theli/scripts/theli_mystd_scamp.cat \
	-CDSCLIENT_EXEC aclient \
	-SAVE_REFCATALOG Y \
	-REFOUT_CATPATH /$1/$2 \
	-NTHREADS ${NPARA} \
	${aheaderstring} \
	${FLAGS} \
	-DISTORT_KEYS    ${V_SCAMP_DISTORTKEYS} \
	-DISTORT_DEGREES ${V_SCAMP_POLYORDER} \
	-DISTORT_GROUPS  ${V_SCAMP_DISTORTGROUPS} \
	-POSITION_MAXERR ${V_SCAMP_MAXOFFSET} \
	-POSANGLE_MAXERR ${V_SCAMP_MAXPOSANGLE} \
	-PIXSCALE_MAXERR ${V_SCAMP_MAXSCALE} \
	-FGROUP_RADIUS   ${V_SCAMP_FGROUPRADIUS} \
	-CROSSID_RADIUS  ${V_SCAMP_CROSSIDRADIUS} \
	-ASTRINSTRU_KEY  ${V_SCAMP_ASTRINSTRUKEY} \
	-PHOTINSTRU_KEY  ${V_SCAMP_PHOTINSTRUKEY} \
	-SN_THRESHOLDS   ${V_SCAMP_SNLOW},${V_SCAMP_SNHIGH} \
	-MOSAIC_TYPE     ${V_SCAMP_MOSAICTYPE} \
	-STABILITY_TYPE  ${V_SCAMP_STABILITY} \
	-MATCH_FLIPPED   ${V_SCAMP_MATCHFLIPPED} \
	-MATCH           ${V_SCAMP_MATCHWCS} \
	-ASTREF_WEIGHT   ${V_SCAMP_ASTREFWEIGHT} >& ${TEMPDIR}/scamp.log_$$
fi

cat ${TEMPDIR}/scamp.log_$$

# delete old backup catalogs
${P_FIND} ../backup/ -maxdepth 1 -name \*_scamp.head -exec rm {} \;

# scamp creates the new headers in the directory where the catalogs are:
${P_FIND} ../cat/ -maxdepth 1 -name \*.head -exec cp {} . \;

# make a backup copy
${P_FIND} ../cat/ -maxdepth 1 -name \*.head -exec cp {} ../backup/ \;

# we want the diagnostic plots in their own directory:
cp *.png *.xml ../plots/

cp ${TEMPDIR}/scamp.log_$$ ../plots/scamp.log

# now get the relative magnitude offsets from the FLXSCALES
# estimated by scamp:
test -f ${TEMPDIR}/photdata.txt_$$ && rm ${TEMPDIR}/photdata.txt_$$

# Because the flux scales refer to an image normalised to one
# second we need to obtain the exposure times of all frames
# first. We also get the SCAMP flux scale and the photometric 
# instrument:
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



cp -r /$1/$2/astrom_photom_scamp/plots/ /$1/$2/

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

      # correcting the catalogs (of standard stars) with aplastrom_scamp.py does not work
      # if no PV terms are present. So, if DISTORT_DEGREE=1, we add PV1_0, PV1_1, PV2_0, and PV2_1

      if [ "${V_SCAMP_POLYORDER}" = "1" ]; then
	  ${P_GAWK} '{if ($1 != "END") print $0; 
                      else {
                           print "PV1_0   = 0.0"
                           print "PV1_1   = 1.0"
                           print "PV2_0   = 0.0"
                           print "PV2_1   = 1.0"
                           print $0}}' ${EXPOSURE}_$i.head > ${EXPOSURE}_$i.headtmp
	  mv ${EXPOSURE}_$i.headtmp ${EXPOSURE}_$i.head
      fi

      cp ${EXPOSURE}_$i.head /$1/$2/headers/
      cp ${EXPOSURE}_$i.head /$1/$2/headers_scamp/
      cp ${EXPOSURE}_$i.head /$1/$2/astrom_photom_scamp/backup/
      j=$(( $j + 1 ))
    fi
    i=$(( $i + 1 ))
  done
done < ${TEMPDIR}/photdata_relzp.txt_$$


cd /$1/$2/
cp .astrefcat headers/.astrefcat_used

cd ${DIR}

# rm ${TEMPDIR}/*_$$

exit 0
