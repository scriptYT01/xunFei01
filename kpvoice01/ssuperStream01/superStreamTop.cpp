#include "superStream.h"


/* _superStream */

void _superStreamBase::_ssTryReopneIfNeeded( ) 
{
    if ( 0 == S_fd_valid1_invalid0_close( & _ssFD ) ) {
        if ( _ssErrAction == _enEreopen ) {
            _ssOpenOrReopen();
        }
    }
} /* _superStreamBase::_ssTryReopneIfNeeded */

