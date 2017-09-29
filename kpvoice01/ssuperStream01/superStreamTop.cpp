#include "superStream.h"

_ssFileIn   *   _SSin   = NULL ;
_ssFileOut  *   _SSout  = NULL ;
_ssFileOut  *   _SSerr  = NULL ;

int          _argc = -1 ;
char **      _argv = NULL ;

void _paraAnalyzeSS( int ___argc , char ** ___argv ) {
    _argc   = ___argc ;
    _argv   = ___argv ;

    _SSin   = new _ssFileIn(    _enSsdIn    , "stdin"   , " standard stream " );
    _SSout  = new _ssFileOut(   _enSsdIn    , "stdout"  , " standard stream " );
    _SSerr  = new _ssFileOut(   _enSsdOut   , "stderr"  , " standard stream " );

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

