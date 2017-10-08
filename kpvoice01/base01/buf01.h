#ifndef     __BUF01_H
#define     __BUF01_H

#include "superStream.h"

class _Buf 
{
    public :
        _Buf( _superStreamBase * ___ss , uint16_t ___bfSize , uint16_t ___bfDepth ) ;
        ~_Buf(){                _bfSS = NULL ; }
    public :
        static _Buf *           _new( _superStreamBase * ___ss , uint16_t ___bfSize , uint16_t ___bfDepth ) ;
        _Buf *                  _ok() ;
        char *                  _buf            = NULL ;
    private :
    protected :
        _superStreamBase *      _bfSS           = NULL ;
} ; /* _Buf */


#endif
