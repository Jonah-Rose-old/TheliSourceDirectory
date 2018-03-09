#!/bin/bash

# $1: dir with the PSF plots (absolute path)
# $2: slideshow_name

if [ ! -d "$1" ]; then
    echo "Could not find directory $1 !"
    exit 1
fi

. progs.ini

title="$2"

DIR=`pwd`

cd /$1/

test -d $2 && rm -rf "$2"

mkdir "$2"

ls *.png > ${TEMPDIR}/imagelist_$$

firstimage=`gawk '(NR==1) {print $1}' ${TEMPDIR}/imagelist_$$`

cp /${CONF}/template_ss1.html /$1/"$2".html

exec < ${TEMPDIR}/imagelist_$$
while read file
do
{
echo ""
echo "s = new slide();"
echo "s.src =  \"./$2/${file}\";"                                       
echo "s.text = unescape(\"\");"
echo "s.link = \"\";"
echo "s.target = \"\";"
echo "s.attr = \"\";"
echo "s.filter = \"\";"
echo "SLIDES.add_slide(s);"
echo ""
} >> /$1/"$2".html
done

cat ${CONF}/template_ss2.html >> /$1/"$2".html

echo "<P><STRONG><FONT COLOR=\"AA0000\">${title}</FONT></STRONG>" \
    >> /$1/"$2".html

cat ${CONF}/template_ss3.html >> /$1/"$2".html

echo "src=\"./$2/${firstimage}\" STYLE=\"\" BORDER=0 alt=\"Slideshow image\"></A>" >> /$1/"$2".html

cat ${CONF}/template_ss4.html >> /$1/"$2".html

exec < ${TEMPDIR}/imagelist_$$
while read file
do
{
echo ""
echo "<P>"
echo "<IMG SRC=\"./$2/${file}\" ALT=\"slideshow image\"><BR>"
echo "</P>"
echo "<HR>"
} >> /$1/"$2".html
done

{
echo "</NOSCRIPT>"
echo "<P>"
echo "<P><B><FONT COLOR=\"AA0000\">Individual images:</FONT></B><BR>"
echo "<P>"
} >> /$1/"$2".html

exec < ${TEMPDIR}/imagelist_$$
while read file
do
{
base=`basename ${file} .png`
echo ""
echo "<A HREF=\"./$2/${file}\">${file}</A><BR>"
} >> /$1/"$2".html
done

mv *.png /$1/$2/
\rm ${TEMPDIR}/imagelist_$$

mkdir -p $1/iconset 
cp ${CONF}/icon*.png /$1/iconset

cd ${DIR}

exit 0
