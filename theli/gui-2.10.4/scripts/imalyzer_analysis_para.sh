#!/bin/bash -xv

# the script runs analyseldac over the object catalogs

# $1: main directory
# $2: instrument
# $3: chips to be processed

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/$2.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/$2.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/$2.ini

if [ ! -d /$1/cat ]; then
    echo "Could not find catalogs! Exiting."
    exit
fi

if [ ! -d /$1/imalyzer ] || [ ! -d /$1/imalyzer/cat ]; then
    mkdir -p /$1/imalyzer/cat
fi

DIR=`pwd`

for CHIP in $3
do
  # all images matching the filter

  # normally one would provide the filter as an commandline argument
  # but if there is a "*" in it, it would be expanded even though
  # contained in double quotes.
  # the variable THELIFILTER is set in the master script calling this script

  ls -1 /$1/${THELIFILTER} > ${TEMPDIR}/psfimages_step1_${CHIP}_$$

  # select matching chips only (ignoring sky subtracted images)
  ${P_GAWK} 'BEGIN {FS="_"} {a="_"$NF; if (a~/_'${CHIP}'OFC[BCDHP]*\.fits/) print}' \
      ${TEMPDIR}/psfimages_step1_${CHIP}_$$ > ${TEMPDIR}/psfimages_${CHIP}_$$

  # continue with next chip if there is nothing to do
  numimages=`wc ${TEMPDIR}/psfimages_${CHIP}_$$ | ${P_GAWK} '{print $1}'`
  if [ ${numimages} = 0 ]; then
      continue
  fi

  while read file
  do
    BASE=`basename ${file} .fits`
    #
    ${P_LDACADDKEY} -i /$1/cat/${BASE}.cat -o ${TEMPDIR}/tmp11.cat_$$ \
        -t OBJECTS -k nu 1.0 FLOAT ""

#    ${P_LDACCALC} -i ${TEMPDIR}/tmp11.cat_$$ \
#                  -o ${TEMPDIR}/tmp12.cat_$$ -t OBJECTS \
#                  -c "(FLUX_RADIUS);" -n "rg" "" -k FLOAT \
#                  -c "(Xpos-1.0);" -n "x" "" -k LONG \
#                  -c "(Ypos-1.0);" -n "y" "" -k LONG \
#                  -c "(Xpos);" -n "xbad" "" -k FLOAT \
#                  -c "(Ypos);" -n "ybad" "" -k FLOAT

    ${P_LDACCALC} -i ${TEMPDIR}/tmp11.cat_$$ \
                  -o ${TEMPDIR}/tmp12.cat_$$ -t OBJECTS \
                  -c "(FLUX_RADIUS);" -n "rg" "" -k FLOAT \
                  -c "(XWIN_IMAGE);" -n "x" "" -k FLOAT \
                  -c "(YWIN_IMAGE);" -n "y" "" -k FLOAT \
                  -c "(XWIN_IMAGE);" -n "Xpos" "" -k FLOAT \
                  -c "(YWIN_IMAGE);" -n "Ypos" "" -k FLOAT \
                  -c "(XWIN_IMAGE);" -n "xbad" "" -k FLOAT \
                  -c "(YWIN_IMAGE);" -n "ybad" "" -k FLOAT

    ${P_LDACTOASC} -i ${TEMPDIR}/tmp12.cat_$$ -b -t FIELDS \
                   -k FITSFILE SEXBKGND SEXBKDEV -s > ${TEMPDIR}/tmp.asc_$$

    # create a config file for asctoldac on the fly
    
    {
      echo "VERBOSE = DEBUG"       
      echo "COL_NAME  = IMAGE"     
      echo "COL_TTYPE = STRING"    
      echo "COL_HTYPE = STRING"    
      echo 'COL_COMM = ""'         
      echo 'COL_UNIT = ""'         
      echo 'COL_DEPTH = 128'       
      echo "COL_NAME  = SKY_MODE"  
      echo "COL_TTYPE = FLOAT"     
      echo "COL_HTYPE = FLOAT"     
      echo 'COL_COMM = ""'         
      echo 'COL_UNIT = ""'         
      echo 'COL_DEPTH = 1'         
      echo "COL_NAME  = SKY_SIGMA" 
      echo "COL_TTYPE = FLOAT"     
      echo "COL_HTYPE = FLOAT"     
      echo 'COL_COMM = ""'         
      echo 'COL_UNIT = ""'         
      echo 'COL_DEPTH = 1'         
    } > ${TEMPDIR}/asctoldac_tmp.conf_$$ 
    
    ${P_ASCTOLDAC} -i ${TEMPDIR}/tmp.asc_$$ \
        -c ${TEMPDIR}/asctoldac_tmp.conf_$$ -t HFINDPEAKS \
      	-o ${TEMPDIR}/hfind.cat_$$ -b 1 -n "KSB"
    rm ${TEMPDIR}/asctoldac_tmp.conf_$$
    
    # now transfer the HFINDPEAKS table to the SEX catalog
    ${P_LDACADDTAB} -i ${TEMPDIR}/tmp12.cat_$$ -o ${TEMPDIR}/${BASE}_tmp1.cat1 \
        -t HFINDPEAKS -p ${TEMPDIR}/hfind.cat_$$
    
    ${P_LDACFILTER} -i ${TEMPDIR}/${BASE}_tmp1.cat1 \
        -o ${TEMPDIR}/${BASE}_tmp.cat1 \
        -c "(((rg>0.5)AND(rg<10.0))AND(Flag=0));"
    
    # now run analyseldac
    ${P_ANALYSELDAC} -i ${TEMPDIR}/${BASE}_tmp.cat1 \
        -o ${TEMPDIR}/${BASE}_ksb.cat1 \
        -p -x 1 -r -3 -f ${file}
    
    ${P_LDACJOINKEY} -i ${TEMPDIR}/${BASE}_ksb.cat1 -o ${TEMPDIR}/${BASE}_ksb.cat2 \
	-p ${TEMPDIR}/${BASE}_tmp.cat1 -t OBJECTS -k FWHM_IMAGE ALPHA_J2000 DELTA_J2000 \
	MAG_AUTO

    ${P_LDACTOASC} -i ${TEMPDIR}/${BASE}_ksb.cat2 -t OBJECTS \
	-k rg MAG_AUTO -b > ${TEMPDIR}/${BASE}_preaniso.asc

    # if the python fitting fails, just read the last line of the output
    ANISOLINE=`./preaniso.py ${TEMPDIR}/${BASE}_preaniso.asc 0.1 | \
	${P_GAWK} '{line[NR] = $0} END {print line[NR]}'`

    ${P_ANISOTROPY} -i ${TEMPDIR}/${BASE}_ksb.cat2 -c ${ANISOLINE} \
        -o ${TEMPDIR}/${BASE}_ksb.cat3 -j 0.5 -e 2.0
    
    ${P_LDACFILTER} -i ${TEMPDIR}/${BASE}_ksb.cat3 \
        -o /$1/imalyzer/cat/${BASE}_stars.cat -c "(cl=2);"

#    rm ${TEMPDIR}/${BASE}_tmp1.cat1
#    rm ${TEMPDIR}/${BASE}_tmp.cat1
#    rm ${TEMPDIR}/${BASE}_ksb.cat[1-3]
#    rm ${TEMPDIR}/${BASE}_preaniso.asc

  done < ${TEMPDIR}/psfimages_${CHIP}_$$
  
#  rm ${TEMPDIR}/psfimages_${CHIP}_$$
done

#rm ${TEMPDIR}/*_$$
