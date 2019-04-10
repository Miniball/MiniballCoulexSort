SUBDIRS = common \
		  MedToRoot \
		  EventTree \
		  TreeBuilder \
		  CLXAna \
		  rootgui

all: MKDIR_P $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

MKDIR_P:
	mkdir -p lib bin

.PHONY: all MKDIR_P $(SUBDIRS)

clean:
	@echo "Cleaning up everything..." ;
	for dir in $(SUBDIRS); do\
		$(MAKE) -C $$dir clean; \
	done


