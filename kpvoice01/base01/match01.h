#ifndef     __MATCH01_H
#define     __MATCH01_H

#include "base01.h"

class _Match01 
{
    public :
        _Match01( ) ;
        _Match01( char ___sep , int __len ) ;
        ~_Match01(){} ;
    public :
        _Match01 *                  _ok() ;
    private :
    protected :
        char *                      _maBuf          = NULL  ;
        int                         _maLen          = -1    ; /* 0 -> use the len in func-para, -1 -> end when zero reached. */
        char                        _maSep          = ' '   ;
} ; /* _Match01 */


#endif

