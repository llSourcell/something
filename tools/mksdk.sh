#!/bin/bash

set -ex

ANDROID_API=19

# need to specify source files manually to avoid having TwilioClientService in there
JAVADOC_SOURCE_FILES="
Channel.java
ChannelListener.java
Channels.java
IPMessagingClientListener.java
Member.java
Members.java
Message.java
Messages.java
TwilioIPMessagingClient.java"
TWILIO_HOWTOS="
android-ip-messaging-demo-android"
#TWILIO_HELPER_LIBS="
#    twilio-java:java
#    twilio-php:php
#    twilio-python:python
#    twilio-ruby:ruby"


function main {

    mydir=$(dirname $0)
    if [ "${mydir}" = "." ]; then
        mydir=$(pwd)
    elif [ "${mydir:0:1}" != "/" ]; then
        mydir="$(pwd)/${mydir}"
    fi

    twsdkroot=${mydir}/..
    twsdkJNIroot=${mydir}/../RTDSDK/jni

    . ${mydir}/functions.sh

    target=debug
    while [ "$1" ]; do
        case "$1" in
            -h|--help)
                print_usage
                exit 0
                ;;
            release)
                target=release
                ;;
            debug)
                target=debug
                ;;
            *)
                echo "Unrecognized option: $1" >&2
                exit 1
                ;;
        esac
        shift
    done

    check_tools
    build_library
    copy_javadocs
    #copy_docs
    #copy_files
    setup_examples
    #pull_helper_lib
    #pull_server_code
    archive

}

function check_tools {

    check_android_tools $ANDROID_API

}

function build_library {


    # check our submodules, make sure fetched and up-to-date
    echo " pushd ${twsdkroot}"
    pushd "${twsdkroot}"
    git submodule init
    git submodule update
    ./RTDSDK/Scripts/combine-dependencies.sh
    popd

    pushd "${twsdkroot}"/RTDSDK

    rm -f build.xml
    android update lib-project -p "${twsdkroot}"/RTDSDK

    ant clean
    new_sdk_version="$(get_sdk_version ${target})"
    echo ${new_sdk_version}
    ./mk-version-class.sh ${new_sdk_version}

    pushd "${twsdkJNIroot}"

    ndk-build clean
    # force the requested build type since ndk-build will otherwise look at AndroidManifest
    [ "$target" = "release" ] && ndk_build_opts="NDK_DEBUG=0" || ndk_build_opts="NDK_DEBUG=1"
    ndk-build -j4 $ndk_build_opts

    popd

    ant $target

    buildroot="$twsdkroot/output"
    rm -rf $buildroot
    mkdir $buildroot

    tarname="${SDK_NAME_STEM}"

    # save intermediates for later debugging
    linkname="${tarname}_INTERMEDIATES"
    ln -sfn obj "${linkname}"
    tar --version 2>&1 | grep -q GNU && follow_opt="h" || follow_opt="H"
    tar c${follow_opt}vjf "${buildroot}/${linkname}.tar.bz2" "${linkname}"
    rm "${linkname}"

    # move intermediates archive to folder
    intermediates="${buildroot}/intermediates"
    mkdir ${intermediates}
    mv "${buildroot}/${linkname}.tar.bz2" "${intermediates}/${linkname}.tar.bz2"

    tarroot="${buildroot}/${tarname}"
    mkdir "${tarroot}"

    # jar and native libs
    mkdir "${tarroot}/libs"
    cp "${twsdkroot}/RTDSDK/bin/classes.jar" "${tarroot}/libs/${tarname}.jar"
    abis=$(sed -ne '/^APP_ABI/s/^APP_ABI :=//p' ${twsdkroot}/RTDSDK/jni/Application.mk)
    for abi in $abis; do
    if [ -d "${twsdkroot}/RTDSDK/libs/${abi}" ]; then
    cp -PR "${twsdkroot}/RTDSDK/libs/${abi}" "${tarroot}/libs"
    fi
    done


}


function copy_javadocs {
    # javadocs
    docdest="${tarroot}/javadoc"
    mkdir "${docdest}"

    for f in $JAVADOC_SOURCE_FILES; do
        jd_source_paths="${jd_source_paths} ${twsdkroot}/RTDSDK/src/com/twilio/ipmessaging/$f"
    done


    # # doclint was introduced in Java 8 and fails to build javadoc on any error. Switching it off.
    if check_java_8; then
        DOCLINT_DISABLE="-Xdoclint:none "
    fi

javadoc \
    -public \
    -sourcepath "${twsdkroot}/RTDSDK/src" \
    -classpath "${ANDROID_SDK_HOME}/platforms/android-${ANDROID_API}/android.jar" \
    -d "${docdest}" \
    -version \
    -top '<h1>Twilio Client for Android</h1>' \
    -windowtitle 'Twilio Client for Android' \
    -charset UTF-8 \
    -docencoding UTF-8 \
    -linkoffline http://developer.android.com/reference/  "$mydir" \
    -stylesheetfile "${twsdkroot}/tools/javadoc-style.css" \
    ${DOCLINT_DISABLE} ${jd_source_paths}
}

function copy_files {

    # quick start
    pushd "${twsdkroot}/Quickstart"
    make -j1 clean all
    mkdir -p "${tarroot}/Quickstart"
    cp -a quickstart.html "${tarroot}/"
    # note: we now only ship the bare skeleton of the quickstart source
    # code to avoid developer confusion.
    cp -a ip-messaging-demo-android "${tarroot}/Quickstart"
    for i in HelloMonkey; do
    pushd "${tarroot}/Quickstart/${i}"
    rm -rf bin gen libs
    mkdir gen
    ln -sfn ../../libs libs

    # original project depends on "sdk" library project, while copied should depend on "sdk.jar" instead
    sed -i '' -e '/android.library.reference.1=.*/d' ./project.properties

    popd
    done
    popd

    # assets
    pushd "${twsdkroot}/assets"
    mkdir -p "${tarroot}/assets"
    cp -aL * "${tarroot}/assets"
    popd
}

function copy_docs {

    # docs dir
    pushd "${twsdkroot}/docs"
    make -j1 clean all
    cp -a *.html "${tarroot}"
    cp -a acknowledgments.txt "${tarroot}"
    popd

}

function setup_examples {
    example_dest="$twsdkroot/output/$SDK_NAME_STEM/ip-messaging-demo-android"
    example_source="${twsdkroot}/ip-messaging-demo-android"
    rm -rf ${example_dest}

    pushd "${twsdkroot}/ip-messaging-demo-android"

    cp -a "${example_source}" "${example_dest}"
    pushd "${example_dest}"
    rm -rf bin gen libs
    mkdir gen
    ln -sfn ../libs libs
    # original project depends on "sdk" library project, while copied should depend on "sdk.jar" instead
    sed -i '' -e '/android.library.reference.1=.*/d' ./project.properties

    popd
    popd
}

function pull_helper_lib {
    # helper libs
    mkdir "${tarroot}/helper-libs"
    pushd "${tarroot}/helper-libs"
    for lib in ${TWILIO_HELPER_LIBS}; do
    name=$(echo "${lib}" | cut -d: -f1)
    dest=$(echo "${lib}" | cut -d: -f2)
    git clone --depth 1 --recursive "git://github.com/twilio/${name}.git" "${dest}"
    rm -rf "${dest}/.git"
    done
    popd
}

function pull_server_code {
    # pull server code
    echo "Pulling down server script"
    mkdir "${tarroot}/Server"
    pushd "${tarroot}"
    git clone --depth 1 https://github.com/twilio/mobile-quickstart.git Server &>/dev/null
    popd

}

function archive {

    pushd "${twsdkroot}"
    pwd
    SDK_VERSION=$(sed -n '/android:versionName=/s/.*"\(.*\)"[^"]*/\1/p' RTDSDK/AndroidManifest.xml)
    GIT_COMMIT=$(git rev-parse --short=7 HEAD)

    # CI_BUILD_NUMBER is Jenkins build number
    if [ -z ${CI_BUILD_NUMBER} ]; then
        buildname="${SDK_NAME_STEM}-${SDK_VERSION}-${GIT_COMMIT}"
    else 
        buildname="${SDK_NAME_STEM}-${SDK_VERSION}-b${CI_BUILD_NUMBER}-${GIT_COMMIT}"
        # if build is done from Jenkins, write build version string to file, later used when uploading to S3
        echo "${SDK_VERSION}-b${CI_BUILD_NUMBER}-${GIT_COMMIT}" > ci_sdk_version.txt
    fi

    if [ "$target" = "debug" ]; then
        # be very clear to ourselves that we just did a debug build
        tarname="${buildname}_DEBUG"
    else
        tarname="${buildname}"
    fi
    popd

    pushd "${tarroot}/.."
    tar cvjf "${buildroot}/${tarname}.tar.bz2" "$(basename ${tarroot})"
    zip -r9 "${buildroot}/${tarname}.zip" "$(basename ${tarroot})"
    popd

    popd

    echo
    echo "New script SDK built in ${buildroot}"
    echo

}

main "$@"

#copy_docs "@"

#popd >/dev/null
