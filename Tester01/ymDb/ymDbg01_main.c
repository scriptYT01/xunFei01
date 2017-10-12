#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include "ymList01.h"


void _exit_and_dump_info01();

int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes */
#if 0
    // blocking read
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;
#else
    // non-blocking read
    tty.c_cc[VMIN] = 0;         // read doesn't block
    tty.c_cc[VTIME] = 1;        // 0.1 seconds read timeout
#endif

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }

    return 0;
} /* set_interface_attribs */

void print_port_speed_info(char *portname, int speed)
{
    switch (speed) {
    case B9600:
        printf("port: %s, speed: B9600\n", portname);
        break;
    case B19200:
        printf("port: %s, speed: B19200\n", portname);
        break;
    case B38400:
        printf("port: %s, speed: B38400\n", portname);
        break;
    case B115200:
        printf("port: %s, speed: B115200\n", portname);
        break;
    default:
        printf("port: %s, speed_t: %d(0x%x)\n", portname, speed, speed);
        break;
    }
} /* print_port_speed_info */

#define _strX( aa ) # aa
#define _write01( aa )      write( _fd_ttyS1 , aa , strlen(aa) ) 
#define _P1( fmt , ... )    fprintf( stdout , fmt , ## __VA_ARGS__ )
#define _P1n( fmt , ... )   _P1( fmt "\n\n" , ## __VA_ARGS__ )
#define _P1d( ddd )         _P1( "%d" , ddd )
#define _P1D(  ddd )         _P1( _strX(ddd) " %d" , ddd )
#define _P1Dn( ddd )         _P1( _strX(ddd) " %d\n" , ddd )
#define _P2( fmt , ... )    fprintf( stderr , fmt , ## __VA_ARGS__ )
#define _P2n( fmt , ... )   _P1( fmt "\n" , ## __VA_ARGS__ )

#define _Pmsg1() _P1n ( "\n" \
        "line %d , time %d " \
        " : <%d> 0x%02x , <%c>" \
        " get <%s>" \
        "     [%s]" \
        " wanted <%s>" \
        , __LINE__ , _time1 \
        , _buf1020[0] , _buf1020[0] , _buf1020[0] , _buf1020 \
        , _listAA[_itemNO] . _fname  \
        , _listAA[_itemNO] . _wanted  \
        );
#define _Pmsg2() _P1n ( "\n" \
        "############## now1 ##############" \
        " seq1 %d,%d, rec1 %d,%d " \
        " get <%s> " \
        "\n" \
        "############## now2 ##############" \
        " itemNO %d , ok %d , not-recognized %d , mistake-recognized %d " \
        " , play awake wav %d , play sentence %d " \
        , _seq1 , _seq2 , _rec1 , _rec2 \
        , _buf1020 \
        , (_itemNO + 1) , _okCNT , _ngCNT , _diCNT \
        , _plCNT0   \
        , _plCNT1   \
        );
#define _diff1() ( 0 != strncmp( _buf1020 , _listAA[_itemNO] . _wanted , 99 ) ) 
#define _Pmsg3() \
    _P1n ( "\n" \
            "diff3 : %d :" \
            "         [%s]" \
            "    get  <%s>" \
            "    want <%s>" \
            , _itemNO \
            , _listAA[_itemNO] . _fname  \
            , _buf1020 \
            , _listAA[_itemNO] . _wanted  \
            )

#define _Pa0()   {_P1("\n") ; fflush( stdout ) ;}
#define _Pa1()   {_P1(".") ; fflush( stdout ) ;}
#define _Pa2()   {_P1("=") ; fflush( stdout ) ;}
#include <signal.h>

volatile int _mainRunning = 1;
void _intHandler1(int ___dummy) { _mainRunning = 0; exit(___dummy); } 
int _read_a_line01( int ___fd , char * ___buf ) {
    int __rt ;
    if ( ___fd < 0 ) return -1 ;
    if ( ___buf == NULL ) return -1 ;
    __rt = read( ___fd , ___buf , 100 ) ;
    if ( __rt > 0 ) {
        ___buf[ __rt ] = 0 ;
        while ( __rt > 0 ) {
            if ( ___buf[ __rt - 1 ] == '\r' || ___buf[ __rt - 1 ] == '\n'  ) {
                ___buf[ __rt - 1 ] = 0 ;
                __rt -- ;
            } else {
                break ;
            }
        }
    }
    return __rt ;
} /* _read_a_line01 */

int         _time0 = -1 ; // prog start time
int         _time1 = -1 ; // last time.
int         _time2 = -1 ; // current time.
int         _fd_ttyS1 ;
int         _argC ;
char    **  _argV ;
char        _buf1020[1024] ; 
int         _active1_inactive0 = 0 ;
int         _itemSize       ;
int         _listAAbyteSize ; // the total size in byte
int         _listAAobjSize  ; // the total list item amount .
int         _testSize       ; // the testing word amount.
_STitemX  * _activeItem     ;
int         _itemNO = 0     ;
int         _seq1  = 0      ; // counter in in-active state
int         _seq2  = 0      ; // counter in in-active state
int         _rec1  = 0      ; // counter in    active state
int         _rec2  = 0      ; // counter in    active state
char        _wavFname0[100]   ; // the awaking wav file name 
char        _playScmd0[200]   ; // the cmd to play awaking wave file
char        _wavFname1[100]   ; // the testing wav file name 
char        _playScmd1[200]   ; // the cmd to play testing wave file
int         _ngCNT      = 0         ;  // ng amount
int       * _ngList     = NULL      ;  // ng List
int         _okCNT      = 0         ;  // ok amount
int       * _okList     = NULL      ;  // ok List
int         _diCNT      = 0         ;  // diff amount
int       * _diList     = NULL      ;  // diff List
int         _plCNT0     = 0         ;  // play Counter total , awake    wav
int         _plCNT1     = 0         ;  // play Counter total , sentence wav
int       * _plList0    = NULL      ;  // play Counter for each item , awake 
int       * _plList1    = NULL      ;  // play Counter for each item , sentence 
_STitemX  * _listAA     = NULL ;

void _genCMD01( char * ___fnameBUF , char * ___systemBUF , int ___itemNO ) {
    snprintf( ___fnameBUF   , 99 , "/vt/VIOMI_test_wav/M2CHN02VM_AAQ0" "%s" ".wav" , _listAA[___itemNO] . _fname ) ;
    snprintf( ___systemBUF  , 199 , "aplay -f S16_LE -r 16000 " "%s" " &> /dev/null",  ___fnameBUF ) ;
} /* _genCMD01 */

void _ItemExit() {
    if ( _itemNO >= _testSize ) {
        _exit_and_dump_info01();
    } 
} /* _ItemExit */

void _ItemFailed()
{
    _ngList[ _ngCNT ] = _itemNO ;
    _ngCNT ++ ;
    _Pmsg2();

    _itemNO ++ ;
    _ItemExit();
    _genCMD01( _wavFname1 , _playScmd1 , _itemNO ) ;
    if(0) sleep( 2 ) ;
} /* _ItemFailed */

void _ItemOk()
{
    _okList[ _okCNT ] = _itemNO ;
    _okCNT ++ ;
    _Pmsg2();

    if( _diff1() ) {
        if(1) _Pmsg3();
        _diList[ _diCNT ] = _itemNO ;
        _diCNT ++ ;
    }

    _itemNO ++ ;
    _ItemExit();
    _genCMD01( _wavFname1 , _playScmd1 , _itemNO ) ;
    if(0)  sleep( 2 ) ;
} /* _ItemOk */

void _initYMdbg() 
{
    _listAAobjSize = _listAAbyteSize / _itemSize ;
    _testSize = _listAAobjSize - 2 ;
    _activeItem = _listAA + _testSize  ;

    _okList  = malloc( _listAAobjSize * sizeof( int ) ) ;
    _ngList  = malloc( _listAAobjSize * sizeof( int ) ) ;
    _diList  = malloc( _listAAobjSize * sizeof( int ) ) ;
    _plList0 = malloc( _listAAobjSize * sizeof( int ) ) ;
    _plList1 = malloc( _listAAobjSize * sizeof( int ) ) ;

    _P1Dn( _itemSize );
    _P1Dn( _listAAobjSize );
    _P1Dn( _testSize );

    _genCMD01( _wavFname0 , _playScmd0 , _testSize ) ;
    if(0) _P1n( " trying <%s>" , _playScmd0 ) ;

} /* _initYMdbg */

void _step1_enable_voice(){
    int __rt ;
    _write01( "voice voice_enable\r" ) ;
    __rt = _read_a_line01( _fd_ttyS1 , _buf1020 ) ; _time1 = _time2 ; 
    if ( 1 && __rt > 0 ) {
        _Pmsg1();
    }
    usleep( 500000 ) ;
} /* _step1_enable_voice */

void _dumpExtDebugInfo01()
{
    int __i01 ;
    for ( __i01 = 0 ; __i01 < _testSize ; __i01 ++ ) {
        if ( _plList0[__i01] > 1 || _plList1[__i01] > 1 ) {
            _P1n( "debug11 : [%s] : awake %d , try %d " 
                    , _listAA[__i01] . _fname 
                    , _plList0[__i01]
                    , _plList1[__i01]
                    ) ;
        }
    }

    //_plList0 = malloc( _listAAobjSize * sizeof( int ) ) ;
    //_plList1 = malloc( _listAAobjSize * sizeof( int ) ) ;
} /* _dumpExtDebugInfo01 */

void _exit_and_dump_info01()
{
    _Pmsg2();
    _time2 = time(0) ;

    _P1n ( "\n\n\n" 
            "###### total tested %d :  ok %d , not-recgonized %d , mistake-recgonized %d." 
            "  %2.1f%%"
            "  %2.1f%%"
            "  %2.1f%%"
            , _testSize , _okCNT , _ngCNT , _diCNT 
            , ( 100.0 * _okCNT / _testSize )
            , ( 100.0 * _ngCNT / _testSize )
            , ( 100.0 * _diCNT / _testSize )
            ) ;
    _P1n ( "######  play awake wav %d , play sentence %d "
            , _plCNT0
            , _plCNT1
         );
    _P1n ( "######  start %d , stop %d , used : %d" "\n\n" , _time0 , _time2 , _time2 - _time0 ) ;

    if ( 1 ) { _dumpExtDebugInfo01() ; }

    exit( 33 ) ;
} /* _exit_and_dump_info01 */

#define _argDebug11()    if ( _argC >= 2 && 0 == strncmp( "1" , _argV[1] , 99 ) ) { _Pmsg1() ; return ; }
#define _argDebug12()    if ( _argC >= 2 && 0 == strncmp( "1" , _argV[1] , 99 ) ) { _Pa1()   ; return ; }
#define _argDebug13()    if ( _argC >= 2 && 0 == strncmp( "1" , _argV[1] , 99 ) ) { _Pa2()   ; return ; }
void _result_analyze1_inactive()
{
    _argDebug12();

    if(0) _Pmsg1();
    if(1) _Pa1();
    if ( _active1_inactive0 != 0 ) {
        //_seq1 = 0 ;
        //_seq2 = 0 ;
        if ( _seq1 != 0 ) {
            _seq1 ++ ;
        }
        _seq2 = 0 ;
    }
    _active1_inactive0 = 0 ;
    if ( _seq1 == 0 ) { // first , inactive , pring header.
        _genCMD01( _wavFname1 , _playScmd1 , _itemNO ) ;
        _P1n( " trying %d : %s , wanted <%s> , _seq1 %d _seq2 %d " , _itemNO , _wavFname1 , _listAA[_itemNO] . _wanted , _seq1 , _seq2 ) ;
        _seq1 = 1 ;
        _seq2 = 0 ;
    } else if ( _seq1 >= 1 && _seq1 <= 3 ) { // play awake wav.  
        if ( _seq2 == 0 ) {
            if(0) _P1n( " trying <%s>" , _playScmd0 ) ;
            if(0) _P1n( " trying <%s>" , _playScmd1 ) ;
            if(1) _P1n( "->-AWAKE-<-" );
            if(1) { system( _playScmd0 ); _plList0[_itemNO] ++ ; _plCNT0 ++ ; } 
            _time1 = _time2 ;
        }
        _seq2 ++ ;
        if ( _seq2 > 6 ) {
            _seq1 ++ ;
            _seq2 = 0 ;
        }
    } else {
        if(1) _P1n( "\n NG %d : %s , wanted <%s> " , _itemNO , _wavFname1 , _listAA[_itemNO] . _wanted ) ;
        _ItemFailed();

        _seq1 = 0 ;
        _seq2 = 0 ;
    }
} /* _result_analyze1_inactive */

void _result_analyze2_active()
{
    _argDebug13();
    if ( _active1_inactive0 == 0 ) {
        _rec1 = 0 ;
        _rec2 = 0 ;
    }

    if ( _rec2 == 0 ) {
        if(1) _P1n( "\n#>#%s#<#" , _wavFname1 );
        if(1) { system( _playScmd1 ); _plList1[_itemNO] ++ ;  _plCNT1 ++ ; } ;
        _time1 = _time2 ;
        _rec2 ++ ;
    } else if ( _rec2 >= 6 ) {
        _rec2 = 0 ;
        if ( _rec1 > 3 ) {
            _rec1 = 0 ;
           _ItemFailed();
        } else {
            _rec1 ++ ;
        }
    } else {
        _rec2 ++ ;
    }

    _active1_inactive0 = 1 ;
} /* _result_analyze2_active */

void _result_analyze3_other()
{
    _argDebug11();
    _active1_inactive0 = 1 ;
    if(1) _Pmsg1();
    _ItemOk();
    _rec1 = 0 ;
    _rec2 = 0 ;
} /* _result_analyze3_other */

void _step2_get_voice_state(){
    int __rt ;
     _write01( "voice get_down\r" ) ;

     __rt = _read_a_line01( _fd_ttyS1 , _buf1020 ) ; _time1 = _time2 ; 
     if ( 1 && __rt > 0 ) {
         if ( 0 == strcmp( _buf1020 , "v down none0" ) ) { /* inactive  */
             _result_analyze1_inactive();
         } else if ( 0 == strcmp( _buf1020 , "v down none1" ) ) { /* active */
             _result_analyze2_active();
         } else if ( 0 == strcmp( _buf1020 , "ok" ) ) { /* ok1 : do nothing . */
         } else {
             _result_analyze3_other();
         }
     }
     usleep( 500000 ) ;
} /* _step2_get_voice_state */

#define _DevNameTTY1 "/dev/ttyS1" 
void _runYMdbg() {

    signal(SIGINT, _intHandler1) ; /* exit at once when ctrl+c */

    //system( " amixer sset TITANIUM  70%                     &> /dev/null " ) ;
    //system( " amixer sset MERCURY   70%                     &> /dev/null " ) ;
    system( " lsof  |grep snd |xargs -n 1 kill -9           &> /dev/null " );
    system( " lsof  |grep ttyS1 |xargs -n 1 kill -9         &> /dev/null " );
    system( " killall -9 miio_wifi                          &> /dev/null " );
    system( " killall -9 pvalg_ymhood                       &> /dev/null " );

    _fd_ttyS1 = open( _DevNameTTY1, O_RDWR | O_NOCTTY | O_SYNC);
    if (_fd_ttyS1 < 0) {
        _P2n( "Error opening %s: %s",  _DevNameTTY1, strerror(errno));
        return ;
    }

    set_interface_attribs(_fd_ttyS1, B115200 );
    print_port_speed_info( _DevNameTTY1, B115200 );

    tcdrain(_fd_ttyS1);    /* delay for output */

    
    while ( 1 ) {
        _time2 = time(0) ;
        //if ( _time1 == -1 || (_time2 - _time1) > 3 ) {
        if ( _time1 == -1 ) {
            _step1_enable_voice() ;

            continue ;
        }

        _step2_get_voice_state() ;
    }

} // _runYMdbg() ;

void _argDebugCMDline()    { 
    int __i01 ;

    _P1n( "argc : %d" , _argC ); 

    for ( __i01 = 0 ; __i01 < _argC ; __i01 ++ ) { 
        _P1n( "argV %d: len <%d> , content <%s>" 
                , __i01 , strlen(_argV[__i01]) , _argV[__i01] ); 
    } 

} /* _argDebugCMDline */

void _showUsageExit()
{
} /* _showUsageExit */

void _paraAnalyzeYmDbg(int ___argc,char** ___argv);
void _paraAnalyzeYmDbg(int ___argc,char** ___argv)
{
    _listAA             = _listA1 ;
    _listAAbyteSize     = sizeof( _listA1 ) ;
    _itemSize           = sizeof( _STitemX ) ;
} /* _paraAnalyzeYmDbg */

int main(int ___argc,char** ___argv)
{

    _argC = ___argc ;
    _argV = ___argv ;
    _time0 = time(0) ;

    _argDebugCMDline();

    _paraAnalyzeYmDbg();

    _initYMdbg();

    _runYMdbg();

    return 0;
}

