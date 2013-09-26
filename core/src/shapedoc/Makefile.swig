ROOTDIR  =../../..
PKGNAME  =touchvg
PROJNAME =shapedoc

INCLUDES += -I$(ROOTDIR)/core/include/geom \
            -I$(ROOTDIR)/core/include/graph \
            -I$(ROOTDIR)/core/include/shape \
            -I$(ROOTDIR)/core/include/shapedoc \
            -I$(ROOTDIR)/core/include/storage \
            -I$(ROOTDIR)/core/include/canvas

SOURCES   =$(wildcard *.cpp) \
           $(wildcard ../geom/*.cpp) \
           $(wildcard ../graph/*.cpp) \
           $(wildcard ../shape/*.cpp)

include $(ROOTDIR)/core/mk/config.mk
include $(ROOTDIR)/core/mk/Makefile.swig
