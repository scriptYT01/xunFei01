#ifndef     __P3OPEN_H
#define     __P3OPEN_H

#include    "base01.h"

class _p3open 
{
    public :
        _p3open( const char * ___cmd ) ;
        ~_p3open( ){} ;
    public :
        bool                        _p3Running( bool  ___exitWhenChildFinished ) ;
        bool                        _p3CanRead() ;
        bool                        _p3CanWrite() ;
    public :
        int                         _p3WriteNonblock(  int ___len , const char * ___buf ) ;
        int                         _p3WriteBlock(     int ___len , const char * ___buf ) ;
        int                         _p3ReadNonblock(   int ___len ,       char * ___buf ) ;
        int                         _p3ReadBlock(      int ___len ,       char * ___buf ) ;
    private :
} ; /* class _p3open */

#endif
