LOCAL_PATH := $(my-dir)

#==== tts_sample ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := tts_sample31.x1000.bin
LOCAL_MODULE_TAGS := tts_sample
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/network
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/xf20
LOCAL_SRC_FILES := samples/tts_sample/tts_sample.c
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lrt -ldl -lpthread -lm -lc
LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc
include $(BUILD_EXECUTABLE)

#==== aiui_sample ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := aiui_sample32.x1000.bin
LOCAL_MODULE_TAGS := aiui_sample
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/$(TARGET_TESTSUIT_DIR)/network
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/xf20
LOCAL_SRC_FILES := \
	samples/aiui_sample/src/jsoncpp/json_reader.cpp \
	samples/aiui_sample/src/jsoncpp/json_value.cpp \
	samples/aiui_sample/src/jsoncpp/json_writer.cpp \
	\
	samples/aiui_sample/src/AIUITest.cpp \
	samples/aiui_sample/src/FileUtil.cpp \
	samples/aiui_sample/src/demo.cpp

#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lrt -ldl -lpthread -lm -lc
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc -lpthread
LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc -lpthread -lstdc++
include $(BUILD_EXECUTABLE)
