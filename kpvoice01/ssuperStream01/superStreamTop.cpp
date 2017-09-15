#include "superStream.h"


/* _superStream */

bool _superStreamBase::_FD_valid1_invalid0_close( int *___fd ) {
    bool __rt ;
    if ( ___fd == NULL ) { __rt = false ; }
    else if ( *___fd < 0 ) { __rt = false ; }
    else {
        if (fcntl(*___fd, F_GETFL) == -1 && errno == EBADF) {
            __rt = false ;
            close( *___fd ) ;
            *___fd = -1 ;/* force fd to invalid */
        } else 
            __rt = true ;
    }
    return __rt ;
} /* _FD_valid1_invalid0_close */

int _superStreamBase::_valid_fd_or_errFD( int *___fd ) {
    int __rt ;
    if ( _FD_valid1_invalid0_close( ___fd ) ) __rt = *___fd ;
    else __rt = 2 ;
    return __rt ;
} /* _valid_fd_or_errFD */

bool _superStreamBase::_fd_canWrite( int *___fd ) {
    struct pollfd __pfds[1] ;
    int __rt ;

    if ( 0 == _FD_valid1_invalid0_close( ___fd ) ) return false ;
    __pfds[0].fd = *___fd ;
    __pfds[0].events = POLLOUT ;
    __pfds[0].revents = 0 ;

    __rt = poll(__pfds, 1, 0);

    if ( __rt < 0 ) {
        close( *___fd ) ; *___fd = -1 ;
        return false ; /* error 1*/
    }
    
    if ( __rt == 0 ) {
        return false ; /* can NOT write , normal */
    }

    if ( __pfds[0].revents & POLLERR ) {
        _prEFn( "POLLERR state : %d : %s " , *___fd , strerror(errno) ) ;
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if ( __pfds[0].revents & POLLHUP ) {
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if ( __pfds[0].revents & POLLNVAL ) {
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if(__pfds[0].revents & POLLOUT) {
        return true ;
    }

    _prEFn( "unknown state" ) ;
    return false ;
} /* _fd_canWrite */

bool _superStreamBase::_fd_canRead( int *___fd ) {
    _prEFn( "under constructing " ) ;
    return false ;
} /* _fd_canRead */

void _superStreamBase::_ssTryReopneIfNeeded( _enErrAction ___eAction ) 
{
    if ( 0 == _FD_valid1_invalid0_close( & _ssFD ) ) {
        if ( ___eAction == _enEreopen ) {
            _ssOpenOrReopen();
        }
    }
} /* _superStreamBase::_ssTryReopneIfNeeded */

void _superStreamBase::_ssDumpSelf( ) {
    _prEFn( "" );
    _prEFn( "_ssDir      '%d'" , _ssDir      ) ;
    _prEFn( "_ssType     '%d'" , _ssType     ) ;
    _prEFn( "_ssPath     '%s'" , _ssPath     ) ;
    _prEFn( "_ssComment  '%s'" , _ssComment  ) ;
    _prEFn( "_ssOK       '%p'" , _ssOK       ) ;
    _prEFn( "_ssFD       '%d'" , _ssFD       ) ;

    _prEFn( "_ssInfoW " 
            " tryCnt,Len  %4lld %4lld " 
            " skipCnt,Len %4lld %4lld " 
            " succCnt,Len %4lld %4lld " 
            , _ssInfoW . _tryCnt     
            , _ssInfoW . _tryLen     
            , _ssInfoW . _skipCnt    
            , _ssInfoW . _skipLen    
            , _ssInfoW . _succCnt    
            , _ssInfoW . _succLen    
            ) ;

    _prEFn( "_ssInfoR " 
            " tryCnt,Len  %4lld %4lld " 
            " skipCnt,Len %4lld %4lld " 
            " succCnt,Len %4lld %4lld " 
            , _ssInfoR . _tryCnt     
            , _ssInfoR . _tryLen     
            , _ssInfoR . _skipCnt    
            , _ssInfoR . _skipLen    
            , _ssInfoR . _succCnt    
            , _ssInfoR . _succLen    
            ) ;
    _prEFn( "" );

} /* _superStreamBase::_ssDumpSelf */

void _superStreamBase::_ssReadNonblock( _enErrAction ___eAction , int ___len , const char * ___buf ) {
} /* _superStreamBase::_ssReadNonblock */

void _superStreamBase::_ssReadBlock( _enErrAction ___eAction , int ___len , const char * ___buf ) {
} /* _superStreamBase::_ssReadBlock */

void _superStreamBase::_ssWriteNonblock( _enErrAction ___eAction , int ___len , const char * ___buf ) {

    _ssTryReopneIfNeeded( ___eAction ) ;

    if ( _fd_canWrite( & _ssFD ) ) {
        if ( 0 ) _prEFn( " can Write at once " );
        _ssWriteBlock( ___eAction , ___len , ___buf ) ;
    } else {
        if ( 1 ) _prEFn( " can NOT Write at once : %d : %s , %s " , _ssFD , _ssPath , _ssComment );
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

    if ( _FD_valid1_invalid0_close( & _ssFD ) ) {
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

_superStreamBase * 
_superStreamBase::_genSS( bool _exitIfErr , _enSsDir ___ssDir , const char * ___path , const char * ___comment ) {
    _superStreamBase * __ssTop  = NULL ;
    _superStreamBase * __ssRt   = NULL;

    _dbSS1 ;
    if ( 0 == _strcmpXX( "-" , ___path ) ) {
    _dbSS1 ;
        switch ( ___ssDir ) {
            case _enSsdIn :
    _dbSS1 ;
                __ssTop = new _ssCin( ___ssDir , ___path , ___comment ) ;
                break ;
            case _enSsdOut :
    _dbSS1 ;
                __ssTop = new _ssCout( ___ssDir , ___path , ___comment ) ;
                break ;
            default :
    _dbSS1 ;
                break ;
        }
    } else if ( 0 == _strcmpXX( ___path , "stdin" )) {
    _dbSS1 ;
        __ssTop = new _ssCerr( ___ssDir , ___path , ___comment ) ;
    } else if ( 0 == _strcmpXX( ___path , "stdout" ) || 0 == _strcmpXX( ___path , "stderr" )) {
    _dbSS1 ;
        __ssTop = new _ssCerr( ___ssDir , ___path , ___comment ) ;
    } else if ( 0 == _strcmpX1( "tcpto:" , ___path ) ) {
    _dbSS1 ;
        __ssTop = new _ssTcpConnectTo( ___ssDir , ___path , ___comment ) ;
    } else if ( 0 == _strcmpX1( "tcpl1:" , ___path ) ) {
    _dbSS1 ;
        __ssTop = new _ssListen1( ___ssDir , ___path , ___comment ) ;
    } else if ( 0 == _strcmpX1( ">:" , ___path ) || 0 == _strcmpX1( ">>:" , ___path ) ) {
    _dbSS1 ;
        __ssTop = new _ssFileOut( ___ssDir , ___path , ___comment ) ;
    } else {
    _dbSS1 ;
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


