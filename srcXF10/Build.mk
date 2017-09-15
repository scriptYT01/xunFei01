LOCAL_PATH := $(my-dir)

#==== tts_sample11 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := tts_sample11.x1000.bin
LOCAL_MODULE_TAGS := tts_sample11
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/network
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/xf10
LOCAL_SRC_FILES := \
	patchXF/pathcXFtts01.c \
	samples/tts_sample/tts_sample.c
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lrt -ldl -lpthread -lm -lc
LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc 
LOCAL_CFLAGS := -I$(LOCAL_PATH) -Wall -Werror
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

#srcXF10/samples/iat_sample/iat_sample.c
#==== iat_sample15 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := iat_sample15.x1000.bin
LOCAL_MODULE_TAGS := iat_sample15
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/xf10
LOCAL_SRC_FILES := samples/iat_sample/iat_sample.c
LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc 
#LOCAL_CFLAGS := -DUSE_dict_0dis_1ena=0
include $(BUILD_EXECUTABLE)

# ise_sample/ise_sample.c
#==== ise_sample17 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := ise_sample17.x1000.bin
LOCAL_MODULE_TAGS := ise_sample17
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/xf10
LOCAL_SRC_FILES := samples/ise_sample/ise_sample.c
LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc 
#LOCAL_CFLAGS := -DUSE_dict_0dis_1ena=0
include $(BUILD_EXECUTABLE)

# iat_record_sample/iat_record_sample.c
#==== iat_record_sample18 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := iat_record_sample18.x1000.bin
LOCAL_MODULE_TAGS := iat_record_sample18
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/xf10
LOCAL_SRC_FILES := \
	patchXF/base01/base01_01.c                             			\
	patchXF/iat_record_sample_patch.c                             	\
	patchXF/pathcXFtts01.c                                          \
	samples/iat_record_sample/linuxrec.c                            \
	samples/iat_record_sample/speech_recognizer.c                   \
	samples/iat_record_sample/iat_record_sample.c

LOCAL_LDLIBS := -Ldevice/common/system/usr/lib -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc -lpthread -lasound
#LOCAL_CFLAGS := -I../tinyalsa/include
#LOCAL_CFLAGS := -I../alsa-lib/include
LOCAL_CFLAGS := -Iexternal/alsa-lib/include -I$(LOCAL_PATH) -Wall -Werror
LOCAL_CFLAGS += -Iexternal/xfClient10/patchXF -Iexternal/xfClient10/patchXF/base01 -Iexternal/xfClient10/patchXF/ssuperStream01 
include $(BUILD_EXECUTABLE)

