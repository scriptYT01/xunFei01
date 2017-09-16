#ifndef     __KP01_01_DATASTRURE_H
#define     __KP01_01_DATASTRURE_H

#include "kp01_99_top.h"

//enum _enTcpDir {
//    _enTDin,        // input only
//    _enTDout,       // output only
//    _enTDinout      // both
//}; /* _enTcpDir */
//
extern int32_t _time1  ;
extern int32_t _time2  ;
extern int32_t _time3  ;

extern const char * _FrawPcm     ;
extern const char * _FreducePcm  ;
extern const char * _FspekerPcm  ;

extern _ssFileIn  * _fGenRawPcm        ;
extern _ssFileIn  * _fGenReducePcm     ;
extern _ssFileOut * _fSpeaker          ;

extern _ssListen1 * _tcpRaw            ;
extern _ssListen1 * _tcpReduce         ;
extern _ssListen1 * _tcpSpeaker        ;

/* 1  ch , 16000 Hz , s16le 
 * 1  sample -> 2 byte 
 * 1  ms -> 16  sample -> 2 * 16  byte == 32  byte 
 * 10 ms -> 160 sample -> 2 * 160 byte == 320 byte 
 * 30 ms -> 480 sample -> 2 * 480 byte == 960 byte 
 */
#define _pcmLenRaw 960

#endif
