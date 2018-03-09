#!/bin/bash 
#call an sykcat process as host
#$1 fits to display
#skycat  REDU/DATA2011-C8R-M66/science/m65_66-001_G_1OFC.fits -default_cmap ramp -default_itt gamma -port 0
#echo "creates host by : skycat -port 0"
#use that skycat host for display
using()
{
echo "Using: "
echo "$0 fitsfile "
echo "$0 'dir/*OFC.fits'  "
echo "$0 'dir/*OFC.fits' 1 "
echo " if a remote skycat is already running, it will be used"
echo " if not, a new will be started"
echo " if an 1 is appended, alle files will be blinked"
echo "                             ---"
echo "Author: Carsten Moos, cmoos@gmx.de"
}
start_session()
{
MAP="-default_cmap ramp -default_itt gamma"
ACTIVE=""
SKYCATPID=`gawk '{print $1}' ~/.rtd-remote`
ACTIVE=`ps p $SKYCATPID >/dev/null && echo "Ja"`
#echo "ACTIVE is $ACTIVE"
if  [ "$ACTIVE" == "" ]
then
echo "New Skycat starting ..."
skycat $MAP -port 0 & 
sleep 2 
else
#echo "Skycat is already running on PID $SKYCATPID"
echo ""
fi
}
if [ "$1" == "" ]
then
using
exit -1
fi
# obligate, check for remote skycat process
start_session 
# load another image
if [ "$2" == "1" ]
# blink all
then
for i in ` ls $1`
do
skycat  $i -remote 1
sleep 1
done
else
if test -f '$1'
then 
skycat  $1 -remote 1
else
# only first image of directory
image=`ls $1|awk 'NR==1{print $1}'`
skycat  $image -remote 1
fi
fi
