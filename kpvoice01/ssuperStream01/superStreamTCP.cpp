#include "superStream.h"

//bool _TTcp::_ttAnalyzeL1( const char * ___tcpPath , struct sockaddr_in * ___sAddr ) {
bool _TTcp::_ttAnalyzeL1( const char * ___tcpPath ) {
    
    if ( _ttpath != NULL ) {
        if ( 0 == _strcmpXX( _ttpath , ___tcpPath ) ) {
            return true ;
        }
        _nExit( 1 , " path can not be reset " ) ; // this line can be comment
        free( _ttpath ) ;
        free( _tthost ) ;
    }
    if ( 0 != _strcmpX1( "tcpL1:" , ___tcpPath ) ) {
        _ttFd     = -500002 ;
        _nExit( 0 , " errorPath " ) ;
        return false ;
    }

    _ttpath = strdup( ___tcpPath + 6 ) ;
    _ttport = strchr( _ttpath , ':' ) ;
    _ttplen = _ttport - _ttpath ;
    if ( NULL == _ttport || _ttplen < 2 ) {
        free( _ttpath ) ;
        _ttpath = NULL ;
        return false ;
    }
    _ttport ++ ;

    _tthost = strdup( _ttpath ) ;
    _tthost[ _ttplen ] = 0 ;
    _nExit( 0 , " <%s> <%d> <%s> <%s> " , _ttpath , _ttplen , _tthost , _ttport ) ;

    return true ;
} /* _TTcp::_ttAnalyzeL1 */

bool _TTcp::_ttAnalyzeL2( ) {
    return true ;
} /* _TTcp::_ttAnalyzeL2 */

bool _TTcp::_ttAnalyzeL3( ) {

    memset(&_ttSaddr, '0', sizeof(_ttSaddr));

    _ttSaddr . sin_family = AF_INET;
    _ttSaddr . sin_addr.s_addr = htonl(INADDR_ANY);
    //inet_pton(AF_INET, _tthost &( _ttSaddr . sin_addr ));
    //inet_pton(AF_INET, "192.0.2.33", &(sa.sin_addr));
    //inet_pton(AF_INET, "192.0.2.33", &( _ttSaddr . sin_addr ));
    //inet_pton(AF_INET, _tthost, &( _ttSaddr . sin_addr ));
    //_ttSaddr . sin_port = htons( atoi( _ttport) );
    _ttSaddr . sin_port = htons( 56789 );

    return true ;
} /* _TTcp::_ttAnalyzeL3 */

bool _TTcp::_ttTryListen01( const char * ___ttPath ) {

    int __yes = 1 ;

    _zExit( _ttAnalyzeL1( ___ttPath ) , " path error ? %s" , ___ttPath ) ;
    _zExit( _ttAnalyzeL2( ) , " convert error " ) ;

    _ttFd   = -300002 ;

    _ttFd = socket(AF_INET, SOCK_STREAM, 0);
    if ( _ttFd < 0 ) {
        _prErrno() ;
        dumpExit(1) ;
        return false ;
    }

    _nExit( setsockopt(_ttFd , SOL_SOCKET, SO_REUSEADDR, &__yes, sizeof(int)) , " set reuse error " );

    _zExit( _ttAnalyzeL3() , " addr or port error " ) ;


    _ttBd = bind(_ttFd, (struct sockaddr*)&_ttSaddr, sizeof(_ttSaddr));
    if ( _ttBd != 0 ) {
        close(_ttFd) ;
        _prErrno() ;
        dumpExit(1) ;
        return false ;
    }

    _ttLd = _setNonblocking( _ttFd ) ;
    _nExit( _ttLd , " nonblock rt value , should be zero , but now %d " , _ttLd ) ; 

    _ttLd = listen(_ttFd, 10);
    if ( _ttLd != 0 ) {
        close(_ttBd) ;
        close(_ttFd) ;
        _prErrno() ;
        dumpExit(1) ;
        return false ;
    }

    _dumpSelf();
    sleep(100);
    dumpExit(1) ;
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
        if ( _FD_valid1_invalid0_close( & _tTcp._ttFd ) ) { // A:1,1 
            return false ;
        }
        _prExit( " C:1,0 --> unknow what happen. please check and run again. ");
    }


    if ( _FD_valid1_invalid0_close( & _tTcp._ttFd ) ) {
        return false ; // B:0,1 
    }

    // D:0,0
    return _tTcp . _ttTryListen01( _ssPath ) ; 

} /* void _ssListen1::_ssOpenTCPListenServerPortAcceptSock */

