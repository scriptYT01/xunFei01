#include "kp01_99_top.h"

void _fill_data(void) {
    char    __buf1024[_pcmLenRaw] ;
    int     __len ;

    if (0) _prExit( " under constructing... " );

    //if ( _tcpRaw -> _canWrite( true ) ) {
    if ( _fSpeaker -> _canWrite( true ) ) {
        __len = _fGenRawPcm -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        if ( __len != _pcmLenRaw ) {
            __len = _fGenRawPcm -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
        if ( __len != _pcmLenRaw ) {
            __len = _fSpeaker -> _ssWriteNonblock(  _pcmLenRaw , __buf1024 ) ;
            //__len = _tcpRaw -> _ssWriteNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
    }
} /* _fill_data */

