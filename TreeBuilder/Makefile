.EXPORT_ALL_VARIABLES:

.PHONY: clean all

BIN_DIR    = ../bin
LIB_DIR    = ../lib
COMMON_DIR = ../common
MBEVTS_DIR = ../EventTree
MED2R_DIR = ../MedToRoot

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTVER      := $(shell root-config --version | head -c1)

ifeq ($(ROOTVER),5)
	ROOTDICT  := rootcint
	DICTEXT   := .h
else
	ROOTDICT  := rootcling
	DICTEXT   := _rdict.pcm
endif

LIBS      = -lm $(ROOTLIBS) -L$(LIB_DIR) -lCommandLineInterface -lMedToRoot -lmbevts -ltrevts

AR        = ar
CC        = $(shell root-config --cc)
CPP       = $(shell root-config --cxx)

CFLAGS    = -pipe -Wall -W -Woverloaded-virtual $(ROOTCFLAGS) -fPIC

INCLUDES  = -I$(COMMON_DIR) -I$(MBEVTS_DIR) -I$(MED2R_DIR) -I./
LFLAGS    = -g

PLATFORM:=$(shell uname)
ifeq ($(PLATFORM),Darwin)
SHAREDSWITCH = -Qunused-arguments -shared -undefined dynamic_lookup -dynamiclib -Wl,-install_name,'@executable_path/../lib/'# NO ENDING SPACE
else
export __LINUX__:= 1
LINKFLAGS_PREFIX += -Wl,--no-as-needed
SHAREDSWITCH = -shared -Wl,-soname,# NO ENDING SPACE
endif

O_FILES = Calibration.o \
		MBGeometry.o \
		AddBack.o \
		ParticleFinder.o \
		ParticleGammaTree.o

all: MKDIR_P TreeBuilder NeSort AQ4Sort

MKDIR_P:
	mkdir -p $(BIN_DIR) $(LIB_DIR)

TreeBuilder: TreeBuilder.cc $(O_FILES) TreeBuilderDictionary.o
	$(CPP) $(CFLAGS) $(INCLUDES) TreeBuilder.cc $(O_FILES) TreeBuilderDictionary.o $(LIBS) -o $@
	$(AR) cru $(LIB_DIR)/libTreeBuilder.so $(O_FILES) TreeBuilderDictionary.o
	cp TreeBuilder $(BIN_DIR)/

NeSort: NeSort.cc Calibration.o NeSortDictionary.o
	$(CPP) $(CFLAGS) $(INCLUDES) NeSort.cc $(O_FILES) NeSortDictionary.o $(LIBS) -o $@
	$(AR) cru $(LIB_DIR)/libNeSort.so $(O_FILES) NeSortDictionary.o
	cp NeSort $(BIN_DIR)/

AQ4Sort: AQ4Sort.cc Calibration.o AQ4SortDictionary.o
	$(CPP) $(CFLAGS) $(INCLUDES) AQ4Sort.cc $(O_FILES) AQ4SortDictionary.o $(LIBS) -o $@
	$(AR) cru $(LIB_DIR)/libNeSort.so $(O_FILES) AQ4SortDictionary.o
	cp AQ4Sort $(BIN_DIR)/

%.o: %.cc %.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@


#---- Root stuff

DEPENDENCIES_BASIC = Calibration.hh

DEPENDENCIES = $(DEPENDENCIES_BASIC) \
				MBGeometry.hh \
				AddBack.hh \
				ParticleFinder.hh \
				ParticleGammaTree.hh

TreeBuilderDictionary.cc: $(DEPENDENCIES) RootLinkDef.h
	$(ROOTDICT) -f $@ -c $(INCLUDES) $(DEPENDENCIES) RootLinkDef.h
	cp $(basename $@)$(DICTEXT) $(LIB_DIR)/
	cp $(basename $@)$(DICTEXT) $(BIN_DIR)/

TreeBuilderDictionary.o: TreeBuilderDictionary.cc TreeBuilderDictionary$(DICTEXT) RootLinkDef.h
	$(CPP) -fPIC $(CFLAGS) $(INCLUDES) -c $< RootLinkDef.h

NeSortDictionary.cc: $(DEPENDENCIES_BASIC) RootLinkDef.h
	$(ROOTDICT) -f $@ -c $(INCLUDES) $(DEPENDENCIES) RootLinkDef.h
	cp $(basename $@)$(DICTEXT) $(LIB_DIR)/
	cp $(basename $@)$(DICTEXT) $(BIN_DIR)/

NeSortDictionary.o: NeSortDictionary.cc NeSortDictionary$(DICTEXT) RootLinkDef.h
	$(CPP) -fPIC $(CFLAGS) $(INCLUDES) -c $< RootLinkDef.h

AQ4SortDictionary.cc: $(DEPENDENCIES_BASIC) RootLinkDef.h
	$(ROOTDICT) -f $@ -c $(INCLUDES) $(DEPENDENCIES) RootLinkDef.h
	cp $(basename $@)$(DICTEXT) $(LIB_DIR)/
	cp $(basename $@)$(DICTEXT) $(BIN_DIR)/

AQ4SortDictionary.o: AQ4SortDictionary.cc AQ4SortDictionary$(DICTEXT) RootLinkDef.h
	$(CPP) -fPIC $(CFLAGS) $(INCLUDES) -c $< RootLinkDef.h

#--------------------------------------------------
clean:
	rm -f TreeBuilder NeSort AQ4Sort *.o
	rm -f TreeBuilderDictionary.cc TreeBuilderDictionary$(DICTEXT) 
	rm -f NeSortDictionary.cc NeSortDictionary$(DICTEXT) 
	rm -f AQ4SortDictionary.cc AQ4SortDictionary$(DICTEXT) 

