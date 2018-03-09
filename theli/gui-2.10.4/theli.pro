TEMPLATE	= app
LANGUAGE	= C++

CONFIG	+= qt warn_on release

HEADERS	+= functions.h \
	section_ap.h \
	section_cal.h \
	section_cc.h \
	section_pre.h \
	section_sf.h \
	section_w.h \
	globalvariables.h \
	logfile.h

SOURCES	+= theli.cpp

FORMS	= theliform.ui \
	animate.ui \
	cni_form.ui \
	color_form.ui \
	combineimages.ui \
	commhist.ui \
	imalyzer.ui \
	manualsky.ui \
	parameters.ui \
	scamp_mosaictype.ui \
	statistics.ui \
	pmonitor.ui \
	zp.ui

IMAGES	= images/filenew \
	images/fileopen \
	images/filesave \
	images/print \
	images/undo \
	images/redo \
	images/editcut \
	images/editcopy \
	images/editpaste \
	images/searchfind \
	images/guiback.png \
	images/pdficon.png \
	images/guiback_blue.png \
	images/guiback_green.png \
	images/guiback_red.png \
	images/redoarrow.png \
	images/redoarrow_inactive.png \
	images/collapse.png \
	images/collapse_x.png \
	images/magnifyer.png

QMAKE_LFLAGS_RELEASE = -Xlinker -zmuldefs

unix {
  UI_DIR = .ui
  MOC_DIR = .moc
  OBJECTS_DIR = .obj
}
