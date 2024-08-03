SUBDIRS += echo
SUBDIRS += term
SUBDIRS += libc

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)