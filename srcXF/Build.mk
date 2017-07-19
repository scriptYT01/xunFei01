LOCAL_PATH := $(my-dir)

#==== tts_sample ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := tts_sample.bin
LOCAL_MODULE_TAGS := tts_sample
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/network
LOCAL_SRC_FILES := samples/tts_sample/tts_sample.c
LOCAL_LDLIBS := -lmsc -lrt -ldl -lpthread 
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

