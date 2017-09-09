#include "superStream.h"


bool _ssCin::_ssOpenOrReopen( )
{
    _ssFP   = 0 ;
    _ssOK   = this ;
    return true ;
} /* _ssCin::_ssOpenOrReopen */

bool _ssCout::_ssOpenOrReopen( )
{
    _ssFP   = 1 ;
    _ssOK   = this ;
    return true ;
} /* _ssCout::_ssOpenOrReopen */

bool _ssCerr::_ssOpenOrReopen( )
{
    _ssFP   = 2 ;
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

