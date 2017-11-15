# Makefile for g_clx analysis
.PHONY: clean all

ROOTCFLAGS	:= $(shell root-config --cflags)
ROOTLIBS    := $(shell root-config --libs)
ROOTVER     := $(shell root-config --version | head -c1)

ifeq ($(ROOTVER),5)
	ROOTDICT  := rootcint
	DICTEXT   := .h
else
	ROOTDICT  := rootcling
	DICTEXT   := _rdict.pcm
endif


BIN_DIR    = ../bin
LIB_DIR     = ../lib
COMMON_DIR = ../common
MBEVTS_DIR  = ../mbevts

LIBS        := $(ROOTLIBS) -L$(LIB_DIR) -lCommandLineInterface

AR          := ar
CC          := $(shell root-config --cc)
CPP         := $(shell root-config --cxx)
CFLAGS      := -pedantic -Wall -g -O3 $(ROOTCFLAGS) -fPIC -Wno-unused-variable -Wno-long-long

INCLUDES  = -I$(MBEVTS_DIR) -I$(COMMON_DIR) -I./

all: TDRIVAna

OBJECTS = g_clx.o \
          $(MBEVTS_DIR)/mbevts.o \
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
	$(CPP) $(CFLAGS) -c $< -o $@

$(MBEVTS_DIR)/mbevts.o: $(MBEVTS_DIR)/mbevts.cc $(MBEVTS_DIR)/mbevts.hh
	$(CPP) $(CFLAGS) -c $< -o $@

doppler.o: doppler.cc doppler.hh
	$(CPP) $(CFLAGS) -c $< -o $@

hists.o: hists.cc hists.hh
	$(CPP) $(CFLAGS) -c $< -o $@

MBGeometry.o: MBGeometry.cc MBGeometry.hh
	$(CPP) $(CFLAGS) -c $< -o $@

SpedeGeometry.o: SpedeGeometry.cc SpedeGeometry.hh
	$(CPP) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(MBEVTS_DIR)/*.o *.o
	rm -f TDRIVAnaDict.cc TDRIVAnaDict$(DICTEXT)

# Root stuff
DEPENDENCIES = g_clx.hh \
               $(MBEVTS_DIR)/mbevts.hh \
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

