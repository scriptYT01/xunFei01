#include "te01_99_top.h"

void _usage( int ___argc , char ** ___argv ) {
} /* _usage */

void _paraAnalyzeMM( int ___argc , char ** ___argv , uint16_t ___exitPort  ) {
    _paraAnalyzeSS( ___argc , ___argv , 60001) ;
    

    //if ( ___argc > 2 ) { _FrawPcm           =  ___argv[1] ; }
    //_fGenRawPcm    = new _ssFileIn(  _enSsdIn    , _FrawPcm     , " _fGenRawPcm    : rawPCM    generater " );
} /* _paraAnalyzeMM */

// /testsuite/network/miio_wifi     /dev/ttyS1
// /usr/bin/pvalg_ymhood            /dev/snd/pcmC0D2c
// /usr/bin/aplay                   /dev/snd/pcmC0D0p
// X1000 : pcmC0D0c   pcmC0D0p   pcmC0D1c   pcmC0D1p   pcmC0D2c
void _initListen(void) {
    _system( "lsof  |grep snd |xargs -n 1 kill -9 " );
    _system( "lsof  |grep ttyS1 |xargs -n 1 kill -9 " );
    _system( "killall -9 miio_wifi " );
    _system( "killall -9 pvalg_ymhood " );

    //_tcpRaw     = new _ssListen1( _enSsdOut 
    //        , "tcpL1:" _kpListenIP ":" _kpTcp_rawpcm    , " _tcpRaw     : when connected , try to output RAW-pcm " ) ;
} /* _initListen */

int main( int ___argc , char ** ___argv ) {

    int __i01 ;

    _usage( ___argc , ___argv ) ;

    _paraAnalyzeMM( ___argc , ___argv , 60001 ) ;

    _initListen() ;

    __i01 = _timeLoop(
            100000
            , &
            _main_loop ) ;
    // SS_testRead

    _prEFn( " loop return %d " , __i01 ) ;

    _dumpStatus_when_exiting() ;

    return 0 ;
} /* main */

