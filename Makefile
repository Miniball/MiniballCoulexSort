SUBDIRS = common \
		  MedToRoot \
		  TreeBuilder \
		  CLXAna \
		  MntAna \
		  TDRIVAna \
		  rootgui

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)

