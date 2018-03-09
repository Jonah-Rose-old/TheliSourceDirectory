BASHPATH -xv

# the script creates sky subtracted images.
# It puts them in the same dir. as the science frames

# $1: directory with images
# $2: extension
# $3: instrument name
# $4: ra1
# $5: ra2
# $6: dec1
# $7: dec2

INSTDIR=instruments_professional

INSTRUMENT=$3

if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

cd $1

test -f skybackground_$2.stat && rm skybackground_$2.stat 

ls -1 *_*$2.fits > ${TEMPDIR}/skysub_images_$$

FILE=`${P_GAWK} '(NR==1) {print $0}' ${TEMPDIR}/skysub_images_$$`

oldbase=""

exec < ${TEMPDIR}/skysub_images_$$
while read file
do
    # extract the basename without the chip number, 
    # i.e. everything before the last "_" in the filename
    BASE=`echo ${file} | ${P_GAWK} '{
          n=split($1,s,"_"); 
	  for (i=1;i<n-1;i++) {printf("%s_",s[i])}; 
	  printf("%s\n"),s[n-1]
	  }'`

    # identify in which chip the region is located
    ls -1 ${BASE}*$2.fits > ${TEMPDIR}/sky2xy_imagelist_$$
    ${P_SKY2XY_HELPER} -i ${TEMPDIR}/sky2xy_imagelist_$$ \
	-c $4 $6 $5 $7 > ${TEMPDIR}/sky2xy_result_$$
    
    image=`${P_GAWK} '{print $1}' ${TEMPDIR}/sky2xy_result_$$`

    if [ ! "${BASE}" = "${oldbase}" ]; then
	section=`${P_GAWK} '{print $2, $3, $4, $5}' ${TEMPDIR}/sky2xy_result_$$`
	result=`${P_IMSTATS} -d 6 ${image} -t -1e8 1e8 -s ${section} | \
	    ${P_GAWK} '($1 != "#") {print $2, $3, $4, $5, $6, $7}'`
	echo ${file} ${result} >> skybackground_$2.stat
    else
	echo ${file} ${oldresult} >> skybackground_$2.stat
    fi

    oldbase=${BASE}
    oldresult=${result}
done
