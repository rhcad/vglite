# The simplest way to compile TouchVG on MinGW, Cygwin:
#
# 1. `cd' to the directory containing the file of 'Makefile'.
#
# 2. Type `make' to compile the projects.
# 
# 3. You can remove the program object files from the source code
#    directory by typing `make clean'.
#
SUBDIRS         =$(subst /,,$(dir $(wildcard */)))
CLEANDIRS       =$(addsuffix .clean, $(SUBDIRS))
INSTALLDIRS     =$(addsuffix .install, $(SUBDIRS))

.PHONY:     $(SUBDIRS) clean install
all:        $(SUBDIRS)
clean:      $(CLEANDIRS)
install:    $(INSTALLDIRS)

$(SUBDIRS):
	@! test -e $@/Makefile || $(MAKE) -C $@

$(CLEANDIRS) $(INSTALLDIRS):
	@ ! test -e $(basename $@)/Makefile || \
	$(MAKE) -C $(basename $@) $(subst .,,$(suffix $@))
