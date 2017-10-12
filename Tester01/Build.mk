LOCAL_PATH := $(my-dir)

#==== te01 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := TEster01.x1000.bin
LOCAL_MODULE_TAGS := te01
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/te01
LOCAL_SRC_FILES :=                                	\
	base01/base01_01.c                            	\
	base01/base01_02.c                            	\
	base01/p3open.cpp                            	\
	base01/buf01.cpp                            	\
	base01/match01.cpp                            	\
    ssuperStream01/streamControllerTop.cpp          \
    ssuperStream01/superStreamBuf.cpp               \
    ssuperStream01/superStreamCheck.cpp             \
    ssuperStream01/superStreamChilds.cpp            \
    ssuperStream01/superStreamConf.cpp              \
    ssuperStream01/superStreamDump.cpp              \
    ssuperStream01/superStreamInit.cpp              \
    ssuperStream01/superStreamOpenOrReopen.cpp      \
    ssuperStream01/superStreamRW.cpp                \
    ssuperStream01/superStreamStatus.cpp            \
	ssuperStream01/superStreamTCP.cpp				\
    ssuperStream01/superStreamTop.cpp               \
	src/te01_loop.cpp 							    \
	src/te01_main.cpp 							    \


LOCAL_LDLIBS := -lc -lpthread -lstdc++ -ldl
LOCAL_CFLAGS := -I$(LOCAL_PATH)/base01
LOCAL_CFLAGS += -I$(LOCAL_PATH)/base01 -I$(LOCAL_PATH)/ssuperStream01 \
				-I$(LOCAL_PATH)/src -I$(LOCAL_PATH)/include
include $(BUILD_EXECUTABLE)

# src/ymDbg01.c
#==== ymDbg01 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := ymDbg01.x1000.bin
LOCAL_MODULE_TAGS := ymDbg01
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/te01
LOCAL_SRC_FILES := ymDb/ymDbg01.c
LOCAL_CFLAGS:= -Wall -Werror
LOCAL_LDLIBS := -lc -lpthread 
include $(BUILD_EXECUTABLE)

