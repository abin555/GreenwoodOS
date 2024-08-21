SUBDIRS += echo
SUBDIRS += term
SUBDIRS += termvp
SUBDIRS += libc
SUBDIRS += explorer

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)