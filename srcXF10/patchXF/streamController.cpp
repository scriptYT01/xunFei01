#include "streamController.h"

void _streamController::_scPrintf( _enErrAction ___eAction , const char *___fmt , ... ) { 
    char    __buf1024[1024] ;
    va_list __args ;
    int     __len ;

    if ( ___fmt ) {
        va_start(__args, ___fmt);
        __len = vsnprintf(__buf1024 , 1023 , ___fmt, __args );
        va_end(__args);

        if ( __len ) {
            //_ssWriteNonblock( ___eAction , __len , __buf1024 ) ; 
        }
    }
} /* _streamController::_scPrintf */


void _streamController::_sendMsg1n( string ___msg1 ) { 
    _sendMsg1( ___msg1 + "\n" ); 
} /* _streamController::_sendMsg1n */

void _streamController::_sendMsg1( string ___msg1 ) {
    cout << ___msg1 ;
} /* _streamController::_sendMsg1 */

bool _streamController::_addPath2( string ___path , string ___comment ) {
    _sVec . push_back( _superStreamBase::
            _genSS( 1 , _ssDir , ___path , ___comment ) ) ;
    return true ;
} /* _streamController::_addPath2 */

