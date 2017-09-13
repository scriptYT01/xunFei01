#include "streamController.h"

void _streamController::_scDumpSS() 
{
    _superStreamBase *  __sVec ;
    int                 __i01 ;
    int                 __size ;

    __size = _sVec . size() ; 
    for ( __i01 = 0 ; __i01 < __size ; __i01 ++ )
    {
        __sVec = _sVec[__i01] ;
        __sVec -> _ssDumpSelf( ) ;
    }
} /* _streamController::_scDumpSS */

void _streamController::_scPrintf( _enErrAction ___eAction , const char *___fmt , ... ) { 
    char                __buf1024[1024] ;
    va_list             __args ;
    int                 __len ;
    _superStreamBase *  __sVec ;
    int                 __i01 ;
    int                 __size ;

    if ( ___fmt ) {
        va_start(__args, ___fmt);
        __len = vsnprintf(__buf1024 , 1023 , ___fmt, __args );
        va_end(__args);

        if ( __len ) {
            //_ssWriteNonblock( ___eAction , __len , __buf1024 ) ; 
            //for_each(_sVec.begin(), _doorVec.end(), mem_fun(&Door::open));
            //for_each(_sVec.begin(), _sVec.end(), mem_fun(&_superStreamBase::_ssWriteNonblock( ___eAction , __len , __buf1024 ) ));
            //for_each(_sVec.begin(), _sVec.end(), _prOBJ( ___eAction , __len , __buf1024 ) ); 
            // _ssWriteNonblock
            //_superStreamBase* __sVec in _sVec)
            __size = _sVec . size() ; 
            for ( __i01 = 0 ; __i01 < __size ; __i01 ++ )
            {
                __sVec = _sVec[__i01] ;
                __sVec -> _ssWriteNonblock( ___eAction , __len , __buf1024 ) ;
            }
        }
    }
} /* _streamController::_scPrintf */


bool _streamController::_addPath2( const char * ___path , const char * ___comment ) {
    _sVec . push_back( _superStreamBase::
            _genSS( 1 , _ssDir , ___path , ___comment ) ) ;
    return true ;
} /* _streamController::_addPath2 */

void _streamController::_scSelfTest01( ) {

    /* _scSelfTest01 */
    _scDumpSS() ; /* _ssDumpSelf */
    if ( 0 ) write( 6 , "yyyy\n" , 5 ) ; /* force use the fd 6 */
    _scPrintf( _enEreopen , " aaaa" ) ;

    _scDumpSS() ; /* _ssDumpSelf */
    if ( 0 ) write( 6 , "zzzz\n" , 5 ) ; /* force use the fd 6 */
    _scPrintf( _enEreopen , " bbbb" ) ; /* _scPrintf _ssWriteNonblock */

    _scDumpSS() ; /* _ssDumpSelf */

} /* _streamController::_scSelfTest01 */

