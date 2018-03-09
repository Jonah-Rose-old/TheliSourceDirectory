#!/bin/bash -xv

# $1: maindir/sciencedir/
# $2: plot type (1 = bg (default) level, 2 = rms 3= star quality, 4 = RZP)

# creates a plotfile, calls gnuplot, epstopdf, pdf2ppm, acroread in bg"
# Output: statfile_type.eps ==> pdf ==> ppm"
# -----"
# X-Axis: Filelist is represented as a numbered list like 0,1,2,3,..."
# Y-Axis is autoscaled"
# Wishes: xaxis starts with 1, all pages in one pdf ;-)"

. progs.ini

if [ ! -f $1/statistics.dat ]; then
    exit
fi

if [ ! -d /$1/statplots/ ]; then
    mkdir /$1/statplots
fi

TARGET=${TEMPDIR}/values_$$
PLOT=${TEMPDIR}/statistics.plt_$$
OUTPUT=`basename /$1/statistics.dat .dat`
FILE1=`${P_GAWK} '(NR==1) {print $1}' /$1/statistics.dat`

touch $PLOT
#DEFAULT
if [ "$2" = "" ]; then
    type=1
else
    type=$2
fi
# Separating cases
case "$type" in
    1)
	${P_GAWK} '(NR>0) {print $2}' $1/statistics.dat > $TARGET.1
	${P_GAWK} '(NR>0) {print $3}' $1/statistics.dat > $TARGET.2
	${P_GAWK} '(NR>0) {print $4}' $1/statistics.dat > $TARGET.3
	YLABEL="ADU"
	SUBTITLE="Sky background"
	;;
    2)
	${P_GAWK} '(NR>0) {print $5}' $1/statistics.dat > $TARGET.1
	YLABEL="ADU"
	SUBTITLE="Background noise (rms)"
	;;
    3)
	${P_GAWK} '(NR>0) {print $6 }' $1/statistics.dat > $TARGET.1
	${P_GAWK} '(NR>0) {print $8 }' $1/statistics.dat > $TARGET.2
	YLABEL="Pixel"
	SUBTITLE="Mean FWHM and HFD"
	;;
    4)
	${P_GAWK} '(NR>0) {print $10}' $1/statistics.dat > $TARGET.1
	YLABEL="MAG"
	SUBTITLE="Relative photometric zeropoints"
	;;
esac

#cat $TARGET
OUTPUT=/$1/statplots/${OUTPUT}_${type}

#echo "Type: $type Label: $YLABEL Target $TARGET Plot: $PLOT"
echo "set title \"${SUBTITLE}\"" >> $PLOT
echo "set xlabel \"file\""         >> $PLOT
echo "set ylabel \"$YLABEL\" "     >> $PLOT
echo "set terminal postscript eps" >> $PLOT
echo "set output \"$OUTPUT.eps\""  >> $PLOT

case "$type" in
    1)
	echo "plot [] [] [] '$TARGET.1' t\"Mean\" ps 0.5 pt 1 with linespoints, '$TARGET.2' t\"Mode\" ps 0.5 pt 2 with linespoints ,'$TARGET.3' t\"Median\" ps 0.5 pt 3 with linespoints">>$PLOT
	;;
    2)
	echo "plot [] '$TARGET.1' t\"RMS\" ps 0.5 pt 5 with linespoints">>$PLOT	
	;;
    3)
	echo "plot [] [] '$TARGET.1' t\"FWHM\" ps 0.5 pt 1 with linespoints, '$TARGET.2' t\"HFD\" ps 0.5 pt 2 with linespoints">>$PLOT
	;;
    4)
	echo "plot [] '$TARGET.1' t\"RZP\" ps 0.5 pt 5 with linespoints">>$PLOT
	;;
esac

echo "Outputfile: $OUTPUT.pdf"
gnuplot $PLOT && epstopdf $OUTPUT.eps
pdftoppm $OUTPUT.pdf $OUTPUT
mv $OUTPUT-000001.ppm $OUTPUT.ppm
rm $TARGET.?
rm $PLOT
rm $OUTPUT.eps

acroread $OUTPUT.pdf &
