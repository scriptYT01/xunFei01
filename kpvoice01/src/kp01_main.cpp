#include "kp01_99_top.h"

#define     _kpListenIP     "127.0.0.1"
#define     _kpTcp_rawpcm   "47811"
#define     _kpTcp_filtered "47813"
#define     _kpTcp_speaker  "47815"
#define     _kpTcp_Exit     "47819"

int32_t _time1 = _timeNow ;
int32_t _time2  ;
int32_t _time3  ;

const char * _FrawPcm     = "/tmp/t.in.pcmRaw.txt" ;
const char * _FreducePcm  = "/tmp/t.in.pcmReduce.txt" ;
const char * _FspekerPcm  = "/tmp/t.out.speker.txt" ;

_ssFileIn  * _fGenRawPcm        = NULL ;
_ssFileIn  * _fGenReducePcm     = NULL ;
_ssFileOut * _fSpeaker          = NULL ;

_ssListen1 * _tcpRaw            = NULL ;
_ssListen1 * _tcpReduce         = NULL ;
_ssListen1 * _tcpSpeaker        = NULL ;

_ssListen1      * _tcpExit      = NULL ;
_ssTcpConnectTo * _tcpSend1     = NULL ;

void _usage( int ___argc , char ** ___argv ) {
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

    _tcpRaw     -> _ssBufSet( 960 , 1 ) ; 
    _tcpReduce  -> _ssBufSet( 960 , 1 ) ; 
    _tcpSpeaker -> _ssBufSet( 960 , 1 ) ; 

    if ( 0 ) {
        _tcpRaw     -> dumpSelfX();
        _tcpReduce  -> dumpSelfX();
        _tcpSpeaker -> dumpSelfX();
        _prExit( " debuging " );
    }

} /* _initListen */

void _dumpStatus_when_exiting(){
    if(1 && _fGenRawPcm     )   _fGenRawPcm         -> dumpSelfX();
    if(1 && _fGenReducePcm  )   _fGenReducePcm      -> dumpSelfX();
    if(1 && _fSpeaker       )   _fSpeaker           -> dumpSelfX();

    if(1 && _tcpRaw            )      _tcpRaw            -> dumpSelfX();
    if(1 && _tcpReduce         )      _tcpReduce         -> dumpSelfX();
    if(1 && _tcpSpeaker        )      _tcpSpeaker        -> dumpSelfX();

    if(1 && _tcpExit           )      _tcpExit           -> dumpSelfX();
} /* _dumpStatus_when_exiting */

int _loopCNT = 1 ;
int _main_loop() {

    //int __running = 0 ;

    if(1)    { // run debuging
        if ( _tcpExit -> _canWrite( true ) ) {
            _prEFn( " debug port conncted. . --- %10d %10d , %10d " , _time2 , _time3 , _loopCNT ) ;
            _testWW() ; 
            // _testW1
            // _testW2
            return -1 ;
        }
    }

    if ( _loopCNT % 165 == 164 ) {
        if ( 1 ) { // gen debug info
            _time2 = _timeNow ;
            _time3 = _time2 - _time1 ;
            _prSFn( " --- %10d %10d , %10d " , _time2 , _time3 , _loopCNT ) ;
            _ffstdout ;
            if( 0 ) return -2 ; // exit....
        }
        if ( 1 ) {
            if ( _tcpExit -> _canWrite( true ) ) {
                return -3 ;
            }
        }
    }

    if(0)   
        _testSS() ; 


    if(1)   
        _fill_data() ; 

    _loopCNT ++ ;
    //if ( 1 )    { _sleep_33ms   ; } 
    //if ( 1 )    { usleep( 1000*1000*960/32000 )  ; } 
    //if ( 1 )    { _sleep_30ms   ; } 
    //if ( 1 )    { usleep(28000)   ; } 
    //if ( 1 )    { usleep( 1000*1000*(_pcmLenRaw)/32000 )  ; } 
    //if ( 1 )    { usleep( 29000 )  ; } 
    return 1 ;
} /* _main_loop */

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

