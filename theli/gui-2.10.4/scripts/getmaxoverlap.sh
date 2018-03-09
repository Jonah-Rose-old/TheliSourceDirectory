#!/bin/bash -xv

# $1: additional cropping value, e.g. "400" on each side

. progs.ini

if [ ! -d orig ]; then
    mkdir orig
fi

${P_DFITS} *.fits | ${P_FITSORT} CRPIX1 CRPIX2 NAXIS1 NAXIS2 \
 | ${P_GAWK} '($1 !~ /FILE/) {print $2, $3, $4, $5}' > tmp_$$

xlow=`${P_GAWK} 'BEGIN {min = 1000000} {if ($1<min) min = $1} END {print (-1)*min}' tmp_$$`
ylow=`${P_GAWK} 'BEGIN {min = 1000000} {if ($2<min) min = $2} END {print (-1)*min}' tmp_$$`
xhigh=`${P_GAWK} 'BEGIN {min = 1000000} {if (($3-$1)<min) min = $3-$1} END {print min}' tmp_$$`
yhigh=`${P_GAWK} 'BEGIN {min = 1000000} {if (($4-$2)<min) min = $4-$2} END {print min}' tmp_$$`

ls -1 *.fits > tmp_$$
cat tmp_$$ |\
{
    while read file
    do
      crpix1=`${P_DFITS} ${file} | ${P_FITSORT} CRPIX1 | ${P_GAWK} '($1 !~ /FILE/) {print $2}'`
      crpix2=`${P_DFITS} ${file} | ${P_FITSORT} CRPIX2 | ${P_GAWK} '($1 !~ /FILE/) {print $2}'`
      BASE=`basename ${file} .fits`
      xlownew=`echo ${xlow} ${crpix1} | ${P_GAWK} '{print $2+$1+1+"'$1'"}'`
      ylownew=`echo ${ylow} ${crpix2} | ${P_GAWK} '{print $2+$1+1+"'$1'"}'`
      xhighnew=`echo ${xhigh} ${crpix1} | ${P_GAWK} '{print $2+$1-"'$1'"}'`
      yhighnew=`echo ${yhigh} ${crpix2} | ${P_GAWK} '{print $2+$1-"'$1'"}'`
      ${P_FITSCUT} -i ${file} -o ${BASE}_cropped.fits \
               -x ${xlownew} ${xhighnew} ${ylownew} ${yhighnew}
      crpix1new=`echo ${xlownew} ${crpix1} | ${P_GAWK} '{print $2-$1+1}'`
      crpix2new=`echo ${ylownew} ${crpix2} | ${P_GAWK} '{print $2-$1+1}'`
      ${P_REPLACEKEY} ${BASE}_cropped.fits "CRPIX1  = ${crpix1new}" CRPIX1
      ${P_REPLACEKEY} ${BASE}_cropped.fits "CRPIX2  = ${crpix2new}" CRPIX2
      mv ${file} orig/
    done
}

\rm tmp_$$
