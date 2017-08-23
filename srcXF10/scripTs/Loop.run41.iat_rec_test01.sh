#!/bin/sh

[ -d /ch/xf10 ]     && cd /ch/xf10 
[ -d /xf10 ]        && cd /xf10 

while [ 1 ]
do
    echo    "./iat_record_sample18.x1000.bin 0 1   'hw:0,2'    1    16000"
             ./iat_record_sample18.x1000.bin 0 1   'hw:0,2'    1    16000

    [ 1 == 2 ] || exit
done
