#include "te01_99_top.h"

void _usage( ___argc , ___argv ) {
} /* _usage */

void _paraAnalyze( int ___argc , char ** ___argv ) {
    if ( ___argc > 2 ) { _FrawPcm           =  ___argv[1] ; }
    if ( ___argc > 3 ) { _FreducePcm        =  ___argv[2] ; }
    if ( ___argc > 4 ) { _FspekerPcm        =  ___argv[3] ; }

    _fGenRawPcm    = new _ssFileIn(  _enSsdIn    , _FrawPcm     , " _fGenRawPcm    : rawPCM    generater " );
    _fGenReducePcm = new _ssFileIn(  _enSsdIn    , _FreducePcm  , " _fGenReducePcm : reducePCM generater " );
    _fSpeaker      = new _ssFileOut( _enSsdOut   , _FspekerPcm  , " _fSpeaker      : pcm for   speaker   " );

} /* _paraAnalyze */

void _initListen(void) {
    _tcpRaw     = new _ssListen1( _enSsdOut 
            , "tcpL1:" _kpListenIP ":" _kpTcp_rawpcm    , " _tcpRaw     : when connected , try to output RAW-pcm " ) ;
    _tcpReduce  = new _ssListen1( _enSsdOut 
            , "tcpL1:" _kpListenIP ":" _kpTcp_filtered  , " _tcpReduce  : when connected , try to output noise-reduce-pcm " ) ;
    _tcpSpeaker = new _ssListen1( _enSsdIn  
            , "tcpL1:" _kpListenIP ":" _kpTcp_speaker   , " _tcpSpeaker : when connected , try to input pcm for speaker" ) ;

    _tcpExit  = new _ssListen1( _enSsdOut 
            , "tcpL1:" _kpListenIP ":" _kpTcp_Exit      , " _tcpExit  : when connected , out debug info , exit. " ) ;

    _tcpSend1  = new _ssTcpConnectTo( _enSsdOut 
            , "tcpT1:127.0.0.1:44444" , " _tcpSend1  : when connected , out debug info . " ) ;

    _tcpRecv1  = new _ssTcpConnectTo( _enSsdIn 
            , "tcpT1:127.0.0.1:55555" , " _tcpRecv1  : when connected , try read data . " ) ;



    _tcpRaw     -> _ssBufSet( 960 , 1 ) ; 
    _tcpReduce  -> _ssBufSet( 960 , 1 ) ; 
    _tcpSpeaker -> _ssBufSet( 960 , 1 ) ; 

    if ( 0 ) {
        _tcpRaw     -> dumpSelfX();
        _tcpReduce  -> dumpSelfX();
        _tcpSpeaker -> dumpSelfX();
        if ( 1 ) _tcpSend1 -> dumpSelfX() ;
        if ( 1 ) _tcpRecv1 -> dumpSelfX() ;
        _prExit( " debuging " );
    }

} /* _initListen */

int main( int ___argc , char ** ___argv ) {

    int __i01 ;

    _usage( ___argc , ___argv ) ;

    _paraAnalyze( ___argc , ___argv ) ;

    _initListen() ;

    __i01 = _timeLoop(
            1000*(_pcmLenRaw)/32
            , &
            _main_loop ) ;

    _prEFn( " loop return %d " , __i01 ) ;

    _dumpStatus_when_exiting() ;

    return 0 ;
} /* main */

