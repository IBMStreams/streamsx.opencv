#!/bin/bash
#set -o xtrace
#set -o pipefail

################### parameters used in this script ##############################

self=${0##*/}
here=$( cd ${0%/*} ; pwd )

date=$( date +%Y-%m-%d )
package=$HOME/streams4.streamsx.opencv.$date.tar.gz

################### functions used in this script #############################

die() { echo ; echo -e "\e[1;31m$*\e[0m" >&2 ; exit 1 ; }
step() { echo ; echo -e "\e[1;34m$*\e[0m" ; }

################################################################################

[ -f $package ] && rm $package

cd $here/.. || die "sorry, could not change to directory '$workspace', $?"


step "creating package $package ..."
tar -cvzf $package \
--exclude='*~' \
--exclude='#*#' \
--exclude='output' \
--exclude='download' \
--exclude='.svn' \
--exclude='.apt_generated' \
--exclude='.settings' \
--exclude='StreamsLogsJob*.tgz' \
--exclude='.metadata' \
--exclude='toolkit.xml' \
--exclude='.toolkitList' \
--exclude='.tempLaunch' \
--exclude='*.splbuild' \
--exclude='*.splmm.*' \
--exclude='*_cpp.pm' \
--exclude='*_h.pm' \
--exclude='*.o' \
--exclude='*.a' \
--exclude='*.so' \
--exclude='html/*.html' \
--exclude='*-out.avi' \
--exclude='.DS_Store' \
streamsx.opencv \
|| die "Sorry, could not create package $package, $?"
step "created package $package"

exit 0
