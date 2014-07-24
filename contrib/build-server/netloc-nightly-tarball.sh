#!/bin/sh

#####
#
# Configuration options
#
#####

# e-mail address to send results to
results_addr=netloc-devel@open-mpi.org

# git repository URL
code_uri=https://github.com/open-mpi/netloc.git
raw_uri=https://raw.github.com/open-mpi/netloc

# where to put built tarballs
outputroot=/l/osl/www/www.open-mpi.org/software/netloc/nightly

# where to find the build script
script_uri=contrib/nightly/make_snapshot_tarball

# The tarballs to make
if [ $# -eq 0 ] ; then
    branches="master"
else
    branches=$@
fi

# Build root - scratch space
build_root=/home/mpiteam/netloc/nightly-tarball-build-root

export PATH=$HOME/local/bin:$PATH
export LD_LIBRARY_PATH=$HOME/local/lib:$LD_LIBRARY_PATH

#####
#
# Actually do stuff
#
#####

# load the modules configuration
. /etc/profile.d/modules.sh
module use ~/modules

# get our nightly build script
mkdir -p $build_root
cd $build_root

# Loop making them
for branch in $branches; do
    # Form a URL-specific script name
    script=$branch-`basename $script_uri`

    wget --quiet --no-check-certificate --tries=10 $raw_uri/$branch/$script_uri -O $script
    if test ! $? -eq 0 ; then
        echo "wget of netloc nightly tarball create script failed."
        if test -f $script ; then
            echo "Using older version of $script for this run."
        else
            echo "No build script available.  Aborting."
            exit 1
        fi
    fi
    chmod +x $script

    module load "autotools/netloc-$branch"
    base=/u/mpiteam/netloc/nightly-tarball-build-root
    export NETLOC_CONFIGURE_FLAGS="--with-jansson=$base/jansson-2.6/local --with-hwloc=$base/hwloc-1.7.2/local"
    export DISTCHECK_CONFIGURE_FLAGS=$NETLOC_CONFIGURE_FLAGS

    ./$script \
        $build_root/$branch \
        $results_addr \
        $outputroot/$branch \
        $code_uri \
        $branch \
        >/dev/null 2>&1

    module unload autotools
done
