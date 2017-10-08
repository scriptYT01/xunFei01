
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


volatile int _mainRunning = 1;

void _intHandler1(int ___dummy) {
    _mainRunning = 0;
    exit(___dummy);
} /* _intHandler1 */

void _intHandler2(int ___dummy) {
    char *  __buf[256] ;
    int     __len ;

    fprintf( 2 , "\n\n" " Ctrl + C pressed. Do you want to exit really ? (y/n)\n" ) ;
    read( 0, __buf, 1 );
    if ( __len <= 0 ) {
        fprintf( 2 , "\n" " unknown met 181811\n" );
        _intHandler1( ___dummy );
    }
    if ( __buf[0] == 'y' || __buf[0] == 'Y' ){
        fprintf( 2 , "\n" " Yes met. exit...\n" );
        _intHandler1( ___dummy );
    }
    fprintf( 2 , "\n" " 'No' met. skip...\n" );
} /* _intHandler2 */
