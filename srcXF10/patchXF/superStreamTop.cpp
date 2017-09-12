#include "superStream.h"


/* _superStream */

bool _superStreamBase::_fd_valid1_invalid0( int *___fd ) {
    bool __rt ;
    if ( ___fd == NULL ) { __rt = false ; }
    else if ( *___fd < 0 ) { __rt = false ; }
    else {
        if (fcntl(*___fd, F_GETFL) == -1 && errno == EBADF) {
            __rt = false ;
            *___fd = -1 ;/* force fd to invalid */
        } else 
            __rt = true ;
    }
    return __rt ;
} /* _fd_valid1_invalid0 */

int _superStreamBase::_valid_fd_or_errFD( int *___fd ) {
    int __rt ;
    if ( _fd_valid1_invalid0( ___fd ) ) __rt = *___fd ;
    else __rt = 2 ;
    return __rt ;
} /* _valid_fd_or_errFD */

bool _superStreamBase::_fd_canWrite( int *___fd ) {
} /* _fd_canWrite */

void _superStreamBase::_ssTryReopneIfNeeded( _enErrAction ___eAction ) 
{
    if ( 0 == _fd_valid1_invalid0( & _ssFD ) ) {
        if ( ___eAction == _enEreopen ) {
            _ssOpenOrReopen();
        }
    }
} /* _superStreamBase::_ssTryReopneIfNeeded */

void _superStreamBase::_ssDumpSelf( ) {
    _prEFn( "_ssDir      '%d'" , _ssDir      ) ;
    _prEFn( "_ssType     '%d'" , _ssType     ) ;
    _prEFn( "_ssPath     '%s'" , _ssPath     ) ;
    _prEFn( "_ssComment  '%s'" , _ssComment  ) ;
    _prEFn( "_ssOK       '%p'" , _ssOK       ) ;
    _prEFn( "_ssFD       '%d'" , _ssFD       ) ;

        //_superStreamInfo        _ssInfoW    ; 
        //_superStreamInfo        _ssInfoR    ; 
} /* _superStreamBase::_ssDumpSelf */

void _superStreamBase::_ssReadNonblock( _enErrAction ___eAction , int ___len , const char * ___buf ) {
} /* _superStreamBase::_ssReadNonblock */

void _superStreamBase::_ssReadBlock( _enErrAction ___eAction , int ___len , const char * ___buf ) {
} /* _superStreamBase::_ssReadBlock */

void _superStreamBase::_ssWriteNonblock( _enErrAction ___eAction , int ___len , const char * ___buf ) {

    _ssTryReopneIfNeeded( ___eAction ) ;

    if ( _fd_canWrite( & _ssFD ) ) {
        _ssWriteBlock( ___eAction , ___len , ___buf ) ;
    } else {
        _ssInfoW . _tryCnt ++ ;
        _ssInfoW . _tryLen += ___len ;
        _ssInfoW . _skipCnt ++ ;
        _ssInfoW . _skipLen += ___len ;
    }
} /* _superStreamBase::_ssWriteNonblock */

void _superStreamBase::_ssWriteBlock( _enErrAction ___eAction , int ___len , const char * ___buf ) {
    _ssInfoW . _tryCnt ++ ;
    _ssInfoW . _tryLen += ___len ;

    _ssTryReopneIfNeeded( ___eAction ) ;

    if ( _fd_valid1_invalid0( & _ssFD ) ) {
        int __Len ;
        __Len = write( _ssFD , ___buf , ___len ) ; 
        if ( __Len <= 0 ) {
            _ssInfoW . _skipCnt ++ ;
            _ssInfoW . _skipLen += ___len ;
        } else if ( __Len == ___len ) {
            _ssInfoW . _succCnt ++ ;
            _ssInfoW . _succLen += ___len ;
        } else {
            _ssInfoW . _succCnt ++ ;
            _ssInfoW . _succLen += __Len ;
            _ssInfoW . _skipCnt ++ ;
            _ssInfoW . _skipLen += ___len - __Len ;
        }
    } else {
        _ssInfoW . _skipCnt ++ ;
        _ssInfoW . _skipLen += ___len ;
    }

} /* _superStreamBase::_ssWriteBlock */

#define genErr " create error para error , exit: %d , %d , %s, %s" , _exitIfErr , ___ssDir , ___path , ___comment 
_superStreamBase * 
_superStreamBase::_genSS( bool _exitIfErr , _enSsDir ___ssDir , const char * ___path , const char * ___comment ) {
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
        __ssTop = new _ssCerr( ___ssDir , ___path , ___comment ) ;
    } else if ( ___path == "stdout" || ___path == "stderr" ) {
        __ssTop = new _ssCerr( ___ssDir , ___path , ___comment ) ;
    } else if ( 0 == _strcmpX1( "tcpto:" , ___path ) ) {
        __ssTop = new _ssTcpConnectTo( ___ssDir , ___path , ___comment ) ;
    } else if ( 0 == _strcmpX1( "tcpl1:" , ___path ) ) {
        __ssTop = new _ssListen1( ___ssDir , ___path , ___comment ) ;
    } else if ( 0 == _strcmpX1( ">:" , ___path ) || 0 == _strcmpX1( ">>:" , ___path ) ) {
        __ssTop = new _ssFileOut( ___ssDir , ___path , ___comment ) ;
    } else {
        if ( 1 ) _prExit( " create error para error , exit: %d , %d , %s, %s" , _exitIfErr , ___ssDir , ___path , ___comment ) ;
    }

    if ( NULL == __ssTop  ) {
        _nExit( _exitIfErr , " create error para error , exit: %d , %d , %s, %s" , _exitIfErr , ___ssDir , ___path , ___comment ) ;
    }

    __ssRt = __ssTop -> _ssOK ;
    if ( _exitIfErr ) {
        _zExit( __ssRt , " create error para error , exit: %d , %d , %s, %s" , _exitIfErr , ___ssDir , ___path , ___comment ) ;
    }

    return __ssRt ;

} /*_superStreamBase::_genSS */

void _superStreamBase::_superStreamInit( _enSsType ___ssType , _enSsDir ___ssDir , const char * ___path , const char * ___comment ) {
    _ssType     =   ___ssType       ;
    _ssDir      =   ___ssDir        ;
    _ssPath     =   strdup( ___path )         ;
    _ssComment  =   strdup( ___comment )      ;
    _ssFD       =   -1              ;
    cerr << " --- _superStreamInit : " << endl ;

    _ssOK       = NULL ;
}; /* _superStreamBase::_superStreamInit */






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



