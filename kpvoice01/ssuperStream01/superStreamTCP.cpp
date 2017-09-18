#include "superStream.h"

// SSFD , SSF2  : FD -> child , F2 -> the accepted listen port.
// A  1    1    -> ok                    : all ok
// B  0    1    -> ok                    : child failed , but listen ok
// C  1    0    -> unkown what happen    : warn and exit.
// D  0    0    -> fail                  : let try to open the listen port.
bool _ssListen1::_ssOpenTCPListenServerPortAcceptSock( )
{
    int listenfd = 0; 
    //int connfd = 0;
    struct sockaddr_in serv_addr;


    if ( _FD_valid1_invalid0_close( & _ssFD ) ) {
        if ( _FD_valid1_invalid0_close( & _ssF2 ) ) { // A:1,1 
            return false ;
        }
        _prExit( " C:1,0 --> unknow what happen. please check and run again. ");
    }
    if ( _FD_valid1_invalid0_close( & _ssF2 ) ) {
        return false ; // B:0,1 
    }

    // D:0,0
    if ( 0 != _strcmpX1( "tcpL1:" , _ssPath ) ) {
        _ssFD   = -200001 ;
        _ssF2   = -200002 ;
        _ssOK   = NULL ;
        return false ;
    }
    _ssFD   = -300001 ;
    _ssF2   = -300002 ;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));
    //memset(sendBuff, '0', sizeof(sendBuff));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000);

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    listen(listenfd, 10);

    _ssF2 = listenfd ;

    ssDumpExit(1) ;
} /* void _ssListen1::_ssOpenTCPListenServerPortAcceptSock */

