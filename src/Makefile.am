## Process this file with automake to produce Makefile.in

bin_PROGRAMS = rcg2data

rcg2data_SOURCES = \
	options.cpp \
	action_event.cpp \
	field_model.cpp \
	field_state.cpp \
	game_analyzer.cpp \
	rcg_reader.cpp \
	main.cpp

noinst_HEADERS = \
	abstract_printer.h \
	action_event.h \
	field_model.h \
	field_state.h \
	game_analyzer.h \
	options.h \
	rcg_reader.h

rcg2data_CPPFLAGS = -I$(top_srcdir)
rcg2data_CXXFLAGS = -W -Wall
rcg2data_LDFLAGS =
rcg2data_LDADD =

AM_CPPFLAGS =
AM_CFLAGS = -W -Wall
AM_CXXFLAGS = -W -Wall
AM_LDFLAGS =

EXTRA_DIST = CMakeLists.txt

CLEANFILES = *~
