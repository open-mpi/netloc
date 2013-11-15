dnl This macro set originally copied from Open MPI:
dnl Copyright (c) 2004-2005 The Trustees of Indiana University and Indiana
dnl                         University Research and Technology
dnl                         Corporation.  All rights reserved.
dnl Copyright (c) 2004-2005 The University of Tennessee and The University
dnl                         of Tennessee Research Foundation.  All rights
dnl                         reserved.
dnl Copyright (c) 2004-2007 High Performance Computing Center Stuttgart, 
dnl                         University of Stuttgart.  All rights reserved.
dnl Copyright (c) 2004-2005 The Regents of the University of California.
dnl                         All rights reserved.
dnl Copyright (c) 2006-2007 Cisco Systems, Inc.  All rights reserved.
dnl and renamed/modified for netloc:
dnl Copyright (c) 2009 inria.  All rights reserved.
dnl Copyright (c) 2009-2010 Université Bordeaux 1
dnl Copyright © 2010-2012 Cisco Systems, Inc.  All rights reserved.
dnl See COPYING in top-level directory.
dnl 
dnl Redistribution and use in source and binary forms, with or without
dnl modification, are permitted provided that the following conditions are
dnl met:
dnl 
dnl - Redistributions of source code must retain the above copyright
dnl   notice, this list of conditions and the following disclaimer.
dnl 
dnl - Redistributions in binary form must reproduce the above copyright
dnl   notice, this list of conditions and the following disclaimer listed
dnl   in this license in the documentation and/or other materials
dnl   provided with the distribution.
dnl 
dnl - Neither the name of the copyright holders nor the names of its
dnl   contributors may be used to endorse or promote products derived from
dnl   this software without specific prior written permission.
dnl 
dnl The copyright holders provide no reassurances that the source code
dnl provided does not infringe any patent, copyright, or any other
dnl intellectual property rights of third parties.  The copyright holders
dnl disclaim any liability to any recipient for claims brought against
dnl recipient by any third party for infringement of that parties
dnl intellectual property rights.
dnl 
dnl THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
dnl "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
dnl LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
dnl A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
dnl OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
dnl SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
dnl LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
dnl DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
dnl THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
dnl (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
dnl OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
dnl

dnl _NETLOC_CHECK_VISIBILITY
dnl --------------------------------------------------------
AC_DEFUN([_NETLOC_CHECK_VISIBILITY],[
    # Be safe for systems that have ancient Autoconf's (e.g., RHEL5)
    m4_ifdef([AC_PROG_GREP], 
             [AC_REQUIRE([AC_PROG_GREP])],
             [GREP=grep])

    # Check if the compiler has support for visibility, like some
    # versions of gcc, icc, Sun Studio cc.
    AC_ARG_ENABLE(visibility, 
        AC_HELP_STRING([--enable-visibility],
            [enable visibility feature of certain compilers/linkers (default: enabled on platforms that support it)]))

    case ${target} in
        *-*-aix*|*-*-mingw*|*-*-cygwin*|*-*-hpux*)
            enable_visibility=no
            ;;
    esac

    netloc_visibility_define=0
    netloc_msg="whether to enable symbol visibility"
    if test "$enable_visibility" = "no"; then
        AC_MSG_CHECKING([$netloc_msg])
        AC_MSG_RESULT([no (disabled)]) 
    else
        CFLAGS_orig=$CFLAGS

        netloc_add=
        case "$netloc_c_vendor" in
        sun)
            # Check using Sun Studio -xldscope=hidden flag
            netloc_add=-xldscope=hidden
            CFLAGS="$CFLAGS_orig $netloc_add -errwarn=%all"
            ;;

        *)
            # Check using -fvisibility=hidden
            netloc_add=-fvisibility=hidden
            CFLAGS="$CFLAGS_orig $netloc_add -Werror"
            ;;
        esac

        AC_MSG_CHECKING([if $CC supports $netloc_add])
        AC_LINK_IFELSE([AC_LANG_PROGRAM([[
            #include <stdio.h>
            __attribute__((visibility("default"))) int foo;
            ]],[[fprintf(stderr, "Hello, world\n");]])],
            [AS_IF([test -s conftest.err],
                   [$GREP -iq visibility conftest.err
                    # If we find "visibility" in the stderr, then
                    # assume it doesn't work
                    AS_IF([test "$?" = "0"], [netloc_add=])])
            ], [netloc_add=])
        AS_IF([test "$netloc_add" = ""],
              [AC_MSG_RESULT([no])],
              [AC_MSG_RESULT([yes])])

        CFLAGS=$CFLAGS_orig
        NETLOC_VISIBILITY_CFLAGS=$netloc_add

        if test "$netloc_add" != "" ; then
            netloc_visibility_define=1
            AC_MSG_CHECKING([$netloc_msg])
            AC_MSG_RESULT([yes (via $netloc_add)]) 
        elif test "$enable_visibility" = "yes"; then
            AC_MSG_ERROR([Symbol visibility support requested but compiler does not seem to support it.  Aborting])
        else 
            AC_MSG_CHECKING([$netloc_msg])
            AC_MSG_RESULT([no (unsupported)]) 
        fi
        unset netloc_add 
    fi

    AC_DEFINE_UNQUOTED([NETLOC_C_HAVE_VISIBILITY], [$netloc_visibility_define],
            [Whether C compiler supports symbol visibility or not])
])
