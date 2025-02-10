SUBDIRS += echo
SUBDIRS += term
SUBDIRS += termvp
SUBDIRS += libc

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)