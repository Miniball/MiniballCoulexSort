.EXPORT_ALL_VARIABLES:

.PHONY: clean all

ALLIBS  = -lm

LIB_DIR = ../lib

AR      = ar
CC      = $(shell root-config --cc)
CPP     = $(shell root-config --cxx)
CFLAGS  =  -O2 -pipe -Wall -W -Woverloaded-virtual

INCLUDES        = -I./

LFLAGS		=  -g 
LIBS 		= $(ALLIBS)

all: CommandLineInterface.o libCommandLineInterface.a
	@echo Done

lib%.a: %.o
	$(AR) cru $(LIB_DIR)/$@ $^

%.o: %.cc %.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

#--------------------------------------------------
clean:
	rm  -f *.o libCommandLineInterface.a
