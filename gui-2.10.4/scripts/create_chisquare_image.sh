BASHPATH -xv

# $2: dir with images
# $3-$6: box coordinates of a region with empty sky
# $7: mode (chisquare or mean)
# $8: do weight masking ('mask' or 'nomask')

cd $1

. progs.ini

. bash.include

cd $2

test -f rmslist && rm rmslist
touch rmslist

cp chisquarelist weightlist

# create a weightlist
test -f weightlist2 && rm weightlist2
touch weightlist2

# 'wexist' remains 0 if all weight images exist
wexist=0

cat weightlist |\
{
  while read file
  do
    base=`basename $file .fits`
    echo "${base}.weight.fits" >> weightlist2
    if [ ! -f ${base}.weight.fits ]; then
	wexist=$((${wexist}+1))
    fi
  done
}

# if a chisquare image was requested, then do the noise normalisation

if [ "$7" = "chisquare" ]; then
    cat chisquarelist |\
	{
	while read file
	  do
	  base=`basename $file .fits`
	  rms=`${P_IMSTATS} -d 6 -s $3 $4 $5 $6 $file | ${P_GAWK} '($1 != "#") {print $7}'`
	  ${P_IC} '%1 '$rms' /' $file > ${base}_chinorm.fits
	  echo $rms >> rmslist
	done
    }

    rmsinv=`${P_GAWK} 'BEGIN{s=0} {s=s+1./$1} END {print s}' rmslist`
    ls *_chinorm.fits > chisquarelist
    \rm rmslist
fi

list=""
weightlist=""
numimages=`wc chisquarelist | ${P_GAWK} '{print $1}'`
icarg=""
weightarg=""

# create the argument for the image combination using "ic"
i=2
while [ "${i}" -le "${numimages}" ]
  do
  icarg="${icarg} %${i} +"
  i=$((${i}+1))
done

# create the argument for the weight mask
i=2
while [ "${i}" -le "${numimages}" ]
  do
  weightarg="${weightarg} %${i} *"
  i=$((${i}+1))
done

# renormalise either by the total noise or the number of exposures
if [ "$7" = "chisquare" ]; then
    icarg=${icarg}" ${rmsinv} "
    sumname="elum_chisquare.fits"
else
    icarg=${icarg}" ${numimages} "
    sumname="elum_mean.fits"
fi

# create the mask file
if [ "${wexist}" = "0" ]; then
    cat weightlist2 |\
    {
      while read file
      do
	weightlist="${weightlist} $file"
      done
      ${P_IC} '%1 '"${weightarg}"'' $weightlist > mask_tmp.fits
      ${P_IC} '%1 1. %1 fabs 1.0e-6 < ?' mask_tmp.fits > mask.fits
      \rm mask_tmp.fits
    }
fi

# do the image combination
cat chisquarelist |\
{
  while read file
  do
    list="$list $file"
  done

  ${P_IC} '%1 '"${icarg}"' /' $list > ${sumname}

  if [ "${wexist}" = "0" ] && [ "$8" = "mask" ]; then
      ${P_IC} '%1 %2 *' ${sumname} mask.fits > ${sumname}_tmp
      mv ${sumname}_tmp ${sumname}
  fi
}

# we need to rename the seeing as it won't be representative anymore
oldseeing=`get_key ${sumname} SEE_IMAG`
if [ "${oldseeing}"_A != "_A" ]; then
    ${P_REPLACEKEY} ${sumname} "SEE_OLD = ${oldseeing}" SEE_IMAG 
fi

# delete the temporary stuff
if [ "$7" = "chisquare" ]; then
    \rm *chinorm.fits rmslist
fi

test -f mask.fits && rm mask.fits
test -f weightlist && rm weightlist
test -f weightlist2 && rm weightlist2
test -f chisquarelist && rm chisquarelist
