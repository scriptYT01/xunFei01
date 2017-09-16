#include "superStream.h"

void _superStreamBase::_ssSetErrAction( _enErrAction ___eAction ) 
{
   _ssErrAction = ___eAction ;
   _ssTryReopneIfNeeded();
} /* _superStreamBase::_ssSetErrAction */

