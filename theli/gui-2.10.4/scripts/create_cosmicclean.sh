#!/bin/bash -xv

# $1: dir

. progs.ini

dir=`pwd`

cd $1

ls *resamp.fits > /${TEMPDIR}/imagelist_$$

# get some "global" parameters
nimages=`wc /${TEMPDIR}/imagelist_$$ | ${P_GAWK} '{print $1}'`

xmax=`${P_DFITS} coadd.fits | ${P_FITSORT} NAXIS1 | ${P_GAWK} '($1!="FILE") {print $2}'`

ymax=`${P_DFITS} coadd.fits | ${P_FITSORT} NAXIS2 | ${P_GAWK} '($1!="FILE") {print $2}'`

xrefc=`${P_DFITS} coadd.fits | ${P_FITSORT} CRPIX1 | ${P_GAWK} '($1!="FILE") {print $2}'`

yrefc=`${P_DFITS} coadd.fits | ${P_FITSORT} CRPIX2 | ${P_GAWK} '($1!="FILE") {print $2}'`

echo ${nimages} ${xmax} ${ymax} ${xrefc} ${yrefc} > cosmicclean.list

# get the individual image data

cat /${TEMPDIR}/imagelist_$$ |\
{
  while read file
  do
    BASE=`basename ${file} .fits`
    xsize=`${P_DFITS} ${file} | ${P_FITSORT} NAXIS1 | ${P_GAWK} '($1!="FILE") {print $2}'`
    ysize=`${P_DFITS} ${file} | ${P_FITSORT} NAXIS2 | ${P_GAWK} '($1!="FILE") {print $2}'`
    xref=`${P_DFITS} ${file} | ${P_FITSORT} CRPIX1 | ${P_GAWK} '($1!="FILE") {print $2}'`
    yref=`${P_DFITS} ${file} | ${P_FITSORT} CRPIX2 | ${P_GAWK} '($1!="FILE") {print $2}'`
    flxscale=`${P_DFITS} ${file} | ${P_FITSORT} FLXSCALE | ${P_GAWK} '($1!="FILE") {print $2}'`

    echo ${file} ${BASE}.weight.fits ${xsize} ${ysize} ${xref} ${yref} \
	${flxscale} >> cosmicclean.list

  done
}
