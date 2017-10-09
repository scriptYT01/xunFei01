#ifndef WM_HOOD_WIFI_EXT_H
#define WM_HOOD_WIFI_EXT_H

#include <linux/ioctl.h>

#include "type.h"
#include "custom.h"


#define SHM_VR_VH_KEY              1488  // Voice Recognition: VIOMI (Kitchen) Hood

struct shared_vrcode_st {
    BOOL new_v2m_f;  // TRUE if a new command from VR module is recognized.
    BOOL processed_v2m_f;  // TRUE if v2m cmd is processed.
    word code_v2m;  // VR => Translate into a code.
    word val_v2m;  // VR => value: i.e. timer_setting 60

    BOOL new_m2v_f;  // TRUE if a new command from MICOM is received.
    BOOL processed_m2v_f;  // TRUE if m2v cmd is processed.
    word code_m2v;  // a command(response) from MICOM
    word val_m2v;  // a value: i.e. "result 5,10" = 0x050A, "ment 0" = 0x0000, "ment 3" = 0x0003
    BOOL emul_mcu_f;  // TRUE if it is MCU emulation. (No real MCU is attached.)
};

#if defined(MIIO_WIFI_COWORK)
#define SHM_VR_MIIO_KEY             1498  // Voice Recognition: MIIO WIFI (miio_wifi)

#define CMD_MIIO_SIZE               (1024+4)

struct shared_vrmiio_st {
    BOOL new_miiocmd_f;  // TRUE if a new command from MIIO is received.
    BOOL processed_miiocmd_f;  // TRUE if miiocmd is processed.
    char cmdbuf[CMD_MIIO_SIZE];
};
#endif //MIIO_WIFI_COWORK

typedef struct code_str_st {
    word code;
    const char *cmdstr;
} code_str_type;

#if defined(TEST_ONLY_OPPLE_DEMO1)
#define RESP_BYTE_SIZE      5

typedef struct code_resp_bytes_st {
    word code;
    word cnt;
    byte rbytes[RESP_BYTE_SIZE];
} code_resp_bytes_type;
#endif //TEST_ONLY_OPPLE_DEMO1

#if defined(THREAD_UART_W2M)
typedef struct fd_buf_st {
    int fd_in;
    int fd_out;
    byte *buf;
} fd_buf_type;
#endif //THREAD_UART_W2M

/******************/
/*  X1000 => MCU  */
/******************/
#define X2M_DOWN_NONE      0x7FFF
#define X2M_POWER_ON       0x0101
#define X2M_POWER_OFF      0x0102
#define X2M_WIND_LOW       0x0201
#define X2M_WIND_HIGH      0x0202
#define X2M_WIND_SUPER     0x0203
#define X2M_AUTO_MODE      0x0204
#define X2M_LIGHT_ON       0x0301
#define X2M_LIGHT_OFF      0x0302
#define X2M_TIMER_SET      0x0401
#define X2M_CURR_TIME      0x0402
#define X2M_AIR_VENTI      0x0501
#define X2M_AIR_POLLUTION  0x0601


/******************/
/*  MCU => X1000  */
/******************/
// "voice ", "wifi "
#define M2X_DEST_LOCAL     0x8001
#define M2X_DEST_WIFI      0x8002
// "get_down\r", "result "ok"\r"
#define M2X_GET_DOWN       0x8101
#define M2X_RESULT_OK      0x8201
// "result 5,10\r", "ment 0\r"
#define M2X_RESULT_TIME    0x8301
#define M2X_MENT_INDEX     0x8302
// MCU does not issue this command. pvalg module's internal use only.
#define M2X_MENT_WAKEUP    0x8303


#define M2X_DEST_LOCAL_SLEN         6  // Refer to M2X_DEST_LOCAL.
#define M2X_DEST_WIFI_SLEN          5  // Refer to M2X_DEST_WIFI.
#define M2X_MENT_INDEX_SLEN         5  // Refer to M2X_MENT_INDEX.
#define M2X_RESULT_TIME_SLEN        7  // Refer to M2X_RESULT_TIME.

typedef enum {
    M2X_CMD_DONE = 0,
    M2X_CMD_CURR_TIME,
    M2X_CMD_FOR_WIFI,
    M2X_CMD_MENT_INDEX,
} m2x_cmd_result_type;

typedef enum {
    CURR_TIME_HOUR = 0,
    CURR_TIME_MIN,
} time_hm_type;


/*
UART configuration : 115200-8-N-1
*/

#define UART_READ_TIMEOUT           3  // tenths of a second

#define COMM_MCU_DEV_NAME           "/dev/ttyS1"
#define COMM_WIFI_DEV_NAME          "/dev/ttyS0"

#if defined(TEST_ONLY_OPPLE_DEMO1)
#define COMM_BAUD_RATE_MCU          B38400
#else
#define COMM_BAUD_RATE_MCU          B115200
#endif //TEST_ONLY_OPPLE_DEMO1

#define COMM_BAUD_RATE_WIFI         B115200


// Refer to "20170224 VIOMI command_list(增加协议)A.xlsx" file.
#define CMD_TX_PKT_SIZE             (1024+4)
#define CMD_RX_PKT_SIZE             (1024+4)
#define RX_BUF_M2V_SIZE             (CMD_RX_PKT_SIZE * 2)


//------------------------------------------------------------------------------


#endif //WM_HOOD_WIFI_EXT_H

