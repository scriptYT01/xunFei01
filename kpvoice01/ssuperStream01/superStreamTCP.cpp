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
        _ttListenFD     = -500002 ;
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

bool _TTcp::_ttAnalyzeT1( const char * ___tcpPath ) {
    
    if ( _ttpath != NULL ) {
        if ( 0 == _strcmpXX( _ttpath , ___tcpPath ) ) {
            return true ;
        }
        _nExit( 1 , " path can not be reset " ) ; // this line can be comment
        free( _ttpath ) ;
        free( _tthost ) ;
    }
    if ( 0 != _strcmpX1( "tcpT1:" , ___tcpPath ) ) {
        _ttClientFD     = -600002 ;
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
} /* _TTcp::_ttAnalyzeT1 */

bool _TTcp::_ttAnalyzeL3( ) {

    memset(&_ttSaddr, '0', sizeof(_ttSaddr));

    _ttSaddr . sin_family = AF_INET;
    //_ttSaddr . sin_addr.s_addr = htonl(INADDR_ANY);
    //inet_pton(AF_INET, _tthost &( _ttSaddr . sin_addr ));
    //inet_pton(AF_INET, "192.0.2.33", &(sa.sin_addr));
    //inet_pton(AF_INET, "192.0.2.33", &( _ttSaddr . sin_addr ));
    //_ttSaddr . sin_port = htons( 56789 );
    inet_pton(AF_INET, _tthost, &( _ttSaddr . sin_addr ));
    _ttSaddr . sin_port = htons( atoi( _ttport) );

    return true ;
} /* _TTcp::_ttAnalyzeL3 */

bool _TTcp::_ttAnalyzeT3( ) {

    memset(&_ttTwantedInfo, '0', sizeof(_ttTwantedInfo));

    _ttTwantedInfo . ai_family  = AF_INET; // AF_UNSPEC:ipv4 & 6
    _ttTwantedInfo.ai_socktype=SOCK_STREAM;
    _ttTwantedInfo.ai_protocol=0;
    _ttTwantedInfo.ai_flags=AI_ADDRCONFIG; // refuse ipv6 if local-host has ipv4 only

    if ( getaddrinfo( _tthost , _ttport,  &_ttTwantedInfo, &_ttTdnsResultInfo) ) {
        dumpExit(1) ;
    }

    if ( NULL == _ttTdnsResultInfo ) {
        dumpExit(1) ;
    }

    return true ;
} /* _TTcp::_ttAnalyzeT3 */

bool _TTcp::_ttTryListen01( const char * ___ttPath ) {

    int __yes = 1 ;

    _zExit( _ttAnalyzeL1( ___ttPath ) , " path error ? %s" , ___ttPath ) ;

    _ttListenFD   = -300002 ;

    _ttListenFD = socket(AF_INET, SOCK_STREAM, 0);
    if ( _ttListenFD < 0 ) {
        _prErrno() ;
        dumpExit(1) ;
        return false ;
    }

    _nExit( setsockopt(_ttListenFD , SOL_SOCKET, SO_REUSEADDR, &__yes, sizeof(int)) , " set reuse error " );

    _zExit( _ttAnalyzeL3() , " addr or port error " ) ;


    _ttBd = bind(_ttListenFD, (struct sockaddr*)&_ttSaddr, sizeof(_ttSaddr));
    if ( _ttBd != 0 ) {
        close(_ttListenFD) ;
        _prErrno() ;
        dumpExit(1) ;
        return false ;
    }

    _ttLd = S_setNonblocking( _ttListenFD ) ;
    _nExit( _ttLd , " nonblock rt value , should be zero , but now %d " , _ttLd ) ; 

    _ttLd = listen(_ttListenFD, 10);
    if ( _ttLd != 0 ) {
        close(_ttBd) ;
        close(_ttListenFD) ;
        _prErrno() ;
        dumpExit(1) ;
        return false ;
    }


    //sleep(100);
    dumpExit(0) ;
    return true ;
} /* _TTcp::_ttTryListen01 */

bool _TTcp::_ttTryConnect01( const char * ___ttPath ) {

    //int __yes = 1 ;

    _zExit( _ttAnalyzeT1( ___ttPath ) , " path error ? %s" , ___ttPath ) ;

    _ttClientFD   = -400002 ;

    _zExit( _ttAnalyzeT3() , " analyze connect to addr error " ) ;

    //_ttClientFD = socket(AF_INET, SOCK_STREAM, 0);
    _ttClientFD = socket( _ttTdnsResultInfo->ai_family, _ttTdnsResultInfo->ai_socktype, _ttTdnsResultInfo->ai_protocol );
    if ( _ttClientFD < 0 ) {
        _prErrno() ;
        dumpExit(1) ;
        return false ;
    }

    dumpExit(1) ;



    _ttBd = bind(_ttClientFD, (struct sockaddr*)&_ttSaddr, sizeof(_ttSaddr));
    if ( _ttBd != 0 ) {
        close(_ttClientFD) ;
        _prErrno() ;
        dumpExit(1) ;
        return false ;
    }

    _ttLd = S_setNonblocking( _ttClientFD ) ;
    _nExit( _ttLd , " nonblock rt value , should be zero , but now %d " , _ttLd ) ; 

    _ttLd = listen(_ttClientFD, 10);
    if ( _ttLd != 0 ) {
        close(_ttBd) ;
        close(_ttClientFD) ;
        _prErrno() ;
        dumpExit(1) ;
        return false ;
    }


    //sleep(100);
    dumpExit(0) ;
    return true ;
} /* _TTcp::_ttTryConnect01 */

// clildFD , listenFD  : clildFD -> dataFD , listenFD -> the accepted listen port.
// A  1    1            -> ok                    : all ok
// B  0    1            -> ok                    : child failed , but listen ok
// C  1    0            -> unkown what happen    : warn and exit.
// D  0    0            -> fail                  : let try to open the listen port.
bool _ssListen1::_ssOpenTCPListenServerPortAcceptSock( )
{
    int *__dataFD   = _getDataFD() ;
    int *__listenFD = _getTcpListenFD();

    if ( S_fd_valid1_invalid0_close( __dataFD ) ) {
        if ( S_fd_valid1_invalid0_close( __listenFD ) ) { // A:1,1 
            return true ;
        }
        _prExit( " C:1,0 --> unknow what happen. please check and run again. ");
    }


    if ( S_fd_valid1_invalid0_close( __listenFD ) ) {
        return false ; // B:0,1 
    }

    // D:0,0
    return _tTcp . _ttTryListen01( _ssPath ) ; 

} /* void _ssListen1::_ssOpenTCPListenServerPortAcceptSock */
bool _ssTcpConnectTo::_ssOpenTCPconnectToSock( )
{
    int *__dataFD   = _getDataFD() ;

    if ( S_fd_valid1_invalid0_close( __dataFD ) ) {
        return true ;
    }

    return _tTcp . _ttTryConnect01( _ssPath ) ; 

} /* void _ssTcpConnectTo::_ssOpenTCPconnectToSock */

bool _TTcp::_ttTryAcceptClient( ) {

    if ( 0 == S_fd_valid1_invalid0_close( &_ttListenFD ) ) { 
        return false ;
    }
    if ( 1 == S_fd_valid1_invalid0_close( &_ttClientFD ) ) { //if ( _ttListenFD < 0 ) {
        return true ;
    }


    if(0)   dumpSelfX();
    if(0)   _prEFn( " before : %d " , _timeNow );

    _ttAddrlen = sizeof( _ttRemoteaddr ) ;
    _ttClientFD = accept( _ttListenFD , ( struct sockaddr *) &_ttRemoteaddr , & _ttAddrlen ) ;

    if(0)   _prEFn( " after  : %d , _ttListenFD %d , _ttClientFD %d " , _timeNow , _ttListenFD , _ttClientFD );

    if ( _ttClientFD < 0 ) { 
        if ( errno != EAGAIN ) { // EWOULDBLOCK == EAGAIN == 11
            _prErrno() ; 
        }
        return false ;
    }

    _ttCntR = 0 ;
    _ttCntW = 0 ;
    return true ;
} /* _TTcp::_ttTryAcceptClient */
