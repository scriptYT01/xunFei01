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
const char * _FspekerPcm  = "/tmp/t.out.speker.txt" ;

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

    _fGenRawPcm    = new _ssFileIn(  _enSsdIn    , _FrawPcm     , " _fGenRawPcm    : rawPCM    generater " );
    _fGenReducePcm = new _ssFileIn(  _enSsdIn    , _FreducePcm  , " _fGenReducePcm : reducePCM generater " );
    _fSpeaker      = new _ssFileOut( _enSsdOut   , _FspekerPcm  , " _fSpeaker      : pcm for   speaker   " );

} /* _paraAnalyze */

void _initListen(void) {
    _tcpRaw     = new _ssListen1( _enSsdOut 
            , "tcpL1:" _kpListenIP ":" _kpTcp_rawpcm   , " _tcpRaw     : when connected , try to output RAW-pcm " ) ;
    _tcpReduce  = new _ssListen1( _enSsdOut 
            , "tcpL1:" _kpListenIP ":" _kpTcp_filtered , " _tcpReduce  : when connected , try to output noise-reduce-pcm " ) ;
    _tcpSpeaker = new _ssListen1( _enSsdIn  
            , "tcpL1:" _kpListenIP ":" _kpTcp_speaker  , " _tcpSpeaker : when connected , try to input pcm for speaker" ) ;

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
    if(1) 
        _fGenRawPcm -> dumpSelfX();
    if(1) 
        _tcpRaw -> dumpSelfX();
    if(1) 
        _fGenRawPcm   -> dumpSelfX();
}

bool _main_loop() {
    int __i01 = 1 ;

    if ( __i01 % 165 == 164 ) {
        if ( 1 ) { // gen debug info
            _time2 = _timeNow ;
            _time3 = _time2 - _time1 ;
            _prSFn( " --- %10d %10d , %10d " , _time2 , _time3 , __i01 ) ;
            _ffstdout ;
            if( 0 ) return false ; // exit....
        }
    }

    if(0)   
        _testSS() ; 


    if(1)   
        _fill_data() ; 

    __i01 ++ ;
    //if ( 1 )    { _sleep_33ms   ; } 
    //if ( 1 )    { usleep( 1000*1000*960/32000 )  ; } 
    //if ( 1 )    { _sleep_30ms   ; } 
    //if ( 1 )    { usleep(28000)   ; } 
    //if ( 1 )    { usleep( 1000*1000*(_pcmLenRaw)/32000 )  ; } 
    //if ( 1 )    { usleep( 29000 )  ; } 
    return true ;
} /* _main_loop */

int main( int ___argc , char ** ___argv ) {

    int __i03 ;
    uint64_t __u641 ;
    uint64_t __u642 ;
    uint64_t __u643 ;
    uint64_t __u644 ;
    int64_t __u1 ;
    int64_t __u2 ;
    int64_t __u3 ;
    int64_t __u4 ;
    int64_t __u5 ;
    int64_t __X1 ;

    _usage( ___argc , ___argv ) ;

    _paraAnalyze( ___argc , ___argv ) ;

    _initListen() ;

    __u641  =   _u64_now() ;
    __X1   = 1000*(_pcmLenRaw)/32 ;
    __u642  =   __u641 - __X1   ;
    while ( 1 ) {
        __u643  =   __u642     ; 
        __u642  =   _u64_now() ;
        if ( false == _main_loop() ) break ;
        _prEFn( " 1: %lld , 2: %lld , 3: %lld , 4: %lld , 5: %lld , X1: %lld " ,
                __u1 , __u2 , __u3 , __u4 , __u5 , __X1 );
        __u644  =   _u64_now() ;



        __u1  =   __u642 - __u643 ; // gap-time
        __u2  =   __u644 - __u642 ; // run-time
        if ( __u2 > __X1 ) {
            _prExit( " delay over-run " ) ;
        }
        if ( __u1 > (__X1 << 1)) {
            _prExit( " why gap over-run " ) ;
        }

        __u3 = __X1 - __u2 ; // sleep-time-base
        __u4 = __X1 - __u1 ; // jiffer
        __u5 = __u3 + __u4 ;

        __i03 = __u5 ;
        usleep( __i03 ) ;
    }


    return 0 ;
} /* main */

