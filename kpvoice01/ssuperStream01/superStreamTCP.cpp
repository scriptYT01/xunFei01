#include "superStream.h"

bool _superStreamBase::_tcpAnalyzeL1( const char * ___tcpPath , struct sockaddr_in * ___sAddr ) {
    char * __path ;
    char * __host ;
    char * __port ;
    int    __len  ;
    

    __path = (char *)___tcpPath ;
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
} /* _ssListen1::_tcpAnalyzeL1 */

bool _tTcp::_tryListen01( const char * ___ttPath ) {
    // D:0,0
    if ( 0 != _strcmpX1( "tcpL1:" , _ssPath ) ) {
        _ssFD           = -200001 ;
        _tTcp._ssF2     = -200002 ;
        _tTcp._ssF3     = -200003 ;
        _ssOK           = NULL ;
        return false ;
    }

    _ssFD         = -300001 ;
    _tTcp._ssF2   = -300002 ;
    _tTcp._ssF3   = -300003 ;

    __fd = socket(AF_INET, SOCK_STREAM, 0);
    if ( __fd < 0 ) {
        _prErrno() ;
        ssDumpExit(1) ;
        return false ;
    }
    memset(&__serv_addr, '0', sizeof(__serv_addr));

    _nExit( _tcpAnalyzeL1( _ssPath , & __serv_addr ) , " path error ? %s" , _ssPath ) ;


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

    _tTcp._ssF2 = __fd ;
    //_ssF3 = __bd ;
    ssDumpExit(0) ;
    return true ;
} /* _tTcp::_tryListen01 */

// SSFD , SSF2  : FD -> child , F2 -> the accepted listen port.
// A  1    1    -> ok                    : all ok
// B  0    1    -> ok                    : child failed , but listen ok
// C  1    0    -> unkown what happen    : warn and exit.
// D  0    0    -> fail                  : let try to open the listen port.
bool _ssListen1::_ssOpenTCPListenServerPortAcceptSock( )
{
    int __fd ;
    int __bd ;
    int __ld ;
    struct sockaddr_in __serv_addr;


    if ( _FD_valid1_invalid0_close( & _ssFD ) ) {
        if ( _FD_valid1_invalid0_close( & _tTcp._ssF2 ) ) { // A:1,1 
            return false ;
        }
        _prExit( " C:1,0 --> unknow what happen. please check and run again. ");
    }


    if ( _FD_valid1_invalid0_close( & _tTcp._ssF2 ) ) {
        return false ; // B:0,1 
    }

    // D:0,0
    return _tTcp . _tryListen01( _ssPath ) ; 

} /* void _ssListen1::_ssOpenTCPListenServerPortAcceptSock */

