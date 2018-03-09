INCLUDE_DIRS := /usr/include/cfitsio /usr/include/CCfits /usr/local/include/cfitsio /usr/local/include/CCfits ./include
LIBRARY_DIRS :=
LIBRARIES    := CCfits cfitsio pthread

# add the include and library (dirs) to the makefile variables
CPPFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))
LDFLAGS  += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))
LDFLAGS  += $(foreach library,$(LIBRARIES),-l$(library))

# compiler flags
#CXXFLAGS += -Wall -Wextra -pedantic -g
CXXFLAGS += -std=c++0x -Wall -Wextra -pedantic -Wno-deprecated -g
BIN := bin

# i don't understand why i need this here, it should be automatically provided by CPPFLAGS.
# otherwise it doesn't build fitstools, myFITS and statistics
vpath %.h ./include
HEADERS=fitstools.h myFITS.h statistics.h histogram.h
OBJCOMMON=fitstools.o myFITS.o statistics.o histogram.o

SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)

EXECALL=$(SOURCES:.cc=)
NOEXEC=fitstools myFITS statistics histogram
EXEC=$(filter-out $(NOEXEC),$(EXECALL))

all: $(EXEC) fftw tiff

# note that make builds the objects automatically if missing!
$(EXEC): $(OBJECTS) $(OBJCOMMON) $(HEADERS)
	$(CXX) -o $(BIN)/$@ $@.o $(OBJCOMMON) $(LDFLAGS) $(CXXFLAGS)

fftw:
	$(MAKE) -C fftw

tiff:
	$(MAKE) -C tiff


.PHONY : all clean fftw tiff

clean :
	rm -f $(BIN)/* *.o fftw/*.o tiff/*.o
