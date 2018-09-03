# Makefile for g_clx analysis
.PHONY: clean all

ROOTCFLAGS	:= $(shell root-config --cflags)
ROOTINC		:= $(shell root-config --prefix)/include
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

INCLUDES  = -I$(MBEVTS_DIR) -I$(COMMON_DIR) -I$(ROOTINC) -I./

all: MKDIR_P TDRIVAna

MKDIR_P:
	mkdir -p $(BIN_DIR) $(LIB_DIR)

OBJECTS = g_clx.o \
          doppler.o \
          hists.o \
          MBGeometry.o \
          SpedeGeometry.o \
          TDRIVAnaDict.o
 
TDRIVAna: TDRIVAna.cc $(OBJECTS)
	$(CPP) $(CFLAGS) $(INCLUDES) $< $(OBJECTS) -o $@ $(LIBS)
	$(AR) cru $(LIB_DIR)/libTDRIVAna.a $(OBJECTS)
	cp TDRIVAna $(BIN_DIR)/

g_clx.o: g_clx.cc g_clx.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

doppler.o: doppler.cc doppler.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

hists.o: hists.cc hists.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

MBGeometry.o: MBGeometry.cc MBGeometry.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

SpedeGeometry.o: SpedeGeometry.cc SpedeGeometry.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f *.o
	rm -f TDRIVAnaDict.cc TDRIVAnaDict$(DICTEXT)

# Root stuff
DEPENDENCIES = g_clx.hh \
               doppler.hh \
               hists.hh \
               MBGeometry.hh \
               SpedeGeometry.hh \
               RootLinkDef.h

TDRIVAnaDict.o: TDRIVAnaDict.cc
	 $(CPP) $(CFLAGS) $(INCLUDES) -c $<

TDRIVAnaDict.cc: $(DEPENDENCIES)
	 $(ROOTDICT) -f $@ -c $(INCLUDES) $(DEPENDENCIES)
	 cp TDRIVAnaDict$(DICTEXT) $(LIB_DIR)/

