#include "kp01_99_top.h"
#include "superStream.h"
//#include "streamController.h"

#define     _kpListenIP     "127.0.0.1"
#define     _kpTcp_rawpcm   "47811"
#define     _kpTcp_filtered "47813"
#define     _kpTcp_speaker  "47815"

/* 1  ch , 16000 Hz , s16le 
 * 1  sample -> 2 byte 
 * 1  ms -> 16  sample -> 2 * 16  byte == 32  byte 
 * 10 ms -> 160 sample -> 2 * 160 byte == 320 byte 
 * 30 ms -> 480 sample -> 2 * 480 byte == 960 byte 
 */

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

} /* _initListen */

void _testSS(void) {
    char    __buf1024[1024] ;
    int     __len ;

    if ( ! ssOK( _fGenRawPcm ) ) {
        _prExit( " file don't exist. " );
    }

    __len = _fGenRawPcm -> 
        _ssReadNonblock(  9 , __buf1024 ) ;
    if ( __len > 0 ) {
        _prEF( "received <%d> [" , __len ); 
        write( 2 , __buf1024 , __len ) ;
        write( 2 , "]\n" , 2 ) ;
    } else {
        _prEFn( "nothing received <%d> " , __len ); 
    }

    if(0) _prExit( " testing " );
} /* _testSS */

void _fill_data(void) {
    _prExit( " under constructing... " );
} /* _fill_data */

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
            if(1) _prExit( " testing " );
        }

        if ( 1 ) _testSS() ; 
        else     _fill_data() ; 

        __i01 ++ ;
        if ( 0 )    { _sleep_30ms   ; } 
        else        { _sleep_500ms  ; }
    }

    return 0 ;
} /* main */

