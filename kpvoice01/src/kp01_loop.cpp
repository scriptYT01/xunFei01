#include "kp01_99_top.h"

void _pipe01_RawPCM(void) {
    char    __buf1024[_pcmLenRaw] ;
    int     __len ;

    if ( _tcpRaw -> _canWrite( true ) ) {
        __len = _fGenRawPcm -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        if ( __len != _pcmLenRaw ) {
            __len = _fGenRawPcm -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
        if ( __len == _pcmLenRaw ) {
            __len = _tcpRaw -> _ssWriteNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
    }
    if( 0 && __len > 0 ) {
        _tcpRaw     -> dumpSelfX();
        _fGenRawPcm   -> dumpSelfX();
        _prExit( " testing " );
    }
} /* _pipe01_RawPCM */

void _pipe02_ReducedPCM(void) {
    char    __buf1024[_pcmLenRaw] ;
    int     __len ;

    if ( _tcpReduce -> _canWrite( true ) ) {
        __len = _fGenReducePcm -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        if ( __len != _pcmLenRaw ) {
            __len = _fGenReducePcm -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
        if ( __len == _pcmLenRaw ) {
            __len = _tcpReduce -> _ssWriteNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
    }
    if( 0 && __len > 0 ) {
        _tcpReduce     -> dumpSelfX();
        _fGenReducePcm   -> dumpSelfX();
        _prExit( " testing " );
    }
} /* _pipe02_ReducedPCM */

void _pipe03_speaker_to_fake_file(void) {
    char    __buf1024[_pcmLenRaw] ;
    int     __len ;

    if ( _fSpeaker -> _canWrite( true ) ) {
        __len = _tcpSpeaker -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        if ( __len != _pcmLenRaw ) {
            __len = _tcpSpeaker -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
        if ( __len == _pcmLenRaw ) {
            __len = _fSpeaker -> _ssWriteNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
    } 
} /* _pipe03_speaker_to_fake_file */

void _pipe04_speaker_to_tcp(void) {
    char    __buf1024[_pcmLenRaw] ;
    int     __len ;

    if ( _tcpSend1 -> _canWrite( true ) ) {
        __len = _tcpSpeaker -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        if ( __len != _pcmLenRaw ) {
            __len = _tcpSpeaker -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
        if ( __len == _pcmLenRaw ) {
            __len = _tcpSend1 -> _ssWriteNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
    } 
} /* _pipe04_speaker_to_tcp */

void _fill_data01(void) {
    if ( 1 ) _pipe01_RawPCM();
    if ( 1 ) _pipe02_ReducedPCM();
    if ( 1 ) _pipe03_speaker_to_fake_file();

} /* _fill_data01 */

void _fill_data02(void) {
    if ( 1 ) _pipe01_RawPCM();
    if ( 1 ) _pipe02_ReducedPCM();
    if ( 1 ) _pipe04_speaker_to_tcp();

} /* _fill_data02 */

#define _debugCNT_MAX  33

int _debugCNT01 = 0 ;
#define _debugCNT01_MAX  (_debugCNT_MAX * 15)
void _debugDump01(void) {
    if ( 1 ) _pipe01_RawPCM();
    _debugCNT01 ++ ;

    if ( _debugCNT01 % _debugCNT01_MAX == ( _debugCNT01_MAX - 1 ) ) {
        _tcpRaw         -> dumpSelfX();
        _fGenRawPcm     -> dumpSelfX();
    }
} /* _debugDump01 */

int _debugCNT02 = 1 ;
#define _debugCNT02_MAX  (_debugCNT_MAX * 3)
void _debugDump02(void) {

    if ( _debugCNT02 % _debugCNT02_MAX == (_debugCNT02_MAX - 1) ) {
        _time2 = _timeNow ;
        _time3 = _time2 - _time1 ;
        _prSFn( " --- %10d %10d , %10d " , _time2 , _time3 , _debugCNT02 ) ;
        _ffstdout ;
    }
    _debugCNT02 ++ ;
} /* _debugDump02 */

void _debugDumpX(void) {
    _debugDump02() ;
    _debugDump01() ;
} /* _debugDumpX */

void _fill_dataX(void) {
    if ( 0 ) 
        _fill_data01();
    if ( 1 ) 
        _fill_data02();

} /* _fill_dataX */

