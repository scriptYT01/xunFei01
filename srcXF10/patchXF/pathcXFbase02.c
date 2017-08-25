
#include "patchXF/pathcXFbase.h"

//  WAVEFORMATEX wavfmt = DEFAULT_FORMAT;

void _dumpWAVEFORMATEX( char * ___hMsg , char * ___tMsg , const char * ___varName , WAVEFORMATEX * ___wavfmt ) {
    if ( NULL != ___hMsg ) { _prSF( ":%s:%s" , ___varName , ___hMsg ) ; }


    _prFFn( "%d" , ___wavfmt , wFormatTag          );
    _prFFn( "%d" , ___wavfmt , nChannels           );
    _prFFn( "%d" , ___wavfmt , nSamplesPerSec      );
    _prFFn( "%d" , ___wavfmt , nAvgBytesPerSec     );
    _prFFn( "%d" , ___wavfmt , nBlockAlign         );
    _prFFn( "%d" , ___wavfmt , wBitsPerSample      );
    _prFFn( "%d" , ___wavfmt , cbSize              );


    if ( NULL != ___tMsg ) { _prOO( "%s" , ___tMsg ) ; }
} // _dumpWAVEFORMATEX

int _ReadCnt01[4] = {0} ;

void _dumpReadCnt01( const char * ___file , int ___line , const char * ___func  ) {
    _prSFn( " _ReadCnt01 : %d , %d, %d , %d" 
            , _ReadCnt01[0] 
            , _ReadCnt01[1] 
            , _ReadCnt01[2] 
            , _ReadCnt01[3] 
            );
} // _dumpReadCnt01

