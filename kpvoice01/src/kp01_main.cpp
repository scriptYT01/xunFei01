#include "kp01_99_top.h"

#define     _kpListenIP     "127.0.0.1"
#define     _kpTcp_rawpcm   "47811"
#define     _kpTcp_filtered "47813"
#define     _kpTcp_speaker  "47815"

int32_t _time1 = _timeNow ;
int32_t _time2  ;
int32_t _time3  ;

const char * _FrawPcm     = "/tmp/t.in.pcmRaw.txt" ;
const char * _FreducePcm  = "/tmp/t.in.pcmReduce.txt" ;
const char * _FspekerPcm  = "/tmp/t.in.speker.txt" ;

_ssFileIn  * _fGenRawPcm        = NULL ;
_ssFileIn  * _fGenReducePcm     = NULL ;
_ssFileOut * _fSpeaker          = NULL ;

_ssListen1 * _tcpRaw            = NULL ;
_ssListen1 * _tcpReduce         = NULL ;
_ssListen1 * _tcpSpeaker        = NULL ;

void _usage( int ___argc , char ** ___argv ) {
} /* _usage */

void _paraAnalyze( int ___argc , char ** ___argv ) {
    if ( ___argc > 2 ) { _FrawPcm           =  ___argv[1] ; }
    if ( ___argc > 3 ) { _FreducePcm        =  ___argv[2] ; }
    if ( ___argc > 4 ) { _FspekerPcm        =  ___argv[3] ; }

    _fGenRawPcm    = new _ssFileIn(  _enSsdIn    , _FrawPcm     , " rawPCM    generater " );
    _fGenReducePcm = new _ssFileIn(  _enSsdIn    , _FreducePcm  , " reducePCM generater " );
    _fSpeaker      = new _ssFileOut( _enSsdOut   , _FspekerPcm  , " pcm for   speaker   " );

} /* _paraAnalyze */

void _initListen(void) {
    _tcpRaw     = new _ssListen1( _enSsdOut 
            , "tcpL1:" _kpListenIP ":" _kpTcp_rawpcm   , " when connected , try to output RAW-pcm " ) ;
    _tcpReduce  = new _ssListen1( _enSsdOut 
            , "tcpL1:" _kpListenIP ":" _kpTcp_filtered , " when connected , try to output noise-reduce-pcm " ) ;
    _tcpSpeaker = new _ssListen1( _enSsdIn  
            , "tcpL1:" _kpListenIP ":" _kpTcp_speaker  , " when connected , try to input pcm for speaker" ) ;

    _tcpRaw     -> _ssBufSet( 960 , 1 ) ; 
    _tcpReduce  -> _ssBufSet( 960 , 1 ) ; 
    _tcpSpeaker -> _ssBufSet( 960 , 1 ) ; 
} /* _initListen */

int main( int ___argc , char ** ___argv ) {

    int __i01 = 1 ;

    _usage( ___argc , ___argv ) ;

    _paraAnalyze( ___argc , ___argv ) ;

    _initListen() ;

    while ( 1 ) {

        if ( __i01 % 33 == 32 ) {
            _time2 = _timeNow ;
            _time3 = _time2 - _time1 ;
            _prSFn( " --- %10d %10d , %10d " , _time2 , _time3 , __i01 ) ;
            _ffstdout ;
            if(1) _fGenRawPcm -> _ssDumpSelf();
            if(1) _prExit( " testing " );
        }

        if(0)   _testSS() ; 

        // _superStreamBase 
        // _canWrite
        // _ssOpenOrReopen
        if(1)   _testTL1() ; 

        if(0)   _fill_data() ; 

        __i01 ++ ;
        if ( 0 )    { _sleep_30ms   ; } 
        else        { _sleep_500ms  ; }
    }

    return 0 ;
} /* main */

