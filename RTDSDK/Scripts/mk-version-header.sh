#!/bin/bash

BASE_DIR=`dirname $0`
pushd "$BASE_DIR" >/dev/null

SDK_VERSION=0
SDK_BUILD_NUMBER=0
SDK_SOURCE_CODE_REVISION=$(git rev-parse --short HEAD)

TW_VERSION_DEFINE=""
TW_VERSION_FILE=""


function createVersionHeader()
{
    [ -f $TW_VERSION_FILE ] && touch $TW_VERSION_FILE
    echo "${SDK_VERSION}.b${SDK_BUILD_NUMBER}-${SDK_SOURCE_CODE_REVISION}"
    echo $TW_VERSION_FILE
    cat <<EOF >"${TW_VERSION_FILE}"
/* this file is auto-generated; do not edit! */

#ifdef DEBUG
#define ${TW_VERSION_DEFINE}  "${SDK_VERSION}.b${SDK_BUILD_NUMBER}-${SDK_SOURCE_CODE_REVISION}"
#else
#define ${TW_VERSION_DEFINE}  "${SDK_VERSION}.b${SDK_BUILD_NUMBER}"
#endif

EOF

}

########################## MAIN ENTRY ########################################

# read arguments
while [ $# -gt 0 ]; do
    case "$1" in
        -h|--help)
        echo "Usage: ./mk-version-header.sh -f <filename> -v 1.2.0 -b 17"
        exit 0
    ;;

    -v)
        SDK_VERSION="$2"
        shift
    ;;

    -d)
        TW_VERSION_DEFINE="$2"
        shift
    ;;

    -f)
        TW_VERSION_FILE="$2"
        shift
    ;;

    -b)
        SDK_BUILD_NUMBER="$2"
        shift
    ;;

    esac
    shift       # Check next set of parameters.
done

echo "******** VERSION HEADER: Building **********"
createVersionHeader
echo "******** VERSION HEADER: Done!  ************"

popd  >/dev/null
