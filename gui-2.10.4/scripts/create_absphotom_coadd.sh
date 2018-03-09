BASHPATH -xv

# The script estimates an absolute photometric zeropoint for the
# coadded image of a set. It relies on presence of information on
# relative photometry and photometric zeropoints. 

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/${INSTRUMENT}.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/${INSTRUMENT:?}.ini

# $1: main dir.
# $2: science dir. (the cat dir is a subdirectory of this)

# leave if chips.cat5 does not exist
if [ ! -f /$1/$2/cat/chips.cat5 ]; then
    exit 1
fi

# check whether the STATS table exists
${P_LDACDESC} -i /$1/$2/cat/chips.cat5 | grep "Extension name" | \
    grep STATS | wc | ${P_GAWK} '{print $1}' > ${TEMPDIR}/testtable
testtable=`cat ${TEMPDIR}/testtable`
if [ "${testtable}" = "0" ]; then
    exit 1
fi

# check whether absphot info is available.
# if not, then there is nothing to do and we can exit
${P_LDACTESTEXIST} -i /$1/$2/cat/chips.cat5 -t STATS -k ZP1
if [ "$?" -gt "0" ]; then
    exit 1
fi

${P_LDACTESTEXIST} -i /$1/$2/cat/chips.cat5 -t STATS -k COEFF1
if [ "$?" -gt "0" ]; then
    exit 1
fi

${P_LDACTESTEXIST} -i /$1/$2/cat/chips.cat5 -t STATS -k COLOR1
if [ "$?" -gt "0" ]; then
    exit 1
fi


# Generate a catalog containing the photometric information
# requested by the user.

test -f /$1/$2/cat/absphot.asc   && rm /$1/$2/cat/absphot.asc
test -f ${TEMPDIR}/nights_$$.asc && rm ${TEMPDIR}/nights_$$.asc

# if we have nights to calibrate on the command line take this info,
# otherwise calibrate with all preselected photometric information
if [ ${V_ABSPHOT_NIGHTSELECTION}_A != "_A" ]; then
  NCALIBS=`echo ${V_ABSPHOT_NIGHTSELECTION} | ${P_GAWK} 'BEGIN{FS="_"} {print NF/2}'`

  i=1
  while [ ${i} -le ${NCALIBS} ]
  do
      NAME=""
      # Extract the night and the choice for that night
      NIGHT=`echo ${V_ABSPHOT_NIGHTSELECTION} ${i}  | sed 's/_/ /g' | ${P_GAWK} '{print $( 2*($NF)-1 )}'`
      CHOICE=`echo ${V_ABSPHOT_NIGHTSELECTION} ${i} | sed 's/_/ /g' | ${P_GAWK} '{print $( 2*($NF) )}'`

      # Nightly calibration
      if [ ${NIGHT} != 0 ]; then
	  ${P_LDACFILTER} -i /$1/$2/cat/chips.cat5 -o ${TEMPDIR}/zp_$$.cat \
              -t STATS -c "(GABODSID=${NIGHT});"

      # Run calibration
      else
	  cp /$1/$2/cat/chips.cat5 ${TEMPDIR}/zp_$$.cat
      fi

      ${P_LDACTOASC} -b -i ${TEMPDIR}/zp_$$.cat -s -t STATS \
          -k IMAGENAME ZP${CHOICE} COEFF${CHOICE} AIRMASS RZP | \
          ${P_GAWK} '{if ($1 !~ /#/ && $2 > 0) {print $1, $2, $3, $4, $5}}' > \
          ${TEMPDIR}/zp_$$.asc
      
      if [ -s ${TEMPDIR}/zp_$$.asc ]; then
	  cat ${TEMPDIR}/zp_$$.asc >> /$1/$2/cat/absphot.asc  
	  echo ${NIGHT} ${CHOICE} >> ${TEMPDIR}/nights_$$.asc
      else
	  echo "Problem with night ${NIGHT} and solution ${CHOICE}; aborting"
	  exit 1
      fi
      NAME="${NAME}_${NIGHT}_${CHOICE}"
      i=$((${i}+1))
  done
fi
#else
#    ${P_LDACTOASC} -b -i /$1/$2/cat/chips.cat5 \
#        -s -t STATS -k IMAGENAME ZP COEFF AIRMASS RZP | \
#	${P_GAWK} '{if ($1 !~ /#/ && $2 > 0) print $1, $2, $3, $4, $5}' \
#        > /$1/$2/cat/absphot.asc
    
#    if [ ! -s /$1/$2/cat/absphot.asc ]; then
#	echo "No default zeropoints and extinction coefficients available !!"
#	echo "Exiting !!"
#	exit 1
#   fi
    
#    ${P_LDACTOASC} -b -i /$1/$2/cat/chips.cat5 -t STATS -k GABODSID ZPCHOICE |\
#      ${P_SORT} -g | uniq | ${P_GAWK} '($2>0) {print $0}' \
#        > ${TEMPDIR}/nights_$$.asc
#fi

# now estimate the absolute photometric zeropoint. If more than
# 20 photometric images are available we perform a sigmaclipping
# algorithm before estimating the zeropoint by a straight mean.
#
# First we filter entries with a 'bad' relative zeropoint. We
# define this as being 0.1 mag below the median of relative zeropoints:
${P_GAWK} 'BEGIN {i = 1} {line[i] = $0; rzp[i] = $5; rzpcopy[i] = $5; i++}
        END {n = asort(rzpcopy);
             if(n > 2) {
               if(n%2==0) {
                 reject = (rzpcopy[n / 2] + rzpcopy[n / 2 + 1]) / 2. - 0.1
               } else {
                 reject = rzpcopy[int(n / 2)] - 0.1; 
               }
             } else {
               reject = -1.0;
             }
             for(i = 1; i <= n; i++) {
               if(rzp[i] > reject) {
                 print line[i];
               }
             }
            }' /$1/$2/cat/absphot.asc |\
${P_GAWK} '{oldline[NR] = $0;
            newline[NR] = $0; 
            oldzp[NR]   = $2+$3*$4-$5; 
            newzp[NR]   = $2+$3*$4-$5;} 
           END { if (NR > 20) {maxiter = 3} else {maxiter = 0}
                oldnelem = 0; 
                newnelem = NR; 
                actuiter = 0;
                while (actuiter <= maxiter && oldnelem != newnelem) {
                  zpmean = 0.0; zpsdev = 0.0;
                  for(i = 1; i <= newnelem; i++) {
                    zpmean += newzp[i]; 
                    zpsdev += newzp[i] * newzp[i];
                  } zpmean /= newnelem;
                  if(newnelem > 2) {
                    zpsdev = sqrt(zpsdev / newnelem - zpmean * zpmean)
                  } else {
                    zpsdev = 0.0
                  }
                  
                  oldnelem = newnelem;
                  if(zpsdev > 0.0 && actuiter < maxiter)
                  {
                    newnelem = 0;
                    for(i = 1; i <= oldnelem; i++) {
                      oldzp[i]   = newzp[i];
                      oldline[i] = newline[i];
                    }
                    for(i = 1; i <= oldnelem; i++) {
                      if(sqrt((oldzp[i] - zpmean) * (oldzp[i] - zpmean)) < 3.0 * zpsdev)
                      {
                        newzp[++newnelem] = oldzp[i];
                        newline[newnelem] = oldline[i];
                      }
                    }
                  }
                  actuiter++;
                }
                for(i=1; i<=oldnelem; i++) {
                  print newline[i], newzp[i], zpmean, zpsdev
                }
               }' > ${TEMPDIR}/phottmp_$$.asc

#
# create tables SOLPHOTOM and ABSPHOTOM containing info on the nights and
# solutions that finally went into the photometric calibration.
if [ -s ${TEMPDIR}/nights_$$.asc ] && [ -s ${TEMPDIR}/phottmp_$$.asc ]; then
  ${P_ASCTOLDAC} -a ${TEMPDIR}/nights_$$.asc -o ${TEMPDIR}/nights_$$.cat \
                 -t SOLPHOTOM -c ${DATACONF}/asctoldac_solphotom.conf

  ${P_ASCTOLDAC} -a ${TEMPDIR}/phottmp_$$.asc -o ${TEMPDIR}/phottmp_$$.cat \
                 -t ABSPHOTOM -c ${DATACONF}/asctoldac_absphotom.conf

  ${P_LDACADDTAB} -i /$1/$2/cat/chips.cat5 -o /$1/$2/cat/chips_tmp.cat5 \
                  -p ${TEMPDIR}/nights_$$.cat -t SOLPHOTOM

  ${P_LDACADDTAB} -i /$1/$2/cat/chips_tmp.cat5 -o /$1/$2/cat/chips_phot.cat5 \
                  -p ${TEMPDIR}/phottmp_$$.cat -t ABSPHOTOM
fi

# clean up temporary files:
test -f /$1/$2/cat/absphot.asc    && rm /$1/$2/cat/absphot.asc 
test -f /$1/$2/cat/chips_tmp.cat5 && rm /$1/$2/cat/chips_tmp.cat5
test -f ${TEMPDIR}/nights_$$.asc  && rm ${TEMPDIR}/nights_$$.asc
test -f ${TEMPDIR}/nights_$$.cat  && rm ${TEMPDIR}/nights_$$.cat
test -f ${TEMPDIR}/phottmp_$$.asc && rm ${TEMPDIR}/phottmp_$$.asc
test -f ${TEMPDIR}/phottmp_$$.cat && rm ${TEMPDIR}/phottmp_$$.cat
