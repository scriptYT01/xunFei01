#include "buf01.h"

_Buf * _Buf::_new( _superStreamBase * ___ss ) {
    _Buf * __ss ;
    __ss = new _Buf( ___ss ) ;

    return NULL ;
} /* _Buf::_new */

_Buf::_Buf( _superStreamBase * ___ss ) {
    _bfSS = ___ss ;

    //return NULL ;
} /* _Buf::_Buf */

_Buf * _Buf::_ok( ) {
    return (_bfSS == NULL )?NULL:this ;
} /* _Buf::_ok */
