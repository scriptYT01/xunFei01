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

void set_mincount(int fd, int should_block)
{
    struct termios tty;

    memset (&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) < 0) {
        printf("Error tcgetattr: %s\n", strerror(errno));
        return;
    }

    /*
    ❑ MIN = 0 and TIME > 0: In this case, the read will return when any character is available to be
    read or when TIME tenths of a second have elapsed. If no character was read because the timer
    expired, read will return zero. Otherwise, it will return the number of characters read.
    ❑ MIN > 0 and TIME = 0: In this case, the read will wait until MIN characters can be read and
    then return that number of characters. Zero is returned on end of file.
    */
    tty.c_cc[VMIN] = should_block ? 1 : 0;
    tty.c_cc[VTIME] = 1;        /* 0.1 second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
        printf("Error tcsetattr: %s\n", strerror(errno));
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


/*
RETURN value: 
     M2X_CMD_DONE : Processed here. (No more process is required.)
     M2X_CMD_CURR_TIME : "current time" is received, needs further processing.
     M2X_CMD_FOR_WIFI : For WIFI module. command through-pass.
*/
m2x_cmd_result_type handle_micom_cmd(int fd, struct shared_vrcode_st *vrcode_st, char *buf)
{
    word code, val;
    m2x_cmd_result_type retval = M2X_CMD_DONE;
    int slen;

	DBGB_PRINTF(DEBUG_BIT_UART, "handle_micom_cmd()\n");

    // First, check if "wifi " or "voice " is at the beginning position of buf[].
    if (!strncmp(buf, codelist_m2v[0].cmdstr, M2X_DEST_LOCAL_SLEN)) {
        //DBGB_PRINTF(DEBUG_BIT_INFO, "=> cmd for X1000 module!\n");
        buf += M2X_DEST_LOCAL_SLEN;
    } else if (!strncmp(buf, codelist_m2v[1].cmdstr, M2X_DEST_WIFI_SLEN)) {
        //DBGB_PRINTF(DEBUG_BIT_INFO, "=> cmd for WIFI module!\n");
#if defined(MIIO_WIFI_COWORK)
        DBGB_PRINTF(DEBUG_BIT_ERROR, "*** \"wifi \" should be processed in miio client!!!\n");
        return M2X_CMD_DONE;
#else
        return M2X_CMD_FOR_WIFI;
#endif //MIIO_WIFI_COWORK
    } else {
        DBGB_PRINTF(DEBUG_BIT_ERROR, "=> voice or wifi? No info!\n");
        return M2X_CMD_DONE;
    }

    // "voice " command
    slen = strlen(buf);
    code = get_code_from_str(codelist_m2v, buf, slen);
	DBGB_PRINTF(DEBUG_BIT_UART, "TO-X1000: code=0x%04X\n", code);
    if (!code) {
        // Check if the string is "result 23,59\r" format.
        if (code_v2m_last == X2M_CURR_TIME) {
            code = get_code_from_str(codelist_m2v, buf, M2X_RESULT_TIME_SLEN);
        } else { // Check if it is "ment N".
            code = get_code_from_str(codelist_m2v, buf, M2X_MENT_INDEX_SLEN);
        }

        if (code) {
            DBGB_PRINTF(DEBUG_BIT_UART, "TO-X1000: code => 0x%04X\n", code);
        } else {
            return M2X_CMD_DONE;  // Nothing is matched.
		}
    }

    switch (code) {
    case M2X_GET_DOWN:
        if (get_vrcode(vrcode_st, &code_v2m_last, &val_v2m_last)) {
            // Send "down none\r"
            handle_vrecog_cmd(fd, X2M_DOWN_NONE, 0);
            waiting_micom_res_f = FALSE;
        } else {
            handle_vrecog_cmd(fd, code_v2m_last, val_v2m_last);
            waiting_micom_res_f = TRUE;
        }
        vrcode_st->processed_v2m_f = TRUE;
        retval = M2X_CMD_DONE;
        break;

    case M2X_RESULT_OK:
        waiting_micom_res_f = FALSE;
        // No ment play is needed.
        //vrcode_st->code_m2v = code;
        //vrcode_st->val_m2v = 0;
        //vrcode_st->processed_m2v_f = FALSE;
        //vrcode_st->emul_mcu_f = FALSE;
        //vrcode_st->new_m2v_f = TRUE; 
        retval = M2X_CMD_DONE;
        break;

    case M2X_RESULT_TIME:
        waiting_micom_res_f = FALSE;
        {
            int hours, minutes;

            sscanf(&buf[M2X_RESULT_TIME_SLEN], "%d,%d", &hours, &minutes);
            DBGB_PRINTF(DEBUG_BIT_UART, "CURR TIME: %dH %dM\n", hours, minutes);

            // Audio play in voice module
            vrcode_st->code_m2v = code;
            vrcode_st->val_m2v = (hours << 8 | minutes);
            vrcode_st->processed_m2v_f = FALSE;
            vrcode_st->emul_mcu_f = FALSE;
            vrcode_st->new_m2v_f = TRUE; 
            retval = M2X_CMD_CURR_TIME;
        }
        break;

    case M2X_MENT_INDEX:
        waiting_micom_res_f = FALSE;
        {
            int ment_index;

            sscanf(&buf[M2X_MENT_INDEX_SLEN], "%d", &ment_index);
            DBGB_PRINTF(DEBUG_BIT_UART, "MENT INDEX: %d\n", ment_index);

            // Ment play in voice module
            vrcode_st->code_m2v = code;
            vrcode_st->val_m2v = ment_index;
            vrcode_st->processed_m2v_f = FALSE;
            vrcode_st->emul_mcu_f = FALSE;
            vrcode_st->new_m2v_f = TRUE; 
            retval = M2X_CMD_MENT_INDEX;
        }
        break;
    } // switch

    return retval;
}


void close_shm(void)
{
    if (shm_at_vrcode != (void *)-1) {
        if (shmdt(shm_at_vrcode) == -1) {
            DBGB_PRINTF(DEBUG_BIT_ERROR, "shmdt failed.\n");
        }
    }
}


#if defined(THREAD_UART_W2M)
void *thread_uart_pass(void *arg)
{
    fd_buf_type *fd_buf_ptr;
    int fdin, fdout;
    byte *buf;
    int rdlen, wrlen;

    fd_buf_ptr = (fd_buf_type *)arg;
    fdin = fd_buf_ptr->fd_in;
    fdout = fd_buf_ptr->fd_out;
    buf = fd_buf_ptr->buf;
	DBGB_PRINTF(DEBUG_BIT_INFO, "thread_uart_pass()\n");

    while (1) {
        rdlen = read(fdin, buf, CMD_RX_PKT_SIZE);
        if (rdlen > 0) {
            buf[rdlen] = 0;
            printf("T:R(%d): %s\n", rdlen, buf);

            // DATA through-pass
            wrlen = write(fdout, buf, rdlen);
            if (wrlen != rdlen) {
                DBGB_PRINTF(DEBUG_BIT_ERROR, "T:write() error! wrlen=%d\n", wrlen);
            }
        } else if (rdlen < 0) {
            DBGB_PRINTF(DEBUG_BIT_ERROR, "\nT:Read error: rdlen=%d: %s\n", rdlen, strerror(errno));
            pthread_exit("thread_uart_pass()-FAIL");
        } else {
            //printf("T"); fflush(stdout);
            usleep(10000);  // Let others run...
        }
    } // while (1)

    pthread_exit("thread_uart_pass()-done");
}
#endif //THREAD_UART_W2M


#if defined(MIIO_WIFI_COWORK)
/*
RETURN value: 
    > 0 : command string length (A new command from miio_wifi)
    <= 0 : No command from miio_wifi
*/
int get_miio_cmd(struct shared_vrmiio_st *vrmiio_st, char *rbuf_miio, int size_max)
{
    int slen;

    if (vrmiio_st->new_miiocmd_f) {
        strncpy(rbuf_miio, vrmiio_st->cmdbuf, size_max);
        slen = strlen(rbuf_miio);
        vrmiio_st->new_miiocmd_f = FALSE;
        DBGB_PRINTF(DEBUG_BIT_VR_CODE, "get_miio_cmd(): cmd=(%d)%s\n", slen, rbuf_miio);
        return slen;
    }

    return -1;
}
#endif //MIIO_WIFI_COWORK


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

#if defined(EMULATE_HOOD_MCU)
	DBGB_PRINTF(DEBUG_BIT_INFO, "ym_hood_wifi: EMULATE_HOOD_MCU\n");
#endif //EMULATE_HOOD_MCU

    /* Shared memory */
    shmid_vrcode = shmget((key_t)SHM_VR_VH_KEY, sizeof(struct shared_vrcode_st), 0666 | IPC_CREAT);
	DBGB_PRINTF(DEBUG_BIT_INFO, "ym_hood_wifi: shmid_vrcode=%d\n", shmid_vrcode);

    if (shmid_vrcode == -1) {
        DBGB_PRINTF(DEBUG_BIT_ERROR, "shmget failed.\n");
        exit(EXIT_FAILURE);
    }

    shm_at_vrcode = shmat(shmid_vrcode, (void *)0, 0);
    if (shm_at_vrcode == (void *)-1) {
        DBGB_PRINTF(DEBUG_BIT_ERROR, "shmat failed.\n");
        exit(EXIT_FAILURE);
    }

    DBGB_PRINTF(DEBUG_BIT_INFO, "Memory attached at 0x%X\n", (int)shm_at_vrcode);
    shm_vrcode_st = (struct shared_vrcode_st *)shm_at_vrcode;

#if defined(MIIO_WIFI_COWORK)
    /* Shared memory */
    shmid_vrmiio = shmget((key_t)SHM_VR_MIIO_KEY, sizeof(struct shared_vrmiio_st), 0666 | IPC_CREAT);
	DBGB_PRINTF(DEBUG_BIT_INFO, "ym_hood_wifi: shmid_vrmiio=%d\n", shmid_vrmiio);

    if (shmid_vrmiio == -1) {
        DBGB_PRINTF(DEBUG_BIT_ERROR, "shmget failed.\n");
        exit(EXIT_FAILURE);
    }

    shm_at_vrmiio = shmat(shmid_vrmiio, (void *)0, 0);
    if (shm_at_vrmiio == (void *)-1) {
        DBGB_PRINTF(DEBUG_BIT_ERROR, "shmat failed.\n");
        exit(EXIT_FAILURE);
    }

    DBGB_PRINTF(DEBUG_BIT_INFO, "Memory attached at 0x%X\n", (int)shm_at_vrmiio);
    shm_vrmiio_st = (struct shared_vrmiio_st *)shm_at_vrmiio;
#endif //MIIO_WIFI_COWORK

    tty_fd_mcu = open(COMM_MCU_DEV_NAME, O_RDWR | O_NOCTTY | O_SYNC);
    if (tty_fd_mcu < 0) {
        printf("Error opening %s: %s\n", COMM_MCU_DEV_NAME, strerror(errno));
        return -1;
    }

    tty_fd_wifi = open(COMM_WIFI_DEV_NAME, O_RDWR | O_NOCTTY | O_SYNC);
    if (tty_fd_wifi < 0) {
        printf("Error opening %s: %s\n", COMM_WIFI_DEV_NAME, strerror(errno));
        return -1;
    }

    // MCU: 8-bit, NO parity, 1 stop bit
    // VIOMI: 115200, OPPLE: 38400
    set_interface_attribs(tty_fd_mcu, COMM_BAUD_RATE_MCU);
    print_port_speed_info(COMM_MCU_DEV_NAME, COMM_BAUD_RATE_MCU);
    //set_mincount(tty_fd_mcu, 0);                /* set to pure timed read */

    // WIFI: 115200, 8-bit, NO parity, 1 stop bit
    set_interface_attribs(tty_fd_wifi, COMM_BAUD_RATE_WIFI);
    print_port_speed_info(COMM_WIFI_DEV_NAME, COMM_BAUD_RATE_WIFI);
    //set_mincount(tty_fd_wifi, 0);                /* set to pure timed read */

    tcdrain(tty_fd_mcu);    /* delay for output */
    tcdrain(tty_fd_wifi);    /* delay for output */

#if defined(THREAD_UART_W2M)
    fd_buf_pass1.fd_in = tty_fd_wifi;
    fd_buf_pass1.fd_out = tty_fd_mcu;
    fd_buf_pass1.buf = rbuf_wifi;

    ret_th1 = pthread_create(&uart_thread1, NULL, thread_uart_pass, (void *)&fd_buf_pass1);
    if (ret_th1 != 0) {
        DBGB_PRINTF(DEBUG_BIT_ERROR, "thread_uart_pass creation failed!\n");
        close_shm();
        exit(1);
    }
    DBGB_PRINTF(DEBUG_BIT_INFO, "thread_uart_pass is created.\n");
#endif //THREAD_UART_W2M

    idx_tx_cmd = 0;

    /* main loop */
    ok_loop = 1;
    cnt_loop = 0;
    while (ok_loop) {
        int rdlen, idx;

#if defined(EMULATE_HOOD_MCU)
        usleep(CHK_INTERVAL_VRCMD * 1000);  // Poll a VR command.
        if (!get_vrcode(shm_vrcode_st, &code_v2m_last, &val_v2m_last)) {
  #if defined(TEST_ONLY_OPPLE_DEMO1)
            handle_vrecog_cmd(tty_fd_mcu, code_v2m_last, val_v2m_last);
  #endif
            shm_vrcode_st->processed_v2m_f = TRUE;

            switch (code_v2m_last) {
            case X2M_CURR_TIME:
                waiting_micom_res_f = FALSE;

                // Audio play in voice module
                shm_vrcode_st->code_m2v = M2X_RESULT_TIME;
                shm_vrcode_st->val_m2v = 0x0825;  // 08H 37M
                shm_vrcode_st->processed_m2v_f = FALSE;
                shm_vrcode_st->emul_mcu_f = TRUE;  // Emulation!
                shm_vrcode_st->new_m2v_f = TRUE; 
                break;

            default:
                waiting_micom_res_f = FALSE;

                // Ment play in voice module
                shm_vrcode_st->code_m2v = M2X_MENT_INDEX;
                shm_vrcode_st->val_m2v = 0;
                shm_vrcode_st->processed_m2v_f = FALSE;
                shm_vrcode_st->emul_mcu_f = TRUE;  // Emulation!
                shm_vrcode_st->new_m2v_f = TRUE; 
                break;
            } // switch
        }
#else  // Original processing routine..
  #if defined(MIIO_WIFI_COWORK)
        rdlen = idx = get_miio_cmd(shm_vrmiio_st, rbuf_micom, sizeof(rbuf_micom) - 1);
        shm_vrmiio_st->processed_miiocmd_f = TRUE;

        // 115200bps = 14400B/s, 1/14400=69.4us/B,
        // 64-byte UART FIFO. (2220us/32B, 4440us/64B)
        usleep(SLEEP_USECONDS);  // Let other processes run...
  #else

        rdlen = idx = read(tty_fd_mcu, rbuf_micom, sizeof(rbuf_micom) - 1);
        // Need or not??? tcflush(tty_fd_mcu ,TCIOFLUSH);
        if (rdlen > 0) {
    #if 0  // i.e. Read 9: 0x77 0x69 0x66 0x69 0x20 0x6E 0x65 0x74 0x0D
            unsigned char   *p;
            printf("\nRead %d:", rdlen);
            for (p = rbuf_micom; idx-- > 0; p++)
                printf(" 0x%02X", *p);
            printf("\n");
    #endif
            rbuf_micom[rdlen] = 0;  // NULL termination..
            if (rbuf_micom[rdlen - 1] == '\r') {
                printf("Read %d: %s\n", rdlen, rbuf_micom);
                //printku_print("Read %d: %s\n", rdlen, (int)rbuf_micom, 0);
            } else {
                printf("No \\r at the end of a command!\n");
            }
        } else if (rdlen < 0) {
            DBGB_PRINTF(DEBUG_BIT_ERROR, "\nRead error: rdlen=%d: %s\n", rdlen, strerror(errno));
        } else {
            //printf("."); fflush(stdout);
            usleep(SLEEP_USECONDS);  // Let other processes run...
        }
  #endif //MIIO_WIFI_COWORK

        if (rdlen > 0) {
            next_step = handle_micom_cmd(tty_fd_mcu, shm_vrcode_st, rbuf_micom);

            // Next: play audio for current time.
            if (next_step == M2X_CMD_CURR_TIME) {  // Needs further process?
                if (waiting_vm_res_f) {
                    DBGB_PRINTF(DEBUG_BIT_ERROR, "Previous code(M2X) is NOT processed yet!\n");
                }
            }

            // Next: play ment (random or ment-N)
            if (next_step == M2X_MENT_INDEX) {  // Needs further process?
                if (waiting_vm_res_f) {
                    DBGB_PRINTF(DEBUG_BIT_ERROR, "Previous code(M2X) is NOT processed yet!\n");
                }
            }

            // Next: pass-through cmd string to WIFI module.
            if (next_step == M2X_CMD_FOR_WIFI) {
                char *rbuf;
                int slen;

                rbuf = rbuf_micom;
                rbuf += M2X_DEST_WIFI_SLEN;
                slen = strlen(rbuf);
                printf("TO-WIFI(%d): %s\n", slen, rbuf);
                wlen = write(tty_fd_wifi, rbuf, slen);
                if (wlen != slen) {
                    DBGB_PRINTF(DEBUG_BIT_ERROR, "write() error! wlen=%d (slen=%d)\n", wlen, slen);
                }
            }
        }
#endif //EMULATE_HOOD_MCU

        // "current time" is processed by Voice module?
        if (waiting_vm_res_f) {
            if (shm_vrcode_st->processed_m2v_f) {
                waiting_vm_res_f = FALSE;
                shm_vrcode_st->new_m2v_f = FALSE;
                DBGB_PRINTF(DEBUG_BIT_INFO, "processed_m2v_f = TRUE (processed)\n");
            }
        }

    } //while (ok_loop)

#if defined(THREAD_UART_W2M)
    // Check if UART pass-through is done.
    ret_th1 = pthread_join(uart_thread1, &result_th1);
    if (ret_th1 != 0) {
        DBGB_PRINTF(DEBUG_BIT_ERROR, "thread join failed!\n");
        close_shm();
        exit(1);
    }
    DBGB_PRINTF(DEBUG_BIT_INFO, "thread joined, result_th1=%s\n", (char *)result_th1);
#endif //THREAD_UART_W2M

    close_shm();
    return 0;
}

