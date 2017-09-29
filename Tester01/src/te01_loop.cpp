#include "te01_99_top.h"

void _dumpStatus_when_exiting(){
    //if(1 && _tcpExit           )      _tcpExit           -> dumpSelfX();
} /* _dumpStatus_when_exiting */

int _main_loop() {


    if ( _SSexit -> _canWrite( true ) ) {
        /* write something... */
        return -33213 ;
    }


#if 0
    char _buf1020[1024] ;
    int  _len ;
    if ( _SSin -> _canRead( true ) ) {
        /* write something... */
        
        _len = _SSin -> _ssReadNonblock( 1000, _buf1020 ) ;
        if ( _len > 0 ) {
            _prEFn( " len %d : <%s>" , _len , _buf1020 ) ;
        } else {
            _prEFn( " len %d " , _len ) ;
        }
    }
#else 
    SS_testRead( _SSin , 1000, 5 ) ;
#endif

    return 1 ;
} /* _main_loop */
