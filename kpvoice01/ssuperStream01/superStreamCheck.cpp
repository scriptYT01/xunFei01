#include "superStream.h"

bool S_fd_valid1_invalid0_close( int *___fd ) {
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
                if(0) _prErrno( " fd : %d , state : %d" , *___fd , __st ) ;
            }
            __rt = true ;
    }
    return __rt ;
} /* S_fd_valid1_invalid0_close */

int S_valid_fd_or_errFD( int *___fd ) {
    int __rt ;
    if ( S_fd_valid1_invalid0_close( ___fd ) ) __rt = *___fd ;
    else __rt = 2 ;
    return __rt ;
} /* S_valid_fd_or_errFD */

#define debugWrite01( aa ) _prEFn( " " aa ": rt %d , fd : %d , : retryCNT %d " \
        ": revents 0x%x , POLLNVAL 0x%x , POLLHUP 0x%x , POLLERR 0x%x , POLLOUT 0x%x " \
        , __rt , *___fd , *___retryCNT \
        , __pfds[0].revents , POLLNVAL , POLLHUP, POLLERR, POLLOUT ) 

#define debugRead01( aa ) _prEFn( " " aa ": rt %d , fd : %d , : retryCNT %d " \
        ": revents 0x%x , POLLNVAL 0x%x , POLLHUP 0x%x , POLLERR 0x%x , POLLIN 0x%x " \
        , __rt , *___fd , *___retryCNT \
        , __pfds[0].revents , POLLNVAL , POLLHUP, POLLERR, POLLIN ) 

bool S_fd_canWrite( int *___fd , int * ___retryCNT ) {
    struct pollfd __pfds[1] ;
    int __rt ;

    if ( 0 == S_fd_valid1_invalid0_close( ___fd ) ) return false ;
    __pfds[0].fd = *___fd ;
    __pfds[0].events = POLLOUT ;
    __pfds[0].revents = 0 ;

    __rt = poll(__pfds, 1, 0);

    if ( __rt < 0 ) {
        debugWrite01( " rt lessThan 0 " ) ;
        close( *___fd ) ; *___fd = -1 ;
        return false ; /* error 1*/
    }

    if ( __rt == 0 ) {
        return false ; /* can NOT Write , normal */
    }

    // 111 Connection refused
    //  11 Resource temporarily unavailable // EWOULDBLOCK == EAGAIN == 11 
    // the following : > 0 . means : poll succeed.
    if ( 0 ) 
        debugWrite01( " rt lessThan 0 " ) ;
    

    if ( __pfds[0].revents & POLLERR ) { 
        //if ( __pfds[0].revents == POLLERR ) { 
        //if ( __pfds[0].revents == (POLLERR | POLLOUT) ) { 
        if ( __pfds[0].revents == (POLLERR | POLLOUT) ) { 
        } else if ( __pfds[0].revents == (POLLHUP | POLLERR | POLLOUT) ) { 
        } else {
            if ( 1 ) 
                debugWrite01( " POLLERR " ) ;
        }
        close( *___fd ) ; *___fd = -1 ;(*___retryCNT) = 0 ;
        return false ;
    }

    // 111 Connection refused 
    if ( __pfds[0].revents & POLLHUP ) {
        //if ( __pfds[0].revents == POLLHUP ) {
        if ( __pfds[0].revents == ( POLLNVAL | POLLHUP | POLLOUT ) ) {
        } else if ( __pfds[0].revents == ( POLLHUP | POLLOUT ) ) {
        } else { // any other error : print , then , close.
            if ( 1 ) 
                debugWrite01( " POLLHUP " ) ;
        }
        close( *___fd ) ; *___fd = -1 ;(*___retryCNT) = 0 ;
        return false ;
    }

    if ( __pfds[0].revents & POLLNVAL ) {
        if ( __pfds[0].revents ==  POLLNVAL ) {
        } else { // any other error : print , then , close.
            if ( 1 ) 
                debugWrite01( " POLLNVAL " ) ;
        }

        close( *___fd ) ; *___fd = -1 ;(*___retryCNT) = 0 ;
        return false ;
    }

    if(__pfds[0].revents & POLLOUT) {
        if ( __pfds[0].revents ==  POLLOUT ) {
        } else { // any other error : print , then , close.
            if ( 1 ) 
                debugWrite01( " POLLOUT " ) ;
        }
        return true ;
    }

    debugRead01( " unknown write state " ) ;
    return false ;
} /* S_fd_canWrite */

bool S_fd_canRead( int *___fd , int * ___retryCNT ) {
    struct pollfd __pfds[1] ;
    int __rt ;

    if ( 0 == S_fd_valid1_invalid0_close( ___fd ) ) return false ;
    __pfds[0].fd = *___fd ;
    __pfds[0].events = POLLIN ;
    __pfds[0].revents = 0 ;

    __rt = poll(__pfds, 1, 0);

    if ( __rt < 0 ) {
        debugRead01( " rt lessThan 0 " ) ;
        close( *___fd ) ; *___fd = -1 ;
        return false ; /* error 1*/
    }
    
    if ( __rt == 0 ) {
        return false ; /* can NOT read , normal */
    }

    if ( 0 ) 
        debugRead01( " rt Large than 0 " ) ;

    if ( __pfds[0].revents & POLLERR ) {
        if ( __pfds[0].revents == POLLERR ) {
        } else {
            debugRead01( " POLLERR " ) ;
        }

        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if ( __pfds[0].revents & POLLHUP ) {
        if ( __pfds[0].revents == POLLHUP ) {
        } else {
            debugRead01( " POLLHUP " ) ;
        }

        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if ( __pfds[0].revents & POLLNVAL ) {
        if ( __pfds[0].revents == POLLNVAL ) {
        } else {
            debugRead01( " POLLNVAL " ) ;
        }

        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if( __pfds[0].revents & POLLIN ) {
        if ( __pfds[0].revents == POLLIN ) {
        } else {
            debugRead01( " POLLIN " ) ;
        }

        return true ;
    }

    debugRead01( " unknown Read state " ) ;
    return false ;
} /* S_fd_canRead */

