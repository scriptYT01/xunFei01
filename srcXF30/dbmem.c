
#include <stdio.h>
#include <stdlib.h>

// LD_PRELOAD=/ch/xf30/dbmem.so ./aiui_sample32.x1000.bi

int _myDebug01 = 0 ;

static void show_dbinfo_once( char ___who ){
    _myDebug01 ++ ;
    switch ( _myDebug01 ) {
        case 0 :
        case 1 :
            printf( "\n" "myDebug init : %c" "\n" , ___who ) ;
            break ;
        default:
            break ;
    }
} // show_dbinfo_once

void *malloc(size_t ___size) {
    show_dbinfo_once( 'm' ) ;

    return malloc( ___size ) ;
} // malloc

void free(void *___ptr){
    show_dbinfo_once( 'f' ) ;

    free( ___ptr );
} // free

