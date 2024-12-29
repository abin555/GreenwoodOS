SUBDIRS := $(wildcard */.)
SUBDIRS := $(filter-out Models/., $(SUBDIRS))
SUBDIRS := $(filter-out Pictures/., $(SUBDIRS))

all: $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)