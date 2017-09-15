#include "kp01_99_top.h"

#define     _kpListenIP     "127.0.0.1"
#define     _kpTcp_rawpcm   47811
#define     _kpTcp_filtered 47813
#define     _kpTcp_speaker  47815

int main( int ___argc , char ** ___argv ) {

    int __i01 = 1 ;
    while ( 1 ) {
        _prSFn( " --- %d %d " , _timeNow , __i01++ ) ;
        _ffstdout ;
        usleep( 1000000 ) ;
    }

    return 0 ;
} /* main */

