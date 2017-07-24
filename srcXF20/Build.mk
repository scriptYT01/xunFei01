LOCAL_PATH := $(my-dir)

#==== tts_sample ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := tts_sample21.bin
LOCAL_MODULE_TAGS := tts_sample
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/network
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/xf20
LOCAL_SRC_FILES := samples/tts_sample/tts_sample.c
LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lrt -ldl -lpthread -lm -lc
include $(BUILD_EXECUTABLE)

#==== miio_wifi ==========================================
#LOCAL_MODULE := miio_wifi
#LOCAL_MODULE_TAGS := optional
#LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/network
#LOCAL_SRC_FILES := miio_wifi.c cJSON.c data_Analysis.c muc_stm32.c         
#########LOCAL_CFLAGS := -Wall -O -Werror -DVERSION=\"0.1.4\"
#LOCAL_CFLAGS := -Wall -O -DVERSION=\"0.1.4\"
#LOCAL_LDLIBS := -lc -lpthread
#LOCAL_LDLIBS := -lc -lpthread -lm
#include $(BUILD_EXECUTABLE)

