BASHPATH -xv

# script to estimate absolute photometric
# calibration on a night basis.

# $1 main dir
# $2 standard dir
# $3 science dir
# $4 image extension 

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

. bash_functions.include
. bash.include

if [ ! -d "/$1/$2/calib" ]; then
  mkdir /$1/$2/calib
fi

# find the chip catalogs that should be used for photometry,
# i.e. reject chips that have the NOTUSE flag set.

CATS=""
i=1
while [ ${i} -le ${NCHIPS} ]
do
  if [ ${NOTUSE[${i}]:=0} -eq 0 ] && [ ${NOTPROCESS[${i}]:=0} -eq 0 ]; then
    if [ -s /$1/$2/cat/chip_${i}_merg.cat ]; then   
      CATS="${CATS} /$1/$2/cat/chip_${i}_merg.cat"
    fi
  fi
  i=$(($i+1))
done

if [ "${CATS}_A" = "_A" ]; then
    exit
fi

${P_LDACPASTE} -i ${CATS} -t PSSC -o ${TEMPDIR}/tmp_chips_$$.cat

${P_LDACFILTER} -i ${TEMPDIR}/tmp_chips_$$.cat -t PSSC \
    -c "(((((${V_ABSPHOT_STDFILTER}mag < 99)AND(${V_ABSPHOT_STDFILTER}mag > -99))AND(${V_ABSPHOT_STDCOLOR} < 99))AND(Mag < 0))AND(${V_ABSPHOT_STDFILTER}mag > ${V_ABSPHOT_STDMINMAG}));" \
    -o /$1/$2/cat/allchips_merg.cat

${P_LDACFILTER} -i /$1/$2/cat/allchips_merg.cat -t PSSC \
    -c "(${V_ABSPHOT_STDCOLOR} > -10) AND (${V_ABSPHOT_STDCOLOR} < 10);" \
    -o /$1/$2/cat/allchips_tmp.cat

LABEL=`echo ${V_ABSPHOT_STDCOLOR} | sed 's/m/-/g'`

# Get a list of all nights and write it to the FIFO
NIGHTS=`${P_LDACTOASC} -i /$1/$2/cat/allchips_tmp.cat  -t PSSC \
    -b -k GABODSID | ${P_SORT} | uniq | ${P_GAWK} '{printf("%s ", $1)}'`

# In case of run calibration we do not loop through each night
if [ "${V_ABSPHOT_CALMODE}" = "RUNCALIB" ]; then
    CALIBNIGHT="${NIGHTS}" # variable that is  written as CALIBNIGHT
                           # to image header
    NIGHTS="0" # we use the dummy night 0 to indicate RUN calibration.
               # The value of this variable is used below for the filename
               # of the result file.
fi

for NIGHT in ${NIGHTS}
do
  if [ "${V_ABSPHOT_CALMODE}" = "RUNCALIB" ]; then
	echo  
	echo "       ---==== Calibrating the whole RUN ====---"
	echo
  else
	echo 
	echo "       ---==== Calibrating night ${NIGHT} ====---"
	echo
  fi

  if [ "${V_ABSPHOT_CALMODE}" = "RUNCALIB" ]; then
      cp /$1/$2/cat/allchips_tmp.cat /$1/$2/cat/night_${NIGHT}.cat
  else
      ${P_LDACFILTER} -i /$1/$2/cat/allchips_tmp.cat  -t PSSC\
    	-o /$1/$2/cat/night_${NIGHT}.cat -c "(GABODSID=${NIGHT});"
  fi

  ${P_LDACTOASC} -i /$1/$2/cat/night_${NIGHT}.cat -t PSSC \
      -b -k Mag MagErr ${V_ABSPHOT_STDFILTER}mag ${V_ABSPHOT_STDCOLOR} AIRMASS > \
      ${TEMPDIR}/night_${V_ABSPHOT_FILTER}_${NIGHT}_$$.asc
  
  zpthresholds=""
  if [ "${V_ABSPHOT_ZPMIN}_A" != "_A" ] && [ "${V_ABSPHOT_ZPMAX}_A" != "_A" ]; then
      zpthresholds="-z ${V_ABSPHOT_ZPMIN} ${V_ABSPHOT_ZPMAX}"
  fi
  
  # Do various fits to the photometric data; this writes the results also into ${TEMPDIR}
  ${P_ZPABS} \
      -i ${TEMPDIR}/night_${V_ABSPHOT_FILTER}_${NIGHT}_$$.asc \
      -p ${TEMPDIR} \
      -e ${V_ABSPHOT_EXTINCTION} \
      -c ${V_ABSPHOT_COLORFIXED} \
      ${zpthresholds} \
      -k ${V_ABSPHOT_KAPPA} \
      -t ${V_ABSPHOT_THRESHOLD} \
      -m ${V_ABSPHOT_MAXITER} \
      -s ${V_ABSPHOT_CONVERGENCE} > \
      /$1/$2/calib/night_${NIGHT}_${V_ABSPHOT_FILTER}_result.asc

  # Plot the solutions
  pycolor=`echo ${V_ABSPHOT_STDCOLOR} | sed 's/m/-/g'`
  ${S_ZPABS} ${TEMPDIR} "Mag_${V_ABSPHOT_FILTER} (STD-INST)" "STD Color (${pycolor})"

  mv ${TEMPDIR}/photcal.png   /$1/$2/calib/night_${NIGHT}_${V_ABSPHOT_FILTER}_result.png
  
  # Extract a comment-free version of the four photometric solutions
  ${P_GAWK} '($0 !~ /#/)' /$1/$2/calib/night_${NIGHT}_${V_ABSPHOT_FILTER}_result.asc > \
      ${TEMPDIR}/photcal_tmp.asc_$$

  i=1
  while read -a SOL
  do
      ZP[$i]=${SOL[0]}
      COEFF[$i]=${SOL[1]}
      COL[$i]=${SOL[2]}
      i=$(($i+1))
  done < ${TEMPDIR}/photcal_tmp.asc_$$

  echo
  echo "Updating image headers ..."
  echo

  if [ "${V_ABSPHOT_CALMODE}" = "RUNCALIB" ]; then
      ls -1 /$1/$3/*$4.fits | ${P_GAWK} -F/ '{print $NF}' > \
          ${TEMPDIR}/night_${NIGHT}_img_list_$$.asc
  else
      ${P_DFITS} /$1/$3/*$4.fits | ${P_FITSORT} -d GABODSID | \
          ${P_GAWK} '{if ($2 == '${NIGHT}') print $1}' > \
          ${TEMPDIR}/night_${NIGHT}_img_list_$$.asc
      
      # variable that is written as CALIBNIGHT to image header;
      # for RUNCALIB it was set above
      CALIBNIGHT="${NIGHT}"
  fi

  while read IMG
  do
    echo $IMG

    i=1
    while [ "${i}" -le "4" ]
    do
      value ${ZP[$i]}
      writekey /$1/$3/${IMG} ZP${i} "${VALUE} / Magnitude zeropoint" REPLACE
      value ${COEFF[$i]}
      writekey /$1/$3/${IMG} COEFF${i} "${VALUE} / Extinction coefficient" REPLACE
      value ${COL[$i]}
      writekey /$1/$3/${IMG} COLOR${i} "${VALUE} / Color coefficient" REPLACE
      i=$(($i+1))
    done

    # put information on the calibration mode (night or run) into the header:
    if [ "${V_ABSPHOT_CALMODE}" = "RUNCALIB" ]; then
      value "'RUN'"
    else
      value "'NIGHT'"
    fi  
  
    writekey /$1/$3/${IMG} PHOTCALM \
        "${VALUE} / photometric calibration mode (RUN or NIGHT)" REPLACE
    writecommhis /$1/$3/${IMG} COMMENT \
         "The following nights were used for photometric parameters:"
    writecommhis /$1/$3/${IMG} COMMENT "${CALIBNIGHT}"
  done < ${TEMPDIR}/night_${NIGHT}_img_list_$$.asc
done

if [ "${V_ABSPHOT_CALMODE}" = "NIGHTCALIB" ]; then    
    images=`${P_FIND} /$1/$3/ -maxdepth 1 -name \*$4.fits`
    for image in ${images}
    do
	checkkey=`get_key ${image} ZP1`
	if [ "${checkkey}"_A = "_A" ]; then
	    i=1
	    while [ "${i}" -le "4" ]
	    do
		value -1.0
		writekey ${image} ZP${i} "${VALUE} / No STD observed this night" REPLACE
		value -1.0
		writekey ${image} COEFF${i} "${VALUE} / No STD observed this night" REPLACE
		value -1.0
		writekey ${image} COLOR${i} "${VALUE} / No STD observed this night" REPLACE
		i=$(($i+1))
	    done
	fi
    done
fi

cp ${TEMPDIR}/night_${V_ABSPHOT_FILTER}_*_$$.asc /$1/$2/calib/night_${NIGHT}_${V_ABSPHOT_FILTER}.dat

rm ${TEMPDIR}/night_${V_ABSPHOT_FILTER}_*_$$.asc \
   ${TEMPDIR}/night_*_img_list_$$.asc \
   ${TEMPDIR}/tmp_chips_$$.cat
