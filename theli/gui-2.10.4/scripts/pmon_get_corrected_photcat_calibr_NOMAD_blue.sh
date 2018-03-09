#!/bin/bash
# reads $1 file
# extracts Red calibration factor of NOMAD and multiplies by $RED
# File expected is  PHOTCAT_calibration.dat
#GREEN=0.9
using()
{
echo "Using: "
echo "$0 science/color_theli/PHOTCAT_calibration/PHOTCAT_calibration.dat"
echo "                             ---"
echo "Author: Carsten Moos, cmoos@gmx.de"
}

if [ -f $1 ]
then
gawk 'NR==2{print $1*0.9}' $1
else
echo " "
fi

