1.
in kp01_main.cpp ,
the _paraAnalyze() is use to generate 3 fake pipe, 
which to generate the raw-pcm / noise-filtered-pcm , and receive the speaker-pcm

2.
_initListen(),
open 3 port and waiting for the raw-pcm-client / noise-filtered-pcm-client / speaker-pcm-client.

3.
the xunfei's buffer need pcmpackage less than 1200 byte.
so, I select 960 byte each frame.
then the gap time is 32000 / 960 == 33.333 ms
the frame size 's name is "_pcmLenRaw"
  we can change the size anytime, 
but the gap-time should be adjusted at the same time.

4.
the _fill_data() ,
deal with the 3 pipe.
4.1
it check the data-source , if data is ready , try to send to the other end.
if src-data is not ready , just ignore.
if the dst-pipe is not connected, just ignore.
4.2.
all things run under nonblock-mode , to fit the realtime requirment.

5.
test :
5.1 : kp01.x1000.bin can be run in X1000
5.2 : 
/tmp/t.in.pcmRaw.txt and /tmp/t.in.pcmReduce.txt should be provide as the fake datasource.
when :
nc 127.0.0.1 47811 | aplay -f S16_LE -r 16000 
the /tmp/t.in.pcmRaw.txt is transport by the pipe in current 32000 Bps.
whne :
nc 127.0.0.1 47813 | aplay -f S16_LE -r 16000 
the /tmp/t.in.pcmReduce.txt is transport by the pipe in current 32000 Bps.
5.3:
KPvoice should replace the two fake PCM-source.

6.
when viomi-client success got PCM and want to play by X1000 :
cat XXX.pcm | nc 127.0.0.1 47815
that is : it will try to open the 47815 and try to fill the PCM data.
KPvoice should play the received PCM with the KPvioce's PCM to the speaker at the same time.







