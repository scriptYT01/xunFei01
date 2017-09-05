#!/bin/sh

[ -d /ch/xf30 ]     && cd /ch/xf30 
[ -d /xf30 ]        && cd /xf30 

export PCM00=/ch/11/20170904_16000hz_S16LE_1channel.pcm.person
export PCM01=${PCM00}.${1}.pcm 

if [ -z "$2" -a ! -f ${PCM01} ]
then
    echo ; echo "file ${PCM01} don't exist. exit."
    echo
    exit
fi

## if $2 is not NULL : to specifiy dstIP 
dstIP=$2
[ -z "$dstIP" ] && dstIP=127.0.0.1

while [ 1 ]
do
    ping -c 5 -q 114.114.114.114 &>/dev/null && pp1=11 || pp1=22
    [ "${pp1}" = "11" ] || continue

    echo
    #echo    "arecord -D hw:0,2 -f S16_LE -r 16000 -c 1 | ./aiui_sample32.x1000.bin - - | nc110.x1000.bin -w 3 127.0.0.1 33221"
    #echo
    #echo;    arecord -D hw:0,2 -f S16_LE -r 16000 -c 1 | ./aiui_sample32.x1000.bin - - | nc110.x1000.bin -w 3 127.0.0.1 33221

    ## if $2 is not NULL : to specifiy dstIP , connect to the dstIP and get the PCM 
    if [ -z "$2" ]
    then
        echo
        echo    "cat ${PCM01} | ./aiui_sample32.x1000.bin - - | nc110.x1000.bin -w 3     ${dstIP}     33221"
        echo
        echo ;   cat ${PCM01} | ./aiui_sample32.x1000.bin - - | nc110.x1000.bin -w 3     ${dstIP}     33221 
    else
        echo
        echo    "nc110.x1000.bin -w 3 ${dstIP} 33225 | ./aiui_sample32.x1000.bin - - | nc110.x1000.bin -w 3     ${dstIP}     33221"
        echo
        echo ;   nc110.x1000.bin -w 3 ${dstIP} 33225 | ./aiui_sample32.x1000.bin - - | nc110.x1000.bin -w 3     ${dstIP}     33221 
    fi

    break ;
    [ 1 == "$1" ] && exit
    sleep 4

    ## if $2 is not NULL : to specifiy dstIP , so exit
    [ -n "$2" ] && exit
    
done

