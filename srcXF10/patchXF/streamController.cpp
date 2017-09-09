#include "streamController.h"

void _streamController::_scPrintf( _enEaction ___eAction , const char *___fmt , ... ) { 
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


