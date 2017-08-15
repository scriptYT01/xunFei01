
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

// LD_PRELOAD=/ch/xf30/dbmem.so ./aiui_sample32.x1000.bi

int _myDebug01 = 0 ;

int _cntMalloc01    = 0 ;
int _cntMalloc02    = 0 ;
int _cntMalloc03    = 0 ;
int _cntMalloc04    = 0 ;
int _cntFree01      = 0 ;

//FILE *(*original_fopen)(const char*, const char*);
//original_fopen = dlsym(RTLD_NEXT, "fopen");
//return (*original_fopen)(path, mode);

void * (*_origin__malloc)(size_t ) = NULL ;
void (*_origin__free)(void *) = NULL ;

static void show_dbinfo_once( char ___who ){
    _myDebug01 ++ ;
    switch ( _myDebug01 ) {
        case 1 :
            printf( "\n" "myDebug init : %c" "\n" , ___who ) ;
            break ;
        default:
            break ;
    }
    if ( NULL == _origin__malloc ) {
        _origin__malloc     = dlsym( RTLD_NEXT , "malloc" ) ;
        _origin__free       = dlsym( RTLD_NEXT , "free" ) ;
        if ( NULL == _origin__malloc || NULL == _origin__free ) {
            printf( "\n" " can not get func. " " %p %p , exit.\n" , _origin__malloc , _origin__free);
            exit( 21 ) ;
        }
        if ( _origin__free == _origin__malloc ) {
            printf( "\n" " why equal . " " %p %p , exit.\n" , _origin__malloc , _origin__free);
            exit( 22 ) ;
        }
        printf( "\n" " malloc , free : " " %p %p , exit.\n" , _origin__malloc , _origin__free);
    }
} // show_dbinfo_once

void *malloc(size_t ___size) {
    void * __v01 ;
    show_dbinfo_once( 'm' ) ;

    __v01 = _origin__malloc( ___size ) ;
    if ( __v01 ) {
        _cntMalloc01    ++ ;
        _cntMalloc02    += ___size;
    } else {
        _cntMalloc03    ++ ;
        _cntMalloc04    += ___size;
    }
    return __v01 ;
} // malloc

void free(void *___ptr){
    show_dbinfo_once( 'f' ) ;

    _cntFree01      ++ ;
    _origin__free( ___ptr );
} // free

void dbmem_Dump_inSO(void) {
    printf( "\n" "\n"

    "_myDebug01  %d" "\n"
    "_cntMalloc01  %d" "\n"
    "_cntMalloc02  %d" "\n"
    "_cntMalloc03  %d" "\n"
    "_cntMalloc04  %d" "\n"
    "_cntFree01  %d" "\n"
    "_origin__malloc  %d" "\n"
    "_origin__free  %d" "\n"

    "\n"

    , _myDebug01              
    , _cntMalloc01                    
    , _cntMalloc02                    
    , _cntMalloc03                    
    , _cntMalloc04                    
    , _cntFree01                      
    , _origin__malloc             
    , _origin__free          
    );

} // dbmem_Dump_inSO

