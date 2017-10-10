#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/shm.h>
#include "ym_hood_wifi.h"
//#include "printku_print.h"

#if defined(THREAD_UART_W2M)
#include <pthread.h>
#endif


static u32 dbg_mask2 = (DEBUG_BIT_ERROR | DEBUG_BIT_INFO | DEBUG_BIT_VR_CODE | DEBUG_BIT_UART);
#define DBGB_PRINTF(level_mask, fmt, arg...)    if (dbg_mask2 & level_mask) \
        printf(fmt, ## arg)

static void *shm_at_vrcode;
#if defined(MIIO_WIFI_COWORK)
static void *shm_at_vrmiio;
#endif

static BOOL waiting_vm_res_f = FALSE;  // TRUE if we are waiting for the response from Voice module.
static BOOL waiting_micom_res_f = FALSE;  // TRUE if we are waiting for the response from MICOM.
static word code_v2m_last = 0, val_v2m_last = 0;

char rbuf_micom[CMD_RX_PKT_SIZE];  // RX buf from MICOM
char rbuf_wifi[CMD_RX_PKT_SIZE];  // RX buf from WIFI module

#if defined(THREAD_UART_W2M)
extern void *thread_uart_pass(void *arg);
#endif

#if defined(EMULATE_HOOD_MCU)
#define CHK_INTERVAL_VRCMD          100  // unit: ms
#endif //EMULATE_HOOD_MCU

#define SLEEP_USECONDS              3000  // unit: us


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
}


/*
RETURN value: 
     0 : A new code is detected.
    -1 : No code is available.
*/
int get_vrcode(struct shared_vrcode_st *vrcode_st, word *code, word *val)
{
    if (vrcode_st->new_v2m_f) {
        *code = vrcode_st->code_v2m;
        *val = vrcode_st->val_v2m;
        vrcode_st->new_v2m_f = FALSE;
        DBGB_PRINTF(DEBUG_BIT_VR_CODE, "get_vrcode(): cmd=0x%04x, val=0x%04x\n", *code, *val);
        //printku_print("get_vrcode(): cmd=0x%04x, val=0x%04x\n", *code, *val, 0);
        return 0;
    }

    return -1;
}


//
// code: recognized code from voice module
// return NULL: The code is not found in the table(codelist).
//
char *get_str_from_code(code_str_type *codelist, word code)
{
    int idx;

    for (idx = 0; ; idx++) {
        if (!codelist[idx].code) {
            break;
        }

        if (code == codelist[idx].code) {
            return (char *)codelist[idx].cmdstr;
        }
    }

    return NULL;
}


#if defined(TEST_ONLY_OPPLE_DEMO1)
int get_rbytes_from_code(code_resp_bytes_type *codebytes, word code, byte **buf_pptr)
{
    int idx;

    for (idx = 0; ; idx++) {
        if (!codebytes[idx].code) {
            break;
        }

        if (code == codebytes[idx].code) {
            *buf_pptr = codebytes[idx].rbytes;
            //printf("buf[]: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X\n", *(*buf_pptr + 0), *(*buf_pptr + 1), *(*buf_pptr + 2), *(*buf_pptr + 3), *(*buf_pptr + 4));
            return codebytes[idx].cnt;
        }
    }

    *buf_pptr = NULL;
    return 0;  // cnt
}
#endif //TEST_ONLY_OPPLE_DEMO1


// 
// buf: cmd string from MCU
// slen: length to be compared. <= strlen(buf)
// return 0: The match-string is not found in the table(codelist).
//
int get_code_from_str(code_str_type *codelist, char *buf, int slen)
{
    int code, idx;

    for (idx = 0; ; idx++) {
        if (codelist[idx].cmdstr == NULL) {
            break;
        }

        if (!strncmp(codelist[idx].cmdstr, buf, slen)) {
            return codelist[idx].code;
        }
    }

    return 0;
}


/*
RETURN value: 
     0 : A command is transferred to MICOM.
    -1 : A command is NOT transferred correctly.
*/
int handle_vrecog_cmd(int fd, word code, word val)
{
    char cmdarray[CMD_TX_PKT_SIZE];
    char numstr[6]; // MAX: 0xFFFF=65535
    char *cmdstr;
    int wlen, slen;
#if defined(TEST_ONLY_OPPLE_DEMO1)
    byte *rbytes_ptr;
#endif

	DBGB_PRINTF(DEBUG_BIT_UART, "handle_vrecog_cmd(): 0x%04x 0x%04x\n", code, val);

#if defined(TEST_ONLY_OPPLE_DEMO1)
    slen = get_rbytes_from_code(codelist_rbytes, code, &rbytes_ptr);
    if (!slen) {
        return -1;
    }

    memcpy(cmdarray, rbytes_ptr, slen);
    {
        unsigned char   *p;
        int idx = slen;
        printf("RESP(OPPLE) %d:", slen);
        for (p = cmdarray; idx-- > 0; p++)
            printf(" 0x%02X", *p);
        printf("\n");
    }

#else
    cmdstr = get_str_from_code(codelist_v2m, code);
    if (!cmdstr) {  // NULL
        return -1;
    }

    strcpy(cmdarray, cmdstr);
    if (code == X2M_TIMER_SET) {
        DBGB_PRINTF(DEBUG_BIT_UART, "TIMER_SET: value=%d\n", val);
        sprintf(numstr, "%d", val);
        strcat(cmdarray, numstr);
        strcat(cmdarray, "\r");
    }

    slen = strlen(cmdarray);
    DBGB_PRINTF(DEBUG_BIT_UART, "X2M(%d):%s\n", slen, cmdarray);
#endif //TEST_ONLY_OPPLE_DEMO1

    wlen = write(fd, cmdarray, slen);
    if (wlen != slen) {
        DBGB_PRINTF(DEBUG_BIT_ERROR, "write() error! wlen=%d\n", wlen);
        return -1;
    }

    return 0;
}








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

#define _write01( aa )      write( __fd_ttyS1 , aa , strlen(aa) ) 
#define _P1( fmt , ... )    fprintf( stdout , fmt , ## __VA_ARGS__ )
#define _P1n( fmt , ... )   _P1( fmt "\r\n" , ## __VA_ARGS__ )
#define _P2( fmt , ... )    fprintf( stderr , fmt , ## __VA_ARGS__ )
#define _P2n( fmt , ... )   _P1( fmt "\r\n\n" , ## __VA_ARGS__ )
#define _Pmsg() _P1n ( "    get %d : <%d> 0x%02x , <%c><%s>" , (int) time(0) , __buf1020[0] , __buf1020[0] , __buf1020[0] , __buf1020 );
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

void _debug01() {
    int __fd_ttyS1 ;
    int __rt ;
    char __buf1020[1024] ; 

    signal(SIGINT, _intHandler1) ; /* exit at once when ctrl+c */

    //system( " amixer sset TITANIUM  70%                     &> /dev/null " ) ;
    //system( " amixer sset MERCURY   70%                     &> /dev/null " ) ;
    system( " lsof  |grep snd |xargs -n 1 kill -9           &> /dev/null " );
    system( " lsof  |grep ttyS1 |xargs -n 1 kill -9         &> /dev/null " );
    system( " killall -9 miio_wifi                          &> /dev/null " );
    system( " killall -9 pvalg_ymhood                       &> /dev/null " );

    __fd_ttyS1 = open(COMM_MCU_DEV_NAME, O_RDWR | O_NOCTTY | O_SYNC);
    if (__fd_ttyS1 < 0) {
        _P2n( "Error opening %s: %s", COMM_MCU_DEV_NAME, strerror(errno));
        return -1;
    }

    set_interface_attribs(__fd_ttyS1, COMM_BAUD_RATE_MCU);
    print_port_speed_info(COMM_MCU_DEV_NAME, COMM_BAUD_RATE_MCU);

    tcdrain(__fd_ttyS1);    /* delay for output */

    _write01( "voice voice_enable\r" ) ;
    __rt = _read_a_line01( __fd_ttyS1 , __buf1020 ) ;
    if ( 1 && __rt > 0 ) {
        _Pmsg();
    }
    
    while ( 1 ) {
        __rt = _read_a_line01( __fd_ttyS1 , __buf1020 ) ;
        if ( 1 && __rt > 0 ) {
            if ( 
                    0 == strcmp( __buf1020 , "v down none0" ) 
               ) {
                if(1) _Pmsg();
            } else {
                _Pmsg();
            }
        }
        usleep( 500000 ) ;
        _write01( "voice get_down\r" ) ;
    }

} // _debug01() ;


int main(int argc,char** argv)
{
    int tty_fd_mcu;
    int tty_fd_wifi;
    int wlen;
    int idx_tx_cmd;
    struct shared_vrcode_st *shm_vrcode_st;
    int shmid_vrcode;
    unsigned long cnt_loop;
    int ok_loop;
    m2x_cmd_result_type next_step;
#if defined(THREAD_UART_W2M)
    int ret_th1;
    void *result_th1;
    pthread_t uart_thread1;
    fd_buf_type fd_buf_pass1;
#endif //THREAD_UART_W2M

#if defined(MIIO_WIFI_COWORK)
    struct shared_vrmiio_st *shm_vrmiio_st;
    int shmid_vrmiio;
#endif //MIIO_WIFI_COWORK

    if ( 1 ) _debug01();

    return 0;
}

