#include "superStream.h"

bool _superStreamBase::_canWrite(  bool ___reopen ) {
    if ( ! _ssOK ) return false ;
    if ( S_fd_canWrite( &_ssFD ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen() ;
    return S_fd_canWrite( &_ssFD ) ;
} /* _superStreamBase::_canWrite */

bool _superStreamBase::_canRead(   bool ___reopen ) {
    if ( ! _ssOK ) return false ;
    if ( S_fd_canRead( &_ssFD ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen() ;
    return S_fd_canRead( &_ssFD ) ;
} /* _superStreamBase::_canRead */

bool _TTcp::_canWrite(  ) {
    if ( _ttListenFD < 0 ) return false ;
    if(0)   dumpSelfX();
    return S_fd_canWrite( &_ttClientFD ) ;
} /* _TTcp::_canWrite */

bool _TTcp::_canRead(   ) {
    if ( _ttListenFD < 0 ) return false ;
    if(0)   dumpSelfX();
    return S_fd_canRead( &_ttClientFD ) ;
} /* _TTcp::_canRead */

bool _ssListen1::_canWrite(  bool ___reopen ) {
    if( _tTcp . _canWrite( ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen();
    return _tTcp . _canWrite() ;
} /* _ssListen1::_canWrite */

bool _ssListen1::_canRead(   bool ___reopen ) {
    if( _tTcp . _canRead( ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen();
    return _tTcp . _canRead() ;
} /* _ssListen1::_canRead */


int S_valid_fd_or_errFD( int *___fd ) {
    int __rt ;
    if ( S_fd_valid1_invalid0_close( ___fd ) ) __rt = *___fd ;
    else __rt = 2 ;
    return __rt ;
} /* S_valid_fd_or_errFD */

bool S_fd_canWrite( int *___fd ) {
    struct pollfd __pfds[1] ;
    int __rt ;

    if ( 0 == S_fd_valid1_invalid0_close( ___fd ) ) return false ;
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
} /* S_fd_canWrite */

bool S_fd_canRead( int *___fd ) {
    struct pollfd __pfds[1] ;
    int __rt ;

    if ( 0 == S_fd_valid1_invalid0_close( ___fd ) ) return false ;
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
} /* S_fd_canRead */

int S_setNonblocking(int fd)
{
    int flags;

    /* If they have O_NONBLOCK, use the Posix way to do it */
#if defined(O_NONBLOCK)
    /* Fixme: O_NONBLOCK is defined but broken on SunOS 4.1.x and AIX 3.2.5. */
    if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
        flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
    /* Otherwise, use the old way of doing it */
    flags = 1;
    return ioctl(fd, FIOBIO, &flags);
#endif
} /* S_setNonblocking */
