# Makefile for mb_sorter GUI
.PHONY: clean all

ROOTCFLAGS	:= $(shell root-config --cflags)
ROOTGLIBS   := $(shell root-config --glibs)
ROOTVER     := $(shell root-config --version | head -c1)

ifeq ($(ROOTVER),5)
	ROOTDICT  := rootcint
	DICTEXT   := .h
else
	ROOTDICT  := rootcling
	DICTEXT   := _rdict.pcm
endif

BIN_DIR     := ../bin
LIB_DIR     := ../lib
MED_DIR     := ../MedToRoot
ANA_INC     := ../CLXAna

CC       := $(shell root-config --cc)
CPP      := $(shell root-config --cxx)
CFLAGS   := $(ROOTCFLAGS) -Wall -fPIC -Wno-unused-variable -g

LIBS     := $(ROOTGLIBS) -L$(LIB_DIR)

INCLUDES := -I.

PLATFORM:=$(shell uname)
ifeq ($(PLATFORM),Darwin)
SHAREDSWITCH = -Qunused-arguments -shared -undefined dynamic_lookup -dynamiclib -Wl,-install_name,'@executable_path/../lib/'# NO ENDING SPACE
else
export __LINUX__:= 1
LINKFLAGS_PREFIX += -Wl,--no-as-needed
SHAREDSWITCH = -shared -Wl,-soname,# NO ENDING SPACE
endif

all: MKDIR_P mb_sorter

MKDIR_P:
	mkdir -p $(BIN_DIR) $(LIB_DIR)

OBJECTS = MBSorter.o \
          mb_sorter_dict.o

lib%.so: %.o %_dict.o
	$(CPP) -fPIC $(OBJECTS) $(SHAREDSWITCH)lib$@.so $(LIBS) -o lib$@.so
	cp $@ $(LIB_DIR)/

%.o: %.cc %.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

mb_sorter: mb_sorter.cc $(OBJECTS)
	$(CPP) $(CFLAGS) $(INCLUDES) $< $(OBJECTS) -o $@ $(LIBS)
	$(CPP) -fPIC $(OBJECTS) $(SHAREDSWITCH)lib$@.so $(LIBS) -o lib$@.so
	cp $@ $(BIN_DIR)/
	cp $@ $(LIB_DIR)/

# Root stuff
DEPENDENCIES = MBSorter.hh \
               RootLinkDef.h

mb_sorter_dict.o: mb_sorter_dict.cc
	$(CPP) $(CFLAGS) $(INCLUDES) -c $<

mb_sorter_dict.cc: $(DEPENDENCIES)
	$(ROOTDICT) -f $@ -c $(INCLUDES) -p $(DEPENDENCIES)
	cp $(basename $@)$(DICTEXT) $(LIB_DIR)/
	cp $(basename $@)$(DICTEXT) $(BIN_DIR)/


clean:
	rm -f *.o *$(DICTEXT) *_dict.cc

