#!/bin/sh

[ -d /ch/xf30 ]     && cd /ch/xf30 
[ -d /xf30 ]        && cd /xf30 

export PCM00=/ch/11/20170904_16000hz_S16LE_1channel.pcm.person
export PCM01=${PCM00}.${1}.pcm 


dstIP=$1
#[ -z "$dstIP" ] && dstIP=127.0.0.1
[ -z "$dstIP" ] && exit

echo ;   
nc110.x1000.bin -w 3 ${dstIP} 33225 \
    | ./aiui_sample32.x1000.bin - - 

#  | nc110.x1000.bin -w 3     ${dstIP}     33221 
