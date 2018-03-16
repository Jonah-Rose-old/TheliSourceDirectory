BASHPATH -xv

# 30.05.04:
# temporary files go to a TEMPDIR directory 

# This file transforms reg files created with DS9
# into saoimage format (needed for this pipeline).

# $1: main dir
# $2: science dir

. progs.ini

${P_FIND} /$1/$2/ -maxdepth 1 -name \*.reg > ${TEMPDIR}/filelist1_$$
existreg=`wc ${TEMPDIR}/filelist1_$$ | ${P_GAWK} '{print $1}'`

if [ ! "${existreg}" = "0" ]; then 

    mkdir $1/$2/reg

    mv $1/$2/*.reg $1/$2/reg

    cd $1/$2/reg

    if [ -f ${TEMPDIR}/filelist_$$ ]; then
	rm ${TEMPDIR}/filelist_$$
    fi

    ls -1 *.reg > ${TEMPDIR}/filelist_$$

    mkdir ORIGINALS
    cp *.reg ORIGINALS/

    cat ${TEMPDIR}/filelist_$$ |\
	{
	while read file
	do
	  trans=`grep TRANSFORMED ${file}`
	  if [ ! "${trans}" = "# TRANSFORMED" ]; then
	      ${P_GAWK} '{
                if (NR==1) print "# TRANSFORMED"
                print "# " $0
                }' ${file} > ${file}2
	      sed 's/image//g' ${file}2 > tmp1_$$
	      sed 's/;//g' tmp1_$$ > tmp2_$$
	      sed 's/# polygon/POLYGON/g' tmp2_$$ > tmp3_$$
	      mv tmp3_$$ ${file}
	      \rm tmp2_$$ tmp1_$$
	  fi
	done
	rm *reg2 ${TEMPDIR}/filelist_$$
    }
fi
