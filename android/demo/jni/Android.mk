# Copyright (c) 2013, Zhang Yungui, https://github.com/rhcad/vglite
#
LOCAL_PATH := $(call my-dir)
COREINC    := $(LOCAL_PATH)/../../../core/include
CORESRC    := ../../../core/src

include $(CLEAR_VARS)

LOCAL_MODULE := touchvg
LOCAL_LDLIBS           := -L$(SYSROOT)/usr/lib -llog
LOCAL_PRELINK_MODULE   := false
LOCAL_CFLAGS           := -frtti -Wall -Wextra

ifeq ($(TARGET_ARCH),arm)
# Ignore "note: the mangling of 'va_list' has changed in GCC 4.4"
LOCAL_CFLAGS += -Wno-psabi
endif

LOCAL_C_INCLUDES := $(COREINC)/callback \
                    $(COREINC)/test \
                    $(COREINC)/view \
                    $(COREINC)

LOCAL_SRC_FILES  := $(CORESRC)/test/testcanvas.cpp \
                    $(CORESRC)/view/gicoreview.cpp \
                    $(CORESRC)/view/GcMainView.cpp \
                    $(CORESRC)/view/GcMagnifierView.cpp \
                    touchvg_java_wrap.cpp

include $(BUILD_SHARED_LIBRARY)
