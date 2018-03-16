VERSION  = 1.0
CC       = gcc
CFLAGS   = -Wall -g -O3 -D_GNU_SOURCE
LDFLAGS  = -lm

#############################################

INCPATH  = -I./include
BIN      = bin

GENOPT  = $(INCPATH) $(LDFLAGS) $(CFLAGS)
DCRAWOPT = -O4 -lm -DNODEPS -w

####### Files

HEADERS = include/stuff.h
SOURCES = get_shift.c \
	  cleanstring.c \
	  get_cdmatrix.c \
	  get_pixscale.c\
	  get_rotimsize.c \
	  sky2xy_helper.c \
	  rename.c \
	  numcpu.c \
	  mergedat.c \
	  mjd.c \
	  lst.c \
	  getstats.c \
	  imalyzer.c \
	  errtest.c \
	  decsexconv.c \
	  dcraw.c \
	  extinction.c \
	  siidensitometry.c \
	  atm_extinction.c \
	  make_cdmatrix.c \
	  get_refcat_radius.c \
	  vac2air.c \
	  background_dynamic.c \
	  background_static.c \
	  e2ell.c

EXECS   = get_shift \
	  cleanstring \
	  get_cdmatrix \
	  get_pixscale \
	  get_rotimsize \
	  sky2xy_helper \
	  rename \
	  numcpu \
	  mergedat \
	  mjd \
	  lst \
	  getstats \
	  imalyzer \
	  errtest \
	  decsexconv \
	  dcraw \
	  extinction \
	  siidensitometry \
	  atm_extinction \
	  make_cdmatrix \
	  get_refcat_radius \
	  vac2air \
	  background_dynamic \
	  background_static \
	  e2ell

all: $(SOURCES) $(EXECS)

get_shift : get_shift.c
	$(CC) get_shift.c -o $(BIN)/get_shift $(GENOPT)

get_refcat_radius : get_refcat_radius.c
	$(CC) get_refcat_radius.c -o $(BIN)/get_refcat_radius $(GENOPT)

cleanstring : cleanstring.c
	$(CC) cleanstring.c -o $(BIN)/cleanstring $(GENOPT)

get_cdmatrix : get_cdmatrix.c
	$(CC) get_cdmatrix.c -o $(BIN)/get_cdmatrix $(GENOPT)

make_cdmatrix : make_cdmatrix.c
	$(CC) make_cdmatrix.c -o $(BIN)/make_cdmatrix $(GENOPT)

get_pixscale : get_pixscale.c
	$(CC) get_pixscale.c -o $(BIN)/get_pixscale $(GENOPT)

get_rotimsize : get_rotimsize.c
	$(CC) get_rotimsize.c -o $(BIN)/get_rotimsize $(GENOPT)

sky2xy_helper : sky2xy_helper.c
	$(CC) sky2xy_helper.c -o $(BIN)/sky2xy_helper $(GENOPT)

decsexconv : decsexconv.c
	$(CC) decsexconv.c -o $(BIN)/decsexconv $(GENOPT)

errtest : errtest.c
	$(CC) errtest.c -o $(BIN)/errtest $(GENOPT)

rename : rename.c
	$(CC) rename.c -o $(BIN)/rename $(GENOPT)

numcpu: numcpu.c
	$(CC) numcpu.c -o $(BIN)/numcpu $(GENOPT)

mergedat: mergedat.c
	$(CC) mergedat.c -o $(BIN)/mergedat $(GENOPT)

mjd : mjd.c
	$(CC) mjd.c -o $(BIN)/mjd $(GENOPT)

lst : lst.c
	$(CC) lst.c -o $(BIN)/lst $(GENOPT)

getstats : getstats.c
	$(CC) getstats.c -o $(BIN)/getstats $(GENOPT)

imalyzer : imalyzer.c
	$(CC) imalyzer.c -o $(BIN)/imalyzer $(GENOPT)

vac2air : vac2air.c
	$(CC) vac2air.c -o $(BIN)/vac2air $(GENOPT)

background_dynamic : background_dynamic.c
	$(CC) background_dynamic.c -o $(BIN)/background_dynamic $(GENOPT)

background_static : background_static.c
	$(CC) background_static.c -o $(BIN)/background_static $(GENOPT)

extinction : extinction.c
	$(CC) extinction.c -o $(BIN)/extinction $(GENOPT) -lgsl -lgslcblas

siidensitometry : siidensitometry.c
	$(CC) siidensitometry.c -o $(BIN)/siidensitometry $(GENOPT) -lgsl -lgslcblas

atm_extinction : atm_extinction.c
	$(CC) atm_extinction.c -o $(BIN)/atm_extinction $(GENOPT) -lgsl -lgslcblas

dcraw : dcraw.c
	$(CC) dcraw.c -o $(BIN)/dcraw $(DCRAWOPT)

e2ell : e2ell.c
	$(CC) e2ell.c -o $(BIN)/e2ell $(GENOPT)

# : .c
#	$(CC) .c -o $(BIN)/ $(GENOPT)

.PHONY : clean
clean :
	rm $(BIN)/*
