#include "kp01_99_top.h"

void _testSS(void) {
    char    __buf1024[1024] ;
    int     __len ;

    if ( ! ssOK( _fGenRawPcm ) ) {
        _prExit( " file don't exist. " );
    }

    __len = _fGenRawPcm -> 
        _ssReadNonblock(  9 , __buf1024 ) ;
//_ssReadBlock
    if ( __len > 0 ) {
        _prEF( "received <%d> [" , __len ); 
        write( 2 , __buf1024 , __len ) ;
        write( 2 , "]\n" , 2 ) ;
    } else {
        _prEFn( "nothing received <%d> " , __len ); 
    }

    if(0) _prExit( " testing " );
} /* _testSS */

