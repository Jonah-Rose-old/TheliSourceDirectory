BASHPATH  
# call soundfile 
# 
#config by user
THELI=`which theli`
THELI=`dirname $THELI`
SOUNDFILE=$THELI/scripts/pmon_gong.mp3
# file is free from http://www.sounddogs.com/sound-effects/101/mp3/103949_SOUNDDOGS__be.mp3

using()
{
echo "$0" 
echo "calls play $SOUNDFILE"
}

if [ "$1" == "-?" ]
then
using
exit -1
fi
#play $SOUNDFILE 2>/dev/null
#echo -e "\a"
mpg123 $SOUNDFILE &
