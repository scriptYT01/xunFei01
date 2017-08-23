
#include "patchXF/pathcXFbase.h"

//  WAVEFORMATEX wavfmt = DEFAULT_FORMAT;

void _dumpWAVEFORMATEX( char * ___hMsg , char * ___tMsg , char * ___varName , WAVEFORMATEX * ___wavfmt ) {
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
