#include "kp01_99_top.h"


void _testSS(void) {
    char    __buf1024[_pcmLenRaw] ;
    int     __len ;

    if ( ! ssOK( _fGenRawPcm ) ) {
        _prExit( " file don't exist. " );
    }

    __len = _fGenRawPcm -> 
        _ssReadNonblock(  9 , __buf1024 ) ;
//_ssReadBlock
    if ( __len > 0 ) {
        _prEF( "received <%d> [" , __len ); 
        write( 2 , __buf1024 , __len ) ;
        write( 2 , "]\n" , 2 ) ;
    } else {
        _prEFn( "nothing received <%d> " , __len ); 
    }

    if(0) _prExit( " testing " );
} /* _testSS */

void _testTL1(void) {
    char    __buf1024[_pcmLenRaw] ;
    int     __len ;

    if ( _tcpRaw -> 
            _canWrite( true ) ) {
        if(1) _prEFn( " can Write " ) ;
        __len = _fGenRawPcm -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        if ( __len != _pcmLenRaw ) {
            __len = _fGenRawPcm -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
        if ( __len != _pcmLenRaw ) {
            if(1) _prEFn( " dataSource error " ) ;
        } else {
            __len = _tcpRaw -> _ssWriteNonblock(  _pcmLenRaw , __buf1024 ) ;
            if(1) _prEFn( " output data len : %d " , __len ) ;
        }
    } else {
        if(1) _prEFn( " can NOT Write " ) ;
    }

    if(1) _tcpRaw -> _ssDumpSelf();
    if(1) _prExit( " testing " );
} /* _testTL1 */
