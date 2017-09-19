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

    while(0) {
        if(0) _prOOn( "\n-0-" ) ;
        if ( _tcpRaw -> _canWrite( true ) ) { 
            if(1) _prOO( "-1ok-" ) ;
        } else {
            if(2) _prOO( "-1fa-" ) ;
        };
        sleep(2);
    }

    while(0) {
        if(0) _prOOn( "\n-0-" ) ;
        if ( _tcpRaw -> _canRead( true ) ) { 
            if(1) _prOO( "-1ok-" ) ;
        } else {
            if(2) _prOO( "-1fa-" ) ;
        };
        sleep(2);
    }

    if ( _tcpRaw -> 
            _canWrite( true ) ) {
        if(1) _prEFn( " can Write " ) ;
        __len = _fGenRawPcm -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        if ( __len != _pcmLenRaw ) {
            if(1) _prEFn( " re- read data len : %d " , __len ) ;
            __len = _fGenRawPcm -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        } else {
            if(1) _prEFn( " first-shot read data len : %d " , __len ) ;
        }
        if ( __len != _pcmLenRaw ) {
            if(1) _prEFn( " dataSource error " ) ;
        } else {
            __len = _fSpeaker -> _ssWriteNonblock(  _pcmLenRaw , __buf1024 ) ;
            if(1) _prEFn( " output data len : %d " , __len ) ;
        }
    } else {
        if(1) _prEFn( " can NOT Write " ) ;
        if(0) _tcpRaw -> dumpSelfX() ;
    }

    if(0) _tcpRaw -> dumpSelfX();
    if(0) _prExit( " testing " );
} /* _testTL1 */

void _testTL2(void) {
    char    __buf1024[_pcmLenRaw] ;
    int     __len ;

    if ( _fSpeaker -> _canWrite( true ) ) {
        __len = _fGenRawPcm -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        if ( __len != _pcmLenRaw ) {
            __len = _fGenRawPcm -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
        if ( __len == _pcmLenRaw ) {
            __len = _fSpeaker -> _ssWriteNonblock(  _pcmLenRaw , __buf1024 ) ;
        }
    }
    if(0) {
        _fSpeaker     -> dumpSelfX();
        _fGenRawPcm   -> dumpSelfX();
        _prExit( " testing " );
    }
} /* _testTL2 */

void _testTL3(void) {
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
} /* _testTL3 */

void _testTL4(void) {
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
    if( 1 ) {
        if ( 1 || __len > 0 ) {
            _fSpeaker     -> dumpSelfX();
            _tcpSpeaker   -> dumpSelfX();
            _prExit( " testing " );
        }
    }
} /* _testTL4 */

