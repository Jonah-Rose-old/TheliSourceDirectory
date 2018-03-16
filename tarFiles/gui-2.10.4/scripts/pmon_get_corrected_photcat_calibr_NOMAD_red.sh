BASHPATH
# reads $1 file
# extracts Red calibration factor of NOMAD and multiplies by $RED
# File expected is  PHOTCAT_calibration.dat
#RED=1.05
using()
{
echo "Using: "
echo "$0 science/color_theli/PHOTCAT_calibration/PHOTCAT_calibartion.dat"
echo "                             ---"
echo "Author: Carsten Moos, cmoos@gmx.de"
}

if [ -f $1 ]
then
gawk 'NR==4{print $1*1.05}' $1
else
echo " "
fi
