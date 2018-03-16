BASHPATH -xv

# takes a series of resampled images, splits it into four
# quadrants and makes a series of TIFF images that can be animated

# $1: script dir
# $2: coaddition directory
# $3-$6: xmin xmax ymin ymax
# $7: tiff min
# $8: tiff max
# $9: range (manual or automatic)
# $10: id
# $11: contrast
# $12: delay
# $13: chipid

DIR=`pwd`

cd $1

. progs.ini

cd $2/movie/cropped_${13}

ls *cropped.fits > ${TEMPDIR}/croppedlist_$$

id=${10}

i=0
cat ${TEMPDIR}/croppedlist_$$ |\
{
    while read file
    do
        j=${i}
	if [ "${i}" -le 99 ]; then
	    j=0${i}
	fi
	if [ "${i}" -le 9 ]; then
	    j=00${i}
	fi
	${P_FITSCUT} -i ${file} -o ${id}_${j}.fits -x $3 $4 $5 $6
	if [ "$9" = "manual" ]; then
	    ${P_FITS2TIFF} -i ${id}_${j}.fits -o ../${id}_${j}.tif -r $7 $8 -b 8
	else
	    ${P_FITS2TIFF} -i ${id}_${j}.fits -o ../${id}_${j}.tif -z -b 8 -c ${11}
	fi
	rm ${id}_${j}.fits
	convert ../${id}_${j}.tif ../${id}_${j}.gif
	i=$(( ${i} + 1 ))
    done
}

# make an animated gif

cd ..
unwantedoutput=`${P_GIFSICLE} --delay ${12} --loop ${id}_*.gif > anim_${id}.gif`
\rm ${id}_*.gif

cd ${DIR}
