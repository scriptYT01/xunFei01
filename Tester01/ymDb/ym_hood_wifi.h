#ifndef WM_HOOD_WIFI_H
#define WM_HOOD_WIFI_H

#include <linux/ioctl.h>

#include "type.h"
#include "custom.h"
#include "ym_hood_wifi_ext.h"


code_str_type codelist_v2m[] = {
    X2M_DOWN_NONE, "v down none\r",
    X2M_POWER_ON, "v down set_power 1\r",
    X2M_POWER_OFF, "v down set_power 0\r",
    X2M_WIND_LOW, "v down set_wind 1\r",
    X2M_WIND_HIGH, "v down set_wind 16\r",
    X2M_WIND_SUPER, "v down set_wind 4\r",
    X2M_AUTO_MODE, "v down auto_mode\r",
    X2M_LIGHT_ON, "v down set_light 1\r",
    X2M_LIGHT_OFF, "v down set_light 0\r",
    X2M_TIMER_SET, "v down timer_setting ",  // MAX: 3H30M, i.e. "down timer_setting 210\r"
    X2M_CURR_TIME, "v down current_time\r",
    X2M_AIR_VENTI, "v down air_ventilation\r",
    0x0000, NULL  // Last item
};

#if defined(TEST_ONLY_OPPLE_DEMO1)
code_resp_bytes_type codelist_rbytes[] = {
    X2M_DOWN_NONE, 5, {0xAA, 0x00, 0x00, 0x00, 0x00},
    X2M_POWER_ON, 5, {0xAA, 0x00, 0x00, 0x00, 0x11},
    X2M_POWER_OFF, 5, {0xAA, 0x00, 0x00, 0x00, 0x10},
    X2M_WIND_LOW, 5, {0xAA, 0x00, 0x00, 0x00, 0x03},
    X2M_WIND_HIGH, 5, {0xAA, 0x00, 0x00, 0x00, 0x04},
    X2M_WIND_SUPER, 5, {0xAA, 0x00, 0x00, 0x00, 0x12},
    X2M_AUTO_MODE, 5, {0xAA, 0x00, 0x00, 0x00, 0x07},
    X2M_LIGHT_ON, 5, {0xAA, 0x00, 0x00, 0x00, 0x01},
    X2M_LIGHT_OFF, 5, {0xAA, 0x00, 0x00, 0x00, 0x02},
    X2M_TIMER_SET, 5, {0xAA, 0x00, 0x00, 0x00, 0x13},
    X2M_CURR_TIME, 5, {0xAA, 0x00, 0x00, 0x00, 0x14},
    X2M_AIR_VENTI, 5, {0xAA, 0x00, 0x00, 0x00, 0x15},
    0x0000, 0, {0, 0, 0, 0, 0}  // Last item
};
#endif //TEST_ONLY_OPPLE_DEMO1


code_str_type codelist_m2v[] = {
    M2X_DEST_LOCAL, "voice ",  // command for local: index=0, refer to handle_micom_cmd().
    M2X_DEST_WIFI, "wifi ",  // command for wifi module: index=1

    M2X_GET_DOWN, "get_down\r",
    M2X_MENT_INDEX, "ment ",  // "ment 0": random ment play, "ment N": ment-N play.
    M2X_RESULT_TIME, "result ",  // 24-hour format, i.e. "result 23,59\r"
    M2X_RESULT_OK, "result \"ok\"\r",  // No need to play a ment.
    0x0000, NULL  // Last item
};


//------------------------------------------------------------------------------


#endif //WM_HOOD_WIFI_H

