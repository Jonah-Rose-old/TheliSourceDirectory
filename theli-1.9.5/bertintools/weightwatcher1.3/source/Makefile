# WeightWatcher V1.2
#
# general makefile
#
# E.BERTIN 06/11/1997
#
include $(PIPELIB)/$(EXT)/Macrodefs.$(EXT)
INCLUDES = -I$(PIPESOFT)/ldactools/library
LIBS  = -L. -lm
SOURCES = field.c fitsmisc.c fitsutil.c main.c \
          makeit.c misc.c prefs.c readimage.c vector.c

ALL   = $(OBJSUB)/field.o \
        $(OBJSUB)/fitsmisc.o \
        $(OBJSUB)/fitsutil.o \
        $(OBJSUB)/main.o \
	$(OBJSUB)/makeit.o \
        $(OBJSUB)/misc.o \
        $(OBJSUB)/prefs.o \
        $(OBJSUB)/readimage.o \
        $(OBJSUB)/vector.o
STDH  = define.h globals.h types.h
STDO  = 

ww: $(OBJSUB) $(ALL)
	$(CC) $(ALL) -o $(OBJSUB)/ww_theli $(LIBS) $(COPTS)

clean:
	$(RM) $(ALL) $(OBJSUB)/ww_theli

$(ALL) : $(SOURCES)
	$(CC) -c $(COPTS) $(INCLUDES) -o $@ $(@F:.o=.c)

$(OBJSUB):
	-mkdir $(OBJSUB)


