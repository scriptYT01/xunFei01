#include "te01_99_top.h"

void _dumpStatus_when_exiting(){
    //if(1 && _tcpExit           )      _tcpExit           -> dumpSelfX();
} /* _dumpStatus_when_exiting */

int _main_loop() {


#if 0
    if ( 1 ) { // when EXIT port is connected, exit.
        if ( _tcpExit -> _canWrite( true ) ) {
            return -3 ;
        }
    }
#endif

    return 1 ;
} /* _main_loop */
