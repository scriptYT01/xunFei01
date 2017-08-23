#include "patchXF/pathcXFiatRec.h"

int _uploadWord_0not_1up = -1 ;
int _src_0wavFile_1mic = -1 ;

static void _usageIatRec( int ___argc , char ** ___argv ) {
    _prSFn( "\n\n" "%s" 
            " <_uploadWord_0not_1up>"
            " <_src_0wavFile_1mic>"
            "\n\n\n"
            , ___argv[0] ) ;
} // _usageIatRec



void _mainPara01( int ___argc , char ** ___argv ) {
    _usageIatRec( ___argc , ___argv );

    if ( ___argc < (2 + 1) ) {
        _prSFn( 
                "\n"
                "no enough parameter. exit."
                "\n" 
              );
        exit( 11 ) ;
    }

    // _paraAtoi
    X_paraAtoi( 1 , _uploadWord_0not_1up ) ;
    X_paraAtoi( 2 , _src_0wavFile_1mic ) ;


} // _mainPara01
