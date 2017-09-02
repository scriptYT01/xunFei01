#!/bin/sh

[ -d /ch/xf30 ]     && cd /ch/xf30 
[ -d /xf30 ]        && cd /xf30 

while [ 1 ]
do
    echo
    echo    "arecord -D hw:0,2 -f S16_LE -r 16000 -c 1 | ./aiui_sample32.x1000.bin - - | nc110.x1000.bin 127.0.0.1 33221"
    echo
    echo;    arecord -D hw:0,2 -f S16_LE -r 16000 -c 1 | ./aiui_sample32.x1000.bin - - | nc110.x1000.bin 127.0.0.1 33221

    [ 1 == "$1" ] || exit
    sleep 3
done
