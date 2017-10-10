#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
//#include <sys/shm.h>
//#include "ym_hood_wifi.h"
////#include "printku_print.h"
//
//#if defined(THREAD_UART_W2M)
//#include <pthread.h>
//#endif




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
}

#define _write01( aa )      write( _fd_ttyS1 , aa , strlen(aa) ) 
#define _P1( fmt , ... )    fprintf( stdout , fmt , ## __VA_ARGS__ )
#define _P1n( fmt , ... )   _P1( fmt "\n\r\n" , ## __VA_ARGS__ )
#define _P2( fmt , ... )    fprintf( stderr , fmt , ## __VA_ARGS__ )
#define _P2n( fmt , ... )   _P1( fmt "\r\n\n" , ## __VA_ARGS__ )
#define _Pmsg() _P1n ( "    get %d : <%d> 0x%02x , <%c><%s>" , _time1 , _buf1020[0] , _buf1020[0] , _buf1020[0] , _buf1020 );
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

int _time1 = -1 ;
int _time2 = -1 ;
int _fd_ttyS1 ;
char _buf1020[1024] ; 

void _step1_enable_voice(){
    int __rt ;
    _write01( "voice voice_enable\r" ) ;
    __rt = _read_a_line01( _fd_ttyS1 , _buf1020 ) ; _time1 = _time2 ; 
    if ( 1 && __rt > 0 ) {
        _Pmsg();
    }
    usleep( 500000 ) ;
} /* _step1_enable_voice */

void _step2_get_voice_state(){
    int __rt ;
     _write01( "voice get_down\r" ) ;

     __rt = _read_a_line01( _fd_ttyS1 , _buf1020 ) ; _time1 = _time2 ; 
     if ( 1 && __rt > 0 ) {
         if ( 
                 0 == strcmp( _buf1020 , "v down none0" ) 
                 ) {
             if(1) _Pmsg();
         } else {
             _Pmsg();
         }
     }
     usleep( 500000 ) ;
} /* _step2_get_voice_state */

#define _DevNameTTY1 "/dev/ttyS1" 
void _debug01() {

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
        if ( _time1 == -1 || (_time2 - _time1) > 3 ) {
            _step1_enable_voice() ;

            continue ;
        }

        _step2_get_voice_state() ;
    }

} // _debug01() ;


int main(int argc,char** argv)
{

    if ( 1 ) _debug01();

    return 0;
}

