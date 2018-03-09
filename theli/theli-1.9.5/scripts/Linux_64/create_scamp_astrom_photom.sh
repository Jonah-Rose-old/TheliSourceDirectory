#!/bin/bash -u

# -----------------------------------------------------------------------
# File Name:           create_scamp_astrom_photom.sh
# Author:              Thomas Erben (terben@astro.uni-bonn.de)
# Last modified on:    10.05.2014
# Description:         Performs astrometric and relative photometric
#                      calibration of THELI sets with scamp V1.4.0-V1.15.0
# -----------------------------------------------------------------------

# Script history information:
# 14.09.2007:
# Project started
#
# 17.09.2007:
# - I updated the documentation
# - I fixed some bugs concerning the location of
#   resulting files
# - I appended NTHREADS to the scamp command line
#   which uses a multi-processor architecture if available.
#   NTHREADS is set to NPARA.
#
# 24.09.2007:
# - Update of documentation text
# - Bug fix for the location of catalogue files
# - FITS conform output of the RZP header keyword in
#   scamp catalogues
#
# 02.10.2007:
# I made the script a bit more robust by adding more sanity
# checks.
#
# 21.12.2007:
# - I extended the script so that different mosaic configurations
#   within one astrometric set can be handled now. To this end we
#   create an artificial external header containing the keyword
#   MISSCHIP listing the missing chips. This keyword is then added
#   to the FITS cards distinguishing astrometric runs.
# - One explicit call to 'ldactoasc' was replaced by an implicit one.
#
# 30.12.2007:
# - I extended the script to treat data from different image directories
#   simultaneously.
# - Multiple photometric contexts are now handled correctly.
#
# 08.12.2008:
# I updated the script documentation
#
# 11.01.2008:
# The script now makes use of focal plane information if present!
# If an external header with name ${INSTRUMENT}.ahead is present in
# the reduction directory it is interpreted as containing focal plane
# information (first order astrometric information). In this case
# 'scamp' is run with the '-MOSAIC_TYPE FIX_FOCALPLANE' option.
# Otherwise (no focal plane information available) we use
# '-MOSAIC_TYPE UNCHANGED'.
#
# 16.01.2008:
# I estimate the 'THELI' relative flux scaling zeropoint. Up to
# now I used the scamp value which is, however, scaled to a fixed
# magnitude (scamp config file). This value is not consistent with
# the THELI convention where the flux scale for each image is defined
# as flxscale=10**(-0.4*relzp)/exptime
#
# 18.01.2008:
# Bug fix in the creation of individual header files; missing catalogs
# were not taken into account properly.
#
# 22.07.2008:
# The directory names where scamp performs its actions carry the name
# of the used standardstar catalogue now. The same applies for the
# final header directories.
#
# 07.08.2008:
# Bug Fix for the new naming convention of appending the name of the
# reference cat to directory names
#
# 21.12.2008:
# The python interpreter is now called via a variable which is defined
# in 'progs.ini'.
#
# 20.01.2009:
# It was found that CFHTS-DEEP files 'shuffled' in time, when given to
# scamp, lead to better results! Better results means better looking
# astrometry control plots!
#
# 23.02.2009:
# Changes to make the creation of scamp MEF catalogues and scamp header
# files much more efficient:
# - The script was adapted to the new structure of scmapcat.py which
#   now can convert multiple images to scamp MEF files with a single
#   call. of the Python interpreter; before it was called one for
#   each input image!
# - Unnecessary splittings of the global ahead file are now avoided.
#   It is splitted once and the splittings are reused for each image;
#   before the same file splitting was done for each input image
#   separately!
#
# 14.04.2009:
# - I made the code more efficient in writing the scamp ahead files.
# - The script now has different processing modes for optimal astrometric
#   and relative photometric calibration.
#
# 05.02.2010:
# I introduced the an estimate of the maximum positional offset of
# sources as a comand line arguments. For WFI, the initial astrometry can be
# off by up to two arcmin!
#
# 31.03.2010:
# I fixed an error in the checking of the number of command line
# arguments - the case when no arguments were provided was not treated
# correctly.
#
# 17.09.2010:
# The mosaic type in case 'ahead' files are present is now 'SAME_CRVAL'
# and no longer 'FIX_FOCALPLANE' - recommendation by Emmanuel in the
# Astrometric scamp forum.
#
# 24.11.2010:
# We limit the maximum parallel channels for scamp to 16. Ludos new
# jade has considerably more and scamp refuses to process with a higher
# setting.
#
# 30.12.2010:
# We can now provide manually created standard star catalogues.
#
# 07.01.2011:
# Manual reference catalogues enter astrometric calibration with an
# astrometric weight of one now. Previously I set this to 1000 but
# this was only necessary because of wrong positional errors in some
# manually created standardstar catalogue.
#
# 27.05.2011:
# I corrected a bug that caused a non-unique identification of catalogue
# names (line CATNAME=`grep ...) for images with names 'image.fits' and
# 'ximage.fits' where 'image' is an arbitrary string. (Thanks to Mischa)
#
# 06.03.2014:
# - I modified the creation of the 'initial' scamp headers to simlify and
#   speed up this process. It involved the creation of many small text files
#   which we avoid as much as possible now.
# - The initial filtering of the LDAC catalogues (Flags and object size) is
#   done in parallel with the GNU parallel script.
# - The script checks whether the catalogue has windowed position quantities.
#   If yes, it uses them instead of the 'normal' unwindowed ones.
# - The relative photometric zeropoints are now correct if there are different
#   phoptometric groups (islands of images instead of a big contiguous patch).
#   In this case, the relative zeropoint has to be estimated separately for
#   each group.
#
# 26.03.2014:
# - temporary files were not written to TEMPDIR but always to the reduce
#   directory.
# - I accelerated creation of the final, single chip scamp header
#   files.
#
# 25.04.2014:
# - Bug fix: I did not test properly whether the necessary catalogue
#   directory for given data locations existed.
# I refined the script for processing short exposed images with a very low
# number of objects:
# - In the case of windowed position quantities we use them to filter the
#   initial catalogue. We saw that we can keep more good objects with a lower
#   threshhold on the semi minor axis size of the winodwed quantity BWIN_IMAGE
#   rather than on B_IMAGE.
# - For short exposed images we use polynomials of order 2 to map distortions.
#   For long exposed data the order is 3 as it already always was.
#
# 10.05.2014:
# The size limits (BWIN_IMAGE) for objects entering calibration was
# too small for sources of OMEGACAM@VST in exceptional seeing
# conditions - fixed.

# File inclusions:
. ./${INSTRUMENT:?}.ini
. ./bash_functions.include
theli_start "$*"

# define THELI_DEBUG and some other variables because of the '-u'
# script flag (the use of undefined variables will be treated as
# errors!)  THELI_DEBUG is used in the cleanTmpFiles function.
#
THELI_DEBUG=${THELI_DEBUG:-""}
P_SCAMP=${P_SCAMP:-""}
P_ACLIENT=${P_ACLIENT:-""}
DIR=${DIR:-""}

##
## function definitions:
##
function printUsage
{
  echo "SCRIPT NAME:"
  echo "    create_scamp_astrom_photom.sh"
  echo ""
  echo "ARGUMENTS:"
  echo "    1. main_dir.         1"
  echo "    2. set_dir.          1"
  echo "    3. image_extension   1"
  echo "    4. main_dir.         2"
  echo "    5. set_dir.          2"
  echo "    6. image_extension   2"
  echo "    ."
  echo "    ."
  echo "    n-2. (n-3) max. positional offset (arcsec)"
  echo "    n-1. (n-2) SHORT/LONG (short or long exposed data)"
  echo "    n. (n-1) standardstar_catalog"
  echo "   (n.) MODE (OPTIONAL: astrom, photom or ALL;"
  echo "              default is ALL) "
  echo ""
  echo "    An arbitrary number of blocks 'main_dir.', 'set_dir.' and"
  echo "    'image_extension' can be provided."
  echo ""
  echo "    The 'max. positional offset' is an estimate of the initial maximum"
  echo "    linear offset of the data to the standardstar catalogue."
  echo ""
  echo "    SHORT should be set very short exposed data which can have 20 or"
  echo "    less objects per chip (e.g. short exposed u-band data)."
  echo " "
  echo "    If a MODE is provided in the nth argument the script calls scamp"
  echo "    with optimised parameters for astrometry or relative"
  echo "    photometry. Omitting this argument (MODE ALL) runs scamp with"
  echo "    optimised parameters for astrometric calibration."
  echo ""
  echo "    \${SET} in the description below refers to the directory(ies)"
  echo "    /main dir./set dir./"
  echo ""
  echo "DESCRIPTION:"
  echo ""
  echo "    This script performs astrometric and relative photometric"
  echo "    calibration of THELI sets with Emanuel Bertins scamp program. The"
  echo "    current version of the script was tested mainly against"
  echo "    scamp-V1.15.2; The scamp program itself is not contained in the"
  echo "    THELI pipeline package and needs, together with 'cdsclient' and"
  echo "    the Python module 'pyfits', to be installed separately (see the"
  echo "    DEPNDENCY section below)"
  echo "    "
  echo "    The script substitutes the scripts"
  echo "    "
  echo "    - create_astrometrix_astrom(_run).sh"
  echo "    - create_photometrix.sh"
  echo "    "
  echo "    of the THELI ASTROMETRIX/PHOTOMETRIX processing. It ends with"
  echo "    header files in a \${SET}/headers_scamp_\${STANDARD}_\${MODE}"
  echo "    directory (the \${STANDARD} stands for the used standardstar"
  echo "    catalogue. In case of a manually created standardstar catalogue it"
  echo "    is 'MANUAL'. the MODE for the use processing mode; the MODE suffix"
  echo "    is omitted if the MODE is ALL).  The resulting header files can"
  echo "    then directly be used for image co-addition.  To use the scamp"
  echo "    headers the 'prepare_coadd_swarp.sh' script has to be called with"
  echo "    the '-eh headers_scamp_..' option."
  echo ""
  echo "    If this script is run with MODES astrom and photom on some data set"
  echo "    the resulting headers can later be combined with the script "
  echo "    'merge_scamp_cats.sh'."
  echo "    "
  echo "    !! NOTE:"
  echo "       A call to 'create_absphotom_photometrix.sh' after this script"
  echo "       is still necessary to calculate the absolute zeropoint for a"
  echo "       coadded image "
  echo "    !!"
  echo ""
  echo "    The standardstar_catalog (n-1 th ot nth command line argument) can"
  echo "    be any astrometric catalogue supported by scamp. If a manually created"
  echo "    standardstar catalogue (in LDAC format) should be used, provide the"
  echo "    full, absolute path to that catalogue."
  echo ""
  echo "    Unless with Astrometrix it is possible to astrometrically and"
  echo "    photometrically calibrate multiple, associated sets"
  echo "    (e.g. multi-coour observations of the same target) simultaneously."
  echo "    The script performs its tasks and puts resulting files in the"
  echo "    directory \${SET}/astrom_photom_scamp (of the first given image"
  echo "    set) and subdirectories therein. From there, final headers are"
  echo "    moved to the appropriate directories at the end of processing."
  echo "         "
  echo "IMPORTANT DEPENDENCIES:"
  echo "    (1) The script calls the Python script 'scampcat.py' which creates"
  echo "        scamp MEF exposure catalogues from the individual THELI chip "
  echo "        catalogues. This Python script itself depends on pyfits."
  echo "    (2) scamp uses the configuration file 'scamp_astrom_photom.scamp'"
  echo "        which is set to define scamp astrometric contexts on the"
  echo "        basis of the FILTER and RUN keywords. "
  echo "    (3) The script checks whether a file ${INSTRUMENT}.ahead is present"
  echo "        in your reduction directory. If yes, it interprets it as a scamp"
  echo "        'additional' header containing 'focal plane' (first order "
  echo "        astrometric) information overriding corresponding entries in image"
  echo "        headers. In this case scamp is run with the "
  echo "        '-MOSAIC_TYPE SAME_CRVAL' option. We use '-MOSAIC_TYPE UNCHANGED' "
  echo "        otherwise."
  echo ""
  echo "TECHNICAL/IMPLEMENTATION INFORMATION:    "
  echo "    Most parts of the scripts concern transformations between"
  echo "    files that scamp needs/returns and the formats needed by"
  echo "    our THELI pipeline. Main differences and some more (random)"
  echo "    comments are:"
  echo "    "
  echo "    (1) Our THELI 'runs' are called 'astrometric contexts' in the scamp"
  echo "        language."
  echo "    (2) For each multi-chip exposure scamp expects ONE object catalogue"
  echo "        in MEF format. In THELI we work with individual catalogues"
  echo "        for each chip. If we would feed all individual chips into scamp"
  echo "        they are not recognised as being part of a specific exposure."
  echo "    (3) scamp outputs ONE header file with all chips for each exposure."
  echo "        In THELI we need one header per chip."
  echo "    (4) scamp only gives a final flux scale w.r.t. a specified zeropoint."
  echo "        We need the 'relative' magnitude offsets of each exposure w.r.t."
  echo "        a zero mean offset."
  echo "    (5) Absolute magnitudes are calculated in THELI outside the "
  echo "        astrometry/relative photometry step. For the time being we"
  echo "        do not provide the necessary interface (header keywords) to use "
  echo "        the absolute calibration within scamp."
  echo "    (6) Note that names for astrometric standardstar catalogues are slightly"
  echo "        different for scmap and THELI/ASTROMETRIX; "
  echo "        e.g. USNOB1 (THELI), but USNO-B1 (scamp)."
  echo ""
  echo "AUTHOR:"
  echo "    Thomas Erben (terben@astro.uni-bonn.de)"
  echo ""
}


function cleanTmpFiles
{
  if [ -z ${THELI_DEBUG} ]; then
    echo "Cleaning temporary files for script $0"
    test -f ${TEMPDIR}/tmp.txt_$$            && \
         rm ${TEMPDIR}/tmp.txt_$$
    test -f ${TEMPDIR}/tmp1.txt_$$           && \
         rm ${TEMPDIR}/tmp1.txt_$$
    test -f ${TEMPDIR}/photdata_relzp.txt_$$ && \
         rm ${TEMPDIR}/photdata_relzp.txt_$$
    test -f ${TEMPDIR}/catlist.txt_$$        && \
         rm ${TEMPDIR}/catlist.txt_$$
    test -f ${TEMPDIR}/exposurefiles.txt_$$  && \
         rm ${TEMPDIR}/exposurefiles.txt_$$
    test -f ${TEMPDIR}/allscampfiles.txt_$$  && \
         rm ${TEMPDIR}/allscampfiles.txt_$$
    test -f ${TEMPDIR}/chips.txt_$$          && \
         rm ${TEMPDIR}/chips.txt_$$
  else
    echo "Variable THELI_DEBUG set! No cleaning of temp. files in script $0"
  fi
}

# Handling of program interruption by CRTL-C
trap "echo 'Script $0 interrupted!! Cleaning up and exiting!'; \
      cleanTmpFiles; exit 1" INT

##
## initial sanity checks
##
# check whether we have the external 'scamp' and 'aclient' programs at all:
if [ -z ${P_SCAMP} ] || [ -z ${P_ACLIENT} ]
then
  echo "You need the external 'scamp' AND 'aclient' programs to"
  echo "use this script! The necessary variable(s) in"
  echo "your progs.ini seem(s) not to be set! Exiting!!"
  exit 1;
fi

# check validity of command line arguments:
# First a check on the number of the arguments:
if [ $# -lt 6 ] || \
  ([ $(( ($# - 3) % 3 )) -ne 0 ] && [ $(( ($# - 4) % 3 )) -ne 0 ]); then
  printUsage
  exit 1
fi

if [ $(( ($# - 3) % 3 )) -eq 0 ]; then
  # The number of different image directories we have to consider:
  NDIRS=$(( ($# - 2) / 3 ))

  # get the estimate for the maximum positional error of sources:
  POSMAXERRARG=$(( $# - 2 ))
  POSMAXERR=${!POSMAXERRARG}

  # get the data type (short or long exposurs)
  DATAKINDARG=$(( $# - 1 ))
  DATAKIND=${!DATAKINDARG}

  # get the used reference catalogue into a variable
  STARCAT=${!#}

  # the 'optimal' mode for the scamp run here is 'all':
  MODE=all

  # the SUFFIX for directory names:
  SUFFIX=${STARCAT}
else
  NDIRS=$(( ($# - 2) / 3 ))
  POSMAXERRARG=$(( $# - 3 ))
  POSMAXERR=${!POSMAXERRARG}
  DATAKINDARG=$(( $# - 2 ))
  DATAKIND=${!DATAKINDARG}
  STARCATARG=$(( $# - 1 ))
  STARCAT=${!STARCATARG}

  MODE=${!#}

  if [ "${MODE}" != "all" ] && [ "${MODE}" != "astrom" ] && \
     [ "${MODE}" != "photom" ]; then
    printUsage
    exit 1
  fi
fi

# the SUFFIX for directory names. If we have a user provided astrometric
# standard star catalogue the sufix is 'MANUAL'. Otherwise it is the
# name of the standard star catalogue.
SUFFIX=${STARCAT}
if [ -f ${STARCAT} ]; then
  SUFFIX=MANUAL
fi

if [ "${MODE}" != "all" ]; then
  SUFFIX=${SUFFIX}_${MODE}
fi


# The location of the reduction directory; we need it to have
# a reference to it below:
REDUCEDIR=`pwd`

# Test existence of image directory(ies) and create headers_scamp
# directories:
i=1
j=2
k=1
while [ ${k} -le ${NDIRS} ]
do
  if [ -d /${!i}/${!j}/cat ]; then
    if [ -d /${!i}/${!j}/headers_scamp_${SUFFIX} ]; then
      echo "deleting old /${!i}/${!j}/headers_scamp_${SUFFIX}"
      rm -rf /${!i}/${!j}/headers_scamp_${SUFFIX}
    fi
    mkdir /${!i}/${!j}/headers_scamp_${SUFFIX}
  else
    echo "Can't find directory /${!i}/${!j}/cat";
    exit 1;
  fi
  i=$(( ${i} + 3 ))
  j=$(( ${j} + 3 ))
  k=$(( ${k} + 1 ))
done

##
## Here the main script starts:
##

# all processing is performed in the 'first' image directory in
# a astrom_photom_scamp subdirectory:
cd /$1/$2/

if [ -d astrom_photom_scamp_${SUFFIX} ]; then
  echo "deleting old /$1/$2/astrom_photom_scamp_${SUFFIX}"
  rm -rf astrom_photom_scamp_${SUFFIX}
fi

mkdir -p astrom_photom_scamp_${SUFFIX}/cat
mkdir astrom_photom_scamp_${SUFFIX}/headers
mkdir astrom_photom_scamp_${SUFFIX}/plots

cd astrom_photom_scamp_${SUFFIX}/cat

# filter input catalogues to reject bad objects
i=1
j=2
k=3
l=1
NCATS=0
POSQUAN=""

while [ ${l} -le ${NDIRS} ]
do
  FILES=`${P_FIND} /${!i}/${!j}/cat/ -maxdepth 1 -name \*${!k}.cat`

  # we need to determine whether we need to use 'normal' or 'windowed'
  # object quantities in scamp. Newer versions of THELI should use
  # windowed but we still want to be able to process old catalogues:
  if [ "${POSQUAN}" = "" ]; then
    CAT=`echo ${FILES} | cut -d " " -f1`

    ${P_LDACTESTEXIST} -i ${CAT} -t LDAC_OBJECTS -k XWIN_IMAGE
    if [ "$?" -eq "0" ]; then
      POSQUAN="WINDOWED"
    else
      POSQUAN="UNWINDOWED"
    fi
  fi

  test -f ${TEMPDIR}/tmp.txt_$$ && rm ${TEMPDIR}/tmp.txt_$$

  for CAT in ${FILES}
  do
    NCATS=$(( ${NCATS} + 1 ))

    BASE=`basename ${CAT} ${!k}.cat`

    # The file tmp.txt_$$ is needed as input for the parallel catalogue
    # filtering (see below)
    echo "${CAT} ${BASE}${!k}.ldac" >> ${TEMPDIR}/tmp.txt_$$

    # The follwowing two arrays are necessary to put headers
    # to the correct directories lateron.
    CATBASE[${NCATS}]=${BASE}
    CATDIR[${NCATS}]=/${!i}/${!j}
  done

  # we filter away flagged objects except THOSE which are saturated!
  # we also require a minimum size (semi minor axis) of two pixels
  SIZEQUAN="B_IMAGE"
  SIZEVAL="2.0"
  if [ "${POSQUAN}" = "WINDOWED" ]; then
    SIZEQUAN="BWIN_IMAGE"
    # a limit of 1.0 was too large for OMEGACAM@VST images in exceptionally
    # good seeing conditions. The new value of 0.9 is still sufficiently
    # far away from the size of cosmic rays (around 0.5)
    SIZEVAL="0.9"
  fi
  ${P_PERL} ${S_PARALLEL} -j ${NPARA} -q --colsep ' ' \
      ${P_LDACFILTER} -i {1} -t LDAC_OBJECTS \
          -c "((FLAGS<16)AND(${SIZEQUAN}>${SIZEVAL}));" \
          -o {2} < ${TEMPDIR}/tmp.txt_$$

  i=$(( ${i} + 3 ))
  j=$(( ${j} + 3 ))
  k=$(( ${k} + 3 ))
  l=$(( ${l} + 1 ))
done

# check whether something needs to be done at all:
if [ ${NCATS} -gt 0 ]; then
  # The aim of the next code part (up to the python call to S_SCAMPCAT)
  # is to create merged MEF catalogues for each exposures. In addition,
  # scamp ahaed files need to be created. Most of the fiddling in the
  # following is necessary because catalogues for individual chips might
  # not be present (bad chips)

  # First a simple list of all available individual catalogues.  Its main
  # purpose is to use it later to check whether certain catalogues are
  # present or not.
  ${P_FIND} . -name \*ldac |\
     ${P_GAWK} -F/ '{print $NF}' > ${TEMPDIR}/allscampfiles.txt_$$

  # The following construct cuts away everything after the last
  # '_' in the image names (including the underscore itself); we
  # implicitely assume that the image extension DOES NOT contain '_'s.
  EXPOSURES=`${P_GAWK} 'BEGIN {FS="_"; OFS="_"} {
               $NF = ""; print $0}' ${TEMPDIR}/allscampfiles.txt_$$ |\
             sed -e 's/_$//' | sort | uniq`

  # Modify the initial instrument ahaed file with a default 'MISSCHIP'
  # setting which should be valiid for most exposures. This default
  # file is then just copied and linked for exposures following the default
  # rules (all chips of the mosaic are valid):
  AHEAD=0
  if [ -f ${REDUCEDIR}/${INSTRUMENT}.ahead ]; then
    ${P_GAWK} '{if($0 !~ /^END/) {
        print $0
      } else {
        print "MISSCHIP= '\''0'\''"; print $0;
      }}' ${REDUCEDIR}/${INSTRUMENT}.ahead > ./${INSTRUMENT}_default.ahead

    AHEAD=1
  fi

  # The file catlist.txt_$$ is built for the python call to scampcat:
  test -f ${TEMPDIR}/catlist.txt_$$ && rm ${TEMPDIR}/catlist.txt_$$

  for EXPOSURE in ${EXPOSURES}
  do
    # If an old scamp catalogue exists the python call for scampcat.py
    # (see below) would fail!
    test -f ./${EXPOSURE}_scamp.cat && rm -f ./${EXPOSURE}_scamp.cat

    i=1
    CATS=""
    MISSCHIPSTRING=0   # contains the missing chips in the form of a pasted
                       # string. If e. g. chips 19 and 25 are bad the variable
                       # would contain 02519 (read 0_25_19; '0' is always at
                       # the beginning)
    MISSCHIPS=""       # contains missed strings, e.g. "29 15" if chips 29
                       # and 15 are missing. The variable is needed the modify
                       # the default ahead file is necessary. Missing chips
                       # must appear in decreasing order here.

    # add the current exposure to the catalogue list to create an exposure
    # catalogue for the scamp call (python call below) and deal with missing
    # chips:
    grep "^${EXPOSURE}" ${TEMPDIR}/allscampfiles.txt_$$  | \
      ${P_GAWK} -F_ '{a = $NF; gsub(/[^0-9]/, "", a); print a, $0}' |\
      ${P_SORT} -n -k1,1 | tee ${TEMPDIR}/tmp.txt_$$ |\
      ${P_GAWK} '{printf("%s ", $2)} END {
         print "./'${EXPOSURE}'_scamp.cat"}' >> ${TEMPDIR}/catlist.txt_$$

    MISSCHIPS="`seq 1 ${NCHIPS} | \
                ${P_GAWK} '{print $1}' - ${TEMPDIR}/tmp.txt_$$ |\
                ${P_SORT} -nr | uniq -c | awk '($1 == 1) {print $2}'`"

    # Dummy external header containing focal plane and missing
    # chip information.
    # They are used to distinguish different chip configurations
    # in an, otherwise, unique astrometric conetxt.
    test -f ./${EXPOSURE}_scamp.ahead && rm ./${EXPOSURE}_scamp.ahead

    # If there are no missing chips we just link the default ahead file.
    # Otherwise we create a special one:
    if [ "${MISSCHIPS}" = "" ]; then
      ln -s ./${INSTRUMENT}_default.ahead ./${EXPOSURE}_scamp.ahead
    else
      for CHIP in ${MISSCHIPS}
      do
        MISSCHIPSTRING="${MISSCHIPSTRING}${CHIP}"
      done

      sed -e 's/MISSCHIP= '\''0'\''/MISSCHIP= '\'${MISSCHIPSTRING}\''/' \
        ./${INSTRUMENT}_default.ahead > ${TEMPDIR}/tmp.txt_$$

      # remove from the ahead file all sections that correspond to
      # missing chips.
      for CHIP in ${MISSCHIPS}
      do
        ${P_GAWK} 'BEGIN {chipsread = 0} {
                   if(chipsread != ('${CHIP}' - 1)) {
                     print $0;
                   }
                   if($0 ~ /^END/) {
                     chipsread++;
                   }}' ${TEMPDIR}/tmp.txt_$$ > ${TEMPDIR}/tmp1.txt_$$
        mv ${TEMPDIR}/tmp1.txt_$$ ${TEMPDIR}/tmp.txt_$$
      done
      mv ${TEMPDIR}/tmp.txt_$$ ./${EXPOSURE}_scamp.ahead
    fi
  done

  ${P_PYTHON} ${S_SCAMPCAT} ${TEMPDIR}/catlist.txt_$$

  # now call scamp:
  cd ../headers

  # scamp mosaic type dependent on the availability of a first order
  # astrometric solution:
  if [ ${AHEAD} -eq 1 ]; then
    MOSAICTYPE="-MOSAIC_TYPE SAME_CRVAL"
  else
    MOSAICTYPE="-MOSAIC_TYPE UNCHANGED"
  fi

  # for some reason scamp gives better results if the
  # 'ordering' of files is shuffled in time! Do not ask!
  if [ "${MODE}" != "photom" ]; then
    FILES=`${P_FIND} ../cat/ -name \*scamp.cat |\
           awk '{line[NR] = $0} END { srand();
             for (i = 1; i <= NR; i++) {
               n = int(rand() * NR) + 1;
               temp = line[i];
               line[i] = line[n];
               line[n] = temp;
             }
             for (i = 1; i <= NR; i++) {
               print line[i];
             }}'`
    # For astrometric calibration we can
    # consider saturated sources:
    FLAGS="-FLAGS_MASK 240"
  else
    FILES=`${P_FIND} ../cat/ -name \*scamp.cat | ${P_SORT}`
    # For photometric calibration we do not want to
    # consider saturated sources:
    FLAGS="-FLAGS_MASK 244"
  fi

  # scamp (at least up to V1.7) has an upper limit on the parallel
  # channels of 16.  With Ludos new jade we reached this:
  if [ ${NPARA} -gt 16 ]; then
    NPARA=16
  fi

  ASTREF=""
  if [ -f ${STARCAT} ]; then
    ASTREF="-ASTREF_CATALOG FILE -ASTREFCAT_NAME ${STARCAT}"
  else
    ASTREF="-ASTREF_CATALOG ${STARCAT}"
  fi

  # positional quantities to use:
  if [ "${POSQUAN}" = "WINDOWED" ]; then
    POSPAR="-CENTROID_KEYS  XWIN_IMAGE,YWIN_IMAGE"
    POSPAR="${POSPAR} \
            -CENTROIDERR_KEYS ERRAWIN_IMAGE,ERRBWIN_IMAGE,ERRTHETAWIN_IMAGE"
    POSPAR="${POSPAR} -DISTORT_KEYS XWIN_IMAGE,YWIN_IMAGE"
  else
    POSPAR="-CENTROID_KEYS  X_IMAGE,Y_IMAGE"
    POSPAR="${POSPAR} \
            -CENTROIDERR_KEYS ERRA_IMAGE,ERRB_IMAGE,ERRTHETA_IMAGE"
    POSPAR="${POSPAR} -DISTORT_KEYS X_IMAGE,Y_IMAGE"
  fi

  # polynomial order used to map astrometric distortions:
  DISTORTDEG="-DISTORT_DEGREES 3"
  if [ "${DATAKIND}" = "SHORT" ]; then
    DISTORTDEG="-DISTORT_DEGREES 2"
  fi

  ${P_SCAMP} ${FILES} \
             -c ${CONF}/scamp_astrom_photom.scamp \
             -CDSCLIENT_EXEC ${P_ACLIENT} \
             -POSITION_MAXERR ${POSMAXERR} \
             -NTHREADS ${NPARA} ${POSPAR} \
             ${DISTORTDEG} ${ASTREF} ${FLAGS} ${MOSAICTYPE}

  if [ $? -ne 0 ]
  then
    echo "scamp call failed!! Exiting!!"
    cleanTmpFiles
    exit 1
  fi

  # scamp creates the headers in the directory where the catalogs
  # are:
  ${P_FIND}  ../cat/ -name \*.head -exec mv {} . \;

  # we want the diagnostic plots in an own directory:
  mv fgroups*         ../plots
  mv distort*         ../plots
  mv astr_interror2d* ../plots
  mv astr_interror1d* ../plots
  mv astr_referror2d* ../plots
  mv astr_referror1d* ../plots
  mv astr_chi2*       ../plots
  mv psphot_error*    ../plots

  # now get the relative magnitude offsets from the FLXSCALES
  # estimated by scamp:
  test -f ${TEMPDIR}/photdata.txt_$$ && rm ${TEMPDIR}/photdata.txt_$$

  # Because the flux scales refer to an image normalised to one
  # second we need to obtain the exposure times of all frames
  # first. We also get the SCAMP flux scale, the photometric
  # instrument and the photometric group:
  for EXPOSURE in ${EXPOSURES}
  do
    EXPTIME=`${P_LDACTOASC} -i ../cat/${EXPOSURE}_scamp.cat \
                       -t LDAC_IMHEAD -s |\
             fold | grep EXPTIME | ${P_GAWK} '{print $3}'`
    FLXSCALE=`grep FLXSCALE ${EXPOSURE}_scamp.head | uniq |\
              ${P_GAWK} '{print $2}'`
    PHOTINST=`grep PHOTINST ${EXPOSURE}_scamp.head | uniq |\
              ${P_GAWK} '{print $2}'`
    PHOTGRP=`grep FGROUPNO ${EXPOSURE}_scamp.head | uniq |\
              ${P_GAWK} '{print $2}'`

    echo ${EXPOSURE}" "${EXPTIME}" "${FLXSCALE}" "${PHOTINST}" "${PHOTGRP} >> \
       ${TEMPDIR}/photdata.txt_$$
  done

  # The following 'awk' script calculates relative zeropoints
  # and THELI fluxscales for the different photometric contexts:
  ${P_GAWK} 'BEGIN {maxphotinst = 1;}
             { exposure[NR]       = $1;
               exptime[NR]        = $2;
               flxscale_scamp[NR] = $3;
               photinst[NR]       = $4;
               photgrp[NR]        = $5;
               val[NR]            = -2.5 * log($3 * $2) / log(10);
               m[$4, $5]          = m[$4, $5] + val[NR]
               nphotinst[$4, $5]  = nphotinst[$4, $5] + 1
               if ($4 > maxphotinst) {maxphotinst = $4}
               if ($5 > maxgrp[$4]) {maxgrp[$4] = $5}}
             END {
               for (i = 1; i <= maxphotinst; i++) {
                 for (j = 1; j <= maxgrp[i]; j++) {
                   if (nphotinst[i, j] > 0) {
                     m[i, j] = m[i, j] / nphotinst[i, j];
                   }
                 }
               }
               for (i = 1; i <= NR; i++) {
                 relzp[i] = val[i] - m[photinst[i], photgrp[i]];
                 flxscale_theli[i] = (10**(-0.4 * relzp[i])) / exptime[i];
                 printf("%s %f %e\n",
                   exposure[i], relzp[i], flxscale_theli[i]);
               }
             }' ${TEMPDIR}/photdata.txt_$$ > ${TEMPDIR}/photdata_relzp.txt_$$

  # now split the exposure catalogues for the individual chips
  # and add the RZP and FLXSCALE header keywords. Put the headers
  # into appropriate headers_scamp directories
  #
  while read EXPOSURE RELZP FLXSCALE
  do
    # get all chips available for the current exposure:
    grep "^${EXPOSURE}" ${TEMPDIR}/allscampfiles.txt_$$  | \
    ${P_GAWK} -F_ '{a = $NF; gsub(/[^0-9]/, "", a); print a}' |\
    ${P_SORT} -n > ${TEMPDIR}/chips.txt_$$

    for CHIP in `seq 1 ${NCHIPS}`
    do
      test -f ${EXPOSURE}_${CHIP}.head && rm ${EXPOSURE}_${CHIP}.head
    done

    # first rename the SCAMP header keyword FLXSCALE to FLSCALE. We
    # need FLXSCALE for the THELI flux scaling later. Then create a
    # separate header file for each chip:
    sed -e 's/FLXSCALE/FLSCALE /' ${EXPOSURE}_scamp.head | \
    ${P_GAWK} 'BEGIN {getline < "'${TEMPDIR}'/chips.txt_'$$'"; chip = $1} {
               if ($1 == "END") {
                 printf("RZP     = %20f / THELI relative zeropoint\n",
                        '${RELZP}') >> "'${EXPOSURE}'_" chip ".head";
                 printf("FLXSCALE= %20E / THELI relative flux scale\n",
                        '${FLXSCALE}') >> "'${EXPOSURE}'_" chip ".head";
                 printf("END\n") >> "'${EXPOSURE}'_" chip ".head";

                 if (getline < "'${TEMPDIR}'/chips.txt_'$$'" > 0) {
                   chip = $1;
                 }
               } else {
                 print $0 >> "'${EXPOSURE}'_" chip ".head";
               }}'
  done < ${TEMPDIR}/photdata_relzp.txt_$$

  i=1
  while [ ${i} -le ${NCATS} ]
  do
    if [ -f ${CATBASE[$i]}.head ]; then
      mv ${CATBASE[$i]}*head ${CATDIR[$i]}/headers_scamp_${SUFFIX}
    fi

    i=$(( ${i} + 1 ))
  done
else # ${NCATS} -gt 0
  # In case that this script did nothing we also
  # remove the created directory structure:
  theli_error "No catalogues available! Nothing to be done!"

  cd ../..
  rm -rf astrom_photom_scamp_${SUFFIX}

  i=1
  j=2
  k=1
  while [ ${k} -le ${NDIRS} ]
  do
    test -d /${!i}/${!j}/headers_scamp_${SUFFIX} && \
      rmdir /${!i}/${!j}/headers_scamp_${SUFFIX}

    i=$(( ${i} + 3 ))
    j=$(( ${j} + 3 ))
    k=$(( ${k} + 1 ))
  done

  exit 1;
fi

# clean up temporary files and bye
cleanTmpFiles

cd ${DIR}

theli_end
exit 0;
