#include "superStream.h"

int _superStreamBase::_valid_fd_or_errFD( int *___fd ) {
    int __rt ;
    if ( _FD_valid1_invalid0_close( ___fd ) ) __rt = *___fd ;
    else __rt = 2 ;
    return __rt ;
} /* _valid_fd_or_errFD */

bool _superStreamBase::_canWrite(  bool ___reopen ) {
    if ( ! _ssOK ) return false ;
    if ( _fd_canWrite( &_ssFD ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen() ;
    return _fd_canWrite( &_ssFD ) ;
} /* _superStreamBase::_canWrite */

bool _superStreamBase::_canRead(   bool ___reopen ) {
    if ( ! _ssOK ) return false ;
    if ( _fd_canRead( &_ssFD ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen() ;
    return _fd_canRead( &_ssFD ) ;
} /* _superStreamBase::_canRead */

bool _superStreamBase::_fd_canWrite( int *___fd ) {
    struct pollfd __pfds[1] ;
    int __rt ;

    if ( 0 == _FD_valid1_invalid0_close( ___fd ) ) return false ;
    __pfds[0].fd = *___fd ;
    __pfds[0].events = POLLOUT ;
    __pfds[0].revents = 0 ;

    __rt = poll(__pfds, 1, 0);

    if ( __rt < 0 ) {
        close( *___fd ) ; *___fd = -1 ;
        return false ; /* error 1*/
    }
    
    if ( __rt == 0 ) {
        return false ; /* can NOT Write , normal */
    }

    if ( __pfds[0].revents & POLLERR ) {
        _prEFn( "POLLERR state : %d : %s " , *___fd , strerror(errno) ) ;
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if ( __pfds[0].revents & POLLHUP ) {
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if ( __pfds[0].revents & POLLNVAL ) {
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if(__pfds[0].revents & POLLOUT) {
        return true ;
    }

    _prEFn( "unknown state" ) ;
    return false ;
} /* _fd_canWrite */

bool _superStreamBase::_fd_canRead( int *___fd ) {
    struct pollfd __pfds[1] ;
    int __rt ;

    if ( 0 == _FD_valid1_invalid0_close( ___fd ) ) return false ;
    __pfds[0].fd = *___fd ;
    __pfds[0].events = POLLIN ;
    __pfds[0].revents = 0 ;

    __rt = poll(__pfds, 1, 0);

    if ( __rt < 0 ) {
        close( *___fd ) ; *___fd = -1 ;
        return false ; /* error 1*/
    }
    
    if ( __rt == 0 ) {
        return false ; /* can NOT read , normal */
    }

    if ( __pfds[0].revents & POLLERR ) {
        _prEFn( "POLLERR state : %d : %s " , *___fd , strerror(errno) ) ;
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if ( __pfds[0].revents & POLLHUP ) {
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if ( __pfds[0].revents & POLLNVAL ) {
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if(__pfds[0].revents & POLLIN) {
        return true ;
    }

    _prEFn( "unknown state" ) ;
    return false ;
} /* _fd_canRead */
