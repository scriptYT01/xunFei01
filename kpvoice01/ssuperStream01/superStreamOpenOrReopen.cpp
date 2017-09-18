#include "superStream.h"


bool _ssCin::_ssOpenOrReopen( )
{
    _dbSS2   ;
    if ( _ssDir != _enSsdIn )  return false ;
    _dbSS2   ;
    _ssFD   = 0 ;
    _ssOK   = this ;
    return true ;
} /* _ssCin::_ssOpenOrReopen */

bool _ssCout::_ssOpenOrReopen( )
{
    if ( _ssDir != _enSsdOut )  return false ;
    _ssFD   = 1 ;
    _ssOK   = this ;
    return true ;
} /* _ssCout::_ssOpenOrReopen */

bool _ssCerr::_ssOpenOrReopen( )
{
    if ( _ssDir != _enSsdOut )  return false ;
    _ssFD   = 2 ;
    _ssOK   = this ;
    return true ;
} /* _ssCerr::_ssOpenOrReopen */

bool _ssTcpConnectTo::_ssOpenOrReopen( )
{
    _ssOK   = NULL ;
    return true ;
} /* _ssTcpConnectTo::_ssOpenOrReopen */

bool _ssListen1::_ssOpenOrReopen( )
{
    if ( _FD_valid1_invalid0_close( & _ssFD ) ) {
        return true ;
    }

    _ssOpenTCPListenServerPortAcceptSock() ;
    if ( 0 == _FD_valid1_invalid0_close( &_ssF2 ) ) { //if ( _ssF2 < 0 ) {
        ssDumpExit(1);
        return false ;
    }

    // try accept the connect.

    ssDumpExit(1);
    _ssOK   = this ;
    return  true ;
    //return (_ssFD < 0)?false:true ;
} /* _ssListen1::_ssOpenOrReopen */

bool _ssFileOut::_ssOpenOrReopen( )
{
    const char *  __path ;
    int     __fd = -1 ;
    if ( _ssDir == _enSsdOut )  {
        if ( _FD_valid1_invalid0_close( & _ssFD ) ) {
            return true ;
        }
        if ( 0 == _strcmpX1( ">:" , _ssPath ) ) {
            __path = _ssPath + 2 ;
            __fd = open( __path     , O_WRONLY | O_CREAT | O_TRUNC , 0666 ) ;
        }
        if ( 0 == _strcmpX1( ">>:"  , _ssPath ) ) {
            __path = _ssPath + 3 ;
            __fd = open( __path     , O_WRONLY | O_CREAT | O_APPEND , 0666 ) ;
        }
        if ( __fd < 0 ) {
            if ( 0 == _strcmpXX( "-"  , _ssPath ) || 0 == _strcmpXX( "stderr"  , _ssPath ) ) {
                __fd = 1 ;// stdout
            } else if ( 0 == _strcmpXX( "stderr"  , _ssPath ) ) {
                __fd = 2 ;// stderr
            } else {
                __path = _ssPath + 0 ; // no header
                __fd = open( __path     , O_WRONLY | O_CREAT | O_TRUNC , 0666 ) ;
            }
        }
        if ( __fd >= 0 ) {
            _ssFD   = __fd ;
            _ssOK   = this ;
            return true ;
        }
    }
    _ssFD   = -1 ;
    _ssOK   = NULL ;
    return false ;
} /* _ssFileOut::_ssOpenOrReopen */

bool _ssFileIn::_ssOpenOrReopen( )
{
    const char *  __path ;
    int     __fd = -1 ;
    if ( _ssDir == _enSsdIn )  {
        if ( _FD_valid1_invalid0_close( & _ssFD ) ) {
            return true ;
        }
        if ( 0 == _strcmpX1( "<:" , _ssPath ) ) {
            __path = _ssPath + 2 ;
            __fd = open( __path     , O_RDONLY ) ;
        }
        if ( 0 == _strcmpX1( "<<:"  , _ssPath ) ) {
            __path = _ssPath + 3 ;
            __fd = open( __path     , O_RDONLY ) ;
        }
        if ( __fd < 0 ) {
            if ( 0 == _strcmpXX( "-"  , _ssPath ) || 0 == _strcmpXX( "stdin"  , _ssPath ) ) {
                __fd = 0 ;// stdin
            } else {
                __path = _ssPath + 0 ; // no header
                __fd = open( __path     , O_RDONLY ) ;
            }
        }
        if ( __fd >= 0 ) {
            _ssFD   = __fd ;
            _ssOK   = this ;
            return true ;
        }
    }
    _ssFD   = -1 ;
    _ssOK   = NULL ;
    return false ;
} /* _ssFileIn::_ssOpenOrReopen */

