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

LOCAL_C_INCLUDES := $(COREINC)/canvas \
                    $(COREINC)/test \
                    $(COREINC)/cmdobserver \
                    $(COREINC)/view \
                    $(COREINC)/cmd \
                    $(COREINC)/cmdbase \
                    $(COREINC)/cmdbasic \
                    $(COREINC)/cmdmgr \
                    $(COREINC)/shape \
                    $(COREINC)/shapedoc \
                    $(COREINC)/storage \
                    $(COREINC)/graph \
                    $(COREINC)/geom \
                    $(COREINC)

LOCAL_SRC_FILES  := $(CORESRC)/test/testcanvas.cpp \
                    $(CORESRC)/test/RandomShape.cpp \
                    $(CORESRC)/educmds/eduobserver.cpp \
                    $(CORESRC)/view/educmds.cpp \
                    $(CORESRC)/view/gicoreview.cpp \
                    $(CORESRC)/view/GcShapeDoc.cpp \
                    $(CORESRC)/view/GcGraphView.cpp \
                    $(CORESRC)/view/GcMagnifierView.cpp \
                    $(CORESRC)/cmdmgr/mgactions.cpp \
                    $(CORESRC)/cmdmgr/mgcmdmgr_.cpp \
                    $(CORESRC)/cmdmgr/mgcmdmgr2.cpp \
                    $(CORESRC)/cmdmgr/mgsnapimpl.cpp \
                    $(CORESRC)/cmdmgr/cmdsubject.cpp \
                    $(CORESRC)/cmdmgr/mgcmdselect.cpp \
                    $(CORESRC)/cmdbase/mgcmddraw.cpp \
                    $(CORESRC)/cmdbase/mgdrawarc.cpp \
                    $(CORESRC)/cmdbase/mgdrawrect.cpp \
                    $(CORESRC)/cmdbase/mgsplock.cpp \
                    $(CORESRC)/cmdbasic/cmdbasic.cpp \
                    $(CORESRC)/cmdbasic/mgcmderase.cpp \
                    $(CORESRC)/cmdbasic/mgdrawcircle.cpp \
                    $(CORESRC)/cmdbasic/mgdrawdiamond.cpp \
                    $(CORESRC)/cmdbasic/mgdrawellipse.cpp \
                    $(CORESRC)/cmdbasic/mgdrawfreelines.cpp \
                    $(CORESRC)/cmdbasic/mgdrawgrid.cpp \
                    $(CORESRC)/cmdbasic/mgdrawline.cpp \
                    $(CORESRC)/cmdbasic/mgdrawlines.cpp \
                    $(CORESRC)/cmdbasic/mgdrawparallel.cpp \
                    $(CORESRC)/cmdbasic/mgdrawpolygon.cpp \
                    $(CORESRC)/cmdbasic/mgdrawsplines.cpp \
                    $(CORESRC)/cmdbasic/mgdrawsquare.cpp \
                    $(CORESRC)/cmdbasic/mgdrawtriang.cpp \
                    $(CORESRC)/cmdtest/TransformCmd.cpp \
                    $(CORESRC)/json/mgjsonstorage.cpp \
                    $(CORESRC)/shape/mgcomposite.cpp \
                    $(CORESRC)/shape/mgellipse.cpp \
                    $(CORESRC)/shape/mggrid.cpp \
                    $(CORESRC)/shape/mgline.cpp \
                    $(CORESRC)/shape/mglines.cpp \
                    $(CORESRC)/shape/mgrdrect.cpp \
                    $(CORESRC)/shape/mgrect.cpp \
                    $(CORESRC)/shape/mgshape.cpp \
                    $(CORESRC)/shape/mgshapes.cpp \
                    $(CORESRC)/shape/mgbasicspreg.cpp \
                    $(CORESRC)/shape/mgsplines.cpp \
                    $(CORESRC)/shapedoc/mgshapedoc.cpp \
                    $(CORESRC)/shapedoc/mglayer.cpp \
                    $(CORESRC)/shapedoc/mglockrw.cpp \
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
