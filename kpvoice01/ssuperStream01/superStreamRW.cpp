#include "superStream.h"

int * _superStreamBase::_getDataFD( ) {
    switch ( _ssType ) {
        case _enSstFileOut      :   
        case _enSstFileIn       :   
        case _enSstCin          :   
        case _enSstCerr         :   
        case _enSstCout         :   
            return   &(_ssFD) ;
        case _enSstTcpConnectTo :
        case _enSstTcpListen1   :
            return   &(_tTcp . _ttClientFD) ;
        default:
            _prExit( " unknow type : %d ." , _ssType );
            break ;
    }
} /* _superStreamBase::_getDataFD */

int * _superStreamBase::_getTcpListenFD( ) {
    switch ( _ssType ) {
        case _enSstTcpConnectTo :
        case _enSstTcpListen1   :
            return   &(_tTcp . _ttListenFD) ;
        default:
            _prExit( " unknow type : %d ." , _ssType );
            break ;
    }
} /* _superStreamBase::_getTcpListenFD( ) */

int _superStreamBase::_ssReadNonblock( int ___len , char * ___buf ) {
    int __rLen = -3333 ;
    int *__dataFD = _getDataFD() ;

    _ssTryReopneIfNeeded( ) ;

    if ( S_fd_canRead( __dataFD , &_ssCntR ) ) {
        if ( 0 ) _prEFn( " can Read at once " );
        __rLen = _ssReadBlock( ___len , ___buf ) ;
    } else {
        if ( 0 ) _prEFn( " can NOT Read at once : %d : %s , %s " , *__dataFD , _ssPath , _ssComment );
        if ( 0 && _enSstTcpListen1 == _ssType ) { _tTcp . dumpSelfX(); }
        _ssInfoW . _tryCnt ++ ;
        _ssInfoW . _tryLen += ___len ;
        _ssInfoW . _skipCnt ++ ;
        _ssInfoW . _skipLen += ___len ;
    }

    if ( 0 ) dumpSelfX() ;
    return __rLen ;
} /* _superStreamBase::_ssReadNonblock */

int _superStreamBase::_ssReadReal( int ___len , char * ___buf ) {
    int __rLen ;
    int *__dataFD = _getDataFD() ;

    __rLen = read( *__dataFD , ___buf , ___len ) ;  // _ssReadBlock

    if ( __rLen == 0 ) {
        *__dataFD = -1 ;
        _ssTryReopneIfNeeded( ) ;// try reopen it
        if ( S_fd_canRead( __dataFD , &_ssCntR ) ) {  // if reopen ok.
            __rLen = read( *__dataFD , ___buf , ___len ) ;  // _ssReadBlock re-read.
        } else {
            if(1) _prEFn( " 0 --> file end , re-read failed." ) ;
        }
    }
    return __rLen ;
} /* _superStreamBase::_ssReadReal */

int _superStreamBase::_ssReadBlock( int ___len , char * ___buf ) {
    int __rLen = -2222 ;
    int *__dataFD = _getDataFD() ;

    _ssInfoW . _tryCnt ++ ;
    _ssInfoW . _tryLen += ___len ;

    _ssTryReopneIfNeeded( ) ;

    if ( S_fd_valid1_invalid0_close( __dataFD ) ) {
        int __Len ;
        __Len = _ssReadReal( ___len , ___buf ) ;  // _ssReadBlock
        if ( __Len <= 0 ) { // less than 0
            _ssInfoW . _skipCnt ++ ;
            _ssInfoW . _skipLen += ___len ;
            if(0)   _prExit( " ---- %d " , __Len ) ;
            __rLen = -2241 ;
        } else if ( __Len == ___len ) {
            _ssInfoW . _succCnt ++ ;
            _ssInfoW . _succLen += ___len ; // _ssReadBlock
            __rLen = __Len ;
        } else {
            _ssInfoW . _succCnt ++ ;
            _ssInfoW . _succLen += __Len ;
            _ssInfoW . _skipCnt ++ ;
            _ssInfoW . _skipLen += ___len - __Len ; // _ssReadBlock
            __rLen = __Len ;
        }
    } else {
        _ssInfoW . _skipCnt ++ ;
        _ssInfoW . _skipLen += ___len ;
        if(1)   _prExit( " === " ) ;
    }

    return __rLen ;
} /* _superStreamBase::_ssReadBlock */

int _superStreamBase::_ssWriteNonblock( int ___len , const char * ___buf ) {
    int __wLen = 0 ;
    int *__dataFD = _getDataFD() ;

    _ssTryReopneIfNeeded( ) ;

    if ( S_fd_canWrite( __dataFD , & _ssCntW ) ) {
        if ( 0 ) _prEFn( " can Write at once " );
        __wLen = _ssWriteBlock( ___len , ___buf ) ;
    } else {
        if ( 1 ) _prEFn( " can NOT Write at once : %d : %s , %s " , *__dataFD , _ssPath , _ssComment );
        _ssInfoW . _tryCnt ++ ;
        _ssInfoW . _tryLen += ___len ;
        _ssInfoW . _skipCnt ++ ;
        _ssInfoW . _skipLen += ___len ;
    }
    return __wLen ;
} /* _superStreamBase::_ssWriteNonblock */

int _superStreamBase::_ssWriteBlock( int ___len , const char * ___buf ) {
    int __wLen = 0 ;
    int *__dataFD = _getDataFD() ;

    _ssInfoW . _tryCnt ++ ;
    _ssInfoW . _tryLen += ___len ;

    _ssTryReopneIfNeeded( ) ;

    if ( S_fd_valid1_invalid0_close( __dataFD ) ) {
        int __Len ;
        __Len = write( *__dataFD , ___buf , ___len ) ;  // _ssWriteBlock
        if ( __Len <= 0 ) {
            _ssInfoW . _skipCnt ++ ;
            _ssInfoW . _skipLen += ___len ;
        } else if ( __Len == ___len ) {
            _ssInfoW . _succCnt ++ ;
            _ssInfoW . _succLen += ___len ;
            __wLen = __Len ;
        } else {
            _ssInfoW . _succCnt ++ ;
            _ssInfoW . _succLen += __Len ;
            _ssInfoW . _skipCnt ++ ;
            _ssInfoW . _skipLen += ___len - __Len ; // _ssWriteBlock
            __wLen = __Len ;
        }
    } else {
        _ssInfoW . _skipCnt ++ ;
        _ssInfoW . _skipLen += ___len ;
    }

    return __wLen ;
} /* _superStreamBase::_ssWriteBlock */

