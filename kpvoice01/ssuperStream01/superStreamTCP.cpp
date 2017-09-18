#include "superStream.h"

bool _TTcp::_ttAnalyzeL1( const char * ___tcpPath , struct sockaddr_in * ___sAddr ) {
    char * __path ;
    char * __host ;
    char * __port ;
    int    __len  ;
    
    if ( 0 != _strcmpX1( "tcpL1:" , ___tcpPath ) ) {
        _ttF2     = -500002 ;
        _ttF3     = -500003 ;
        return false ;
    }

    __path = strdup( ___tcpPath + 6 ) ;
    __path += 6 ;
    __port = strchr( __path , ':' ) ;
    __len = __port - __path ;
    if ( NULL == __port || __len < 2 ) {
        return false ;
    }
    __port ++ ;

    __host = strdup( __path ) ;
    __host[ __len ] = 0 ;
    _nExit( 1 , " <%s> <%d> <%s> <%s> " , __path , __len , __host , __port ) ;




    ___sAddr -> sin_family = AF_INET;
    ___sAddr -> sin_addr.s_addr = htonl(INADDR_ANY);
    ___sAddr -> sin_port = htons(5000);
    return true ;
} /* _TTcp::_ttAnalyzeL1 */

bool _TTcp::_ttTryListen01( const char * ___ttPath ) {


    _ssFD         = -300001 ;
    _tTcp._ttF2   = -300002 ;
    _tTcp._ttF3   = -300003 ;

    __fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( __fd < 0 ) {
        _prErrno() ;
        ssDumpExit(1) ;
        return false ;
    }
    memset(&__serv_addr, '0', sizeof(__serv_addr));

    _nExit( _ttAnalyzeL1( _ssPath , & __serv_addr ) , " path error ? %s" , _ssPath ) ;


    __bd = bind(__fd, (struct sockaddr*)&__serv_addr, sizeof(__serv_addr));
    if ( __bd != 0 ) {
        close(__fd) ;
        _prErrno() ;
        ssDumpExit(1) ;
        return false ;
    }

    __ld = _setNonblocking( __fd ) ;
    if (__ld) { _prExit( " nonblock rt value , should be zero , but now %d " , __ld ) ; }

//    __ld = listen(__fd, 10);
//    if ( __ld < 0 ) {
//        close(__bd) ;
//        close(__fd) ;
//        _prErrno() ;
//        ssDumpExit(1) ;
//        return false ;
//    }

    _tTcp._ttF2 = __fd ;
    //_ttF3 = __bd ;
    ssDumpExit(0) ;
    return true ;
} /* _TTcp::_ttTryListen01 */

// SSFD , SSF2  : FD -> child , F2 -> the accepted listen port.
// A  1    1    -> ok                    : all ok
// B  0    1    -> ok                    : child failed , but listen ok
// C  1    0    -> unkown what happen    : warn and exit.
// D  0    0    -> fail                  : let try to open the listen port.
bool _ssListen1::_ssOpenTCPListenServerPortAcceptSock( )
{

    if ( _FD_valid1_invalid0_close( & _ssFD ) ) {
        if ( _FD_valid1_invalid0_close( & _tTcp._ttF2 ) ) { // A:1,1 
            return false ;
        }
        _prExit( " C:1,0 --> unknow what happen. please check and run again. ");
    }


    if ( _FD_valid1_invalid0_close( & _tTcp._ttF2 ) ) {
        return false ; // B:0,1 
    }

    // D:0,0
    return _tTcp . _ttTryListen01( _ssPath ) ; 

} /* void _ssListen1::_ssOpenTCPListenServerPortAcceptSock */

