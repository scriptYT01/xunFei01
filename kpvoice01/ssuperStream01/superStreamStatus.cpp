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

bool _TTcp::_ttCanWrite1(  ) {
    return S_fd_canWrite( &_ttClientFD , &_ttCntW ) ;
} /* _TTcp::_ttCanWrite1 */

bool _TTcp::_ttCanRead1(   ) {
    return S_fd_canRead( &_ttClientFD , &_ttCntR ) ;
} /* _TTcp::_ttCanRead1 */

bool _TTcp::_ttCanWrite2(  ) {
    if ( _ttListenFD < 0 ) return false ;
    if(0)   dumpSelfX();
    return S_fd_canWrite( &_ttClientFD , &_ttCntW ) ;
} /* _TTcp::_ttCanWrite2 */

bool _TTcp::_ttCanRead2(   ) {
    if ( _ttListenFD < 0 ) return false ;
    if(0)   dumpSelfX();
    return S_fd_canRead( &_ttClientFD , &_ttCntR ) ;
} /* _TTcp::_ttCanRead2 */

bool _ssListen1::_canWrite(  bool ___reopen ) {
    if ( _ssDir != _enSsdOut && _ssDir != _enSsdOutIn ) { return false ; }

    if( _tTcp . _ttCanWrite2( ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen();
    return _tTcp . _ttCanWrite2() ;
} /* _ssListen1::_canWrite */

bool _ssListen1::_canRead(   bool ___reopen ) {
    if ( _ssDir != _enSsdIn && _ssDir != _enSsdOutIn ) { return false ; }

    if( _tTcp . _ttCanRead2( ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen();
    return _tTcp . _ttCanRead2() ;
} /* _ssListen1::_canRead */

bool _ssTcpConnectTo::_canWrite(  bool ___reopen ) {
    if ( _ssDir != _enSsdOut && _ssDir != _enSsdOutIn ) { return false ; }

    if( _tTcp . _ttCanWrite1() ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen();
    return _tTcp . _ttCanWrite1() ;
} /* _ssTcpConnectTo::_canWrite */

bool _ssTcpConnectTo::_canRead(   bool ___reopen ) {
    if ( _ssDir != _enSsdIn && _ssDir != _enSsdOutIn ) { return false ; }

    if( _tTcp . _ttCanRead1( ) ) return true ;
    if ( ! ___reopen ) return false ;
    _ssOpenOrReopen();
    return _tTcp . _ttCanRead1() ;
} /* _ssTcpConnectTo::_canRead */


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
