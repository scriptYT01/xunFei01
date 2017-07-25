LOCAL_PATH := $(my-dir)

#==== tts_sample11 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := tts_sample11.x1000.bin
LOCAL_MODULE_TAGS := tts_sample11
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/network
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/xf10
LOCAL_SRC_FILES := samples/tts_sample/tts_sample.c
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lrt -ldl -lpthread -lm -lc
LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc 
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc 
include $(BUILD_EXECUTABLE)

#==== tts_tcp12 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := tts_tcp12.x1000.bin
LOCAL_MODULE_TAGS := tts_tcp12
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/network
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/xf10
LOCAL_SRC_FILES := samples/tts_sample/tts_tcp.c
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lrt -ldl -lpthread -lm -lc
LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc 
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc 
include $(BUILD_EXECUTABLE)

#==== asr_sample13 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := asr_sample13.x1000.bin
LOCAL_MODULE_TAGS := asr_sample13
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/network
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/xf10
LOCAL_SRC_FILES := samples/asr_sample/asr_sample.c
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lrt -ldl -lpthread -lm -lc
LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc 
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc 
include $(BUILD_EXECUTABLE)

#==== asr_tcp14 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := asr_tcp14.x1000.bin
LOCAL_MODULE_TAGS := asr_tcp14
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/network
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/xf10
LOCAL_SRC_FILES := samples/asr_sample/asr_tcp.c
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lrt -ldl -lpthread -lm -lc
LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc 
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc 
include $(BUILD_EXECUTABLE)

