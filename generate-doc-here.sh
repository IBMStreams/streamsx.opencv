#!/bin/bash

## Copyright (C) 2016  International Business Machines Corporation
## All Rights Reserved

################### parameters used in this script ##############################

#set -o xtrace
#set -o pipefail

here=$( cd ${0%/*} ; pwd )

################### functions used in this script #############################

die() { echo ; echo -e "\e[1;31m$*\e[0m" >&2 ; exit 1 ; }
step() { echo ; echo -e "\e[1;34m$*\e[0m" ; }

################################################################################

step "generating documentation files ..."
make doc || die "sorry, could not make documentation files"
cp -r -p $here/com.ibm.streamsx.opencv/doc $here || die "sorry, could not copy files to $here"

exit 0
