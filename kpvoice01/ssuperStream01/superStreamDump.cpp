#include "superStream.h"

static const char * _strEA( int ___aa ) {
    switch ( ___aa ) {
        _caseXstr( _enEreopen ) ;
        _caseXstr( _enEignore ) ;
        _caseXstr( _enEexit   ) ;
        default : return __func__ ;
    }
} /* _strEA */

static const char * _strDI( int ___aa ) {
    switch ( ___aa ) {
        _caseXstr( _enSsdUnknown ) ;
        _caseXstr( _enSsdOutIn   ) ;
        _caseXstr( _enSsdOut     ) ;
        _caseXstr( _enSsdIn      ) ;
        default : return __func__ ;
    }
} /* _strDI */

static const char * _strTT( int ___aa ) {
    switch ( ___aa ) {
        _caseXstr( _enSstUnknown       ) ;
        _caseXstr( _enSstFileOut       ) ;
        _caseXstr( _enSstTcpConnectTo  ) ;
        _caseXstr( _enSstTcpListen1    ) ;
        _caseXstr( _enSstCin           ) ;
        _caseXstr( _enSstCerr          ) ;
        _caseXstr( _enSstCout          ) ;
        default : return __func__ ;
    }
} /* _strTT */

void _superStreamBase::_dumpSelf( const char * ___file , int ___lineno , const char * ___func ) {
    _prEFn( "-------- 11 " );
    _prEFn( " %s - %d - %s " , basename((char*)___file) , ___lineno , ___func );
    _prEFn( "_ssErrAction   '%d' : %s"  , _ssErrAction , _strEA( _ssErrAction     ) ) ;
    _prEFn( "_ssDir         '%d' : %s"  , _ssDir       , _strDI( _ssDir           ) ) ;
    _prEFn( "_ssType        '%d' : %s"  , _ssType      , _strTT( _ssType          ) ) ;
    _prEFn( "_ssPath        '%s'"       , _ssPath                                   ) ;
    _prEFn( "_ssComment     '%s'"       , _ssComment                                ) ;
    _prEFn( "_ssOK          '%p'"       , _ssOK                                     ) ;
    _prEFn( "_ssFD,tcpFD    '%d,%d'"    , _ssFD , _tTcp._ttListenFD                       ) ;

    if ( _ssType == _enSstTcpListen1 || _ssType == _enSstTcpConnectTo ) {
        _tTcp.dumpSelfY();
    }

    _prEFn( "_ssInfoW " 
            " tryCnt,Len  %4lld %4lld " 
            " skipCnt,Len %4lld %4lld " 
            " succCnt,Len %4lld %4lld " 
            , _ssInfoW . _tryCnt     
            , _ssInfoW . _tryLen     
            , _ssInfoW . _skipCnt    
            , _ssInfoW . _skipLen    
            , _ssInfoW . _succCnt    
            , _ssInfoW . _succLen    
            ) ;

    _prEFn( "_ssInfoR " 
            " tryCnt,Len  %4lld %4lld " 
            " skipCnt,Len %4lld %4lld " 
            " succCnt,Len %4lld %4lld " 
            , _ssInfoR . _tryCnt     
            , _ssInfoR . _tryLen     
            , _ssInfoR . _skipCnt    
            , _ssInfoR . _skipLen    
            , _ssInfoR . _succCnt    
            , _ssInfoR . _succLen    
            ) ;
    _prEFn( "-------- 99 " );

} /* _superStreamBase::_dumpSelf */

void _TTcp::_dumpSelf( const char * ___file , int ___lineno , const char * ___func ) {
    if ( 0 != ___lineno ) {
        _prEFn( "" );
        _prEFn( " %s - %d - %s " , basename((char*)___file) , ___lineno , ___func );
    }
    _prEFn( "_ttListenFD,Bd,Ld       '%d,%d,%d'" "_ttClientFD %d" , _ttListenFD , _ttBd, _ttLd , _ttClientFD) ;
    _prEFn( "path , host , port , _ttplen       '%s , %s , %s , %d'" , _ttpath , _tthost , _ttport , _ttplen ) ;
} /* _TTcp::_dumpSelf */

void _readStdinAndTryHexDump( _superStreamBase * ___ss , int ___len , int ___hexLen ) {
    char    __buf4096[1001] ;
    int     __Len ;
    int     __i01 ;

    if ( ___ss == NULL || ___len <= 0 ) {
        _prExit( " why parameter error ?" ) ;
    }

    if ( ___len > 1000 ) {
        ___len = 1000 ;
    }

    if ( ___ss -> _canRead( true ) ) {
        /* write something... */
        
        __Len = ___ss -> _ssReadNonblock( ___len, __buf4096 ) ;
        if ( __Len > 0 ) {
            __buf4096[__Len] = 0 ;
            _prEFn( " len %d : <%s>" , __Len , __buf4096 ) ;
        } else {
            _prEFn( " len %d " , __Len ) ;
        }

        if ( ___hexLen > 0 ) {
            _prEE( " hexDump %d :<" , ___hexLen ) ;
            __i01 = 0 ;
            while ( __i01 < ___hexLen && __i01 < __Len ) {
                _prEX10( __buf4096[__i01] ) ; 
                __i01 ++ ;
            }
            _prEEn( " >" );
            if(0) _prEX10( __buf4096[__i01] ) ; 
        }
    }
} /* _readStdinAndTryHexDump */

