#include "streamController.h"

//void _superStreamBase::_dumpSelf( const char * ___file , int ___lineno , const char * ___func ) {
void _streamController::_dumpSelf( const char * ___file , int ___lineno , const char * ___func )
{
    _superStreamBase *  __scVec ;
    int                 __i01 ;
    int                 __vecSize ;

    __vecSize = _scVec . size() ; 
    for ( __i01 = 0 ; __i01 < __vecSize ; __i01 ++ )
    {
        __scVec = _scVec[__i01] ;
        __scVec -> dumpSelfX( ) ;
    }
} /* _streamController::_dumpSelf */

void _streamController::_scSetErrAction( _enErrAction ___eAction ) {
    _superStreamBase *  __scVec ;
    int                 __i01 ;
    int                 __vecSize ;

    __vecSize = _scVec . size() ; 
    for ( __i01 = 0 ; __i01 < __vecSize ; __i01 ++ )
    {
        __scVec = _scVec[__i01] ;
        __scVec -> _ssSetErrAction( ___eAction ) ;
    }
} /* _streamController::_scSetErrAction */

void _streamController::_scPrintf( const char *___fmt , ... ) { 
    char                __buf1024[1024] ;
    va_list             __args ;
    int                 __len ;
    _superStreamBase *  __scVec ;
    int                 __i01 ;
    int                 __vecSize ;

    if ( ___fmt ) {
        va_start(__args, ___fmt);
        __len = vsnprintf(__buf1024 , 1023 , ___fmt, __args );
        va_end(__args);

        if ( __len ) {
            __vecSize = _scVec . size() ; 
            for ( __i01 = 0 ; __i01 < __vecSize ; __i01 ++ )
            {
                __scVec = _scVec[__i01] ;
                __scVec -> _ssWriteNonblock( __len , __buf1024 ) ;
            }
        }
    }
} /* _streamController::_scPrintf */

int _streamController::_scReadNonblock(         int ___len , char * ___buf ) {
        //     _ssReadNonblock
    //char                __buf1024[1024] ;
    //va_list             __args ;
    //int                 __len ;
    _superStreamBase *  __scVec ;
    int                 __i01 ;
    int                 __vecSize ;
    int                 __rt = -1 ;

    if ( ___len > 0 && ___buf != NULL ) {
        __vecSize = _scVec . size() ; 
        for ( __i01 = 0 ; __i01 < __vecSize ; __i01 ++ )
        {
            __scVec = _scVec[__i01] ;
            __rt = __scVec -> _ssReadNonblock( ___len , __buf1024 ) ;
            if ( __rt > 0 ) break ;
        }
    }
    return __rt ;
} /* _streamController::_scReadNonblock */


bool _streamController::_addPath2( const char * ___path , const char * ___comment ) {
    _scVec . push_back( _superStreamBase::
            _genSS( 1 , _scDir , ___path , ___comment ) ) ;
    return true ;
} /* _streamController::_addPath2 */

void _streamController::_scSelfTest01( ) {

    /* _scSelfTest01 */
    dumpSelfX() ; /* _dumpSelf */
    if ( 1 ) write( 6 , "yyyy\n" , 5 ) ; /* force use the fd 6 */
    _scPrintf( " aaaa" ) ;

    dumpSelfX() ; /* _dumpSelf */
    if ( 1 ) write( 6 , "zzzz\n" , 5 ) ; /* force use the fd 6 */
    _scPrintf( " bbbb" ) ; /* _scPrintf _ssWriteNonblock */

    dumpSelfX() ; /* _dumpSelf */

} /* _streamController::_scSelfTest01 */

