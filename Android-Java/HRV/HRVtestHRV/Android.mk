LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-subdir-java-files)

LOCAL_C_INCLUDES += $(JNI_H_INCLUDE)

LOCAL_PACKAGE_NAME := HrvDemos

LOCAL_JNI_SHARED_LIBRARIES := libhello-jni

LOCAL_SDK_VERSION := current

include $(BUILD_PACKAGE)

# Use the folloing include to make our test apk.
include $(call all-makefiles-under,$(LOCAL_PATH))
