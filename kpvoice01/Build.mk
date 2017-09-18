LOCAL_PATH := $(my-dir)

#==== kp01 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := kp01.x1000.bin
LOCAL_MODULE_TAGS := kp01
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/kp01
LOCAL_SRC_FILES :=                                				\
	base01/base01_01.c                            				\
    ssuperStream01/streamControllerTop.cpp            \
    ssuperStream01/superStreamBuf.cpp                 \
    ssuperStream01/superStreamCheck.cpp               \
    ssuperStream01/superStreamChilds.cpp              \
    ssuperStream01/superStreamConf.cpp                \
    ssuperStream01/superStreamDump.cpp                \
    ssuperStream01/superStreamInit.cpp                \
    ssuperStream01/superStreamOpenOrReopen.cpp        \
    ssuperStream01/superStreamRW.cpp                  \
    ssuperStream01/superStreamStatus.cpp              \
	ssuperStream01/superStreamTCP.cpp				  \
    ssuperStream01/superStreamTop.cpp                 \
	src/kp01_loop.cpp 							      \
	src/kp01_main.cpp 							      \
	src/kp01_test.cpp 							      \


LOCAL_LDLIBS := -lc -lpthread -lstdc++ -ldl
LOCAL_CFLAGS := -I$(LOCAL_PATH)/base01
LOCAL_CFLAGS += -I$(LOCAL_PATH)/base01 -I$(LOCAL_PATH)/ssuperStream01 
include $(BUILD_EXECUTABLE)

