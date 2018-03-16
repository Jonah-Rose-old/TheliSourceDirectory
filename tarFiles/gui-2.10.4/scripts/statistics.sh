BASHPATH -xv

#$1: main directory
#$2: instrument
#$3: xmin
#$4: xmax
#$5: ymin
#$6: ymax

INSTDIR=instruments_professional
if [ ! -f ${INSTDIR}/$2.ini ]; then
    INSTDIR=instruments_commercial
fi
if [ ! -f ${INSTDIR}/$2.ini ]; then
    INSTDIR=~/.theli/instruments_user/
fi
. ${INSTDIR}/$2.ini

DIR=`pwd`

cd /$1/

SECTION=""
if [ ! "$4"_A = "_A" ] && [ ! "$5"_A = "_A" ] && [ ! "$6"_A = "_A" ] && [ ! "$7"_A = "_A" ]; then
    SECTION=" -s $3 $4 $5 $6"
fi

# get the image background etc
pwd
echo $1
${P_IMSTATS} -d 2 ${THELIFILTER} -t -1e6 1e6 ${SECTION} | ${P_GAWK} '($1!~/#/)' > \
    ${TEMPDIR}/statistics.dat_$$

# get the image seeing and the RZP
test -f /$1/statistics.dat && rm /$1/statistics.dat
exec < ${TEMPDIR}/statistics.dat_$$
while read file mode lq median uq mean rms d1 d2
do
  BASE=`echo ${file} | ${P_GAWK} '{n=split($0,a,"_")} END {
    for(i=1;i<n-1;i++) {printf("%s_",a[i])}
    printf("%s",a[n-1])
    split(a[n],b,"OFC")
    printf("_%s\n",b[1])}'`
  # the seeing
  if [ -d cat ]; then
      cd cat 
      ls ${BASE}OFC*.cat > ${TEMPDIR}/statistics.cats0_$$
      ${P_GAWK} '($0!~/_norm.cat/ && $0!~/_corr.cat/)' \
	  ${TEMPDIR}/statistics.cats0_$$ > \
	  ${TEMPDIR}/statistics.cats1_$$
      catalog=`${P_GAWK} '(NR==1) {print $1}' ${TEMPDIR}/statistics.cats1_$$`
      fwhm_image=`${P_LDACTOASC} -i ${catalog} -t FIELDS -k FWHM_IMAGE -b`
      fwhm_world=`${P_LDACTOASC} -i ${catalog} -t FIELDS -k FWHM_WORLD -b`
      hfd_image=`${P_LDACTOASC} -i ${catalog} -t FIELDS -k HFD_IMAGE -b`
      hfd_world=`${P_LDACTOASC} -i ${catalog} -t FIELDS -k HFD_WORLD -b`
      fwhm_image=`echo ${fwhm_image} | ${P_GAWK} '{printf("%.2f\n", $1)}'`
      fwhm_world=`echo ${fwhm_world} | ${P_GAWK} '{printf("%.2f\n", $1)}'`
      hfd_image=`echo ${hfd_image} | ${P_GAWK} '{printf("%.2f\n", $1)}'`
      hfd_world=`echo ${hfd_world} | ${P_GAWK} '{printf("%.2f\n", $1)}'`
      airmass=`${P_LDACTOASC} -i ${catalog} -t FIELDS -k AIRMASS -b`
      cd ..
  else
      fwhm_world="NA"
      fwhm_image="NA"
      hfd_world="NA"
      hfd_image="NA"
      airmass="NA"
  fi

  # the RZP
  if [ -d headers ]; then
      cd headers
      if [ -f ${BASE}.head ]; then
	  RZP=`grep RZP ${BASE}.head | ${P_GAWK} '{printf("%.3f\n", $3)}'`
      else
	  RZP="NA"
      fi
      cd ..
  else
      RZP="NA"
  fi

  echo ${file} ${mean} ${mode} ${median} ${rms} \
      ${fwhm_image} ${fwhm_world} ${hfd_image} ${hfd_world} ${RZP} ${airmass} >> \
      /$1/statistics.dat
done

${DIR}/pythonimstats.py /$1 ${PIXSCALE}

mv statistics.png /$1/
