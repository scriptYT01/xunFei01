#include "superStream.h"


_ssCin::_ssCin( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) 
{
    _superStreamInit( _enSstCin , ___ssDir , ___path , ___comment ) ;
    _ssOpenOrReopen();
} /* _ssCin::_ssCin */

_ssCout::_ssCout( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) 
{
    _superStreamInit( _enSstCout , ___ssDir , ___path , ___comment ) ;
    _ssOpenOrReopen();
} /* _ssCout::_ssCout */

_ssCerr::_ssCerr( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) 
{
    _superStreamInit( _enSstCerr , ___ssDir , ___path , ___comment ) ;
    _ssOpenOrReopen();
} /* _ssCerr::_ssCerr */

_ssTcpConnectTo::_ssTcpConnectTo( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) 
{
    _superStreamInit( _enSstTcpConnectTo , ___ssDir , ___path , ___comment ) ;
    _ssOpenOrReopen();
} /* _ssTcpConnectTo::_ssTcpConnectTo */

_ssListen1::_ssListen1( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) 
{
    _superStreamInit( _enSstTcpListen1 , ___ssDir , ___path , ___comment ) ;
    _ssOpenOrReopen();
} /* _ssListen1::_ssListen1 */

_ssFileOut::_ssFileOut( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) 
{
    _superStreamInit( _enSstFileOut , ___ssDir , ___path , ___comment ) ;
    _ssOpenOrReopen();
} /* _ssFileOut::_ssFileOut */

_ssFileIn::_ssFileIn( _enSsDir ___ssDir , const char * ___path , const char * ___comment ) 
{
    _superStreamInit( _enSstFileOut , ___ssDir , ___path , ___comment ) ;
    _ssOpenOrReopen();
} /* _ssFileIn::_ssFileIn */



