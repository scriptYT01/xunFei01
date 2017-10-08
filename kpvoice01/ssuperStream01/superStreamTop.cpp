#include "superStream.h"

_ssFileIn   *   _SSin   = NULL ;
_ssFileOut  *   _SSout  = NULL ;
_ssFileOut  *   _SSerr  = NULL ;
_ssListen1  *   _SSexit = NULL ;

int          __attribute__((weak))  _argc = -1 ;
char **      __attribute__((weak))  _argv = NULL ;

void _paraAnalyzeSS( int ___argc , char ** ___argv , uint16_t ___exitPort ) {
    char __buf40[41] ;
    int  __len ;

    _argc   = ___argc ;
    _argv   = ___argv ;


    __len = snprintf( __buf40 , 40 , "tcpL1:127.0.0.1:%d" , ___exitPort ) ;

    _SSin   = new _ssFileIn(    _enSsdIn    , "stdin"   , " standard stream " );
    _SSout  = new _ssFileOut(   _enSsdIn    , "stdout"  , " standard stream " );
    _SSerr  = new _ssFileOut(   _enSsdOut   , "stderr"  , " standard stream " );
    _SSexit = new _ssListen1(   _enSsdOut   , __buf40   , " exit port " );

    if( _SSerr -> _canWrite(1) ) {
        _SSerr -> _ssWriteNonblockStr(  "\nListening ExitPort : " ) ;
        _SSerr -> _ssWriteNonblock(  __len , __buf40 ) ;
        _SSerr -> _ssWriteNonblock(  1 , "\n" ) ;
    }

} /* _paraAnalyzeSS */

/* _superStream */
void _superStreamBase::_ssTryReopneIfNeeded( ) 
{
    int *__dataFD = _getDataFD() ;

    if ( 0 == S_fd_valid1_invalid0_close( __dataFD ) ) {
        if ( _ssErrAction == _enEreopen ) {
            _ssOpenOrReopen();
        }
    }
} /* _superStreamBase::_ssTryReopneIfNeeded */

