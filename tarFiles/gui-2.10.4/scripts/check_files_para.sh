BASHPATH -xv

# checks images for integrity. For the moment
# this consists of calculating the mode and
# shifting images that are under/over a given mode
# to a BADMODE directory
# the script cannot be parallelised as chips in
# one parallel channel may be filtered and not in
# another. But we want to filter all chips of an
# image that has ONE or more bad chips
#
# 30.05.04:
# temporary files go to a TEMPDIR directory 
#
# 11.04.2005:
# I rewrote the script to use the 'imstats' program
# instead of the FLIPS based 'immode'.
#
# 14.08.2005:
# The call of the UNIX 'find' program is now done
# via a variable 'P_FIND'.
#
# 09.09.2005:
# I corrected a bug in the FIND command. We need to
# test regular files and links, not only regular files.
#
# 12.09.2005:
# The imstats call is now done chip by chip. This prevents
# an "Argument list too long e_r_r_o_r" for cameras with many
# chips (e.g. MMT with 72).
#
# 05.12.2005:
# - Chips whose NOTUSE or NOTPROCESS flag is set are not considered in
#   the statistics that determine whether to reject exposures.
# - Temporary files are now cleaned up at the end of the
#   script
# - I added a missing TEMPDIR
#
# 28.11.2008:
# temporary files are cleaned at the end of the script!
#
# 30.03.2010:
# parallelised; move only chips whose mode is wrong, not the entire exposure

# $1: main directory (filter)
# $2: science directory
# $3: ending (OFCSFF etc.)
# $4: low limit of files labelled as GOOD
# $5: high limit of files labelled as GOOD
# $6: chips to be processed

# preliminary work:
INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash.include

STATUS=$3
if [ "${STATUS}" = "empty" ]; then
    STATUS=""
fi

test -f ${TEMPDIR}/alldata_$$.dat && rm ${TEMPDIR}/alldata_$$.dat 


for CHIP in $6
do    
    get_statminsection
    
    if [ ${NOTUSE[${CHIP}]:=0} -eq 0 ] && [ ${NOTPROCESS[${CHIP}]:=0} -eq 0 ] 
    then
	${P_IMSTATS} -d 6 `${P_FIND} /$1/$2/ \( -type f -o -type l \) \
            -name \*_${CHIP}${STATUS}.fits -maxdepth 1` \
            -o ${TEMPDIR}/immode.dat_$$ \
	    -s ${statminsection_imstats}
	
	cat ${TEMPDIR}/immode.dat_$$ >> ${TEMPDIR}/alldata_$$.dat
    else
	echo "Chip ${CHIP} will not be used in $0"  
    fi
    i=$(( $i + 1 ))
done

# now get all sets that have to be moved:
${P_GAWK} '($1 != "#") { 
           if ($2 < '$4' || $2 > '$5') {print $1}}' \
               ${TEMPDIR}/alldata_$$.dat > ${TEMPDIR}/move_$$

# now do the moving
while read file
do
    DIR=`dirname ${file}`
    if [ ! -d "${DIR}/BADMODE" ]; then
	mkdir "${DIR}/BADMODE"
    fi
    
    # the following 'if' is necessary as the
    # file(s) may already have been moved
    test -f ${file} && mv ${file} ${DIR}/BADMODE
done < ${TEMPDIR}/move_$$

# clean up:
rm -f ${TEMPDIR}/*_$$.dat *_$$
