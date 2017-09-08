#include "superStream.h"


/* _superStream */
static bool _fd_valid1_invalid0( int ___fd ) {
} /* _fd_valid1_invalid0 */

void _superStreamBase::_ssOut( _enEaction ___eAction , const char *___fmt , ... ) { 
    char    __buf1024[1024] ;
    va_list __args ;
    int     __len ;

    if ( ___fmt ) {
        va_start(__args, ___fmt);
        __len = vsnprintf(__buf1024 , 1023 , ___fmt, __args );
        va_end(__args);

        if ( __len ) {
            _ssWriteNonblock( ___eAction , __len , __buf1024 ) ; 
        }
    }
} /* _superStreamBase::_ssOut */

void _superStreamBase::_ssWriteNonblock( _enEaction ___eAction , int ___len , const char * ___buf ) {
} /* _superStreamBase::_ssWriteNonblock */

void _superStreamBase::_ssWriteBlock( _enEaction ___eAction , int ___len , const char * ___buf ) {
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
    if ( ___ssDir != _enSsdIn ) {
        _ssOK = NULL ;
    } else {
        _ssType     =   _enSstCin   ;
        _ssFP       =   0           ;
    }
} /* _ssCin::_ssCin */

_ssCout::_ssCout( _enSsDir ___ssDir , string ___path , string ___comment ) 
    : _superStreamBase( ___ssDir , ___path , ___comment ) 
{
    if ( ___ssDir != _enSsdOut ) {
        _ssOK = NULL ;
    } else {
        _ssType     =   _enSstCout  ;
        _ssFP       =   1           ;
    }
} /* _ssCout::_ssCout */

_ssCerr::_ssCerr( _enSsDir ___ssDir , string ___path , string ___comment ) 
    : _superStreamBase( ___ssDir , ___path , ___comment ) 
{
    if ( ___ssDir != _enSsdOut ) {
        _ssOK = NULL ;
    } else {
        _ssType     =   _enSstCerr  ;
        _ssFP       =   2           ;
    }
} /* _ssCerr::_ssCerr */

