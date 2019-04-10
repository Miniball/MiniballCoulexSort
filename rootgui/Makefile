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
ANA_INC     := -I../CLXAna

CC       := $(shell root-config --cc)
CPP      := $(shell root-config --cxx)
CFLAGS   := $(ROOTCFLAGS) -Wall -fPIC -Wno-unused-variable -g

LIBS     := $(ROOTGLIBS) -L$(LIB_DIR) -lCLXAna -lTreeBuilder

INCLUDES := $(ANA_DIR)

all: MKDIR_P mb_sorter

MKDIR_P:
	mkdir -p $(BIN_DIR) $(LIB_DIR)

OBJECTS = MBSorter.o \
          mb_sorter_dict.o

MBSorter.o: MBSorter.cc
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

mb_sorter: mb_sorter.cc $(OBJECTS)
	$(CPP) $(CFLAGS) $(INCLUDES) $< $(OBJECTS) -o $@ $(LIBS)
	cp $@ $(BIN_DIR)

# Root stuff
DEPENDENCIES = MBSorter.hh \
               RootLinkDef.h

mb_sorter_dict.o: mb_sorter_dict.cc
	$(CPP) $(CFLAGS) $(INCLUDES) -c $<

mb_sorter_dict.cc: $(DEPENDENCIES)
	$(ROOTDICT) -f $@ -c $(INCLUDES) $(DEPENDENCIES)
	cp mb_sorter_dict$(DICTEXT) $(LIB_DIR)


clean:
	rm -f *.o *_dict$(DICTEXT) *_dict.cc

