#include "kp01_99_top.h"


void _testL0(void) {
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
} /* _testL0 */

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
    while ( 1 ) {
        __len = _tcpSpeaker -> _ssReadNonblock(  _pcmLenRaw , __buf1024 ) ;
        if ( __len > 0 ) {
            _prEF( " len : %d ," , __len ); 
            write( 2 , __buf1024 , __len ) ;
            write( 2 , "\n" , 1 ) ;
        } else {
            _prEFn( " len : %d " , __len ) ;
        }
        sleep(1) ;
    }
} /* _testTL4 */

void _testTL5(void) {
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
    } else {
        _prExit( " what happens ? " );
    }
    if( 0 ) {
        if ( __len > 0 ) {
            _fSpeaker     -> dumpSelfX();
            _tcpSpeaker   -> dumpSelfX();
            _prExit( " testing : %d " , __len);
        }
    }
} /* _testTL5 */

void _testSS(void) {
    // _superStreamBase 
    // _canWrite
    // _ssOpenOrReopen
    if(0)   
        _testTL1() ; 
    if(0)   
        _testTL2() ; 
    if(0)   
        _testTL3() ; 
    if(0)   
        _testTL4() ; 
    if(0)   
        _testTL5() ; 
} /* _testSS */

char _bufW1[1024] = "1234567890\n" ;
void _testW1(void) {
    int __len ;

    while ( 1 ) {
        if ( _tcpSend1 -> _canWrite( true ) ) { 
            __len = strlen( _bufW1 )  ;
            _prEFn( " can write , try send length : %d " , __len  ) ;
            __len = _tcpSend1 -> 
                _ssWriteNonblock(  __len   , _bufW1 ) ;
            _prEFn( " sent length : %d " , __len  ) ;
        } else {
            _prEFn( " _tcpSend1 is %p . can NOT Write " , _tcpSend1 );
            if ( 0 ) _tcpSend1 -> dumpSelfX() ;
        }
        //_prExit( " debuging . " );
        sleep( 1 ) ;

        if ( ! _tcpExit -> _canRead( true ) ) {
            if ( 1 ) _tcpSend1 -> dumpSelfX() ;
        }
    }
} /* _testW1 */

#define testW2_loopCNT 30
void _testW2(void) {
    int __cnt01 = 0 ;

    while ( 1 ) {
        _prEFn( " can write : %d " , _tcpSend1 -> _canWrite( true ) ) ;
        _prEFn( " can read : %d "  , _tcpSend1 -> _canRead ( true ) ) ;

        sleep ( 1 );
        __cnt01 ++ ;

        if ( __cnt01 % testW2_loopCNT == (testW2_loopCNT - 1 ) ) {
            if ( 1 ) _tcpSend1 -> dumpSelfX() ;
        }
    }
} /* _testW2 */

void _testWW(void) {

    if ( _tcpSend1 == NULL ) {
        _tcpSend1  = new _ssTcpConnectTo( _enSsdOut 
            , "tcpT1:127.0.0.1:44444" , " _tcpExit  : when connected , out debug info , exit. " ) ;
    }

    if(0)   
        _testW1() ; 
    if(1)   
        _testW2() ; 
} /* _testWW */

