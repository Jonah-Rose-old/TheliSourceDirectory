INCLUDE_DIRS := /usr/include/cfitsio/ /usr/include/CCfits/ /usr/local/include/cfitsio/ /usr/local/include/CCfits/ ../include
LIBRARY_DIRS :=
LIBRARIES    := CCfits cfitsio tiff

# add the include and library (dirs) to the makefile variables
CPPFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))
LDFLAGS  += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))
LDFLAGS  += $(foreach library,$(LIBRARIES),-l$(library))

# compiler flags
CXXFLAGS += -Wall -Wextra -pedantic -O3

BIN         = ../bin

vpath %.h ../include
HEADERS=fitstools.h myFITS.h statistics.h
OBJCOMMON=../fitstools.o ../myFITS.o ../statistics.o

SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)
EXEC=$(SOURCES:.cc=)

# wee need this one to clean up
DELBIN += $(foreach executable,$(EXEC),$(BIN)/$(executable))

all: $(EXEC)

$(EXEC): $(OBJECTS) $(OBJCOMMON) $(HEADERS)
	$(CXX) -o $(BIN)/$@ $@.o $(OBJCOMMON) $(LDFLAGS) $(CFLAGS)

.PHONY : all clean

clean :
	rm -f $(DELBIN) *.o
