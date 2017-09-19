#include "superStream.h"


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

