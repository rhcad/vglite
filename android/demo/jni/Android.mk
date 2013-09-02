# Copyright (c) 2013, Zhang Yungui, https://github.com/rhcad/touchvg
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
                    $(COREINC)/tradecmd \
                    $(COREINC)/view \
                    $(COREINC)/command \
                    $(COREINC)/shape \
                    $(COREINC)/graph \
                    $(COREINC)/geom \
                    $(COREINC)

LOCAL_SRC_FILES  := $(CORESRC)/test/testcanvas.cpp \
                    $(CORESRC)/test/RandomShape.cpp \
                    $(CORESRC)/tradecmd/tradecmd.cpp \
                    $(CORESRC)/view/gicoreview.cpp \
                    $(CORESRC)/view/GcShapeDoc.cpp \
                    $(CORESRC)/view/GcGraphView.cpp \
                    $(CORESRC)/view/GcMagnifierView.cpp \
                    $(CORESRC)/command/mgactions.cpp \
                    $(CORESRC)/command/mgcmddraw.cpp \
                    $(CORESRC)/command/mgcmderase.cpp \
                    $(CORESRC)/command/mgcmdmgr.cpp \
                    $(CORESRC)/command/mgcmds.cpp \
                    $(CORESRC)/command/mgcmdselect.cpp \
                    $(CORESRC)/command/mgdrawline.cpp \
                    $(CORESRC)/command/mgdrawlines.cpp \
                    $(CORESRC)/command/mgdrawrect.cpp \
                    $(CORESRC)/command/mgdrawsplines.cpp \
                    $(CORESRC)/command/mgdrawtriang.cpp \
                    $(CORESRC)/command/mgdrawarc.cpp \
                    $(CORESRC)/command/TransformCmd.cpp \
                    $(CORESRC)/json/mgjsonstorage.cpp \
                    $(CORESRC)/shape/mgcomposite.cpp \
                    $(CORESRC)/shape/mgellipse.cpp \
                    $(CORESRC)/shape/mggrid.cpp \
                    $(CORESRC)/shape/mglayer.cpp \
                    $(CORESRC)/shape/mgline.cpp \
                    $(CORESRC)/shape/mglines.cpp \
                    $(CORESRC)/shape/mgrdrect.cpp \
                    $(CORESRC)/shape/mgrect.cpp \
                    $(CORESRC)/shape/mgshape.cpp \
                    $(CORESRC)/shape/mgshapedoc.cpp \
                    $(CORESRC)/shape/mgshapes.cpp \
                    $(CORESRC)/shape/mgspfactory.cpp \
                    $(CORESRC)/shape/mgsplines.cpp \
                    $(CORESRC)/shape/mgsplock.cpp \
                    $(CORESRC)/graph/gipath.cpp \
                    $(CORESRC)/graph/gixform.cpp \
                    $(CORESRC)/graph/gigraph.cpp \
                    $(CORESRC)/geom/mgmat.cpp \
                    $(CORESRC)/geom/mgbase.cpp \
                    $(CORESRC)/geom/mgbnd.cpp \
                    $(CORESRC)/geom/mgbox.cpp \
                    $(CORESRC)/geom/mgcurv.cpp \
                    $(CORESRC)/geom/mglnrel.cpp \
                    $(CORESRC)/geom/mgnear.cpp \
                    $(CORESRC)/geom/mgnearbz.cpp \
                    $(CORESRC)/geom/mgvec.cpp \
                    touchvg_java_wrap.cpp

include $(BUILD_SHARED_LIBRARY)
