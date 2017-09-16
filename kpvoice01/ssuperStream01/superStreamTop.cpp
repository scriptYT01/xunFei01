#include "superStream.h"


/* _superStream */

void _superStreamBase::_ssTryReopneIfNeeded( ) 
{
    if ( 0 == _FD_valid1_invalid0_close( & _ssFD ) ) {
        if ( _ssErrAction == _enEreopen ) {
            _ssOpenOrReopen();
        }
    }
} /* _superStreamBase::_ssTryReopneIfNeeded */

