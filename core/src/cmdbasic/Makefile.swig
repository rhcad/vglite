ROOTDIR  =../../..
PKGNAME  =touchvg
PROJNAME =cmdbasic

INCLUDES += -I$(ROOTDIR)/core/include/geom \
            -I$(ROOTDIR)/core/include/graph \
            -I$(ROOTDIR)/core/include/shape \
            -I$(ROOTDIR)/core/include/storage \
            -I$(ROOTDIR)/core/include/cmd \
            -I$(ROOTDIR)/core/include/cmdbase \
            -I$(ROOTDIR)/core/include/cmdbasic \
            -I$(ROOTDIR)/core/include/cmdobserver \
            -I$(ROOTDIR)/core/include/canvas

SOURCES   =$(wildcard *.cpp) \
           $(wildcard ../geom/*.cpp) \
           $(wildcard ../graph/*.cpp) \
           $(wildcard ../shape/*.cpp) \
           $(wildcard ../cmdbase/*.cpp)

include $(ROOTDIR)/core/mk/config.mk
include $(ROOTDIR)/core/mk/Makefile.swig
