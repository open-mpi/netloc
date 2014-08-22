# Copyright © 2009-2014 Inria.  All rights reserved.
# Copyright © 2009-2010 Cisco Systems, Inc.  All rights reserved.
# See COPYING in top-level directory.

DOCDIR = $(abs_top_builddir)/doc
DOX_CONFIG = $(abs_top_srcdir)/doc/doxygen.cfg

DOX_DIR = doxygen-doc
DOX_HTML_DIR = $(DOX_DIR)/html
DOX_MAN_DIR = $(DOX_DIR)/man
DOX_LATEX_DIR = $(DOX_DIR)/latex
DOX_PDF = doxygen-doc/$(PACKAGE).pdf
DOX_TAG = $(DOX_DIR)/$(PACKAGE).tag

#
# The goal is that Doxygen output (i.e., the documentation) is
# included in release tarballs; there is no need to build anything
# from tarballs.  Developers will need to build the docs the first
# time they build after a checkout (or if something in the docs
# changes, etc.).
#
# Note that by listing directories in EXTRA_DIST, we pick up the whole
# tree (e.g., everything in the man and latex directories).
#

# images already available in repository
PREBUILT_IMAGES = 

EXTRA_DIST = \
        netloc.doxy \
        doxygen.css \
        doxygen.cfg \
        $(PREBUILT_IMAGES)

#
# Files where the doxygen inputs live (i.e., dependencies).  Make the
# generated tagfile depend on these files, which will force them to be
# regenerated (i.e., re-run doxygen) whenever any of these files
# change.
#

dox_inputs = $(DOX_CONFIG) \
       $(srcdir)/netloc.doxy \
       $(abs_top_srcdir)/include/netloc.h \
       $(abs_top_srcdir)/include/netloc_dc.h \
       $(abs_top_srcdir)/include/netloc_map.h

if NETLOC_BUILD_DOXYGEN

RENAME_SED = $(SED_I) \
	-e 's/NETLOC_DECLSPEC //g' \
	-e 's/NETLOC\\_\\-DECLSPEC //g'

$(DOX_TAG): $(BUILT_SOURCES) $(dox_inputs) $(PREBUILT_IMAGES)
	rm -fr $(DOX_DIR)
	$(DOXYGEN) $(DOX_CONFIG)
	-$(RENAME_SED) $(DOX_DIR)/html/*.html $(DOX_DIR)/latex/*.tex $(DOX_DIR)/man/man3/*.3
	@echo "Removing useless manpages..."
	@mkdir $(DOX_DIR)/man.tmp
	@mv $(DOX_MAN_DIR)/man3/netloc* $(DOX_MAN_DIR)/man3/NETLOC* $(DOX_DIR)/man.tmp/
	@rm -rf $(DOX_MAN_DIR)/man3
	@mv $(DOX_DIR)/man.tmp $(DOX_MAN_DIR)/man3

$(DOX_PDF): $(DOX_TAG)
	cd $(DOX_LATEX_DIR); \
	rm -f *.aux *.toc *.idx *.ind *.ilg *.log *.out; \
	$(PDFLATEX) refman.tex; \
	$(MAKEINDEX) refman.idx; \
	$(PDFLATEX) refman.tex; \
	done=0; repeat=5; \
	while test $$done = 0 -a $$repeat -gt 0; do \
           if $(EGREP) 'Rerun (LaTeX|to get cross-references right)' refman.log > /dev/null 2>&1; then \
	       $(PDFLATEX) refman.tex; \
	       repeat=`expr $$repeat - 1`; \
	   else \
	       done=1; \
	   fi; \
	done; \
	mv refman.pdf $(DOCDIR)/$(DOX_PDF)

#
# Note that we want to use our own doxygen.css file; not the one that
# doxygen installs in the HTML directory.  So manually copy it over.
# Be a little clever: only copy the doxygen.css file over if it exists
# in DOX_HTML_DIR (which is in the build tree).  If the html tree
# doesn't exist in the build tree, then we're using an html tree in
# the source tree, and we don't need to copy over the doxygen.css
# because we didn't build the html tree (e.g., the html tree came
# pre-bundled in a tarball).
#
all-local: $(DOX_TAG)
	if test -d $(DOX_HTML_DIR) -a -f $(DOX_HTML_DIR)/doxygen.css; then \
	    cp -f $(srcdir)/doxygen.css $(DOX_HTML_DIR); \
	fi

# Put in these rules to force the generation of the man pages and get
# all the dependencies right.  Additionally, this forces a
# serialization during parallel builds (e.g., "make -j X") to ensure
# that we don't get partial target errors (i.e., doxygen must be run
# *first* before we can try to process/install man pages, the HTML,
# run pdflatex, .etc.).

$(dist_man3_MANS): $(DOX_TAG)
$(DOX_PDF): $(DOX_TAG)
$(DOX_HTML_DIR): $(DOX_TAG)
$(DOX_LATEX_DIR): $(DOX_TAG)
$(DOX_MAN_DIR): $(DOX_TAG)

endif


if NETLOC_INSTALL_DOXYGEN

#
# Un/install the generated PDF and man pages (just like BUILD_DOXYGEN,
# INSTALL_DOXYGEN will automatically be false if we're not building in
# standalone mode).
#
dist_pdf_DATA = $(DOX_PDF)

#
# Install the NETLOC_* and netloc_* man pages.  It would be great to
# figure out how to not have to list every installable man pages here
# in the Makefile.am...  :-(
#
dist_man3_MANS = \
        $(DOX_MAN_DIR)/man3/netloc_api.3 \
        $(DOX_MAN_DIR)/man3/netloc_attach.3 \
        $(DOX_MAN_DIR)/man3/netloc_dc_api.3 \
        $(DOX_MAN_DIR)/man3/netloc_map_api_main.3 \
        $(DOX_MAN_DIR)/man3/netloc_map_api_map.3 \
        $(DOX_MAN_DIR)/man3/netloc_map_api_servers_ports.3 \
        $(DOX_MAN_DIR)/man3/netloc_map_api_paths.3 \
        $(DOX_MAN_DIR)/man3/netloc_map_api_misc.3

endif

#
# Only remove the actual generated documentation files for maintainers
#

MAINTAINERCLEANFILES = $(DOX_TAG) \
    -r \
    $(DOX_HTML_DIR) \
    $(DOX_MAN_DIR) \
    $(DOX_LATEX_DIR) \
    $(DOX_PDF)
