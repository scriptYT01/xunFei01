
all target list as following :
target ==-->>> kp01.x1000.bin ( patchDIR__x1000/KPvoice01/Build.mk )  ========================
target ==-->>> TEster01.x1000.bin ( patchDIR__x1000/tEster01/Build.mk )  ========================
target ==-->>> tts_sample11.x1000.bin ( patchDIR__x1000/XFclient10/Build.mk )  ========================
target ==-->>> tts_tcp12.x1000.bin ( patchDIR__x1000/XFclient10/Build.mk ) 
target ==-->>> asr_sample13.x1000.bin ( patchDIR__x1000/XFclient10/Build.mk ) 
target ==-->>> asr_tcp14.x1000.bin ( patchDIR__x1000/XFclient10/Build.mk ) 
target ==-->>> iat_sample15.x1000.bin ( patchDIR__x1000/XFclient10/Build.mk ) 
target ==-->>> ise_sample17.x1000.bin ( patchDIR__x1000/XFclient10/Build.mk ) 
target ==-->>> tts_sample21.x1000.bin ( patchDIR__x1000/XFclient20/Build.mk ) 
target ==-->>> tts_sample31.x1000.bin ( patchDIR__x1000/XFclient30/Build.mk ) 
target ==-->>> aiui_sample32.x1000.bin ( patchDIR__x1000/XFclient30/Build.mk )  ========================
target ==-->>> dbmem ( patchDIR__x1000/XFclient30/Build.mk ) 
target ==-->>> ym_hood_wifi ( patchDIR__x1000/YMclient01/Build.mk ) 
target ==-->>> miio_wifi ( patchDIR__x1000/YMclient01/Build.mk ) 
target ==-->>> ymTester_1001_unix_socket_show_only.bin  ( patchDIR__x1000/YMclient03/Build.mk ) 
target ==-->>> ymClient13001_tty.bin ( patchDIR__x1000/YMclient03/Build.mk ) 
target ==-->>> ymClient14001_xiaomi.bin ( patchDIR__x1000/YMclient03/Build.mk ) 
target ==-->>> ymClient15001_offline_vin.bin ( patchDIR__x1000/YMclient03/Build.mk ) 
target ==-->>> ymClient16001_offline_vout.bin ( patchDIR__x1000/YMclient03/Build.mk ) 
target ==-->>> ymClient17001_ym_debuging.bin ( patchDIR__x1000/YMclient03/Build.mk ) 
target ==-->>> ymClient11_startStopService_daemon ( patchDIR__x1000/YMclient03/Build.mk ) 


001.
target ==-->>> kp01.x1000.bin ( patchDIR__x1000/KPvoice01/Build.mk ) 
this is a fake server daemon to simulation the function of 
KPvioice's raw-pcm / noise-filetered-pcm / speaker-pcm port
to service the AIUI client.
under constructing.

2.
target ==-->>> TEster01.x1000.bin ( patchDIR__x1000/tEster01/Build.mk ) 
this is the daemon to work in X1000 as the testbench to test another target X1000, 
testing the base offline-recognization function.
under constructing.

3.
target ==-->>> tts_sample11.x1000.bin ( patchDIR__x1000/XFclient10/Build.mk ) 
the XunFei's TTS client. ok. and will used in the online-voice-recognization.
text-to-pcm / text-to-speaker.

4.
target ==-->>> tts_tcp12.x1000.bin ( patchDIR__x1000/XFclient10/Build.mk ) 
the XunFei's TTS client. testing only. no other used.
text-to-pcm / text-to-speaker.

5.
target ==-->>> asr_sample13.x1000.bin ( patchDIR__x1000/XFclient10/Build.mk ) 
the XunFei's ASR client. ok. but not used in our product.
pcm-to-text, input pcm is size limited.

6.
target ==-->>> asr_tcp14.x1000.bin ( patchDIR__x1000/XFclient10/Build.mk ) 
the XunFei's ASR client. testing only. no other used.
pcm-to-text, input pcm is size limited.

7.
target ==-->>> iat_sample15.x1000.bin ( patchDIR__x1000/XFclient10/Build.mk ) 
the XunFei's IAT client. ok. but not used in our product.
pcm-to-text, continue pcm.

8.
target ==-->>> ise_sample17.x1000.bin ( patchDIR__x1000/XFclient10/Build.mk ) 
the XunFei's ISE client. ok. but havn't testing or used.

9.
target ==-->>> tts_sample21.x1000.bin ( patchDIR__x1000/XFclient20/Build.mk ) 
the XunFei's TTS client. testing only. no other used.

10.
target ==-->>> tts_sample31.x1000.bin ( patchDIR__x1000/XFclient30/Build.mk ) 
the XunFei's TTS client. testing only. no other used.

11.
target ==-->>> aiui_sample32.x1000.bin ( patchDIR__x1000/XFclient30/Build.mk ) 
the XunFei's AIUI client. ok... waiting for the input / ouput from kpvioce's API.

12.
target ==-->>> dbmem ( patchDIR__x1000/XFclient30/Build.mk ) 
a debug to to test the memony leak.
it will generate the debug info every time when the "malloc" and "free" func are called.

13.
target ==-->>> ym_hood_wifi ( patchDIR__x1000/YMclient01/Build.mk ) 
kpvice's client 1.

14.
target ==-->>> miio_wifi ( patchDIR__x1000/YMclient01/Build.mk ) 
kpvice's client 2.

15.
target ==-->>> ymTester_1001_unix_socket_show_only.bin  ( patchDIR__x1000/YMclient03/Build.mk ) 
16.
target ==-->>> ymClient13001_tty.bin ( patchDIR__x1000/YMclient03/Build.mk ) 
17.
target ==-->>> ymClient14001_xiaomi.bin ( patchDIR__x1000/YMclient03/Build.mk ) 
18.
target ==-->>> ymClient15001_offline_vin.bin ( patchDIR__x1000/YMclient03/Build.mk ) 
19.
target ==-->>> ymClient16001_offline_vout.bin ( patchDIR__x1000/YMclient03/Build.mk ) 
20.
target ==-->>> ymClient17001_ym_debuging.bin ( patchDIR__x1000/YMclient03/Build.mk ) 
21.
target ==-->>> ymClient11_startStopService_daemon ( patchDIR__x1000/YMclient03/Build.mk ) 

15-21 : the message-exchange-daemon to exclusive equipments.
15 , 20 , 21 are debug tools.
16,17,18,19 are daemon to tty , xiaomi, offline-vin, offline-vout 

