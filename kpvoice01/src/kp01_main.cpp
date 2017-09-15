#include "kp01_99_top.h"
#include "superStream.h"
//#include "streamController.h"

#define     _kpListenIP     "127.0.0.1"
#define     _kpTcp_rawpcm   "47811"
#define     _kpTcp_filtered "47813"
#define     _kpTcp_speaker  "47815"

/* 1  ch , 16000 Hz , s16le 
 * 1  sample -> 2 byte 
 * 1  ms -> 16  sample -> 2 * 16  byte == 32  byte 
 * 10 ms -> 160 sample -> 2 * 160 byte == 320 byte 
 * 30 ms -> 480 sample -> 2 * 480 byte == 960 byte 
 */

int32_t _time1 = _timeNow ;
int32_t _time2  ;
int32_t _time3  ;

_ssListen1 * _tcpRaw     = new _ssListen1( _enSsdOut , "tcpL1:" _kpListenIP ":" _kpTcp_rawpcm   , " when connected , try to output RAW-pcm " ) ;
_ssListen1 * _tcpReduce  = new _ssListen1( _enSsdOut , "tcpL1:" _kpListenIP ":" _kpTcp_filtered , " when connected , try to output noise-reduce-pcm " ) ;
_ssListen1 * _tcpSpeaker = new _ssListen1( _enSsdIn  , "tcpL1:" _kpListenIP ":" _kpTcp_speaker  , " when connected , try to input pcm for speaker" ) ;

int main( int ___argc , char ** ___argv ) {

    int __i01 = 1 ;
    while ( 1 ) {

        if ( __i01 % 33 == 1 ) {
            _time2 = _timeNow ;
            _time3 = _time2 - _time1 ;
            _prSFn( " --- %10d %10d , %10d " , _time2 , _time3 , __i01 ) ;
            _ffstdout ;
        }

        __i01 ++ ;
        if ( 0 )    { _sleep_30ms   ; } 
        else        { _sleep_500ms  ; }
    }

    return 0 ;
} /* main */

