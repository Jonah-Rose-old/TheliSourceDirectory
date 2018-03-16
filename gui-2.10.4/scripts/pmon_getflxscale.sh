BASHPATH
# reads FLXSCALES, scales them to the first image
# groups by FILTER Key in OFC.fits
# $1 = sciencepath

# Global Variable
SCIENCE=$1
PWD=`pwd`
cd $SCIENCE
GROUP="OFC.fits"
FILTER1=""
FILTER2=""
FILTER3=""
FILTER4=""
FILTER5=""

G1=""
G2=""
G3=""
G4=""
G5=""

using()
{
echo "Using: "
echo "$0 sciencedir"
echo "Reads FLXSCALE key from  headers_scamp/${BASE}.head"
echo "                             ---"
echo "Author: Carsten Moos, cmoos@gmx.de"
}


getfilter()
{
{
for i in `ls *$GROUP`
do
# sehr langsam!!
getkey $i  FILTER clean
done
}|sort -u>filters.tmp
}

out()
{
#$1 Filter $2 Grouplist
FIRSTFILE=`echo $2|awk '{print $1}' 2>/dev/null`
BASE=`basename $FIRSTFILE $GROUP 2>/dev/null`
FIRSTFLXSCALE=`grep FLXSCALE headers_scamp/${BASE}.head 2>/dev/null|awk '{print $2}' 2>/dev/null`
echo "erstes File: $FIRSTFILE MASTER Flux : $FIRSTFLXSCALE"
ZIEL="${1}_relflxscales.txt"
echo "Ziel: $ZIEL"
> flxscales.txt
echo "Filter: $1 " >$ZIEL
echo "file Theli-FLXSCALE scaled-GAIN " >>$ZIEL
echo "====================================" >>$ZIEL
# export Values
for i in $2
do
BASE=`basename $i $GROUP 2>/dev/null`
echo "working on $i ... "
FLXSCALE=`grep FLXSCALE headers_scamp/${BASE}.head 2>/dev/null|awk '{print $2}' 2>/dev/null`
echo "$i $FLXSCALE" >> flxscales.txt
done
gawk -v M=$FIRSTFLXSCALE '{print $1" " $2" " M/$2}' flxscales.txt >>$ZIEL 2>/dev/null
rm flxscales.txt 2>/dev/null
echo "scaled-GAIN = \"First-FLXSCALE / FLXSCALE\"" >>$ZIEL
echo "\"higher values mean more signal\"" >>$ZIEL
echo "\"profile under THELI\\relflxscales.gnumeric\"" >>$ZIEL
}

outhtml()
{
#$1 Filter $2 Grouplist
FIRSTFILE=`echo $2|awk '{print $1}' 2>/dev/null`
BASE=`basename $FIRSTFILE $GROUP 2>/dev/null`
FIRSTFLXSCALE=`grep FLXSCALE headers_scamp/${BASE}.head 2>/dev/null|awk '{print $2}' 2>/dev/null`
echo "erstes File: $FIRSTFILE MASTER Flux : $FIRSTFLXSCALE"
ZIEL="${1}_relflxscales.html"
echo "Ziel: $ZIEL"
> flxscales.txt
echo "<html><head><title>FLXSCALE</title></head><body>">$ZIEL
echo "Filter: $1 <br>" >>$ZIEL
echo "file Theli-FLXSCALE scaled-GAIN <br>" >>$ZIEL
echo "<table border=3 bgcolor=yellow>" >>$ZIEL
echo "<tr><th align=center>file</th><th align=center>rel. Fluxes</th><tr>" >>$ZIEL
# export Values
for i in $2
do
BASE=`basename $i $GROUP 2>/dev/null`
echo "working on $i ... "
FLXSCALE=`grep FLXSCALE headers_scamp/${BASE}.head 2>/dev/null|awk '{print $2}' 2>/dev/null`
echo "$i $FLXSCALE" >> flxscales.txt
done
gawk -v M=$FIRSTFLXSCALE '{print "<tr><td>" $1 "</td><td>" M/$2 "</td></tr>"}' flxscales.txt >>$ZIEL 2>/dev/null
rm flxscales.txt 2>/dev/null
echo "</table><br>scaled-GAIN = \"First-FLXSCALE / FLXSCALE\"" >>$ZIEL
echo "\"higher values mean more signal\"" >>$ZIEL
echo "\"profile under THELI\\relflxscales.gnumeric\"" >>$ZIEL
echo "</body></html>">>$ZIEL
}

getgroup()
{
for i in `ls *$GROUP`
do
F=`getkey $i FILTER clean`
if [ "$F" == "$FILTER1" ] 
then
G1=`echo "$G1 $i"`
fi
if [ "$F" == "$FILTER2" ] 
then
G2=`echo "$G2 $i"`
fi
if [ "$F" == "$FILTER3" ] 
then
G3=`echo "$G3 $i"`
fi
if [ "$F" == "$FILTER4" ] 
then
G4=`echo "$G4 $i"`
fi
if [ "$F" == "$FILTER5" ] 
then
G5=`echo "$G5 $i"`
fi
done
echo "Gruppe1: $G1" 
echo "Gruppe2: $G2" 
echo "Gruppe3: $G3" 
echo "Gruppe4: $G4" 
echo "Gruppe5: $G5" 
}

getfilter 
#FILTERS=`cat filters.tmp`
FILTER1=`awk 'NR==1{print $1}' filters.tmp 2>/dev/null 2>/dev/null`
FILTER2=`awk 'NR==2{print $1}' filters.tmp 2>/dev/null 2>/dev/null`
FILTER3=`awk 'NR==3{print $1}' filters.tmp 2>/dev/null 2>/dev/null`
FILTER4=`awk 'NR==4{print $1}' filters.tmp 2>/dev/null 2>/dev/null`
FILTER5=`awk 'NR==5{print $1}' filters.tmp 2>/dev/null 2>/dev/null`
rm filters.tmp
echo "Filter: $FILTERS F1: $FILTER1 F2: $FILTER2 F3: $FILTER3 F4: $FILTER4 F5: $FILTER5"

getgroup 

if [ "$FILTER1" != "" ]
then
out $FILTER1 "$G1"
outhtml $FILTER1 "$G1"
fi
if [ "$FILTER2" != "" ]
then
out $FILTER2 "$G2"
outhtml $FILTER2 "$G2"
fi
if [ "$FILTER3" != "" ]
then
out $FILTER3 "$G3"
outhtml $FILTER3 "$G3"
fi
if [ "$FILTER4" != "" ]
then
out $FILTER4 "$G4"
outhtml $FILTER4 "$G4"
fi
if [ "$FILTER5" != "" ]
then
out $FILTER5 "$G5"
outhtml $FILTER5 "$G5"
fi

cd $PWD
