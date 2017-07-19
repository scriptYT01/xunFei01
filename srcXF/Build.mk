LOCAL_PATH := $(my-dir)

#==== ym_hood_wifi ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := ym_hood_wifi
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/network
LOCAL_SRC_FILES := ym_hood_wifi.c
LOCAL_LDLIBS := -lc -lpthread
include $(BUILD_EXECUTABLE)
#==== miio_wifi ==========================================
LOCAL_MODULE := miio_wifi
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/network
LOCAL_SRC_FILES := miio_wifi.c cJSON.c data_Analysis.c muc_stm32.c         
########LOCAL_CFLAGS := -Wall -O -Werror -DVERSION=\"0.1.4\"
LOCAL_CFLAGS := -Wall -O -DVERSION=\"0.1.4\"
LOCAL_LDLIBS := -lc -lpthread -lm
include $(BUILD_EXECUTABLE)

