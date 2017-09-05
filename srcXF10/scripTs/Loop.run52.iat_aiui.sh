#!/bin/sh

[ -d /ch/xf30 ]     && cd /ch/xf30 
[ -d /xf30 ]        && cd /xf30 

while [ 1 ]
do
    ping -c 2 -q 114.114.114.114 &>/dev/null && pp1=11 || pp1=22
    [ "${pp1}" = "11" ] || continue

    sleep 2

    echo
    #echo    "arecord -D hw:0,2 -f S16_LE -r 16000 -c 1 | ./aiui_sample32.x1000.bin - - | nc110.x1000.bin -w 15 -l -p 33221"
    #echo
    #echo;    arecord -D hw:0,2 -f S16_LE -r 16000 -c 1 | ./aiui_sample32.x1000.bin - - | nc110.x1000.bin -w 15 -l -p 33221
    echo    "arecord -D hw:0,2 -f S16_LE -r 16000 -c 1 | ./aiui_sample32.x1000.bin - - | nc110.x1000.bin -w 15 127.0.0.1 33221"
    echo
    echo;    arecord -D hw:0,2 -f S16_LE -r 16000 -c 1 | ./aiui_sample32.x1000.bin - - | nc110.x1000.bin -w 15 127.0.0.1 33221

    if [ 2 = 1 ]
    then

        echo    "killall aiui_sample32.x1000.bin "
        echo ;   killall aiui_sample32.x1000.bin 

        echo    "killall nc110.x1000.bin "
        echo ;   killall nc110.x1000.bin 

        echo    "killall aplay "
        echo ;   killall aplay 

        echo    "killall arecord "
        echo ;   killall arecord 
    fi


    [ 1 == "$1" ] && exit
done
