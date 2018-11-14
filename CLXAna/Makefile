# Makefile for g_clx analysis
.PHONY: clean all

ROOTCFLAGS	:= $(shell root-config --cflags)
ROOTLIBS	:= $(shell root-config --libs)
ROOTVER		:= $(shell root-config --version | head -c1)

ifeq ($(ROOTVER),5)
	ROOTDICT  := rootcint
	DICTEXT   := .h
else
	ROOTDICT  := rootcling
	DICTEXT   := _rdict.pcm
endif


BIN_DIR     = ../bin
LIB_DIR     = ../lib
COMMON_DIR  = ../common
MBEVTS_DIR  = ../EventTree

LIBS        := $(ROOTLIBS) -L$(LIB_DIR) -lCommandLineInterface -lmbevts

AR          := ar
CC          := $(shell root-config --cc)
CPP         := $(shell root-config --cxx)
CFLAGS      := -pedantic -Wall -g -O3 $(ROOTCFLAGS) -fPIC -Wno-unused-variable -Wno-long-long

INCLUDES  = -I$(MBEVTS_DIR) -I$(COMMON_DIR) -I./

all: MKDIR_P CLXAna

MKDIR_P:
	mkdir -p $(BIN_DIR) $(LIB_DIR)

OBJECTS = g_clx.o \
          doppler.o \
          hists.o \
          SpedeGeometry.o \
          CLXAnaDict.o
 
CLXAna: CLXAna.cc $(OBJECTS)
	$(CPP) $(CFLAGS) $(INCLUDES) $< $(OBJECTS) -o $@ $(LIBS)
	$(AR) cru $(LIB_DIR)/libCLXAna.a $(OBJECTS)
	cp CLXAna $(BIN_DIR)/

%.o: %.cc %.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f *.o
	rm -f CLXAnaDict.cc CLXAnaDict$(DICTEXT)

# Root stuff
DEPENDENCIES = g_clx.hh \
               doppler.hh \
               hists.hh \
               SpedeGeometry.hh \
               RootLinkDef.h

CLXAnaDict.o: CLXAnaDict.cc
	 $(CPP) $(CFLAGS) $(INCLUDES) -c $<

CLXAnaDict.cc: $(DEPENDENCIES)
	 $(ROOTDICT) -f $@ -c $(INCLUDES) $(DEPENDENCIES)
	 cp CLXAnaDict$(DICTEXT) $(LIB_DIR)/

