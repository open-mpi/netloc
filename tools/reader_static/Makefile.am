# Copyright (c) 2013-2014 Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2013-2014 University of Wisconsin-La Crosse.
#                         All rights reserved.
#
# See COPYING in top-level directory.
#
# $HEADER$
#

AM_CPPFLAGS = \
        $(JANSSON_CPPFLAGS) \
        -I$(top_builddir)/include \
        -I$(top_srcdir)/include \
        -I$(top_srcdir)

bin_PROGRAMS = \
	netloc_reader_static

netloc_reader_static_SOURCES = \
	netloc_reader_static.c

netloc_reader_static_LDADD = \
	$(top_builddir)/src/libnetloc.la

