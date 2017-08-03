#!/bin/sh

cd /ch/xf10

killall led_flash.sh

aa11=$(date +%s)
echo '# before record ' ${aa11}
aaa61_arecord_s16_le_16k__mono -d 5 /tmp/wav01.wav 
aa12=$(date +%s)
echo '# end record '  "$((${aa12}-${aa11})) , ${aa12}"

aa11=$(date +%s)
echo '# before recognize '
# ./asr_sample13.x1000.bin /tmp/wav01.wav 
nohup ./asr_sample13.x1000.bin /tmp/wav01.wav > /tmp/w1.txt
echo '# end recognize '

cat /tmp/w1.txt |grep ^confidence= |head -n 1 > /tmp/w2.txt
cat /tmp/w2.txt |sed -e 's;^.*input=;;g' |tr -d '\r' |tr -d '\n' > /tmp/w3.txt

aa11=$(date +%s)
echo '# before tts '
./tts_sample11.x1000.bin /tmp/w3.txt
echo '# end tts '

aa11=$(date +%s)
echo '# before play '
aplay  Tts_sample.wav 
echo '# end play '
