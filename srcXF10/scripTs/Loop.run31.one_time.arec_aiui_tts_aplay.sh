#!/bin/sh

[ -d /ch/xf10 ] && cd /ch/xf10 || cd /xf10    

(lsof |grep snd |grep pvalg_ymhood ) && killall pvalg_ymhood

killall led_flash.sh

/etc/YMwifi/led_on_off.sh on
aa11=$(date +%s)
aa00=${aa11}
echo '# before record ' ${aa11}
nohup aaa61_arecord_s16_le_16k__mono -d 5 /tmp/wav01.wav > /tmp/logW1.arecord.txt
aa12=$(date +%s)
echo '# end record '  "$((${aa12}-${aa11})) , ${aa12}"
/etc/YMwifi/led_on_off.sh off

if [ $((${aa12}-${aa11})) -lt 3 ]
then
    echo 
    echo ' mic is hold by another prog, please check and run again '
    echo
    sleep 30
    exit
fi


[ -d /ch/xf30 ] && cd /ch/xf30 || cd /xf30    
nohup ./aiui_sample32.x1000.bin    /tmp/wav01.wav  > /tmp/w0.txt

exit




aa11=$(date +%s)
echo '# before recognize ' ${aa11}
nohup ./iat_sample15.x1000.bin /tmp/wav01.wav > /tmp/w1.txt
aa12=$(date +%s)
echo '# end recognize '  "$((${aa12}-${aa11})) , ${aa12}"

cat /tmp/w1.txt |grep ^succee_iat_result: |head -n 1 > /tmp/w2.txt
cat /tmp/w2.txt |sed -e 's;^succee_iat_result:;;g' |tr -d '\r' |tr -d '\n' > /tmp/w3.txt
cat /tmp/w1.txt |grep ^'QISRGetResult failed' |tr -d '\r' |tr -d '\n' > /tmp/w4.txt
ll1=$(cat /tmp/w3.txt  |wc -c)
ll2=$(cat /tmp/w3.txt)
ll3=$(cat /tmp/w4.txt|wc -c )
if [ \
    "${ll1}" != 0 \
    -a "${ll3}" == 0 \
    -a "${ll2}" != 'nomatch:noisy' \
    -a "${ll2}" != 'nomatch:out-of-voca' \
    ]
then
    echo "${ll2} : /tmp/w3.txt : recognize ok  : ${ll1}"
else
    echo " recognize failed  : ${ll1}"
    cat /tmp/w1.txt

    echo
    echo ' you can use the following to test the result : '
    echo '    aplay /tmp/wav01.wav '
    echo

    exit 22
fi

aa11=$(date +%s)
echo '# before tts ' ${aa11}
nohup ./tts_sample11.x1000.bin /tmp/w3.txt > /tmp/logW4.tts.recognize.txt
aa12=$(date +%s)
echo '# end tts '  "$((${aa12}-${aa11})) , ${aa12}"

# /etc/YMwifi/led_on_off.sh on
aa11=$(date +%s)
echo '# before play ' ${aa11}
aplay  Tts_sample.wav 
aa12=$(date +%s)
echo '# end play '  "$((${aa12}-${aa11})) , ${aa12}"
# /etc/YMwifi/led_on_off.sh off

echo " total $((${aa12}-${aa00}))  "

