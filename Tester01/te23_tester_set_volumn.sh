#!/bin/sh

if [ -z "$1" ]
then
    amixer  sget TITANIUM |grep Playback |tail -n 1 |awk '{print $5}'
else
    amixer sset TITANIUM  ${1}%                     &> /dev/null 
    amixer sset MERCURY   ${1}%                     &> /dev/null 
fi

