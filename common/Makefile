.EXPORT_ALL_VARIABLES:

.PHONY: clean all

ALLIBS  = -lm

LIB_DIR = ../lib

AR      = ar
CPP     = $(shell root-config --cxx)
CFLAGS  =  -O2 -pipe -Wall -W -Woverloaded-virtual

INCLUDES	= -I./

LFLAGS		=  -g 
LIBS 		= $(ALLIBS)

all: MKDIR_P libCommandLineInterface.a

MKDIR_P:
	mkdir -p $(BIN_DIR) $(LIB_DIR)

lib%.a: %.o
	$(AR) cru $@ $^
	cp $@ $(LIB_DIR)/

%.o: %.cc %.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

#--------------------------------------------------
clean:
	rm  -f *.o libCommandLineInterface.a
