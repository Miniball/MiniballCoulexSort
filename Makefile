SUBDIRS = common \
		  MedToRoot \
		  TreeBuilder \
		  CLXAna \
		  rootgui

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

