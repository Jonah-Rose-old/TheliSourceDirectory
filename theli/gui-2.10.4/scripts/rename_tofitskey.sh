#!/bin/bash -xv

# $1: dir
# $2: FITSKEY

. progs.ini

cd /$1

ls -1 *.fits *.fit *.FITS *.FIT *.fts *.FTS > ${TEMPDIR}/fitskey_$$

cat ${TEMPDIR}/fitskey_$$ |\
{
  while read file
  do
    newname=`${P_DFITS} ${file} | ${P_FITSORT} -d ${V_RENAME_FITSKEY} | ${P_GAWK} '{print $2}'`
    mv ${file} ${newname}
  done
}

\rm ${TEMPDIR}/fitskey_$$
