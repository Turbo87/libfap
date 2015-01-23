# $Id: Makefile.am 111 2009-10-05 11:00:50Z oh2gve $

# Real men use this.
AM_CFLAGS = -Wall -Werror

# Set library version info from configure.ac
libfap_la_LDFLAGS= -version-info $(FAP_LIBRARY_VERSION)


# Include the library.
lib_LTLIBRARIES = libfap.la
libfap_la_SOURCES = fap.h fap.c regs.h helpers.h helpers.c helpers2.h helpers2.c

# Include header file.
library_includedir=$(includedir)
library_include_HEADERS = fap.h


# Specify test app.
check_PROGRAMS = smoketest
smoketest_SOURCES = smoketest.c fap.h
smoketest_LDADD = -lfap
EXTRA_DIST = smoketest.data
# Run the test app automagically on make check.
TESTS = $(check_PROGRAMS)
