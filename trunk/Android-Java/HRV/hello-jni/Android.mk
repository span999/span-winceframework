LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(call all-subdir-java-files)

LOCAL_C_INCLUDES += $(JNI_H_INCLUDE)

LOCAL_PACKAGE_NAME := hello-jni

LOCAL_JNI_SHARED_LIBRARIES := libhello-jni

### LOCAL_CERTIFICATE := shared

include $(BUILD_PACKAGE)

MY_PATH := $(LOCAL_PATH)
include $(MY_PATH)/jni/Android.mk