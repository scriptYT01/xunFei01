#include "superStream.h"

bool _superStreamBase::_FD_valid1_invalid0_close( int *___fd ) {
    bool    __rt ;
    int     __st ;

    if ( ___fd == NULL ) { __rt = false ; }
    else if ( *___fd < 0 ) { __rt = false ; }
    else {
        __st =  fcntl(*___fd, F_GETFL) ;
        if ( __st < 0 ) {
            if( errno == EBADF ) {
                __rt = false ;
                close( *___fd ) ;
                *___fd = -1 ;/* force fd to invalid */
            } else {
                _prExit( "unKnown" );
            }
        } else 
            if ( __st != 0 ) {
                _prEFn( " state : %d" , __st ) ;
            }
            __rt = true ;
    }
    return __rt ;
} /* _FD_valid1_invalid0_close */

