.EXPORT_ALL_VARIABLES:

.PHONY: clean all

ALLIBS  = -lm

LIB_DIR = ../lib

AR      = ar
CPP     = $(shell root-config --cxx)
CFLAGS  =  -fPIC -O2 -pipe -Wall -W -Woverloaded-virtual

INCLUDES	= -I./

LFLAGS		=  -g 
LIBS 		= $(ALLIBS)

PLATFORM:=$(shell uname)
ifeq ($(PLATFORM),Darwin)
SHAREDSWITCH = -Qunused-arguments -shared -undefined dynamic_lookup -dynamiclib -Wl,-install_name,'@executable_path/../lib/'# NO ENDING SPACE
else
export __LINUX__:= 1
LINKFLAGS_PREFIX += -Wl,--no-as-needed
SHAREDSWITCH = -shared -Wl,-soname,# NO ENDING SPACE
endif

all: MKDIR_P libCommandLineInterface.so

MKDIR_P:
	mkdir -p $(BIN_DIR) $(LIB_DIR)

lib%.so: %.o
	$(CPP) -fPIC $^ $(SHAREDSWITCH)$@ $(LIBS) -o $@
	cp $@ $(LIB_DIR)/

%.o: %.cc %.hh
	$(CPP) $(CFLAGS) $(INCLUDES) -c $< -o $@

#--------------------------------------------------
clean:
	rm  -f *.o libCommandLineInterface.so
