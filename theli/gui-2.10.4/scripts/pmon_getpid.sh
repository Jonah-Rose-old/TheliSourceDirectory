#!/bin/bash 
# reads pid
# $1 = string to search

# Global Variable
using()
{
echo "Using: "
echo "$0 string"
echo "reads pid"
echo "                             ---"
echo "Author: Carsten Moos, cmoos@gmx.de"
}
if [ "$1" == "" ]
then
using
echo 0
exit -1
fi
#if pgrep -U $USER -f $1 >/dev/null; then pgrep -U $USER -f $1 |grep -v getpid|head -n1;else echo 0; fi;
#pgrep -f $1
#ps -U $USER ax |grep -v grep|grep -v getpid|grep $1|head -n1|cut -d" " -f2
#echo: write error: Broken pipe without 2>>/dev/null ???? no idea why
ps -U $USER ax |grep -v grep|grep -v getpid|grep $1|head -n1|awk '{print $1}'
echo  "0" 2>>/dev/null
