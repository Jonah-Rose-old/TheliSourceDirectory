#!/bin/bash -xv

# this script creates a STATS table from the catalogs created with
# 'create_astromcats_weights_para.sh'
#
# The STATS table contains 'global' information on the exposure level
# (e.g. seeing. zeropoints, sky background), i.e. condensed
# information from all the chips. Typically corresponding quantities
# from individual chips are averaged for the STATS table.


# $1: main directory
# $2: science directory
# $3: image extension
# $4: headers directory (subdirectory of /$1/$2; OPTIONAL)

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

test -f ${TEMPDIR}/statstable.txt_$$ && rm  ${TEMPDIR}/statstable.txt_$$

CATDIR=/$1/$2/cat
IMENDING=$3

HEADERDIR=""
if [ $# -eq 4 ]; then
  HEADERDIR=/$1/$2/$4
fi

# obtain a list of the present exposures:
${P_FIND} ${CATDIR} -maxdepth 1 -name \*${IMENDING}.cat | \
    ${P_GAWK} -F/ '{print $NF}' > \
    ${TEMPDIR}/allfiles.txt_$$

# The following 'awk' construct cuts away everything after 
# the last '_' in the image names (including the underscore itself);
# we implicitely assume that the image extension DOES NOT contain '_'s. 
EXPOSURES=`${P_GAWK} '{ n = split($1, a, "_"); 
                        name = ""; 
                        for(i = 1; i < (n-1); i++) {
                          name = name a[i] "_"
                        } 
                        name = name a[n-1]; 
                        print name;}' ${TEMPDIR}/allfiles.txt_$$ | sort | uniq`

# go exposure by exposure to build up the statistics table:
KEYTEST=0 # was a test which keys are available already performed?
ABSPHOTINFO=0
RELPHOTINFO=0
KEYSTRING="OBJECT SEXBKGND SEXSFWHM GABODSID EXPTIME CRVAL1 CRVAL2"
KEYSTRING="${KEYSTRING} FILTER AIRMASS"

ZPSTRING=""
for EXPOSURE in ${EXPOSURES}
do
  ALLCATS=`ls ${CATDIR}/${EXPOSURE}*${IMENDING}.cat1`
  ALLIMGS=`ls $1/$2/${EXPOSURE}*${IMENDING}.fits`
  
  # skip a catalog if the image does not exist (i.e., the user moved it to a parking folder)
  if [ "${ALLIMGS}"_A = "_A" ]; then
      continue
  fi

  GOODCATS=""

  for CAT in ${ALLCATS}
  do
      BADCCD=`${P_LDACTOASC} -i ${CAT} -t LDAC_IMHEAD -s | fold | \
              grep BADCCD | awk '{print $3}'`
      
      if [ "${BADCCD}" != "1" ]; then
	  GOODCATS="${GOODCATS} ${CAT}"
      fi 
  done

  ${P_LDACCONV} -i ${GOODCATS} -o ${CATDIR}/${EXPOSURE}.cat -c ${INSTRUMENT} \
      -f dummy -b 1

  if [ ${KEYTEST} -eq 0 ]; then
      
      # assume that all four ZP solutions are available if one is found
      ${P_LDACTESTEXIST} -i ${CATDIR}/${EXPOSURE}.cat -t FIELDS -k ZP1
      if [ $? -eq 0 ]; then
	  ABSPHOTINFO=1
	  ZPSTRING="${ZPSTRING} ZP1 COEFF1 COLOR1"
	  ZPSTRING="${ZPSTRING} ZP2 COEFF2 COLOR2"
	  ZPSTRING="${ZPSTRING} ZP3 COEFF3 COLOR3"
	  ZPSTRING="${ZPSTRING} ZP4 COEFF4 COLOR4"
      fi
      
      CAT=`echo ${ALLCATS} | ${P_GAWK} '{print $1}'`
      BASE=`basename ${CAT} ${IMENDING}.cat1`
      
      if [ -f ${HEADERDIR}/${BASE}.head ]; then
	  RELPHOTINFO=`${P_GAWK} 'BEGIN {FS = "[=/]"; relzp = 0} {
                        if ($1 ~ /^RZP/) { 
                          relzp = ($2 ~ /[0-9]+\.[0-9]*/) 
                        }} END {print relzp}' ${HEADERDIR}/${BASE}.head`
      fi
      
      KEYTEST=1  
  fi
  
  ${P_LDACTOASC} -b -s -i ${CATDIR}/${EXPOSURE}.cat \
                 -t FIELDS -k ${KEYSTRING} ${ZPSTRING} > ${TEMPDIR}/table.dat_$$
  OBJECT=`${P_GAWK}   'NR==1 {print $1}' ${TEMPDIR}/table.dat_$$`
  GABODSID=`${P_GAWK} 'NR==1 {print $4}' ${TEMPDIR}/table.dat_$$`
  EXPTIME=`${P_GAWK}  'NR==1 {print $5}' ${TEMPDIR}/table.dat_$$`
  RA=`${P_GAWK}       'NR==1 {print $6}' ${TEMPDIR}/table.dat_$$`
  DEC=`${P_GAWK}      'NR==1 {print $7}' ${TEMPDIR}/table.dat_$$`
  FILTER=`${P_GAWK}   'NR==1 {print $8}' ${TEMPDIR}/table.dat_$$`
  AIRMASS=`${P_GAWK}  'NR==1 {print $9}' ${TEMPDIR}/table.dat_$$`
  BACK=`${P_GAWK} 'BEGIN {m=0; n=0} {m+=($2/$5); n++} END {print m/n} ' \
        ${TEMPDIR}/table.dat_$$`
  BACKSDEV=`${P_GAWK} 'BEGIN {s=0; n=0; m=0} {
            s+=($2*$2)/($5*$5); m+=($2/$5); n++} END {m=m/n; print sqrt(s/n-m*m)}' \
            ${TEMPDIR}/table.dat_$$`
  SEEING=`${P_GAWK} 'BEGIN {m=0; n=0} {m+=$3; n++} END {print m/n}' ${TEMPDIR}/table.dat_$$`
  SEEINGSDEV=`${P_GAWK} 'BEGIN {s=0; n=0; m=0} {
            s+=($3*$3); m+=$3; n++} END {m=m/n; print sqrt(s/n-m*m)}' \
            ${TEMPDIR}/table.dat_$$`

  if [ ${ABSPHOTINFO} -eq 1 ]; then
    ZP1=`${P_GAWK}    'NR==1 {print $10}' ${TEMPDIR}/table.dat_$$`    
    COEFF1=`${P_GAWK} 'NR==1 {print $11}' ${TEMPDIR}/table.dat_$$`    
    COLOR1=`${P_GAWK} 'NR==1 {print $12}' ${TEMPDIR}/table.dat_$$`    
    ZP2=`${P_GAWK}    'NR==1 {print $13}' ${TEMPDIR}/table.dat_$$`    
    COEFF2=`${P_GAWK} 'NR==1 {print $14}' ${TEMPDIR}/table.dat_$$`    
    COLOR2=`${P_GAWK} 'NR==1 {print $15}' ${TEMPDIR}/table.dat_$$`    
    ZP3=`${P_GAWK}    'NR==1 {print $16}' ${TEMPDIR}/table.dat_$$`    
    COEFF3=`${P_GAWK} 'NR==1 {print $17}' ${TEMPDIR}/table.dat_$$`    
    COLOR3=`${P_GAWK} 'NR==1 {print $18}' ${TEMPDIR}/table.dat_$$`    
    ZP4=`${P_GAWK}    'NR==1 {print $19}' ${TEMPDIR}/table.dat_$$`    
    COEFF4=`${P_GAWK} 'NR==1 {print $20}' ${TEMPDIR}/table.dat_$$`    
    COLOR4=`${P_GAWK} 'NR==1 {print $21}' ${TEMPDIR}/table.dat_$$`    
  fi

  if [ ${RELPHOTINFO} -eq 1 ]; then
      BASE=`basename ${CATDIR}/${EXPOSURE}.cat .cat`
      RZP=`${P_GAWK} 'BEGIN {FS = "[=/]"; relzp = 0} {
                    if ($1 ~ /^RZP/) {print $2}}' ${HEADERDIR}/${BASE}_1.head`
  fi
  
  EXPLINE="${EXPOSURE} ${OBJECT} ${EXPTIME} ${GABODSID} ${SEEING} ${SEEINGSDEV}"
  EXPLINE="${EXPLINE} ${BACK} ${BACKSDEV} ${RA} ${DEC} ${FILTER} ${AIRMASS}"

  if [ ${ABSPHOTINFO} -eq 1 ]; then
      EXPLINE="${EXPLINE} ${ZP1} ${COEFF1} ${COLOR1}"
      EXPLINE="${EXPLINE} ${ZP2} ${COEFF2} ${COLOR2}"
      EXPLINE="${EXPLINE} ${ZP3} ${COEFF3} ${COLOR3}"
      EXPLINE="${EXPLINE} ${ZP4} ${COEFF4} ${COLOR4}"
  fi
  
  if [ ${RELPHOTINFO} -eq 1 ]; then
      EXPLINE="${EXPLINE} ${RZP}"
  fi
  
  echo ${EXPLINE} >> ${TEMPDIR}/statstable.txt_$$
done

{
  echo 'COL_NAME  = IMAGENAME' 
  echo 'COL_TTYPE = STRING'    
  echo 'COL_HTYPE = STRING'    
  echo 'COL_COMM = ""'         
  echo 'COL_UNIT = ""'         
  echo 'COL_DEPTH = 128'       
  echo 'COL_NAME  = OBJECT'    
  echo 'COL_TTYPE = STRING'    
  echo 'COL_HTYPE = STRING'    
  echo 'COL_COMM = ""'         
  echo 'COL_UNIT = ""'         
  echo 'COL_DEPTH = 128'       
  echo 'COL_NAME  = EXPTIME'    
  echo 'COL_TTYPE = FLOAT'    
  echo 'COL_HTYPE = FLOAT'    
  echo 'COL_COMM = ""'         
  echo 'COL_UNIT = ""'         
  echo 'COL_DEPTH = 1'       
  echo 'COL_NAME  = GABODSID'
  echo 'COL_TTYPE = SHORT'   
  echo 'COL_HTYPE = INT'     
  echo 'COL_COMM = ""'       
  echo 'COL_UNIT = ""'       
  echo 'COL_DEPTH = 1'       
  echo 'COL_NAME  = SEEING'    
  echo 'COL_TTYPE = FLOAT'    
  echo 'COL_HTYPE = FLOAT'    
  echo 'COL_COMM = ""'         
  echo 'COL_UNIT = ""'         
  echo 'COL_DEPTH = 1'       
  echo 'COL_NAME  = SEEING_SDEV'    
  echo 'COL_TTYPE = FLOAT'    
  echo 'COL_HTYPE = FLOAT'    
  echo 'COL_COMM = ""'         
  echo 'COL_UNIT = ""'         
  echo 'COL_DEPTH = 1'       
  echo 'COL_NAME  = BACKGR'    
  echo 'COL_TTYPE = FLOAT'    
  echo 'COL_HTYPE = FLOAT'    
  echo 'COL_COMM = ""'         
  echo 'COL_UNIT = ""'         
  echo 'COL_DEPTH = 1'       
  echo 'COL_NAME  = BACKGR_SDEV'    
  echo 'COL_TTYPE = FLOAT'    
  echo 'COL_HTYPE = FLOAT'    
  echo 'COL_COMM = ""'         
  echo 'COL_UNIT = ""'         
  echo 'COL_DEPTH = 1'       
  echo 'COL_NAME  = RA'    
  echo 'COL_TTYPE = FLOAT'    
  echo 'COL_HTYPE = FLOAT'    
  echo 'COL_COMM = ""'         
  echo 'COL_UNIT = ""'         
  echo 'COL_DEPTH = 1'       
  echo 'COL_NAME  = DEC'    
  echo 'COL_TTYPE = FLOAT'    
  echo 'COL_HTYPE = FLOAT'    
  echo 'COL_COMM = ""'         
  echo 'COL_UNIT = ""'         
  echo 'COL_DEPTH = 1'       
  echo 'COL_NAME  = FILTER'    
  echo 'COL_TTYPE = STRING'    
  echo 'COL_HTYPE = STRING'    
  echo 'COL_COMM = ""'         
  echo 'COL_UNIT = ""'         
  echo 'COL_DEPTH = 128'       
  echo 'COL_NAME  = AIRMASS'  
  echo 'COL_TTYPE = FLOAT'   
  echo 'COL_HTYPE = FLOAT'   
  echo 'COL_COMM = ""'       
  echo 'COL_UNIT = ""'       
  echo 'COL_DEPTH = 1'       
} > ${TEMPDIR}/asctoldac.conf_$$

if [ "${ABSPHOTINFO}" -eq 1 ]; then
    {
	echo 'COL_NAME  = ZP1'
	echo 'COL_TTYPE = FLOAT'
	echo 'COL_HTYPE = FLOAT'
	echo 'COL_COMM = ""'
	echo 'COL_UNIT = ""'
	echo 'COL_DEPTH = 1'
	echo 'COL_NAME  = COEFF1'
	echo 'COL_TTYPE = FLOAT'
	echo 'COL_HTYPE = FLOAT'
	echo 'COL_COMM = ""'
	echo 'COL_UNIT = ""'
	echo 'COL_DEPTH = 1'
	echo 'COL_NAME  = COLOR1'
	echo 'COL_TTYPE = FLOAT'
	echo 'COL_HTYPE = FLOAT'
	echo 'COL_COMM = ""'
	echo 'COL_UNIT = ""'
	echo 'COL_DEPTH = 1'
	echo 'COL_NAME  = ZP2'
	echo 'COL_TTYPE = FLOAT'
	echo 'COL_HTYPE = FLOAT'
	echo 'COL_COMM = ""'
	echo 'COL_UNIT = ""'
	echo 'COL_DEPTH = 1'
	echo 'COL_NAME  = COEFF2'
	echo 'COL_TTYPE = FLOAT'
	echo 'COL_HTYPE = FLOAT'
	echo 'COL_COMM = ""'
	echo 'COL_UNIT = ""'
	echo 'COL_DEPTH = 1'
	echo 'COL_NAME  = COLOR2'
	echo 'COL_TTYPE = FLOAT'
	echo 'COL_HTYPE = FLOAT'
	echo 'COL_COMM = ""'
	echo 'COL_UNIT = ""'
	echo 'COL_DEPTH = 1'
	echo 'COL_NAME  = ZP3'     
	echo 'COL_TTYPE = FLOAT'   
	echo 'COL_HTYPE = FLOAT'   
	echo 'COL_COMM = ""'       
	echo 'COL_UNIT = ""'       
	echo 'COL_DEPTH = 1'       
	echo 'COL_NAME  = COEFF3'  
	echo 'COL_TTYPE = FLOAT'   
	echo 'COL_HTYPE = FLOAT'   
	echo 'COL_COMM = ""'       
	echo 'COL_UNIT = ""'       
	echo 'COL_DEPTH = 1'       
	echo 'COL_NAME  = COLOR3'  
	echo 'COL_TTYPE = FLOAT'   
	echo 'COL_HTYPE = FLOAT'   
	echo 'COL_COMM = ""'       
	echo 'COL_UNIT = ""'       
	echo 'COL_DEPTH = 1'       
	echo 'COL_NAME  = ZP4'     
	echo 'COL_TTYPE = FLOAT'   
	echo 'COL_HTYPE = FLOAT'   
	echo 'COL_COMM = ""'       
	echo 'COL_UNIT = ""'       
	echo 'COL_DEPTH = 1'       
	echo 'COL_NAME  = COEFF4'  
	echo 'COL_TTYPE = FLOAT'   
	echo 'COL_HTYPE = FLOAT'   
	echo 'COL_COMM = ""'       
	echo 'COL_UNIT = ""'       
	echo 'COL_DEPTH = 1'       
	echo 'COL_NAME  = COLOR4'  
	echo 'COL_TTYPE = FLOAT'   
	echo 'COL_HTYPE = FLOAT'   
	echo 'COL_COMM = ""'       
	echo 'COL_UNIT = ""'       
	echo 'COL_DEPTH = 1'       
    } >> ${TEMPDIR}/asctoldac.conf_$$
fi

if [ "${RELPHOTINFO}" -eq 1 ]; then
    {
	echo 'COL_NAME  = RZP'     
	echo 'COL_TTYPE = FLOAT'   
	echo 'COL_HTYPE = FLOAT'   
	echo 'COL_COMM = ""'       
	echo 'COL_UNIT = ""'       
	echo 'COL_DEPTH = 1'       
    } >> ${TEMPDIR}/asctoldac.conf_$$
fi

${P_ASCTOLDAC} -i ${TEMPDIR}/statstable.txt_$$ -c ${TEMPDIR}/asctoldac.conf_$$ \
    -o ${TEMPDIR}/chips.cat -t STATS

##################################################################
# replace the seeing estimate (THELI GUI obtains it differently)
##################################################################

${P_LDACRENKEY} -i ${TEMPDIR}/chips.cat -o ${TEMPDIR}/chips.cat1 -t STATS \
    -k SEEING OLD_SEEING SEEING_SDEV OLD_SEEING_SDEV

${P_LDACTOASC} -i ${TEMPDIR}/chips.cat1 -t STATS -s -k IMAGENAME -b > ${TEMPDIR}/catlist_$$

# transfer the image seeing and its rms to the FIELDS table
echo 'COL_NAME  = SEEING'      >  ${TEMPDIR}/asctoldac.conf_$$
echo 'COL_TTYPE = FLOAT'       >> ${TEMPDIR}/asctoldac.conf_$$
echo 'COL_HTYPE = FLOAT'       >> ${TEMPDIR}/asctoldac.conf_$$
echo 'COL_COMM = ""'           >> ${TEMPDIR}/asctoldac.conf_$$
echo 'COL_UNIT = ""'           >> ${TEMPDIR}/asctoldac.conf_$$
echo 'COL_DEPTH = 1'           >> ${TEMPDIR}/asctoldac.conf_$$
echo 'COL_NAME  = SEEING_SDEV' >> ${TEMPDIR}/asctoldac.conf_$$
echo 'COL_TTYPE = FLOAT'       >> ${TEMPDIR}/asctoldac.conf_$$
echo 'COL_HTYPE = FLOAT'       >> ${TEMPDIR}/asctoldac.conf_$$
echo 'COL_COMM = ""'           >> ${TEMPDIR}/asctoldac.conf_$$
echo 'COL_UNIT = ""'           >> ${TEMPDIR}/asctoldac.conf_$$
echo 'COL_DEPTH = 1'           >> ${TEMPDIR}/asctoldac.conf_$$

test -f ${TEMPDIR}/seeing.table_$$ && rm ${TEMPDIR}/seeing.table_$$

exec < ${TEMPDIR}/catlist_$$
while read base
do
    CHIP=1
    test -f ${TEMPDIR}/seeing.list_$$ && rm ${TEMPDIR}/seeing.list_$$
    while [ "${CHIP}" -le "${NCHIPS}" ]
    do
	${P_LDACTOASC} -i ${CATDIR}/${base}_${CHIP}${IMENDING}.cat \
	    -t FIELDS -k FWHM_WORLD -b >> ${TEMPDIR}/seeing.list_$$
	CHIP=$((${CHIP}+1))
    done
    seeing=`${P_GETSTATS} -i ${TEMPDIR}/seeing.list_$$ -m mean`
    seeingsdev=`${P_GETSTATS} -i ${TEMPDIR}/seeing.list_$$ -m sigma`
    echo ${seeing} ${seeingsdev} >> ${TEMPDIR}/seeing.table_$$    
done

${P_ASCTOLDAC} -i ${TEMPDIR}/seeing.table_$$ \
    -o ${TEMPDIR}/seeing_tmp.cat_$$ -t STATS \
    -c ${TEMPDIR}/asctoldac.conf_$$

${P_LDACJOINKEY} -i ${TEMPDIR}/chips.cat1 \
    -o ${CATDIR}/chips.cat5 \
    -p ${TEMPDIR}/seeing_tmp.cat_$$ \
    -t STATS -k SEEING SEEING_SDEV

##################################################################
# END seeing replacement
##################################################################

rm ${TEMPDIR}/*_$$
