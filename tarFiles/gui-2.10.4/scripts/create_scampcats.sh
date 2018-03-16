BASHPATH -xv

# The scripts creates scamp catalogs used for astrometry.

#$1: main directory
#$2: science dir.
#$3: image extension

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini 


cd /$1/$2/

${P_FIND} . -maxdepth 1 -name \*$3.fits | sed 's/\.fits$/\.cat1/g' | \
    ${P_GAWK} -F/ '{print $NF}' > ${TEMPDIR}/allscampfiles.txt_$$

cd /$1/$2/cat/

# The following 'awk' construct cuts away everything after 
# the last '_' in the image names (including the underscore itself);
# we implicitely assume that the image extension DOES NOT contain '_'s. 

EXPOSURES=`cat ${TEMPDIR}/allscampfiles.txt_$$ |\
           ${P_GAWK} '{ n = split($1, a, "_"); 
                        name = ""; 
                        for(i = 1; i < (n-1); i++) {
                          name = name a[i] "_"
                        } 
                        name = name a[n-1]; 
                        print name;}' | ${P_SORT} | uniq`

# The file catlist.txt_$$ is built for the python call to scampcat:
test -f ${TEMPDIR}/catlist.txt_$$ && rm ${TEMPDIR}/catlist.txt_$$

for EXPOSURE in ${EXPOSURES}
do
  # If an old scamp catalogue exists the python call for scampcat.py
  # (see below) would fail!
  test -f ./${EXPOSURE}_scamp.cat && rm -f ./${EXPOSURE}_scamp.cat

  i=1
  CATS=""

  while [ ${i} -le ${NCHIPS} ]
  do
    # The following test for an image implicitely assumes that the
    # image ending does NOT start with a number: obvious but I mention
    # it just in case ....
    # It is necessary as we allow for images with different endings in the 
    # image directories:
    CATNAME=`grep -x ''${EXPOSURE}'_'${i}'[^0-9].*\.cat1' \
             ${TEMPDIR}/allscampfiles.txt_$$`
  
    if [ -n "${CATNAME}" ]; then
      CATS="${CATS} ${CATNAME}"
    fi
    i=$(( ${i} + 1 ))
  done
  echo "${CATS} ./${EXPOSURE}_scamp.cat" >> ${TEMPDIR}/catlist.txt_$$
done

${P_PYTHON} ${S_SCAMPCAT} ${TEMPDIR}/catlist.txt_$$
 
exit 0;
