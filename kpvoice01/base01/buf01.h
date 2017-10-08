#ifndef     __BUF01_H
#define     __BUF01_H

#include "superStream.h"

class _Buf 
{
    public :
        _Buf( _superStreamBase * ___ss ) ;
        ~_Buf(){ _bfSS = NULL ; }
    public :
        static _Buf *       _new( _superStreamBase * ___ss ) ;
        _Buf *              _ok() ;
    private :
    protected :
        _superStreamBase *      _bfSS           = NULL ;
} ; /* _Buf */


#endif
