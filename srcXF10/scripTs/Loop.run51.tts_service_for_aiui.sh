#!/bin/sh

[ -d /ch/xf10 ]     && cd /ch/xf10 
[ -d /xf10 ]        && cd /xf10 

ifconfig lo up
ip link set lo up

if [ 1 = 0 ]
then
    echo  "lsof  |grep snd |awk '{print \$1}' |xargs -n 1 kill -9"
    echo ; lsof  |grep snd |awk '{print  $1}' |xargs -n 1 kill -9


    echo    "killall aiui_sample32.x1000.bin "
    echo ;   killall aiui_sample32.x1000.bin 

    echo    "killall nc110.x1000.bin "
    echo ;   killall nc110.x1000.bin 

    echo    "killall aplay "
    echo ;   killall aplay 

    echo    "killall arecord "
    echo ;   killall arecord 
fi

while [ 1 ]
do
    echo
    #echo    "killall aiui_sample32.x1000.bin ; nc110.x1000.bin -l -p 33221 |./tts_sample11.x1000.bin - |aaa91_aplay__S16_LE_16000_pcm_or_wav"
    #echo ;   killall aiui_sample32.x1000.bin ; nc110.x1000.bin -l -p 33221 |./tts_sample11.x1000.bin - |aaa91_aplay__S16_LE_16000_pcm_or_wav

    if [ 1 = 1 ]
    then
        echo  "lsof  |grep snd |awk '{print \$1}' |xargs -n 1 kill -9"
        echo ; lsof  |grep snd |awk '{print  $1}' |xargs -n 1 kill -9


        echo    "killall aiui_sample32.x1000.bin "
        echo ;   killall aiui_sample32.x1000.bin 

        echo    "killall nc110.x1000.bin "
        echo ;   killall nc110.x1000.bin 

        echo    "killall aplay "
        echo ;   killall aplay 

        echo    "killall arecord "
        echo ;   killall arecord 
    fi

    ping -c 2 -q 114.114.114.114 &>/dev/null && pp1=11 || pp1=22
    [ "${pp1}" = "11" ] || continue


    #echo    "nc110.x1000.bin -l -p 33221 -w 5 |./tts_sample11.x1000.bin - |aplay -f S16_LE  -r 16000 "
    #echo ;   nc110.x1000.bin -l -p 33221 -w 5 |./tts_sample11.x1000.bin - |aplay -f S16_LE  -r 16000 
    #echo    "nc110.x1000.bin -w 15 127.0.0.1 33221 |./tts_sample11.x1000.bin - |aplay -f S16_LE  -r 16000 "
    #echo ;   nc110.x1000.bin -w 15 127.0.0.1 33221 |./tts_sample11.x1000.bin - |aplay -f S16_LE  -r 16000 
    echo    "nc110.x1000.bin -w 15 -l -p 33221 |./tts_sample11.x1000.bin - |aplay -f S16_LE  -r 16000 "
    echo ;   nc110.x1000.bin -w 15 -l -p 33221 |./tts_sample11.x1000.bin - |aplay -f S16_LE  -r 16000 
    echo

    

    [ 1 == "$1" ] && exit
    sleep 1
done
