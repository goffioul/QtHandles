#!/bin/bash

ROOT="`octave-config -p OCTFILEDIR`"
if test -z "$ROOT"; then
	echo "octave installation not found"
	exit 1
fi
DESTDIR="$ROOT/QtHandles"
test -d "$DESTDIR" || mkdir -p "$DESTDIR"
cp __init_qt__.oct PKG_ADD COPYING "$DESTDIR"
test -f octave-qt.exe && cp octave-qt.exe "`octave-config -p BINDIR`"
test -f octave-qt && cp octave-qt "`octave-config -p BINDIR`"
