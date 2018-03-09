#!/bin/bash 
#call an ds9 process as host
#$1 fits to display
# uses region instead of catalog
using()
{
echo "Using: "
echo "$0 fitsfile"
echo "$0 'dir/*OFC.fits'  "
echo "$0 'dir/*OFC.fits' 1 "
echo " if a remote ds9 is already running, it will be used"
echo " if not, a new will be started"
echo " if an 1 is appended, alle files will be blinked"
echo "                             ---"
echo "Author: Carsten Moos, cmoos@gmx.de"
}
start_session()
{
CMD="xpaset -p ds9 "
SCALE="scale sqrt"
SCALEM="scale mode 99.5"
MAP="cmap grey"

XPA=`xpaget xpans`
ACTIVE=`xpaget xpans 2>/dev/null && echo "Ja"`
if  [ "$ACTIVE" == "" ]
then
echo "New DS9 starting ..."
ds9 -$MAP -$SCALE -$SCALEM & 
sleep 2 
else
#echo "ds9 is already running on $XPA"
echo ""
fi
}
if [ "$1" == "" ]
then
using
exit -1
fi
# obligate, check for remote ds9 process
start_session 
# load another image
#$CMD  file $1
if [ "$2" == "1" ]
# blink all
then
$CMD frame tile yes
for i in ` ls $1`
do
FILE=`basename $i .fits`
$CMD frame new
$CMD file $i
#$CMD catalog load $2/$FILE.skycat
$CMD regions load $2/$FILE.reg
done
$CMD blink yes interval 1
else
# stop blinking
$CMD tile no
if test -f '$1'
then
FILE=`basename $1 .fits`
$CMD  file $1
$CMD regions load $2/$FILE.reg
else
# only first image of directory
image=`ls $1|awk 'NR==1{print $1}'`
FILE=`basename $image .fits`
$CMD file  $image
$CMD regions load $2/$FILE.reg
fi
fi
