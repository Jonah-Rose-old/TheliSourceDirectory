BASHPATH
#ds9 -view  horizontal -rgb -red n6960b-020_R_1OFC.fits -scale sqrt -zscale #-green n6960b-020_G_1OFC.fits -scale sqrt -zscale -blue #n6960b-020_B_1OFC.fits -scale sqrt -zscale -rgb lock scale yes -rgb lock #colorbar yes

EXT=1OFC
SUF=fits
SCL="-scale sqrt -zscale"
DS9="ds9 -view horizontal -rgb"
#CLOSE="-rgb close"
LOCK1="-rgb lock scale yes"
#LOCK2="-rgb lock bin yes" 
LOCK3="-rgb lock colorbar yes" 

IMAGE=""
   
usage()
{
echo " version 1.0"
echo "$0 sciencedir"
echo combines the first image as RGB
echo "by Carsten Moos"
}

check()
{
if [ ! -f "$1" ]
then
echo "$1 not found, closing"
exit -1
fi
}

get_first_file()
{
# only first image of directory
image=`ls $1/*_G_1OFC.$SUF|awk 'NR==1{print $1}'`
IMAGE=`basename $image _G_$EXT.$SUF`
}

if [ "$1" == "" ]
then
usage
exit -2
fi
get_first_file $1
R=$1/${IMAGE}_R_1OFC.$SUF
G=$1/${IMAGE}_G_1OFC.$SUF
B=$1/${IMAGE}_B_1OFC.$SUF
check $R
check $G
check $B

red="-red $R $SCL"
green="-green $G $SCL"
blue="-blue $B $SCL"
#echo "calls: $DS9 $red $green $blue $CLOSE "
$DS9 $red $green $blue $LOCK1 $LOCK3 &
