ROOTDIR  =../../..
PKGNAME  =touchvg
PROJNAME =gview

INCLUDES += -I$(ROOTDIR)/core/include/geom \
            -I$(ROOTDIR)/core/include/graph \
            -I$(ROOTDIR)/core/include/shape \
            -I$(ROOTDIR)/core/include/storage \
            -I$(ROOTDIR)/core/include/command \
            -I$(ROOTDIR)/core/include/tradecmd \
            -I$(ROOTDIR)/core/include/view \
            -I$(ROOTDIR)/core/include/canvas \
            -I$(ROOTDIR)/core/include/test

SOURCES   =$(wildcard *.cpp) \
           $(wildcard ../geom/*.cpp) \
           $(wildcard ../graph/*.cpp) \
           $(wildcard ../shape/*.cpp) \
           $(wildcard ../command/*.cpp) \
           $(wildcard ../tradecmd/*.cpp) \
           $(wildcard ../json/*.cpp) \
           $(wildcard ../test/*.cpp)

include $(ROOTDIR)/core/mk/config.mk
include $(ROOTDIR)/core/mk/Makefile.swig
