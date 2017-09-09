#include "superStream.h"


/* _superStream */
static bool _fd_valid1_invalid0( int ___fd ) {
} /* _fd_valid1_invalid0 */

void _superStreamBase::_ssReadNonblock( _enErrAction ___eAction , int ___len , const char * ___buf ) {
} /* _superStreamBase::_ssReadNonblock */

void _superStreamBase::_ssReadBlock( _enErrAction ___eAction , int ___len , const char * ___buf ) {
} /* _superStreamBase::_ssReadBlock */

void _superStreamBase::_ssWriteNonblock( _enErrAction ___eAction , int ___len , const char * ___buf ) {
} /* _superStreamBase::_ssWriteNonblock */

void _superStreamBase::_ssWriteBlock( _enErrAction ___eAction , int ___len , const char * ___buf ) {
} /* _superStreamBase::_ssWriteBlock */

_superStreamBase * 
_superStreamBase::_genSS( bool _exitIfErr , _enSsDir ___ssDir , string ___path , string ___comment ) {
    _superStreamBase * __ssTop  = NULL ;
    _superStreamBase * __ssRt   = NULL;

    if ( ___path == "-" ) {
        switch ( ___ssDir ) {
            case _enSsdIn :
                __ssTop = new _ssCin( ___ssDir , ___path , ___comment ) ;
                break ;
                __ssTop = new _ssCout( ___ssDir , ___path , ___comment ) ;
                break ;
            default :
                break ;
        }
    }

    if ( NULL == __ssTop  ) {
        _nExit( _exitIfErr , " create error Type error , exit when error met. exit." ) ;
    } else {

        __ssRt = __ssTop -> _ssOK ;
        if ( _exitIfErr ) {
            _zExit( __ssRt , " create error Para error, exit when error met. exit." ) ;
        }
    }

    return __ssRt ;

} /*_superStreamBase::_genSS */

_superStreamBase::_superStreamBase( _enSsDir ___ssDir , string ___path , string ___comment ) {
    _ssDir      =   ___ssDir        ;
    _ssType     =   _enSstUnknown   ;
    _ssPath     =   ___path         ;
    _ssComment  =   ___comment      ;
    _ssFP       =   -1              ;
    cerr << " --- _superStreamBase : " << endl ;

    _ssOK = this ;
}; /* _superStreamBase::_superStreamBase */






_ssCin::_ssCin( _enSsDir ___ssDir , string ___path , string ___comment ) 
    : _superStreamBase( ___ssDir , ___path , ___comment ) 
{
        _ssType     =   _enSstCin   ;
} /* _ssCin::_ssCin */

_ssCout::_ssCout( _enSsDir ___ssDir , string ___path , string ___comment ) 
    : _superStreamBase( ___ssDir , ___path , ___comment ) 
{
        _ssType     =   _enSstCout  ;
} /* _ssCout::_ssCout */

_ssCerr::_ssCerr( _enSsDir ___ssDir , string ___path , string ___comment ) 
    : _superStreamBase( ___ssDir , ___path , ___comment ) 
{
        _ssType     =   _enSstCerr  ;
} /* _ssCerr::_ssCerr */



