#include "superStream.h"


/* _superStream */

bool _fd_valid1_invalid0( int ___fd ) {
    bool __rt ;
    if ( ___fd < 0 ) { __rt = false ; }
    else {
        //if (fcntl(___fd, F_GETFL) == -1 && errno == EBADF) 
        if (fcntl(___fd, F_GETFL) == -1 && errno == EBADFD) 
            __rt = false ;
        else 
            __rt = true ;
    }
    return __rt ;
} /* _fd_valid1_invalid0 */

static int _valid_fd_or_errFD( int ___fd ) {
    int __rt ;
    if ( _fd_valid1_invalid0( ___fd ) ) __rt = ___fd ;
    else __rt = 2 ;
    return __rt ;
} /* _fd_valid1_invalid0 */

void _superStreamBase::_ssReadNonblock( _enErrAction ___eAction , int ___len , const char * ___buf ) {
} /* _superStreamBase::_ssReadNonblock */

void _superStreamBase::_ssReadBlock( _enErrAction ___eAction , int ___len , const char * ___buf ) {
} /* _superStreamBase::_ssReadBlock */

void _superStreamBase::_ssWriteNonblock( _enErrAction ___eAction , int ___len , const char * ___buf ) {
    int __fd ;

    __fd = _valid_fd_or_errFD( _ssFD ) ;
    if ( 1 ) { write( __fd , ___buf , ___len ) ; }

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
            case _enSsdOut :
                __ssTop = new _ssCout( ___ssDir , ___path , ___comment ) ;
                break ;
            default :
                break ;
        }
    } else if ( ___path == "stdin" ) {
        if ( ___ssDir == _enSsdIn ) {
            __ssTop = new _ssCerr( ___ssDir , ___path , ___comment ) ;
        }
    } else if ( ___path == "stdout" || ___path == "stderr" ) {
        if ( ___ssDir == _enSsdOut ) {
            __ssTop = new _ssCerr( ___ssDir , ___path , ___comment ) ;
        }
    } else if ( 0 == ___path . find( "tcpto:" ) ) {
        __ssTop = new _ssTcpConnectTo( ___ssDir , ___path , ___comment ) ;
    } else if ( 0 == ___path . find( "tcpl1:" ) ) {
        __ssTop = new _ssListen1( ___ssDir , ___path , ___comment ) ;
    } else {
        if ( 1 ) _prExit( " create error para error , exit when error met. exit." ) ;
    }

    if ( NULL == __ssTop  ) {
        _nExit( _exitIfErr , " create error Type error , exit when error met. exit." ) ;
    }

    __ssRt = __ssTop -> _ssOK ;
    if ( _exitIfErr ) {
        _zExit( __ssRt , " create error Para error, exit when error met. exit." ) ;
    }

    return __ssRt ;

} /*_superStreamBase::_genSS */

void _superStreamBase::_superStreamInit( _enSsType ___ssType , _enSsDir ___ssDir , string ___path , string ___comment ) {
    _ssType     =   ___ssType       ;
    _ssDir      =   ___ssDir        ;
    _ssPath     =   ___path         ;
    _ssComment  =   ___comment      ;
    _ssFD       =   -1              ;
    cerr << " --- _superStreamInit : " << endl ;

    _ssOK = this ;
}; /* _superStreamBase::_superStreamInit */






_ssCin::_ssCin( _enSsDir ___ssDir , string ___path , string ___comment ) 
{
    _superStreamInit( _enSstCin , ___ssDir , ___path , ___comment ) ;
    _ssOpenOrReopen();
} /* _ssCin::_ssCin */

_ssCout::_ssCout( _enSsDir ___ssDir , string ___path , string ___comment ) 
{
    _superStreamInit( _enSstCout , ___ssDir , ___path , ___comment ) ;
    _ssOpenOrReopen();
} /* _ssCout::_ssCout */

_ssCerr::_ssCerr( _enSsDir ___ssDir , string ___path , string ___comment ) 
{
    _superStreamInit( _enSstCerr , ___ssDir , ___path , ___comment ) ;
    _ssOpenOrReopen();
} /* _ssCerr::_ssCerr */

_ssTcpConnectTo::_ssTcpConnectTo( _enSsDir ___ssDir , string ___path , string ___comment ) 
{
    _superStreamInit( _enSstTcpConnectTo , ___ssDir , ___path , ___comment ) ;
    _ssOpenOrReopen();
} /* _ssTcpConnectTo::_ssTcpConnectTo */

_ssListen1::_ssListen1( _enSsDir ___ssDir , string ___path , string ___comment ) 
{
    _superStreamInit( _enSstTcpListen1 , ___ssDir , ___path , ___comment ) ;
    _ssOpenOrReopen();
} /* _ssListen1::_ssListen1 */



