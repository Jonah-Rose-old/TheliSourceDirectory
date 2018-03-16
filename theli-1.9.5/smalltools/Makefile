#
# Generic Makefile 
#
all: 
	$(MAKE) everything
everything: $(OBJSUB) airmass dfits hierarch28 replacekey fitsort \
            readlink caldate install

include $(PIPELIB)/Makefile
#
INCLUDES =
LIBS = -lm
#
AIRMASSSRC=airmass.c 
AIRMASSOBJ=$(OBJSUB)/airmass.o 
DFITSSRC=dfits.c 
DFITSOBJ=$(OBJSUB)/dfits.o
IOFITSSRC=iofits.c 
IOFITSOBJ=$(OBJSUB)/iofits.o
HIERARCHSRC=hierarch28.c 
HIERARCHOBJ=$(OBJSUB)/hierarch28.o
REPLACESRC=replacekey.c 
REPLACEOBJ=$(OBJSUB)/replacekey.o
FITSORTSRC=fitsort.c 
FITSORTOBJ=$(OBJSUB)/fitsort.o
READLINKSRC=readlink.c 
READLINKOBJ=$(OBJSUB)/readlink.o
NIGHTSRC=nightid.c 
NIGHTOBJ=$(OBJSUB)/nightid.o
MJDSRC=mjd.c 
MJDOBJ=$(OBJSUB)/mjd.o
LSTSRC=lst.c 
LSTOBJ=$(OBJSUB)/lst.o
CALSRC=caldate.c 
CALOBJ=$(OBJSUB)/caldate.o

ALL_EXECS=$(OBJSUB)/airmass   $(OBJSUB)/hierarch28 $(OBJSUB)/replacekey \
          $(OBJSUB)/fitsort   $(OBJSUB)/dfits $(OBJSUB)/readlink \
          $(OBJSUB)/nightid   $(OBJSUB)/mjd $(OBJSUB)/lst \
          $(OBJSUB)/caldate   $(OBJSUB)/iofits

ALL_SOURCES=$(AIRMASSSRC) $(DFITSSRC) $(HIERARCHSRC) $(REPLACESRC) \
            $(FITSORTSRC) $(READLINKSRC) $(NIGHTSRC) $(LSTSRC) \
            $(MJDSRC) $(CALSRC) $(IOFITSSRC)

ALL_OBJECTS=$(AIRMASSOBJ) $(DFITSOBJ) $(HIERARCHOBJ) $(REPLACEOBJ)\
            $(FITSORTOBJ) $(READLINKOBJ) $(NIGHTOBJ) $(LSTOBJ) \
            $(MJDOBJ) $(CALOBJ) $(IOFITSOBJ) 

airmass: $(OBJSUB)/airmass
	@echo

dfits: $(OBJSUB)/dfits
	@echo

iofits: $(OBJSUB)/iofits
	@echo

hierarch28: $(OBJSUB)/hierarch28
	@echo

replacekey: $(OBJSUB)/replacekey
	@echo

fitsort: $(OBJSUB)/fitsort
	@echo

readlink: $(OBJSUB)/readlink
	@echo

nightid: $(OBJSUB)/nightid
	@echo

lst: $(OBJSUB)/lst
	@echo

mjd: $(OBJSUB)/mjd
	@echo

caldate: $(OBJSUB)/caldate
	@echo

$(OBJSUB)/airmass: $(AIRMASSOBJ)
	$(LINK.c) -o $(OBJSUB)/airmass $(AIRMASSOBJ) $(LIBS)

$(OBJSUB)/dfits: $(DFITSOBJ)
	$(LINK.c) -o $(OBJSUB)/dfits $(DFITSOBJ) $(LIBS)

$(OBJSUB)/iofits: $(IOFITSOBJ)
	$(LINK.c) -o $(OBJSUB)/iofits $(IOFITSOBJ) $(LIBS)

$(OBJSUB)/hierarch28: $(HIERARCHOBJ)
	$(LINK.c) -o $(OBJSUB)/hierarch28 $(HIERARCHOBJ) $(LIBS)

$(OBJSUB)/replacekey: $(REPLACEOBJ)
	$(LINK.c) -o $(OBJSUB)/replacekey $(REPLACEOBJ) $(LIBS)

$(OBJSUB)/fitsort: $(FITSORTOBJ)
	$(LINK.c) -o $(OBJSUB)/fitsort $(FITSORTOBJ) $(LIBS)

$(OBJSUB)/readlink: $(READLINKOBJ)
	$(LINK.c) -o $(OBJSUB)/readlink $(READLINKOBJ) $(LIBS)

$(OBJSUB)/nightid: $(NIGHTOBJ)
	$(LINK.c) -o $(OBJSUB)/nightid $(NIGHTOBJ) $(LIBS)

$(OBJSUB)/mjd: $(MJDOBJ)
	$(LINK.c) -o $(OBJSUB)/mjd $(MJDOBJ) $(LIBS)

$(OBJSUB)/lst: $(LSTOBJ)
	$(LINK.c) -o $(OBJSUB)/lst $(LSTOBJ) $(LIBS)

$(OBJSUB)/caldate: $(CALOBJ)
	$(LINK.c) -o $(OBJSUB)/caldate $(CALOBJ) $(LIBS)

$(OBJSUB): 
	-mkdir $(OBJSUB)

install: $(ALL_EXECS)
	-cp $(OBJSUB)/airmass      $(PIPEBIN)/$(EXT)
	-cp $(OBJSUB)/dfits        $(PIPEBIN)/$(EXT)
	-cp $(OBJSUB)/iofits       $(PIPEBIN)/$(EXT)
	-cp $(OBJSUB)/hierarch28   $(PIPEBIN)/$(EXT)
	-cp $(OBJSUB)/replacekey   $(PIPEBIN)/$(EXT)
	-cp $(OBJSUB)/fitsort      $(PIPEBIN)/$(EXT)
	-cp $(OBJSUB)/readlink     $(PIPEBIN)/$(EXT)
	-cp $(OBJSUB)/nightid      $(PIPEBIN)/$(EXT)
	-cp $(OBJSUB)/mjd          $(PIPEBIN)/$(EXT)
	-cp $(OBJSUB)/lst          $(PIPEBIN)/$(EXT)
	-cp $(OBJSUB)/caldate      $(PIPEBIN)/$(EXT)


$(ALL_OBJECTS): $(OBJSUB) $(ALL_SOURCES)
	$(COMPILE.c) -o $@ $(@F:.o=.c)

clean:
	rm -f $(OBJSUB)/*.o

very_clean: 
	rm -f $(OBJSUB)/*.o *.bak core

export:
	tar cvf $(TARFILE) $(ALL_SOURCES) Makefile






