SUBDIRS = common \
		  MedToRoot \
		  TreeBuilder \
		  TreeAna \
		  rootgui

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

