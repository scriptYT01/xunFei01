
#ifndef __PATCHXF_H 
#define __PATCHXF_H 

#include <libgen.h>
#include <stdio.h>
#define _prSF( fmt , ... ) printf( "--debuging: %s %d %s : " fmt , basename(__FILE__), __LINE__, __func__ , ## __VA_ARGS__ )
#define _prSFn( fmt , ... ) _prSF( fmt "\n" , ## __VA_ARGS__ )

#define _prD( aa ) _prSFn( "%d" , aa )
#define _prDS( dd , ss ) _prSFn( "<%d:%s>" , dd , ss )

extern void _mainPara01( int ___argc , char ** ___argv ) ;

#endif
