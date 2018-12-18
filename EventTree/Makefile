.EXPORT_ALL_VARIABLES:

.PHONY: clean all

BIN_DIR    = ../bin
LIB_DIR    = ../lib

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTLDFLAGS  := $(shell root-config --ldflags)
ROOTVER      := $(shell root-config --version | head -c1)

ifeq ($(ROOTVER),5)
	ROOTDICT  := rootcint
	DICTEXT   := .h
else
	ROOTDICT  := rootcling
	DICTEXT   := _rdict.pcm
endif

LIBS      = -lm $(ROOTLIBS) -L$(LIB_DIR)

AR        = ar
CC        = $(shell root-config --cc)
CPP       = $(shell root-config --cxx)

CFLAGS    = -O2 -pipe -Wall -W -Woverloaded-virtual $(ROOTCFLAGS) -fPIC

INCLUDES  = -I./
LFLAGS    = -O2 -g $(ROOTLDFLAGS)

O_FILES = mbevts.o trevts.o

all: MKDIR_P libmbevts.a libtrevts.a

MKDIR_P:
	mkdir -p $(BIN_DIR) $(LIB_DIR)

libmbevts.a: mbevts.o mbevtsDict.o
	$(AR) cru $@ mbevts.o mbevtsDict.o
	cp $@ $(LIB_DIR)/

libtrevts.a: trevts.o trevtsDict.o
	$(AR) cru $@ trevts.o trevtsDict.o
	cp $@ $(LIB_DIR)/

mbevts.o: mbevts.cc mbevts.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c mbevts.cc -o mbevts.o

trevts.o: trevts.cc trevts.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c mbevts.cc -o trevts.o

mbevtsDict.o: mbevtsDict.cc
	$(CPP) -c $(CFLAGS) $(INCLUDES) -o mbevtsDict.o mbevtsDict.cc

trevtsDict.o: trevtsDict.cc
	$(CPP) -c $(CFLAGS) $(INCLUDES) -o trevtsDict.o trevtsDict.cc

mbevtsDict.cc: mbevts.hh RootLinkDef.h
	rm -f mbevtsDict.cc mbevtsDict$(DICTEXT)
	$(ROOTDICT) -f mbevtsDict.cc -c $(INCLUDES) -p $^
	cp mbevtsDict$(DICTEXT) $(LIB_DIR)/

trevtsDict.cc: trevts.hh RootLinkDef.h
	rm -f trevtsDict.cc trevtsDict$(DICTEXT)
	$(ROOTDICT) -f trevtsDict.cc -c $(INCLUDES) -p $^
	cp trevtsDict$(DICTEXT) $(LIB_DIR)/


#--------------------------------------------------
clean:
	rm -f *.o lib*.a *Dict$(DICTEXT) *Dict*cc
