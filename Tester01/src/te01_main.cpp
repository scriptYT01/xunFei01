#include "te01_99_top.h"

void _usage( int ___argc , char ** ___argv ) {
} /* _usage */

void _paraAnalyze( int ___argc , char ** ___argv ) {
    _paraAnalyzeSS( ___argc , ___argv ) ;
    //if ( ___argc > 2 ) { _FrawPcm           =  ___argv[1] ; }
    //_fGenRawPcm    = new _ssFileIn(  _enSsdIn    , _FrawPcm     , " _fGenRawPcm    : rawPCM    generater " );
} /* _paraAnalyze */

void _initListen(void) {
    //_tcpRaw     = new _ssListen1( _enSsdOut 
    //        , "tcpL1:" _kpListenIP ":" _kpTcp_rawpcm    , " _tcpRaw     : when connected , try to output RAW-pcm " ) ;
} /* _initListen */

int main( int ___argc , char ** ___argv ) {

    int __i01 ;

    _usage( ___argc , ___argv ) ;

    _paraAnalyze( ___argc , ___argv ) ;

    _initListen() ;

    __i01 = _timeLoop(
            100000
            , &
            _main_loop ) ;

    _prEFn( " loop return %d " , __i01 ) ;

    _dumpStatus_when_exiting() ;

    return 0 ;
} /* main */

