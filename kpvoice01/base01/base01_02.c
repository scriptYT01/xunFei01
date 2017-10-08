
#include "base01.h"

int _system(const char *___cmd) {
    char * __buf[256] ;
    if ( NULL == ___cmd ) {
        return -182819 ;
    }
    snprintf( __buf, 255 , " %s &> /dev/null " , ___cmd ) ;
    __buf[255] = 0 ;

    return system( __buf ) ;
} /* _system */


static volatile int _keepRunning = 1;

void _intHandler(int ___dummy) {
    _keepRunning = 0;
    exit(___dummy);
} /* _intHandler */
