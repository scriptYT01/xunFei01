#!/bin/sh

[ -d /ch/xf10 ]     && cd /ch/xf10 
[ -d /xf10 ]        && cd /xf10 

while [ 1 ]
do
    echo
    #echo    "killall aiui_sample32.x1000.bin ; nc110.x1000.bin -l -p 33221 |./tts_sample11.x1000.bin - |aaa91_aplay__S16_LE_16000_pcm_or_wav"
    #echo ;   killall aiui_sample32.x1000.bin ; nc110.x1000.bin -l -p 33221 |./tts_sample11.x1000.bin - |aaa91_aplay__S16_LE_16000_pcm_or_wav

    echo  "lsof  |grep snd |awk '{print \$1}' |xargs -n 1 kill -9"
    echo ; lsof  |grep snd |awk '{print  $1}' |xargs -n 1 kill -9

    echo    "killall aiui_sample32.x1000.bin "
    echo ;   killall aiui_sample32.x1000.bin 

    echo    "nc110.x1000.bin -l -p 33221 |./tts_sample11.x1000.bin - |aplay -f S16_LE  -r 16000 "
    echo ;   nc110.x1000.bin -l -p 33221 |./tts_sample11.x1000.bin - |aplay -f S16_LE  -r 16000 
    echo
    

    [ 1 == "$1" ] && exit
    sleep 1
done
