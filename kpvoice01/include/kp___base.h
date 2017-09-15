#ifndef     __KP___BASE_H
#define     __KP___BASE_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <libgen.h>
#include <string.h>
#include <fcntl.h>
#include <stdarg.h>
#include <assert.h>
#include <error.h>
#include <err.h>
#include <errno.h>
#include <poll.h>
#include <include/linux/delay.h>



#define _strX( aa ) # aa

#define _prOO(  fmt , ... ) printf( fmt      , ## __VA_ARGS__ )
#define _prOOn( fmt , ... ) _prOO(  fmt "\n" , ## __VA_ARGS__ )
#define _prSF(  fmt , ... ) printf( "=-=:%s %d %s: " fmt , basename((char*)__FILE__), __LINE__, __func__ , ## __VA_ARGS__ )
#define _prSFn( fmt , ... ) _prSF( fmt "\n" , ## __VA_ARGS__ )

#define _prEF(  fmt , ... ) fprintf( stderr , "=-=:%s %d %s: " fmt , basename((char*)__FILE__), __LINE__, __func__ , ## __VA_ARGS__ )
#define _prEFn( fmt , ... ) _prEF( fmt "\n" , ## __VA_ARGS__ )

#define _prFF(  fmt , pp , aa ) _prOO( "%s -> " fmt , _strX( aa ) , pp -> aa )
#define _prFFn( fmt , ... ) _prFF( fmt "\n" , ## __VA_ARGS__ )

#define _prD( aa ) _prSFn( "%d" , aa )
#define _prDS( dd , ss ) _prSFn( "<%d:%s>" , dd , ss )

#define _prExit( fmt , ... ) { _prSFn( fmt "\n exit..." , ## __VA_ARGS__ ) ; exit( 32 ) ; }
#define _zExit( mm , fmt , ... ) if ( 0 == mm ) _prExit( fmt , ## __VA_ARGS__ ) ;
#define _nExit( mm , fmt , ... ) if ( 0 != mm ) _prExit( fmt , ## __VA_ARGS__ ) ;

#define X_paraAtoi( must , idx , iii ) _paraAtoi( must , ___argc , ___argv , idx , _strX( iii ) , & iii )
#define X_paraAtoS( must , idx , sss ) _paraAtoS( must , ___argc , ___argv , idx , _strX( sss ) , &(sss) )
#define _memZS( om )    memset( & om    , 0 , sizeof( om    ) )
#define _memZP( om )    memset( om      , 0 , sizeof( (*om) ) )

#ifdef __cplusplus
    extern "C" {
#endif

#ifdef __cplusplus
    }
#endif

#ifdef __cplusplus
    #include <string>
    #include <iostream>
    #include <vector>
    #include <algorithm>
    #include <functional>

    using namespace std;
#endif




#endif
