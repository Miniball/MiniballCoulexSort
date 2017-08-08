.EXPORT_ALL_VARIABLES:

.PHONY: clean all

BIN_DIR    = ../bin
LIB_DIR    = ../lib
COMMON_DIR = ../common
MED_DIR    = ./med
MBEVTS_DIR = ../mbevts

ROOTCFLAGS   := $(shell root-config --cflags)
ROOTLIBS     := $(shell root-config --libs)
ROOTINC      := $(shell root-config --incdir)
ROOTVER      := $(shell root-config --version | head -c1)

ifeq ($(ROOTVER),5)
	ROOTDICT  := rootcint
	DICTEXT   := .h
else
	ROOTDICT  := rootcling
	DICTEXT   := _rdict.pcm
endif

LIBS      = -lm $(ROOTLIBS) -L$(LIB_DIR) -lCommandLineInterface

AR        = ar
CC        = $(shell root-config --cc)
CPP       = $(shell root-config --cxx)

CFLAGS    = -O2 -pipe -Wall -W -Woverloaded-virtual $(ROOTCFLAGS) -fPIC

INCLUDES  = -I$(MED_DIR)/inc -I$(COMMON_DIR) -I$(MBEVTS_DIR) -I./
LFLAGS    = -O2 -g

O_FILES = BuiltEvent.o \
		Datas.o \
        EventBuffer.o \
        EventBuilder.o \
        GlobalSettings.o \
        Modules.o \
        SubEvents.o \
        UnpackedEvent.o \
        $(MED_DIR)/mbsio.o \
        $(MED_DIR)/byte_order.o \
		$(COMMON_DIR)/CommandLineInterface.o

all: MedToRoot

MedToRoot: MedToRoot.cc $(O_FILES) MedToRootDictionary.o
	$(CPP) $(CFLAGS) $(INCLUDES) MedToRoot.cc $(O_FILES) MedToRootDictionary.o $(LIBS) -o $@
	$(AR) cru $(LIB_DIR)/libMedToRoot.a $(O_FILES) MedToRootDictionary.o
	cp MedToRoot $(BIN_DIR)/

$(MED_DIR)/mbsio.o: $(MED_DIR)/src/mbsio.c $(MED_DIR)/inc/mbsio.h
	$(CC) -I$(ROOTINC) $(INCLUDES) -c $< -o $@

$(MED_DIR)/byte_order.o: $(MED_DIR)/src/byte_order.c $(MED_DIR)/inc/byte_order.h 
	$(CC) -I$(ROOTINC) $(INCLUDES) -c $< -o $@

$(COMMON_DIR)/CommandLineInterface.o:
	@cd $(COMMON_DIR); make

%.o: %.cc %.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@


#---- Root stuff

DEPENDENCIES = BuiltEvent.hh \
			Datas.hh \
			EventBuffer.hh \
			EventBuilder.hh \
			GlobalSettings.hh \
			Modules.hh \
			SubEvents.hh \
			UnpackedEvent.hh \
			RootLinkDef.h

MedToRootDictionary.cc: $(DEPENDENCIES)
	rm -f MedToRootDictionary.cc MedToRootDictionary$(DICTEXT)
	$(ROOTDICT) -f MedToRootDictionary.cc -c $(INCLUDES) $(DEPENDENCIES)
	cp MedToRootDictionary$(DICTEXT) $(LIB_DIR)/

MedToRootDictionary.o: MedToRootDictionary.cc MedToRootDictionary$(DICTEXT)
	$(CPP) -fPIC $(CFLAGS) $(INCLUDES) -c $<

#--------------------------------------------------
clean:
	rm -f MedToRoot *.o $(MED_DIR)/*.o $(MBEVTS_DIR)/*.o
	rm -f MedToRootDictionary.cc MedToRootDictionary$(DICTEXT) 
	@cd $(COMMON_DIR); make clean

