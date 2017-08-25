#include "patchXF/pathcXFiatRec.h"

int     _uploadWord_0not_1up = -1 ;
int     _src_0wavFile_1mic = -1 ;
char  * _micDevName = "default" ;
int     _chAmount   = 1 ;
int     _sampleFreq = 16000 ;
char  * _wavFileName = "wav/iflytek02.wav" ;

static void _usageIatRec( int ___argc , char ** ___argv ) {
    _prSFn( "\n\n" "%s" 
            " <_uploadWord_0not_1up>"
            " <_src_0wavFile_1mic>"
            " [_micDevName]"
            " [_chAmount]"
            " [_sampleFreq]"
            " [_wavFileName]"
            "\n\n\n"
            , ___argv[0] ) ;
} // _usageIatRec



// _paraAtoi
void _mainParaIatRec( int ___argc , char ** ___argv ) {
    _usageIatRec( ___argc , ___argv );

    if ( ___argc < (2 + 1) ) {
        _prSFn( 
                "\n"
                "no enough parameter. exit."
                "\n" 
              );
        exit( 11 ) ;
    }

    _prSFn( "\n" " ---=== parameter analyze : start " );

    // _paraAtoi
    X_paraAtoi( 1 , 1 , _uploadWord_0not_1up    ) ;
    X_paraAtoi( 1 , 2 , _src_0wavFile_1mic      ) ;
    X_paraAtoS( 0 , 3 , _micDevName             ) ;
    X_paraAtoi( 0 , 4 , _chAmount               ) ;
    X_paraAtoi( 0 , 5 , _sampleFreq             ) ;
    X_paraAtoS( 0 , 6 , _wavFileName            ) ;

    _prSFn( "\n" " ---=== parameter analyze : end " "\n\n" );

} // _mainParaIatRec

int _ReadCnt01[7] = {0} ;

void _dumpReadCnt01( const char * ___file , int ___line , const char * ___func  ) {
    //_prSFn( " _ReadCnt01 : %d , %d, %d , %d" 
    _prOOn( " %s %d %s : _ReadCnt01 : %d , %d, %d = %d , %d , %d , %d " 
            , ___file , ___line , ___func
            , _ReadCnt01[0] 
            , _ReadCnt01[1] 
            , _ReadCnt01[2] 
            , _ReadCnt01[3] 
            , _ReadCnt01[4] 
            , _ReadCnt01[5] 
            , _ReadCnt01[6] 
            );
} // _dumpReadCnt01

