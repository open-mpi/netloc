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
        -I$(top_srcdir) \
        -DNETLOC_ABS_TOP_SRCDIR=\"$(abs_top_srcdir)/\"

check_PROGRAMS = \
	test_API \
	test_ETH_API \
	test_ETH_verbose \
	test_find_neighbors \
	test_metadata \
	test_conv \
	test_map \
	test_map_hwloc \
	hwloc_compress \
	lsmap \
	map_paths \
	netloc_hello \
	netloc_nodes \
	netloc_all

LDADD = $(top_builddir)/src/libnetloc.la

test_map_LDADD = $(LDADD) -lhwloc
lsmap_LDADD = $(LDADD) -lhwloc
map_paths_LDADD = $(LDADD) -lhwloc
