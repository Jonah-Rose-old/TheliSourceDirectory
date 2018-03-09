#!/bin/bash
#file: protocol_theli2.sh
# erstellt ein umfassendes Verarbeitungsprotokoll
# benoetigt ~/.theli ~/.qt
# tools: dfits, gawk, sort, grep, cut, basename, ls, dirname, getkey,dcraw
# images tools optional: tifftopnm, ppmtojpeg, pnmscale
# $1 = OBJECT-name NGC_4711
#QTDIR=~/.qt
# hangs if OFC images not found
OBJECT=$1
BROWSER=firefox
#--------------------------------------------
usage(){
echo "creates a processing protocol"
echo "Version 1.2"
echo "by C.Moos cmoos@gmx.de"
echo "usage: $0 [-?] [NGC_4711]"
echo "you must not use blanks in object name"
exit
}

getThelilog(){
LASTLOG=`grep lastlog ~/.qt/theligeneral_rc|cut -d"=" -f2`
LASTLOG=`echo $LASTLOG|tr "[:upper:]" "[:lower:]"`
THELILOG=~/.qt/theli_${LASTLOG}_rc
BTHELILOG=`basename $THELILOG _rc`

}

getMaindir(){
MAINDIR=`grep ^maindir $THELILOG |cut -d"=" -f2`
if [ ! -d $MAINDIR ]
then
echo "$MAINDIR not found"
exit -1

fi
}

getSciencedir(){
SCIENCEDIR=`grep ^sciencedir $THELILOG |cut -d"=" -f2`
RGBDIR="$SCIENCEDIR"
if [ ! -d $MAINDIR/$SCIENCEDIR ]
then
echo "$SCIENCEDIR not found"
exit -1
fi

}

getCoadddir(){
FILTER=`grep ^coaddition/coaddfiltertext $THELILOG |cut -d"=" -f2`
ID=`grep ^coaddition/id $THELILOG |cut -d"=" -f2`
#if [ "$ID" == "" ]
#then
#COADDDIR="coadd_${ID}_$FILTER1"
# suche erst besten coadd

COADDDIR=`ls $MAINDIR/$SCIENCEDIR/coadd_*/coadd.fits|gawk 'NR==1{print $1}'`
COADDDIR=`dirname $COADDDIR`
COADD="$COADDDIR/coadd.fits"
if [ ! -f $COADD ]
then
   #exit -1
   echo "$COADD not found"
   COADD=""
fi
#fi
#Sonderfall vernachlaessigt
# wenn coaddfilteritem=0, heisst es nur coadd_$ID
}


getColorthelidir(){
COLORDIR="color_theli"
}

getObjectname(){
echo "Type in the objectname (e.g. NGC 4711)"
#read OBJECT
OBJECT="Black_Hole"
}

getZiel(){
#ZIEL="$MAINDIR/$SCIENCEDIR/$COLORDIR/theli_protocol_$OBJECT.html"
ZIEL="$MAINDIR/$SCIENCEDIR/theli_protocol_$OBJECT.html"
}

createHeader(){
echo "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">">$ZIEL
echo "<html lang=\"de\">">>$ZIEL
echo "<head>">>$ZIEL
echo "<title>THELI processing protocol $BTHELILOG</title>">>$ZIEL
echo "<meta http-equiv=\"content-type\" content=\"text/html; charset=ISO-8859-1\">">>$ZIEL
echo "<meta http-equiv=\"content-language\" content=\"EN\">">>$ZIEL
echo "</head>">>$ZIEL
}

createBody(){
echo "<body lang=\"de\" text=\"#000000\" bgcolor=\"azure\" link=\"#0000ff\" alink=\"#0000ff\" vlink=\"#0000ff\">">>$ZIEL
GUI=`which theli`
GUI=`dirname $GUI`
echo "<TABLE><TR> <td align=\"left\"><h1>THELI: Protocol for $BTHELILOG</h1></td><td align=\"right\"><A href=\"http://www.astro.uni-bonn.de/~mischa/theli.html\"> <IMG src=\"$GUI/theli_splash.png\" alt=\"Dragon-Logo\" width=\"100\"  align=\"left\" border=\"0\"></A></td> </TR></TABLE>">>$ZIEL
}

createFooter(){
echo -n " <p> printed on ">>$ZIEL
date>>$ZIEL
echo " best print by 72% with A4-paper</p>">>$ZIEL
echo " <p> Created with protocol_theli2.sh 1.2 by cmoos@gmx.de ">>$ZIEL
echo " </p>">>$ZIEL
echo "<H3>FITS_Header </H3>" >>$ZIEL
echo "<p>" >>$ZIEL
echo "$COADD<BR>">>$ZIEL
echo "Footer: $COADD"
echo "<tt>">>$ZIEL
if [ -f "$COADD" ]
then
dfits $COADD|grep -v DUMMY|awk  '{ print $0,"<BR>"}' >> $ZIEL
else
echo "no coadd file found">>$ZIEL
fi
echo "</tt>">>$ZIEL
echo "</p>" >>$ZIEL

echo "</body>">>$ZIEL
echo "</html>">>$ZIEL
}

startBrowser(){
echo "Now starting $BROWSER $ZIEL"
($BROWSER $ZIEL) &
}

debugInfo(){
echo "THELILOG  : $THELILOG"
echo "LASTLOG   : $LASTLOG"
echo "BTHELILOG : $BTHELILOG"
echo "MAINDIR   : $MAINDIR"
echo "SCIENCEDIR: $SCIENCEDIR"

echo "COADDDIR  : $COADDDIR"
echo "COADD     : $COADD"
echo "COLORDIR  : $COLORDIR"
echo "OBJECT    : $OBJECT"
echo "AVGDIR    : $AVGDIR"
echo "BVDIR     : $BVDIR"
echo "Targetfile: $ZIEL"
echo "Filter1   : $FILTER2"
echo ""
#echo "Press any key to collect data and show it"
#read dummy

}

get_infos(){
#### collect infos
echo "<H3>Projectdata : $OBJECT Logfile : $BTHELILOG </H3>">>$ZIEL
echo "<table><tr>">>$ZIEL
echo "<td><strong>Current log file </strong></td><td> $THELILOG </td></tr>">>$ZIEL
echo "<tr><td><strong>Mainpath </strong></td><td> $MAINDIR </td></tr>">>$ZIEL
echo "<tr><td><strong>OBJECT</strong></td><td> $OBJECT </td></tr>">>$ZIEL
EXPOSURET=`dfits ${MAINDIR}/${RGBDIR}/*.fits 2>/dev/null|grep EXP|tail -n1|cut -d' ' -f17`
if [ "$EXPOSURET" == "" ]; then
EXPOSURET=`dcraw -i -v ${MAINDIR}/${SCIENCEDIR}/ORIGINALS/*.cr2 2>/dev/null|grep Shutter|tail -n1|cut -d':' -f2|cut -d" " -f2`
fi
if [ "$EXPOSURET" == "" ]; then
EXPOSURET=`dfits ${MAINDIR}/${RGBDIR}/*FC.fits 2>/dev/null|grep EXP|tail -n1|cut -d' ' -f17`
fi
if [ "$EXPOSURET" == "" ]; then
EXPOSURET=`dfits ${MAINDIR}/${RGBDIR}/r/*FC.fits 2>/dev/null|grep EXP|tail -n1|cut -d' ' -f17`
fi

#ISO=`dcraw -i -v ${MAINDIR}/${SCIENCEDIR}/ORIGINALS/*.cr2 2>/dev/null|grep ISO|tail -n1|cut -d':' -f2`
ISO=`dcraw -i -v ${MAINDIR}/${SCIENCEDIR}/ORIGINALS/*.cr2 2>/dev/null|grep ISO|tail -n1|cut -d':' -f2`
if [ "$ISO" = "" ]
then
  ISO="(CCD)"
fi
NUMBER=`ls ${MAINDIR}/${RGBDIR}/*OFC.fits 2>/dev/null|wc -l`
if [ "$NUMBER" == "0" ]; then
NUMBER=`ls ${MAINDIR}/${SCIENCEDIR}/ORIGINALS/* 2>/dev/null|wc -l`
fi
if [ "$NUMBER" == "0" ]; then
NUMBER=`ls ${MAINDIR}/${SCIENCEDIR}$R/ORIGINALS/* 2>/dev/null|wc -l`
fi

echo "<tr><td><strong>Images</strong></td><td> $NUMBER * $EXPOSURET s bei $ISO iso </td></tr>" >> $ZIEL
echo -n "<tr><td><strong>Camera</strong></td><td>">>$ZIEL
KAMERA=`grep ^instrumentname= $THELILOG |cut -d"=" -f2`
echo "$KAMERA</td></tr>" >>$ZIEL 

if [ ! -d ${MAINDIR}/${SCIENCEDIR}/ORIGINALS ]
then
echo "${MAINDIR}/${SCIENCEDIR}/ORIGINALS not found"
exit -1
else
echo -n "<tr><td><strong>Science-Files</strong></td><td> ">>$ZIEL
for i in `ls ${MAINDIR}/${SCIENCEDIR}/ORIGINALS/*`
do
if [ -f "$i" ]
then
echo -n "`basename $i`, " >>$ZIEL
fi
done
echo "</td></tr>" >> $ZIEL
echo "</table>">>$ZIEL
fi
}

#CALIBRATING
calibrating(){
echo "<H3>Calibrating </H3>" >>$ZIEL
echo -n "<table><tr><td width=\"10%\">Path</td><td width=\"10%\">" >>$ZIEL
echo -n "<strong>Bias:</strong>" >>$ZIEL
echo "</td><td width=\"10%\">">>$ZIEL
grep ^biasdir= $THELILOG |cut -d"=" -f2 >>$ZIEL
echo "</td><td width=\"10%\">">>$ZIEL
echo -n "<strong>Dark:</strong>" >>$ZIEL
echo "</td><td width=\"10%\">">>$ZIEL
grep ^darkdir= $THELILOG |cut -d"=" -f2 >>$ZIEL
echo "</td><td width=\"10%\">">>$ZIEL
echo -n "<strong>Flat:</strong>" >>$ZIEL
echo "</td><td width=\"10%\">">>$ZIEL
grep ^flatdir= $THELILOG |cut -d"=" -f2 >>$ZIEL
echo "</td>">>$ZIEL
echo "<tr><td>Steps used:</td><td>">>$ZIEL
echo -n "<strong>Bias corrected:</strong>" >>$ZIEL
echo "</td><td>" >>$ZIEL
DONE=`grep ^dobias= $THELILOG |cut -d"=" -f2`
TEXT="YES"
if [ "$DONE" == "true" ]
then
TEXT="NO"
fi
echo "$TEXT</td>" >>$ZIEL

echo -n "<td><strong>Dark corrected:</strong>" >>$ZIEL
echo "</td><td>" >>$ZIEL
DONE=`grep ^usedark= $THELILOG |cut -d"=" -f2`
TEXT="NO"
if [ "$DONE" == "true" ]
then
TEXT="YES"
fi
echo "$TEXT</td>" >>$ZIEL
echo -n "<td><strong>Flat corrected:</strong>" >>$ZIEL
echo "</td><td>" >>$ZIEL
DONE=`grep ^doflat= $THELILOG |cut -d"=" -f2`
TEXT="YES"
if [ "$DONE" == "true" ]
then
TEXT="NO"
fi
echo "$TEXT</td>" >>$ZIEL

echo "</tr></table>">>$ZIEL
}

#WEIGHTING
weighting(){
echo "<H3>Weighting</H3>">>$ZIEL
echo -n "Cosmics Threshold: " >>$ZIEL
grep ^cosmicthreshold= $THELILOG |cut -d"=" -f2 >>$ZIEL
echo "<BR>" >>$ZIEL 
}

#ASTROMETRY PHOTOMETRY
astromphotom(){
echo "<H3>Astrometry / Photometry  </H3>">>$ZIEL
echo "<table><tr>">>$ZIEL
echo -n "<td width=\"10%\"><strong>R.A.:</strong> </td><td width=\"10%\">" >>$ZIEL
grep ^ra= $THELILOG |cut -d"=" -f2 >>$ZIEL
echo -n "</td><td width=\"15%\"><strong>Declination:</strong></td><td width=\"10%\">" >>$ZIEL
grep ^dec= $THELILOG |cut -d"=" -f2 >>$ZIEL
echo "</td></tr>" >>$ZIEL 
echo -n "<tr><td><strong>Mag-Limit:</strong></td><td>" >>$ZIEL
grep ^maglim= $THELILOG |cut -d"=" -f2 >>$ZIEL
echo -n "</td><td><strong> Radius:</strong> </td><td>" >>$ZIEL
grep ^radius= $THELILOG |cut -d"=" -f2 >>$ZIEL
echo " '</td></tr>" >>$ZIEL 
echo -n "<tr><td><strong>Astrometry: Instrument KEY:</strong></td><td> " >>$ZIEL
grep astrinstrukey= $THELILOG |cut -d"=" -f2 >>$ZIEL
echo -n "</td><td><strong>Photometry: Instrument KEY:</strong> </td><td>" >>$ZIEL
grep photinstrukey= $THELILOG |cut -d"=" -f2 >>$ZIEL
echo "</td></tr>" >>$ZIEL 
echo -n "<tr><td><strong>Stability type:</strong></td><td>" >>$ZIEL
TYPE=`grep scamp/stability= $THELILOG |cut -d"=" -f2` 
case "$TYPE" in
  0) TEXT="Instrument";;
  1) TEXT="Exposure";;
esac
echo "$TYPE:$TEXT</td><td></td><td></td></tr>" >>$ZIEL
echo -n "<tr><td><strong>Configure SOURCE CAT:</strong></td><td> " >>$ZIEL
MINAERA=`grep sourcecat/detectminarea= $THELILOG |cut -d"=" -f2`
DT=`grep sourcecat/detectthresh= $THELILOG |cut -d"=" -f2`
FWHM=`grep sourcecat/sexcatminfwhm= $THELILOG |cut -d"=" -f2`
echo "DT=$DT</td><td><strong>Minaera=</strong>$MINAERA</td><td><strong> Min. FWHM=</strong>$FWHM </td></tr>" >>$ZIEL
echo -n "<tr><td><strong>Method: </strong></td><td>" >>$ZIEL
TYPE=`grep astrommethod= $THELILOG |cut -d"=" -f2` 
case "$TYPE" in
  0) TEXT="scamp";;
  1) TEXT="astrometrix";;
  2) TEXT="shift(float)";;
  3) TEXT="shift(int)";;
esac
echo "$TYPE:$TEXT</td><td></td><td></td></tr>" >>$ZIEL
echo -n "<tr><td><strong>Pos Angle Max Err: </strong></td><td> " >>$ZIEL
grep scamp/maxposangle= $THELILOG |cut -d"=" -f2 >>$ZIEL
echo -n "�</td><td><strong>Pos Max Err:</strong> </td><td>" >>$ZIEL
grep scamp/maxoffset= $THELILOG |cut -d"=" -f2 >>$ZIEL
echo "</td></tr></table>" >>$ZIEL 
#getfilter
echo "<table><tr><td><strong>Filters:</strong></td>">>$ZIEL
echo "<td>Total: $FNUMBER</td><td> $FILTER</td>" >>$ZIEL
echo "</tr></table>">>$ZIEL
if test -f $MAINDIR/$SCIENCEDIR/plots/distort_1.png
then
echo "<H5>Scamp result plots</H5>" >>$ZIEL
echo "<table><tr><td><a href=\"$MAINDIR/$SCIENCEDIR/plots/distort_1.png \"><img src=\"$MAINDIR/$SCIENCEDIR/plots/distort_1.png\" width=\"100\"></a></td><td><a href=\"$MAINDIR/$SCIENCEDIR/plots/fgroups_1.png \"><img src=\"$MAINDIR/$SCIENCEDIR/plots/fgroups_1.png\" width=\"100\"></a></td></tr></table> ">>$ZIEL
fi
}

#GETFILTER
getfilter(){

FILES=`ls  $MAINDIR/$SCIENCEDIR/*OFC.fits`
FNUMBER=`for i in $FILES; do getkey $i FILTER clean; done|sort -u|wc -l`
# only first 5 Filters
FILTER=`for i in $FILES; do getkey $i FILTER clean; done|sort -u`
FILTER1=`echo $FILTER|gawk 'NR==1{print $1}'`
FILTER2=`echo $FILTER|gawk 'NR==1{print $2}'`
FILTER3=`echo $FILTER|gawk 'NR==1{print $3}'`
FILTER4=`echo $FILTER|gawk 'NR==1{print $4}'`
FILTER5=`echo $FILTER|gawk 'NR==1{print $5}'`
}

#SUBSKY
subsky(){
echo "<H3>Subsky</H3>">>$ZIEL
AUTO=`grep skysub/radiobuttonID= $THELILOG |cut -d"=" -f2 `
if [ "$AUTO" == "0" ]
then
echo -n "Parameter for Automatic: " >>$ZIEL
DT=`grep skysub/detectthresh= $THELILOG |cut -d"=" -f2 `
DMIN=`grep skysub/detectminaera= $THELILOG |cut -d"=" -f2 `
SIZE=`grep skysub/backsize $THELILOG |cut -d"=" -f2 `
echo "DT= $DT DMIN= $DMIN SIZE= $SIZE <BR>" >>$ZIEL
else
echo -n "Parameter for Subtract a constant: " >>$ZIEL
echo -n "Box for manual sky estimate xy: " >>$ZIEL
XMAX=`grep skysub/cskyxmax $THELILOG |cut -d"=" -f2 `
XMIN=`grep skysub/cskyxmin $THELILOG |cut -d"=" -f2 `
YMAX=`grep skysub/cskyymax $THELILOG |cut -d"=" -f2 `
YMIN=`grep skysub/cskyymin $THELILOG |cut -d"=" -f2 `
echo " x - y: $XMIN | $XMAX - $YMIN | $YMAX <BR>" >> $ZIEL
echo -n "Box for manual sky estimate by coordinates: " >>$ZIEL
XMAX=`grep skysub/cskyra2 $THELILOG |cut -d"=" -f2 `
XMIN=`grep skysub/cskyra1 $THELILOG |cut -d"=" -f2 `
YMAX=`grep skysub/cskydec2 $THELILOG |cut -d"=" -f2 `
YMIN=`grep skysub/cskydec1 $THELILOG |cut -d"=" -f2 `
echo " x - y: $XMIN | $XMAX - $YMIN | $YMAX <BR>" >> $ZIEL
echo -n "Method for statistical estimate: ">>$ZIEL
TYPE=`grep skysub/method= $THELILOG |cut -d"=" -f2`
case "$TYPE" in
  0) TEXT="mode";;
  1) TEXT="median";;
  2) TEXT="mean";;
  3) TEXT="low quantile";;
esac
echo "$TYPE:$TEXT<BR>" >>$ZIEL
fi
}

#COADDING
coadding(){
echo "<H3>Coaddition </H3>">>$ZIEL
echo "<table><tr><td width=\"25%\">" >>$ZIEL
echo -n "<strong>Combine-type</strong></td><td width=\"75%\"> " >>$ZIEL
TYPE=`grep coaddition/combinetype= $THELILOG |cut -d"=" -f2` 
case "$TYPE" in
  0) TEXT="Weighted";;
  1) TEXT="Median";;
  2) TEXT="Average";;
  3) TEXT="Min";;
  4) TEXT="Max";;
esac
echo " $TYPE:$TEXT  </td></tr>" >>$ZIEL
echo "<tr><td>" >>$ZIEL
echo -n "<strong>Resampling-Kernel</strong></td><td> " >>$ZIEL
TYPE=`grep coaddition/kernel= $THELILOG |cut -d"=" -f2` 
case "$TYPE" in
  0) TEXT="Nearest";;
  1) TEXT="Bilinear";;
  2) TEXT="Lanczos2";;
  3) TEXT="Lanczos3";;
  4) TEXT="Lanczos4";;
esac
echo " $TYPE:$TEXT  </td></tr></table>" >>$ZIEL
PIXSCALE=`grep PIXSCALE ~/.theli/instruments_user/${KAMERA}.ini |cut -d"=" -f2`
echo -n "<strong>Pixel-Scale</strong> :Original $PIXSCALE (100%) Resampling auf: " >>$ZIEL
PIXSCALEN=`grep coaddition/pixscale= $THELILOG |cut -d"=" -f2 `
if [ "$PIXSCALEN" == "" ];then
PIXSCALEN=$PIXSCALE
fi
echo " $PIXSCALEN (= " >>$ZIEL
echo "$PIXSCALE $PIXSCALEN"|gawk 'BEGIN {FS = " "}{ print $1/$2*100 }' >>$ZIEL
echo " %) <BR>" >>$ZIEL
getRGBbalance
echo -n "<strong>FilteringOutliers:</strong>  " >>$ZIEL
TH=`grep coaddition/filterthreshold= $THELILOG |cut -d"=" -f2` 
echo "Threshold: $TH <BR>" >>$ZIEL 

echo "<strong>Relative zeropoints (RZP) for filelist in $FILTER2 :</strong>" >>$ZIEL
>merke
for i in `ls $RG/*OFC.fits`
do
dfits $i >${i}.h
F=""
F=`grep -H $FILTER2 ${i}.h|cut -d":" -f1`
if [ "${F}-" != "-" ]
then
NFile=`basename $F OFC.fits.h`
echo "$RG/headers/${NFile}.head">>merke
fi
rm ${i}.h
done
FILES=`cat merke`
#echo "Files: $FILES"
rm merke 
#RZP=`grep RZP $RG/headers/*_G*.head|cut -d":" -f2|awk '{printf "%0.2f, ", $3}'`
if [ "$FILES" != "" ]
then
RZP=`grep RZP $FILES|cut -d":" -f2|awk '{printf "%0.2f, ", $3}'`
for i in $RZP
do
echo -n "$i " >>$ZIEL
done
echo " mag" >>$ZIEL
else
echo " OFC not found" >>$ZIEL
fi
}

#GETRGBBALANCE
getRGBbalance(){
RG=`dirname $COADD`
RG=`dirname $RG`
FRED=` grep  FLXSCALE $RG/headers/*_R*.head|head -n10|cut -d"=" -f2|cut -d"/" -f1| awk '{ ORS = " "}  {print $1 } '|awk '{ print  ($1 + $2 +$3 +$4 +$5 +$6 +$7 +$8 +$9 +$10 )/NF*100}'`
FGRE=` grep  FLXSCALE $RG/headers/*_G*.head|head -n10|cut -d"=" -f2|cut -d"/" -f1| awk '{ ORS = " "}  {print $1 } '|awk '{ print  ($1 + $2 +$3 +$4 +$5 +$6 +$7 +$8 +$9 +$10 )/NF*100}'`
FBLU=` grep  FLXSCALE $RG/headers/*_B*.head|head -n10|cut -d"=" -f2|cut -d"/" -f1| awk '{ ORS = " "}  {print $1 } '|awk '{ print  ($1 + $2 +$3 +$4 +$5 +$6 +$7 +$8 +$9 +$10 )/NF*100}'`
FRED=`echo "$FRED $FGRE"|gawk 'BEGIN {FS = " "}{ printf"%0.2f", $1/$2}' `
FBLU=`echo "$FBLU $FGRE"|gawk 'BEGIN {FS = " "}{ printf"%0.2f", $1/$2}' `
FGRE=`echo "$FGRE $FGRE"|gawk 'BEGIN {FS = " "}{ printf"%0.2f", $1/$2}' `
#echo "-----------------------------------------------------------------"
if [ "$FGRE" == "" ]
then
echo ""
else
echo "<strong>Bayer-RGB-BALANCE after photometry using FLXSCALES</strong> : $FRED       :      $FGRE        :    $FBLU<BR>">>$ZIEL
echo " (normal to Green*100, Average over up to 10 probes per Channel)<BR>">>$ZIEL
fi
}

#CREATE COLOUR PICTURE
createcolour(){
echo "<H3>Create colour picture </H3>">>$ZIEL
#echo "${MAINDIR}/${RGBDIR}/${COLORDIR}/${BVDIR}/PHOTCAT_calibration.dat"
if test -f ${MAINDIR}/${RGBDIR}/${COLORDIR}/${BVDIR}/PHOTCAT_calibration.dat
then 
echo "<H5>B-V Calibration at B-V </H5>" >>$ZIEL
awk  'NR==1{ print $0," stars<BR>"}' ${MAINDIR}/${RGBDIR}/${COLORDIR}/${BVDIR}/PHOTCAT_calibration.dat  >> $ZIEL
awk  'NR==2{ print "<tt>BLUE :</tt>" $0,"<BR>"}' ${MAINDIR}/${RGBDIR}/${COLORDIR}/${BVDIR}/PHOTCAT_calibration.dat  >> $ZIEL
awk  'NR==3{ print "<tt>GREEN:</tt>" $0,"<BR>"}' ${MAINDIR}/${RGBDIR}/${COLORDIR}/${BVDIR}/PHOTCAT_calibration.dat  >> $ZIEL
awk  'NR==4{ print "<tt>RED  :</tt>" $0,"<BR>"}' ${MAINDIR}/${RGBDIR}/${COLORDIR}/${BVDIR}/PHOTCAT_calibration.dat  >> $ZIEL
fi
if test -f ${MAINDIR}/${RGBDIR}/${COLORDIR}/${AVGDIR}/AVGWHITE_calibration.dat
then 
echo "<H5>Calibration at Average White</H5>" >>$ZIEL
awk  '{ print $0,"<BR>"}' ${MAINDIR}/${RGBDIR}/${COLORDIR}/${AVGDIR}/AVGWHITE_calibration.dat  >> $ZIEL
fi
}

#FITS to TIFF
getrgbtotiff(){
echo "<H5>Converting fits to tiff: Thresholds for Black- and Whitepoint </H5>">>$ZIEL
echo "<table>">>$ZIEL
echo "<tr><th>Filter</th><th>Blackpoint in fits (is 0 in tiff)</th><th>Whitepoint in fits ( is 255/65535 in tiff )</th></tr>">>$ZIEL
TH=`grep P_FITS2TIF ~/.theli/scripts/fits2tiff.sh |gawk '{print "<tr><td>" $5 "</td><td> " $7 "</td><td>" $8 " </td></tr>"}'`
echo "$TH" >>$ZIEL
echo "</table>">>$ZIEL
echo "<H5>Converting fits to tiff: Selected Area for background statistics</H5>">>$ZIEL 
echo "<table>">>$ZIEL
echo "<tr><th>x1</th><th>x2</th><th>y1</th><th>y2</th></tr><tr>">>$ZIEL
gawk 'NR==1{print "<td>" $2 "</td><td>" $3 "</td><td>" $4 "</td><td>" $5 "</td></tr>"}'  $MAINDIR/$SCIENCEDIR/$COLORDIR/stats.dat >> $ZIEL
echo "</table>">>$ZIEL
echo "<H5>Converting fits to tiff: Skylevel and noise (rms)</H5>">>$ZIEL 
echo "<table>">>$ZIEL
echo "<tr><th>sky</th><th>rms</th></tr>">>$ZIEL
gawk 'NR>1{print "<tr><td>" $1 "</td><td>" $2 "</td></tr>"}'  $MAINDIR/$SCIENCEDIR/$COLORDIR/stats.dat >> $ZIEL
echo "</table>">>$ZIEL
if test -f  $MAINDIR/$SCIENCEDIR/$COLORDIR/rgb_stiff.tif
then
echo "<H5>RGB-Stiff Preview </H5>">>$ZIEL
tifftopnm $MAINDIR/$SCIENCEDIR/$COLORDIR/rgb_stiff.tif |pnmscale -xsize=500 |pnmtojpeg >$MAINDIR/$SCIENCEDIR/$COLORDIR/rgb_stiff.jpg
echo "<a href=\"$MAINDIR/$SCIENCEDIR/$COLORDIR/rgb_stiff.jpg \"><img src=\"$MAINDIR/$SCIENCEDIR/$COLORDIR/rgb_stiff.jpg \" width=\"100\"></a>">>$ZIEL
fi
}


############
if [ "$1" == "-?" ]
then
usage
fi

if [ "$1" == "" ]
then
getObjectname
fi

#SETUP
getThelilog
getMaindir
getSciencedir
getCoadddir
getColorthelidir
getZiel

#SCIENCEDIR=`grep ^sciencedir= $THELILOG |cut -d"=" -f2`
## ist noch nicht allgemeingueltig

#BVDIR=B-V_calibration
AVGDIR=AVGWHITE_calibration
BVDIR=PHOTCAT_calibration

#########################################################
debugInfo
createHeader
createBody
get_infos
getfilter
calibrating
weighting
astromphotom
subsky
coadding
createcolour
getrgbtotiff
createFooter
startBrowser
