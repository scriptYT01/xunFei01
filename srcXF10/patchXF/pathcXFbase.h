
#ifndef __PATCHXFBASE_H 
#define __PATCHXFBASE_H 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>

#define _prSF( fmt , ... ) printf( "--debuging: %s %d %s : " fmt , basename(__FILE__), __LINE__, __func__ , ## __VA_ARGS__ )
#define _prSFn( fmt , ... ) _prSF( fmt "\n" , ## __VA_ARGS__ )

#define _prD( aa ) _prSFn( "%d" , aa )
#define _prDS( dd , ss ) _prSFn( "<%d:%s>" , dd , ss )

#define _prExit( fmt , ... ) { _prSFn( fmt "\n exit..." , ## __VA_ARGS__ ) ; exit( 32 ) ; }

#define _strX( aa ) # aa

extern void _mainPara01( int ___argc , char ** ___argv ) ;

extern void _paraAtoi( int ___argc , char ** ___argv , int ___index , char * ___varName , int * ___varLP ) ;
#define X_paraAtoi( idx , iii ) _paraAtoi( ___argc , ___argv , idx , _strX( iii ) , & iii ) 

#endif
