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
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <netdb.h>

//#include <include/asm/delay.h>
//#include <include/linux/delay.h>


#define _timeNow    ((int)time(0))

#define _strX( aa ) # aa
#define _caseXstr( aa )    case aa : return _strX( aa ) 

#define _prOO(  fmt , ... ) printf( fmt      , ## __VA_ARGS__ )
#define _prOOn( fmt , ... ) _prOO(  fmt "\n" , ## __VA_ARGS__ )
#define _prSF(  fmt , ... ) printf( "=-=:%s %d %s: " fmt , basename((char*)__FILE__), __LINE__, __func__ , ## __VA_ARGS__ )
#define _prSFn( fmt , ... ) _prSF( fmt "\n" , ## __VA_ARGS__ )
#define _ffstdout   fdatasync( 1 )
#define _ffstderr   fdatasync( 2 )

#define _prErrno(fmt , ... ) _prEFn( "errorno %d , %s " fmt , errno , strerror( errno ) ,  ## __VA_ARGS__ ) 

//#define _prEF(  fmt , ... ) fprintf( stderr , "=-=:%s %d %s: " fmt , basename((char*)__FILE__), __LINE__, __func__ , ## __VA_ARGS__ )
#define _prEE(  fmt , ... ) fprintf( stderr , fmt , ## __VA_ARGS__ )
#define _prEEn(  fmt , ... ) _prEE( fmt "\n" , ## __VA_ARGS__ )
#define _prEF(  fmt , ... ) fprintf( stderr , "=%s %d: " fmt , basename((char*)__FILE__), __LINE__, ## __VA_ARGS__ )
#define _prEFn( fmt , ... ) _prEF( fmt "\n" , ## __VA_ARGS__ )
#define _prEX00( aa ) _prEE( "%02x"     , aa )
#define _prEX01( aa ) _prEE( "%02x "    , aa )
#define _prEX10( aa ) _prEE( " %02x"    , aa )
#define _prEX11( aa ) _prEE( " %02x "   , aa )

#define _prFF(  fmt , pp , aa ) _prOO( "%s -> " fmt , _strX( aa ) , pp -> aa )
#define _prFFn( fmt , ... ) _prFF( fmt "\n" , ## __VA_ARGS__ )

#define _prD( aa ) _prSFn( "%d" , aa )
#define _prDS( dd , ss ) _prSFn( "<%d:%s>" , dd , ss )

#define _prExit( fmt , ... ) { _prSFn( fmt "\n exit..." , ## __VA_ARGS__ ) ; exit( 32 ) ; }
#define _zExit( mm , fmt , ... ) if ( 0 == mm ) _prExit( fmt , ## __VA_ARGS__ ) ;
#define _nExit( mm , fmt , ... ) if ( 0 != mm ) _prExit( fmt , ## __VA_ARGS__ ) ;
#define _zExitD( mm )  _zExit( mm , " " _strX( mm ) " -> %d , exit " , mm )
#define _nExitD( mm )  _nExit( mm , " " _strX( mm ) " -> %d , exit " , mm )

#define X_paraAtoi( must , idx , iii ) _paraAtoi( must , ___argc , ___argv , idx , _strX( iii ) , & iii )
#define X_paraAtoS( must , idx , sss ) _paraAtoS( must , ___argc , ___argv , idx , _strX( sss ) , &(sss) )
#define _memZS( om )    memset( & om    , 0 , sizeof( om    ) )
#define _memZP( om )    memset( om      , 0 , sizeof( (*om) ) )

#define dumpSelfX() _dumpSelf( __FILE__, __LINE__ , __func__ )
#define dumpSelfY() _dumpSelf( NULL , 0 , NULL )

#ifdef __cplusplus
    extern "C" {
#endif
extern uint64_t     _u64_now() ;
extern int          _strcmpX1( const char * ___preStr , const char * ___bufStr ) ;
extern int          _strcmpXX( const char * ___preStr , const char * ___bufStr ) ;
extern void         _paraAtoi( int _para_must1_ignore0 , int ___argc , char ** ___argv , int ___index , char * ___varName , int  *  ___varLP ) ;
extern void         _paraAtoS( int _para_must1_ignore0 , int ___argc , char ** ___argv , int ___index , char * ___varName , char ** ___varLP ) ;
extern int          _timeLoop( uint32_t ___delayUS , int (*___loopBackFunc)() ) ;

extern int          _argc ;
extern char **      _argv ;

void        _dumpStatus_when_exiting();
int         _main_loop() ;

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

#define _sleep_1ms      usleep( 1000 ) 
#define _sleep_2ms      usleep( 2000 ) 
#define _sleep_3ms      usleep( 3000 ) 
#define _sleep_5ms      usleep( 5000 ) 
#define _sleep_10ms     usleep( 10000 ) 
#define _sleep_20ms     usleep( 20000 ) 
#define _sleep_30ms     usleep( 30000 ) 
#define _sleep_33ms     usleep( 33333 ) 
#define _sleep_50ms     usleep( 50000 ) 
#define _sleep_100ms    usleep( 100000 ) 
#define _sleep_200ms    usleep( 200000 ) 
#define _sleep_300ms    usleep( 300000 ) 
#define _sleep_500ms    usleep( 500000 ) 


#endif
