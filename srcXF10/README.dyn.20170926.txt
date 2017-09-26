1.
major execute bin file :

1.1: <dir KPvoice01>
kp01/kp01.x1000.bin
the simulation server , the same function as the KPvioce's prog,
provide 2 port for rawPCM & filterdPCM output , 127.0.0.1:47811 , 127.0.0.1:47813 , 
and 1 port for speakerPCM generation , 127.0.0.1:47815 . 


1.2: <dir XFclient30>
xf30/aiui_sample32.x1000.bin
the aiui SDK's execute binary file.
work under wifi-on

1.3: <dir XFclient10>
xf10/tts_sample11.x1000.bin
the text-to-spoken SDK's execute binary file.
normally , work under wifi-on.
but can work when wifi-off , if pay, accroding to XunFei's document.

2.
Usage :
2.1
cd /ch/kp01 
./kp01.x1000.bin
then the fake kpvioce-servier start , listen on 47811,47813,47815
if , need debug info , connect to 47819 , the the service exit , and print the debug info

2.2
get the PCM from kp01 and play to speaker directly :
nc110.x1000.bin 127.0.0.1 47811 | ./aiui_sample32.x1000.bin - - |/ch/xf10/tts_sample11.x1000.bin - | aplay -f S16_LE  -r 16000

2.3
get the PCM from kp01 and play to speaker directly :
nc110.x1000.bin 127.0.0.1 47811 | ./aiui_sample32.x1000.bin - - |/ch/xf10/tts_sample11.x1000.bin - | nc110.x1000.bin 127.0.0.1 47815 
then , the generated PCM replay be fake-kpvioce.

3.
then other director, 
is some small tool using for test.
for example <XFi386DST??> is used to test under the base i386 platform.

