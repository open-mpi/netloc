# Copyright © 2014 Cisco Systems, Inc.  All rights reserved.
# Copyright (c) 2014      University of Wisconsin-La Crosse.
#                         All rights reserved.
#
# See COPYING in top-level directory.
#
# $HEADER$
#

# Note that the -I directory must *exactly* match what was specified
# via AC_CONFIG_MACRO_DIR in configure.ac.
ACLOCAL_AMFLAGS = -Iconfig

SUBDIRS = include src tools tests

EXTRA_DIST = \
        config/netloc_get_version.sh \
        README VERSION COPYING AUTHORS

# We need doc/ if NETLOC_BUILD_DOXYGEN, or during make install if NETLOC_INSTALL_DOXYGEN.
# There's no INSTALL_SUBDIRS, so always enter doc/ and check NETLOC_BUILD/INSTALL_DOXYGEN there
SUBDIRS += doc

# pkg-config file
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = netloc.pc
