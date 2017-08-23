
#ifndef __PATCHXFBASE_H 
#define __PATCHXFBASE_H 



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>

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
#define _prSF(  fmt , ... ) printf( "--debuging: %s %d %s : " fmt , basename(__FILE__), __LINE__, __func__ , ## __VA_ARGS__ )
#define _prSFn( fmt , ... ) _prSF( fmt "\n" , ## __VA_ARGS__ )

#define _prFF(  fmt , pp , aa ) _prOO( "%s -> " fmt , _strX( aa ) , pp -> aa )
#define _prFFn( fmt , ... ) _prFF( fmt "\n" , ## __VA_ARGS__ )

#define _prD( aa ) _prSFn( "%d" , aa )
#define _prDS( dd , ss ) _prSFn( "<%d:%s>" , dd , ss )

#define _prExit( fmt , ... ) { _prSFn( fmt "\n exit..." , ## __VA_ARGS__ ) ; exit( 32 ) ; }


extern void _mainPara01( int ___argc , char ** ___argv ) ;

extern void _paraAtoi( int _para_must1_ignore0 , int ___argc , char ** ___argv , int ___index , char * ___varName , int  *  ___varLP ) ;
extern void _paraAtoS( int _para_must1_ignore0 , int ___argc , char ** ___argv , int ___index , char * ___varName , char ** ___varLP ) ;
#define X_paraAtoi( must , idx , iii ) _paraAtoi( must , ___argc , ___argv , idx , _strX( iii ) , & iii ) 
#define X_paraAtoS( must , idx , sss ) _paraAtoS( must , ___argc , ___argv , idx , _strX( sss ) , &(sss) ) 

extern void _dumpWAVEFORMATEX( char * ___hMsg , char * ___tMsg , char * ___varName , WAVEFORMATEX * ___wavfmt ) ;

#endif
