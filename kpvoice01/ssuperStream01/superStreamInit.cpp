#include "superStream.h"

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
    _ssType         =   ___ssType               ;
    _ssDir          =   ___ssDir                ;
    _ssPath         =   strdup( ___path )       ;
    _ssComment      =   strdup( ___comment )    ;
    _ssFD           =   -1                      ;
    _ssErrAction    =   _enEreopen              ;

    _prEFn( "ssType %d , ssDir %d , path %s , comment %s" ,
           ___ssType , ___ssDir , ___path , ___comment )    ;

    _ssOK       = NULL                          ;
}; /* _superStreamBase::_superStreamInit */





