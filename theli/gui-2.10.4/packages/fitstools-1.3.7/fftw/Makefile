INCLUDE_DIRS := /usr/include/cfitsio/ /usr/include/CCfits/ /usr/local/include/cfitsio/ /usr/local/include/CCfits/ ./include ../include
LIBRARY_DIRS :=
LIBRARIES    := CCfits cfitsio fftw3 fftw3_threads gsl gslcblas

# add the include and library (dirs) to the makefile variables
CPPFLAGS += $(foreach includedir,$(INCLUDE_DIRS),-I$(includedir))
LDFLAGS  += $(foreach librarydir,$(LIBRARY_DIRS),-L$(librarydir))
LDFLAGS  += $(foreach library,$(LIBRARIES),-l$(library))

# compiler flags
CXXFLAGS += -std=c++0x -fopenmp -Wall -Wextra -pedantic -Wno-deprecated -O3 -g

BIN         = ../bin

vpath %.h ./include:../include
HEADERS=fitstools.h myFITS.h statistics.h convolve.h fftw++.h fitgauss1d.h histogram.h
OBJCOMMON=../fitstools.o ../myFITS.o ../statistics.o convolve.o fftw++.o fitgauss1d.o ../histogram.o

SOURCES=$(wildcard *.cc)
OBJECTS=$(SOURCES:.cc=.o)

EXECALL=$(SOURCES:.cc=)
NOEXEC=convolve fftw++ Array gaussian fitgauss1d
EXEC=$(filter-out $(NOEXEC),$(EXECALL))

# wee need this one to clean up
DELBIN += $(foreach executable,$(EXEC),$(BIN)/$(executable))

all: $(EXEC)

$(EXEC): $(OBJECTS) $(OBJCOMMON) $(HEADERS)
	$(CXX) -o $(BIN)/$@ $@.o $(OBJCOMMON) $(LDFLAGS) $(CXXFLAGS)

.PHONY : all clean

clean :
	rm -f $(DELBIN) *.o
