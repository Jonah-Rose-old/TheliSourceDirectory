BASHPATH  
# exports tiffs
# $1 = sciencepath
#config by user
EXPORT=/mnt/nas/bilder/theliexport/
C=color_theli
TDIR="Color_theli"

using()
{
echo "Using: "
echo "$0 sciencedir"
echo "exports tiffs, htmls, txts & plots in $TDIR, makes tar and moves tar to $EXPORT"
echo "                             ---"
echo "Author: Carsten Moos, cmoos@gmx.de"
}

if [ "$1" == "" ]
then
using
exit -1
fi

main=`dirname $1`
main=`basename $main`

PWD=`pwd`
cd $1
TARGET=$main.tgz
mkdir $TDIR

cp $C/*.tif $TDIR
cp *.html $TDIR
cp *.txt $TDIR
cp -r plots $TDIR
for i in `ls -d coadd*`
do
cp $i/swarp.xml ${TDIR}/${i}_swarp.xml 
done
tar cvzf $TARGET $TDIR/*.tif $TDIR/*.html $TDIR/plots/* $TDIR/*.txt $TDIR/*.xml
echo "$TARGET created"
mv $TARGET $EXPORT
echo "$TARGET moved to $EXPORT"
rm -r $TDIR
cd $PWD
