
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void * (*_origin__dbmem_Dump_inSO)(void ) = NULL ;
int dbmemDebugShow_inited = 0 ;

void dbmemDebugShow( void ){
    if ( 0 == dbmemDebugShow_inited ) {
        _origin__dbmem_Dump_inSO     = dlsym( RTLD_NEXT , "dbmem_Dump_inSO" ) ;
        dbmemDebugShow_inited = 1;
    }
    if ( NULL != _origin__dbmem_Dump_inSO ) {
        _origin__dbmem_Dump_inSO();
    }
} // dbmemDebugShow

