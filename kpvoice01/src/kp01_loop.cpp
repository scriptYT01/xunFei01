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

void _pipe03_speaker(void) {
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
    if( 0 && __len > 0 ) {
        _fSpeaker     -> dumpSelfX();
        _tcpSpeaker   -> dumpSelfX();
        _prExit( " testing " );
    }
} /* _pipe03_speaker */

void _fill_data(void) {
    if ( 1 ) _pipe01_RawPCM();
    if ( 1 ) _pipe02_ReducedPCM();
    if ( 0 ) _pipe03_speaker();

} /* _fill_data */

