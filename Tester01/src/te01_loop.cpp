#include "te01_99_top.h"

void _dumpStatus_when_exiting(){
    //if(1 && _tcpExit           )      _tcpExit           -> dumpSelfX();
} /* _dumpStatus_when_exiting */

void _te_stdin_analyze() {
    char __buf1020[1024] ;
    int  __len ;
    int  __i01 ;
    int  __i02 ;
    if ( _SSin -> _canRead( true ) ) {
        /* write something... */
        
        __len = _SSin -> _ssReadNonblock( 1000, __buf1020 ) ;
        if ( __len > 0 ) {
            __i01 = __len - 1 ;
            while ( __i01 >= 0 ) {
                if ( __buf1020[__i01] == '\r' || __buf1020[__i01] == '\n' ) {
                } else {
                    break ;
                }
                __buf1020[__i01] = 0 ;
                __i01 -- ;
                __len -- ;
            }
        }
        if ( __len > 0 ) {
            __i02 = atoi( __buf1020 ) ;
            _prEFn( " len %d : <%s>%d" , __len , __buf1020 , __i02 ) ;
        } else {
            if(0) _prEFn( " len %d " , __len ) ;
        }
    }
} /* _te_stdin_analyze */

int _main_loop() {


    if ( _SSexit -> _canWrite( true ) ) {
        /* write something... */
        return -33213 ;
    }


#if 0 
    _readStdinAndTryHexDump( _SSin , 1000, 5 ) ;
#endif

#if 1 
    _te_stdin_analyze();
#endif

    return 1 ;
} /* _main_loop */
