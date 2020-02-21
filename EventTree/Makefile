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

PLATFORM:=$(shell uname)
ifeq ($(PLATFORM),Darwin)
SHAREDSWITCH = -Qunused-arguments -shared -undefined dynamic_lookup -dynamiclib -Wl,-install_name,'@executable_path/../lib/'# NO ENDING SPACE
else
export __LINUX__:= 1
LINKFLAGS_PREFIX += -Wl,--no-as-needed
SHAREDSWITCH = -shared -Wl,-soname,# NO ENDING SPACE
endif

O_FILES = mbevts.o \
		  trevts.o

all: MKDIR_P libmbevts.so libtrevts.so

MKDIR_P:
	mkdir -p $(BIN_DIR) $(LIB_DIR)

lib%.so: %.o %Dict.o
	$(AR) cru $@ $*.o $*Dict.o
	cp $@ $(LIB_DIR)/

%.o: %.cc %.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

%Dict.o: %Dict.cc
	$(CPP) -c $(CFLAGS) $(INCLUDES) -o $@ $<

%Dict.cc: %.hh RootLinkDef.h
	rm -f $@ $*Dict$(DICTEXT)
	$(ROOTDICT) -f $@ -c $(INCLUDES) -p $<
	cp $*Dict$(DICTEXT) $(LIB_DIR)/
	cp $*Dict$(DICTEXT) $(BIN_DIR)/


#--------------------------------------------------
clean:
	rm -f *.o lib*.so *Dict$(DICTEXT) *Dict*cc
