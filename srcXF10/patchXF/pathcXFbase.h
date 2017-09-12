
#ifndef __PATCHXFBASE_H 
#define __PATCHXFBASE_H 

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






#define _strX( aa ) # aa

#ifndef WAVE_FORMAT_PCM  
#define WAVE_FORMAT_PCM  1
typedef struct tWAVEFORMATEX {
	unsigned short	  wFormatTag;
	unsigned short    nChannels;
	unsigned int      nSamplesPerSec;
	unsigned int      nAvgBytesPerSec;
	unsigned short	  nBlockAlign;
	unsigned short    wBitsPerSample;
	unsigned short    cbSize;
} WAVEFORMATEX;
#endif



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

extern int _strcmpX1( const char * ___preStr , const char * ___bufStr ) ;
extern void _paraAtoi( int _para_must1_ignore0 , int ___argc , char ** ___argv , int ___index , char * ___varName , int  *  ___varLP ) ;
extern void _paraAtoS( int _para_must1_ignore0 , int ___argc , char ** ___argv , int ___index , char * ___varName , char ** ___varLP ) ;

extern void _dumpWAVEFORMATEX( char * ___hMsg , char * ___tMsg , const char * ___varName , WAVEFORMATEX * ___wavfmt ) ;
extern int text_to_speech_from_file_continue(FILE * ___fd , const char* params);

#ifdef __cplusplus
}
#endif

#endif
