#!/bin/bash -xv

# this script splits a set of raw data into BIAS, DARK, DOMEFLAT, SKYFLAT, 
# STANDARD and SCIENCE

# $1: main dir
# $2: FITS key used for the sorting
# $3: BIAS key
# $4: DARK key
# $5: DOMEFLAT key
# $6: SKYFLAT key
# $7: STANDARD key

. progs.ini

cd /$1

FILES=`ls -1l $1 | ${P_GAWK} '($1 ~ /^-/) {print $NF}'`

for file in ${FILES}
do
  # Check if file is FITS
  isfits=`${P_ISFITS} ${file}`
  if [ ${isfits} = 0 ]; then 
      continue
  fi

  type=`${P_DFITS} ${file} | ${P_FITSORT} -d ${V_SORT_FITSKEY} | ${P_GAWK} '{print $2}'`

  # search for BIAS frames
  if [ "${type}" = "${V_SORT_BIAS}" ] && [ ${type}_A != "_A" ]; then
      if [ ! -d $1/BIAS ]; then
	  mkdir $1/BIAS
      fi
      mv ${file} $1/BIAS
  fi
  
  # search for DARK frames
  if [ "${type}" = "${V_SORT_DARK}" ] && [ ${type}_A != "_A" ]; then
      if [ ! -d $1/DARK ]; then
	  mkdir $1/DARK
      fi
      mv ${file} /$1/DARK
  fi
  
  # search for DOMEFLAT frames
  if [ "${type}" = "${V_SORT_DOMEFLAT}" ] && [ ${type}_A != "_A" ]; then
      if [ ! -d $1/DOMEFLAT ]; then
	  mkdir $1/DOMEFLAT
      fi
      mv ${file} /$1/DOMEFLAT
  fi
  
  # search for SKYFLAT frames
  if [ "${type}" = "${V_SORT_SKYFLAT}" ] && [ ${type}_A != "_A" ]; then
      if [ ! -d $1/SKYFLAT ]; then
	  mkdir $1/SKYFLAT
      fi
      mv ${file} /$1/SKYFLAT
  fi
  
  # search for STANDARD frames
  if [ "${type}" = "${V_SORT_STD}" ] && [ ${type}_A != "_A" ]; then
      if [ ! -d $1/STANDARD ]; then
	  mkdir $1/STANDARD
      fi
      mv ${file} /$1/STANDARD
  fi
  
  # move the rest into the SCIENCE dir
  if [ ! -d $1/SCIENCE ]; then
      mkdir $1/SCIENCE
  fi
  mv ${file} /$1/SCIENCE
  
done
