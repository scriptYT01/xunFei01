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

    if ( _fSpeaker -> _canWrite( true ) ) {
        __len = _tcpSpeaker -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        if ( __len != _pcmLenRaw ) {
            __len = _tcpSpeaker -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
        if ( __len == _pcmLenRaw ) {
            __len = _fSpeaker -> _ssWriteNonblock(  _pcmLenRaw , __buf1024 ) ;
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

void _fill_dataX(void) {
    if ( 0 ) 
        _fill_data01();
    if ( 1 ) 
        _fill_data02();

} /* _fill_dataX */

