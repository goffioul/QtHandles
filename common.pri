win32:CONFIG += release warn_off
!win32:CONFIG += debug

QMAKE_CXXFLAGS += $$system(mkoctfile -p ALL_CXXFLAGS)
win32 {
	#QMAKE_CXXFLAGS_WARN_ON -= -W3
	DEFINES -= WIN32
	QMAKE_CXXFLAGS += -MP
	QMAKE_CXXFLAGS -= -g
	QMAKE_CXXFLAGS_RELEASE += -Zi
	QMAKE_CXXFLAGS_RELEASE -= -O2
	QMAKE_LFLAGS_RELEASE += -debug
	QMAKE_CLEAN += $(DESTDIR)$(QMAKE_TARGET).lib $(DESTDIR)$(QMAKE_TARGET).pdb
}
QMAKE_LIBDIR += $$system(octave-config -p OCTLIBDIR)
QMAKE_RPATHDIR += $$system(octave-config -p OCTLIBDIR)
LIBS += $$system(mkoctfile -p OCTAVE_LIBS)
