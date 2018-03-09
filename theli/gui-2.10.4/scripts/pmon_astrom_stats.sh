#!/bin/bash
#creates statistics tables in html
#$1  sciencepath
BROWSER=firefox
#BROWSER=iceweasel
ZIEL=$1/astrom_stats.html
SCRIPTDIR=`dirname $0`
# Author: Carsten Moos, cmoos@gmx.de
using()
{
echo "Using: "
echo "$0 sciencepath"
echo "                             ---"
echo "Author: Carsten Moos, cmoos@gmx.de"
}
write_head()
{
{
echo "<HTML><HEAD><TITLE>Statistics of astrometry</TITLE></HEAD>"
echo "<BODY>"
}
}
write_foot()
{
{
echo "created by Result Monitor "
echo "</BODY></HTML>"
}
}
create_stats_detections()
{
echo "<H1>Counts of Detections</H1>"
echo "<table border=3 bgcolor=yellow>"
echo "<TR><TH align=center>file</TH><TH align=center>stars detected</TH><TH align=center>Mag *</TH><TR>"
for i in `ls ${1}/cat/skycat/*.skycat`
do
echo "<TR><TD>"
basename $i
echo "</TD><TD align=center>"
tail -n1 $i |awk {'print $1"</TD><TD align=center>"  $4 "</TD>"'}
echo "</TR>" 
done
echo "</table>"
echo "<p>* Mag of last entry </p>"
}

create_stats_rzp()
{
echo "<H1>Relative Zero Points of the sky in Mag</H1>"
echo "<table border=3 bgcolor=lightblue>"
echo "<TR><TH align=center>file</TH><TH align=center>RZP</TH><TR>"
for i in `ls ${1}/headers_scamp/*.head`
do
echo "<TR><TD>"
basename $i
echo "</TD><TD align=center>"
grep RZP $i| gawk {'print $3'}
echo "</TD>"
echo "</TR>"
done
echo "</table>"
}

create_stats_flxscales()
{
echo "<H1>Fluxscales of transparency</H1>"
echo "<table border=3 bgcolor=lightgreen>"
echo "<TR><TH align=center>file</TH><TH align=center>Flux</TH><TR>"
for i in `ls ${1}/headers_scamp/*.head`
do
echo "<TR><TD>"
basename $i
echo "</TD><TD align=center>"
grep FLXSCALE $i| gawk {'print $2'}
echo "</TD>"
echo "</TR>"
done
echo "</table>"
}

create_stats_flxscales2()
{
$SCRIPTDIR/pmon_getflxscale.sh $1 
}

if test -d $1 
then
write_head >$ZIEL
create_stats_detections $1 >>$ZIEL
create_stats_rzp $1 >>$ZIEL
create_stats_flxscales $1 >>$ZIEL
write_foot >>$ZIEL
($BROWSER $ZIEL) &
create_stats_flxscales2 $1
fi
