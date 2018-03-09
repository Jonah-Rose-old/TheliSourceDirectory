BASHPATH

# $1: script dir
# $2: full path to the .ahead file

cd $1

. progs.ini

DIR=$1

# the instrument.ini file:
path=`dirname $2`
base=`basename $2 .ahead`

# extract the NAXIS1 and NAXIS2 keywords
gawk '($0~/SIZEX=/)' ${path}/${base}.ini | \
    sed 's/=/ /g' | \
    sed 's/(//g' | \
    sed 's/)//g' | \
    gawk 'BEGIN{i=1} {for (i=1;i<=NF; i++) x[i] = $i} END {for (i=3; i<=NF; i=i+2) printf "%s\n", x[i]}' > \
    ${TEMPDIR}/naxis1.dat
gawk '($0~/SIZEY=/)' ${path}/${base}.ini | \
    sed 's/=/ /g' | \
    sed 's/(//g' | \
    sed 's/)//g' | \
    gawk 'BEGIN{i=1} {for (i=1;i<=NF; i++) x[i] = $i} END {for (i=3; i<=NF; i=i+2) printf "%s\n", x[i]}' > \
    ${TEMPDIR}/naxis2.dat

# get the CD matrix and CRPIX1/2 from the .ahead file
awk '($1!="END") {if ($1!~/CD2_2/) {printf "%s ", $3} else {printf "%s\n", $3}}' $2 > ${TEMPDIR}/cd.dat

${P_MERGEDAT} -i ${TEMPDIR}/naxis1.dat -j ${TEMPDIR}/naxis2.dat -k ${TEMPDIR}/cd.dat -o ${TEMPDIR}/geometries.txt

rad=`${P_GETREFCATRADIUS} -i ${TEMPDIR}/geometries.txt`
echo "${rad}"
