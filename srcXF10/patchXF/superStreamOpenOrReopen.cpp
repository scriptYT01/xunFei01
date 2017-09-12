#include "superStream.h"


bool _ssCin::_ssOpenOrReopen( )
{
    if ( _ssDir != _enSsdIn )  return false ;
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
    _ssOK   = NULL ;
    return true ;
} /* _ssListen1::_ssOpenOrReopen */

bool _ssFileOut::_ssOpenOrReopen( )
{
    string  __path ;
    int     __fd = -1 ;
    if ( _ssDir != _enSsdOut )  return false ;
    if ( _fd_valid1_invalid0( & _ssFD ) ) {
        return true ;
    }
    if ( 0 == _ssPath . find( ">:" ) ) {
        __path = _ssPath . substr( 2 ) ;
        __fd = open( __path . c_str() , O_WRONLY | O_CREAT ) ;
    }
    if ( 0 == _ssPath . find( ">>:" ) ) {
        __path = _ssPath . substr( 3 ) ;
        __fd = open( __path . c_str() , O_WRONLY | O_CREAT | O_APPEND ) ;
    }
    if ( __fd >= 0 ) {
        _ssFD   = __fd ;
        _ssOK   = this ;
        return true ;
    }
    _ssOK   = NULL ;
    return false ;
} /* _ssFileOut::_ssOpenOrReopen */

