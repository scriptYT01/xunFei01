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

#endif
