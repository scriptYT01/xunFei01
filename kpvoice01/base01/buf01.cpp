#include "buf01.h"

_Buf * _Buf::_new( _superStreamBase * ___ss , uint16_t ___bfSize , uint16_t ___bfDepth ) {
    _Buf * __ss ;
    __ss = new _Buf( ___ss , ___bfSize , ___bfDepth ) ;

    if ( __ss -> _ok() ) {
        return __ss ;
    }

    delete __ss ;
    return NULL ;
} /* _Buf::_new */

_Buf::_Buf( _superStreamBase * ___ss , uint16_t ___bfSize , uint16_t ___bfDepth ) {
    if ( _buf ) { 
        free( _buf ) ; 
        _buf    = NULL ;
    }

    if ( ___ss && ___bfSize && ___bfDepth ) {
        _buf    = (char*) malloc( ___bfSize * ___bfDepth ) ;
    }
    if ( _buf ) {
        _bfSS   = ___ss ;
    } else {
        _bfSS   = NULL ;
    }
    //return NULL ;
} /* _Buf::_Buf */

_Buf * _Buf::_ok( ) {
    return (_bfSS == NULL )?NULL:this ;
} /* _Buf::_ok */
