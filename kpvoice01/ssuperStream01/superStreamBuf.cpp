#include "superStream.h"

void    _superStreamBase::_ssBufSet(  int ___ssBufAlignSize    ,   int ___ssBufDeep ) {
    _ssBufAlignSize     = ___ssBufAlignSize     ;
    _ssBufDeep          = ___ssBufDeep          ;
} /* _superStreamBase::_ssBufSet */

int     _superStreamBase::_ssBufR(    int ___len          ,   const char * ___buf ) {
    return 0 ;
} /* _superStreamBase::_ssBufR */

int     _superStreamBase::_ssBufW(    int ___len          ,         char * ___buf ) {
    return 0 ;
} /* _superStreamBase::_ssBufW */
