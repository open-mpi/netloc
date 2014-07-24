# Copyright (c) 2013-2014 Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2013      University of Wisconsin-La Crosse.
#                         All rights reserved.
#
# See COPYING in top-level directory.
#
# $HEADER$
#

AM_CPPFLAGS = \
        $(JANSSON_CPPFLAGS) \
        -I$(top_builddir)/include \
        -I$(top_srcdir)/include

lib_LTLIBRARIES = libnetloc.la

libnetloc_la_SOURCES = \
        support.h \
	topology.c \
	metadata.c \
	api.c \
	support.c \
	data_types.c \
	data_collect.c \
	pathfinder.c \
	lookup_table.c \
	export.c \
        map.c

libnetloc_la_LDFLAGS = $(JANSSON_LDFLAGS)
libnetloc_la_LIBADD = \
        -lhwloc $(JANSSON_LIB) $(top_builddir)/jansson/src/libjansson.la
