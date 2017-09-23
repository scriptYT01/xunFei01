#include "superStream.h"

bool _superStreamBase::_canWrite(  bool ___reopen ) {
    if ( ! _ssOK ) return false ;
    if ( S_fd_canWrite( &_ssFD , &_ssCntW ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen() ;
    return S_fd_canWrite( &_ssFD , &_ssCntW ) ;
} /* _superStreamBase::_canWrite */

bool _superStreamBase::_canRead(   bool ___reopen ) {
    if ( ! _ssOK ) return false ;
    if ( S_fd_canRead( &_ssFD , &_ssCntR ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen() ;
    return S_fd_canRead( &_ssFD , &_ssCntR ) ;
} /* _superStreamBase::_canRead */

bool _TTcp::_ttCanWrite(  ) {
    if ( _ttListenFD < 0 ) return false ;
    if(0)   dumpSelfX();
    return S_fd_canWrite( &_ttClientFD , &_ttCntW ) ;
} /* _TTcp::_ttCanWrite */

bool _TTcp::_ttCanRead(   ) {
    if ( _ttListenFD < 0 ) return false ;
    if(0)   dumpSelfX();
    return S_fd_canRead( &_ttClientFD , &_ttCntR ) ;
} /* _TTcp::_ttCanRead */

bool _ssListen1::_canWrite(  bool ___reopen ) {
    if( _tTcp . _ttCanWrite( ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen();
    return _tTcp . _ttCanWrite() ;
} /* _ssListen1::_canWrite */

bool _ssListen1::_canRead(   bool ___reopen ) {
    if( _tTcp . _ttCanRead( ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen();
    return _tTcp . _ttCanRead() ;
} /* _ssListen1::_canRead */

bool _ssTcpConnectTo::_canWrite(  bool ___reopen ) {
    if( _tTcp . _ttCanWrite( ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen();
    return _tTcp . _ttCanWrite() ;
} /* _ssTcpConnectTo::_canWrite */

bool _ssTcpConnectTo::_canRead(   bool ___reopen ) {
    if( _tTcp . _ttCanRead( ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen();
    return _tTcp . _ttCanRead() ;
} /* _ssTcpConnectTo::_canRead */


int S_valid_fd_or_errFD( int *___fd ) {
    int __rt ;
    if ( S_fd_valid1_invalid0_close( ___fd ) ) __rt = *___fd ;
    else __rt = 2 ;
    return __rt ;
} /* S_valid_fd_or_errFD */

bool S_fd_canWrite( int *___fd , int * ___retryCNT ) {
    struct pollfd __pfds[1] ;
    int __rt ;

    if ( 0 == S_fd_valid1_invalid0_close( ___fd ) ) return false ;
    __pfds[0].fd = *___fd ;
    __pfds[0].events = POLLOUT ;
    __pfds[0].revents = 0 ;

    __rt = poll(__pfds, 1, 0);

    if ( __rt < 0 ) {
        _prEFn( "rt %d , fd : %d , err : %d %s " , __rt , *___fd , errno , strerror(errno) ) ;
        close( *___fd ) ; *___fd = -1 ;
        return false ; /* error 1*/
    }
    
    if ( __rt == 0 ) {
        return false ; /* can NOT Write , normal */
    }

    if ( __pfds[0].revents & POLLERR ) { // EWOULDBLOCK == EAGAIN == 11 
        if ( errno == EAGAIN ) {
            (*___retryCNT) ++ ;
            if(0)   _prEFn( "POLLERR : rt %d , fd : %d , err : %d %s : CNT %d " , __rt , *___fd , errno , strerror(errno) , *___retryCNT ) ;
            close( *___fd ) ; *___fd = -1 ; (*___retryCNT) = 0 ;
        } else {
            _prEFn( "POLLERR : rt %d , fd : %d , err : %d %s : CNT %d " , __rt , *___fd , errno , strerror(errno) , *___retryCNT ) ;
        }
        return false ;
    }

    if ( __pfds[0].revents & POLLHUP ) {
        _prEFn( "POLLHUP : rt %d , fd : %d , err : %d %s " , __rt , *___fd , errno , strerror(errno) ) ;
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if ( __pfds[0].revents & POLLNVAL ) {
        _prEFn( "POLLNVAL : rt %d , fd : %d , err : %d %s " , __rt , *___fd , errno , strerror(errno) ) ;
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if(__pfds[0].revents & POLLOUT) {
        return true ;
    }

    _prEFn( "unknow state : rt %d , fd : %d , err : %d %s " , __rt , *___fd , errno , strerror(errno) ) ;
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
        _prEFn( " error : rt %d , fd : %d , err : %d %s " , __rt , *___fd , errno , strerror(errno) ) ;
        close( *___fd ) ; *___fd = -1 ;
        return false ; /* error 1*/
    }
    
    if ( __rt == 0 ) {
        return false ; /* can NOT read , normal */
    }

    if ( __pfds[0].revents & POLLERR ) {
        _prEFn( " POLLERR : rt %d , fd : %d , err : %d %s " , __rt , *___fd , errno , strerror(errno) ) ;
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if ( __pfds[0].revents & POLLHUP ) {
        _prEFn( " POLLHUP : rt %d , fd : %d , err : %d %s " , __rt , *___fd , errno , strerror(errno) ) ;
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if ( __pfds[0].revents & POLLNVAL ) {
        _prEFn( " POLLNVAL : rt %d , fd : %d , err : %d %s " , __rt , *___fd , errno , strerror(errno) ) ;
        close( *___fd ) ; *___fd = -1 ;
        return false ;
    }

    if(__pfds[0].revents & POLLIN) {
        return true ;
    }

    _prEFn( " unkown state : rt %d , fd : %d , err : %d %s " , __rt , *___fd , errno , strerror(errno) ) ;
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
