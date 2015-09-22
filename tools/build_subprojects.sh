#!/bin/bash

set -ex

ANDROID_API=8

function main {
    mydir=$(dirname $0)
    if [ "${mydir}" = "." ]; then
        mydir=$(pwd)
    elif [ "${mydir:0:1}" != "/" ]; then
        mydir="$(pwd)/${mydir}"
    fi

    . ${mydir}/functions.sh

    check_android_tools $ANDROID_API

    twsdkroot="${mydir}/.."
    buildroot="${twsdkroot}/output"

    build_project "${buildroot}/${SDK_NAME_STEM}/ip-messaging-demo-android"
}

function build_project {
    PROJECT_DIR=$1

    pushd $PROJECT_DIR
    echo "Building project at ${PROJECT_DIR}..."

    rm -f build.xml
    cp -a ${buildroot}/${SDK_NAME_STEM}/ip-messaging-demo-android/thirdparty/easyadapter-1.5.0.jar ${buildroot}/${SDK_NAME_STEM}/ip-messaging-demo-android/libs/.
    android update project -p  ${PROJECT_DIR}

    ant clean
    ant debug

    echo "Project built succesffully."
    popd
}

main "$@"
