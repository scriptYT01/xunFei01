#include "superStream.h"


bool _ssCin::_ssOpenOrReopen( )
{
    _ssFD   = 0 ;
    _ssOK   = this ;
    return true ;
} /* _ssCin::_ssOpenOrReopen */

bool _ssCout::_ssOpenOrReopen( )
{
    _ssFD   = 1 ;
    _ssOK   = this ;
    return true ;
} /* _ssCout::_ssOpenOrReopen */

bool _ssCerr::_ssOpenOrReopen( )
{
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

