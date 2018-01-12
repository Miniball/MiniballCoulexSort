SUBDIRS = common \
		  MedToRoot \
		  TreeBuilder \
		  CLXAna \
		  MntAna \
		  TDRIVAna \
		  rootgui

all: MKDIR_P $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

MKDIR_P:
	mkdir -p lib bin

.PHONY: all MKDIR_P $(SUBDIRS)

