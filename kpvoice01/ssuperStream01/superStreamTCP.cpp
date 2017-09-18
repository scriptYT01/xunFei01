#include "superStream.h"

// SSFD , SSF2  : FD -> child , F2 -> the accepted listen port.
// A  1    1    -> ok                    : all ok
// B  0    1    -> ok                    : child failed , but listen ok
// C  1    0    -> unkown what happen    : warn and exit.
// D  0    0    -> fail                  : let try to open the listen port.
bool _ssListen1::_ssOpenTCPListenServerPortAcceptSock( )
{

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



    ssDumpExit(1) ;
} /* void _ssListen1::_ssOpenTCPListenServerPortAcceptSock */

