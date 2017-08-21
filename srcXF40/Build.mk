LOCAL_PATH := $(my-dir)

# alsa-record-example41.c
#==== alsa-record-example41 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := alsa-record-example41.x1000.bin
LOCAL_MODULE_TAGS := alsa-record-example41
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/xf40
LOCAL_SRC_FILES := \
	alsa-record-example41.c
LOCAL_LDLIBS := -Ldevice/common/system/usr/lib -lc -lasound 
LOCAL_CFLAGS := -Iexternal/alsa-lib/include
include $(BUILD_EXECUTABLE)

# alsa-record-example42.c
#==== alsa-record-example42 ==========================================
include $(CLEAR_VARS)
LOCAL_MODULE := alsa-record-example42.x1000.bin
LOCAL_MODULE_TAGS := alsa-record-example42
LOCAL_MODULE_PATH:=$(TARGET_FS_BUILD)/ch/xf40
LOCAL_SRC_FILES := \
	alsa-record-example42.c
LOCAL_LDLIBS := -Ldevice/common/system/usr/lib -lc -lasound 
LOCAL_CFLAGS := -Iexternal/alsa-lib/include
include $(BUILD_EXECUTABLE)

