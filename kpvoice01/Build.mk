LOCAL_PATH := $(my-dir)

#==== kp01 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := kp01.x1000.bin
LOCAL_MODULE_TAGS := kp01
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/kp01
LOCAL_SRC_FILES := \
	src/kp01_main.cpp 

#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lrt -ldl -lpthread -lm -lc
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc -lpthread
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -lc -lpthread -lstdc++
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -laiui -lc -lpthread -lstdc++
#LOCAL_LDLIBS := -L$(LOCAL_PATH)/libs/mipsX1000 -lmsc -laiui -lc -lpthread -lstdc++ -ldl
LOCAL_LDLIBS := -lc -lpthread -lstdc++ -ldl
LOCAL_CFLAGS := -Iexternal/xfClient10
LOCAL_CFLAGS := -I$(LOCAL_PATH)/base01
include $(BUILD_EXECUTABLE)

