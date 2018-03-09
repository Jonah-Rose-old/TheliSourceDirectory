#!/bin/bash
#call an ds9 process as host
#$1 fits to display
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
DIR=`dirname $1`
#ANALYSIS="analysis ~/ds9-tools/pmon_ds9_analysis.ds9"
ANALYSIS="analysis $DIR/pmon_ds9_analysis.ds9"

XPA=`xpaget xpans`
ACTIVE=`xpaget xpans 2>/dev/null && echo "Ja"`
if  [ "$ACTIVE" == "" ]
then
echo "New DS9 starting ..."
ds9 -$MAP -$SCALE -$SCALEM -$ANALYSIS & 
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
# obligate, check for remote skycat process
start_session $0
# load another image
#$CMD  file $1
if [ "$2" == "1" ]
# blink all
then
$CMD frame tile yes
for i in ` ls $1`
do
$CMD frame new
$CMD file $i
done
$CMD blink yes interval 1
else
#blink off
$CMD tile no
if test -f '$1'
then
$CMD  file $1
else
# only first image of directory
image=`ls $1|awk 'NR==1{print $1}'`
$CMD file  $image
fi
fi
