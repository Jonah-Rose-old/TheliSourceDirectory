BASHPATH
#call an  imageviewer
#$1 image to display
# Author: Carsten Moos, cmoos@gmx.de
using()
{
echo "Using: "
echo "$0 imagefile "
echo "                             ---"
echo "Author: Carsten Moos, cmoos@gmx.de"
}
VIEWER=eog
#VIEWER=display
if test -f "$1" 
then
($VIEWER $1) &
fi
