#!/bin/bash

BASE_DIR=`dirname $0`
pushd "$BASE_DIR"/.. >/dev/null

if [ -z "$CI_TARBALL_NAME" ]; then
	echo "CI_TARBALL_NAME not specified. Using \"package.tar.bz2\" as artifact filename."
    CI_TARBALL_NAME=package.tar.bz2
fi

# reading the arguments
if [ "$#" -ne 2 ]; then
	echo "Error: Expecting 2 arguments: release-version and public-version"
	exit 1
fi
# RELEASE_VERSION and PUBLIC_VERSION should be like "0.8.3.b54-deadbee" & "v0.8", respectively
RELEASE_VERSION="$1"
PUBLIC_VERSION="$2"


# paths
WORKSPACE_ROOT_DIR=`pwd`
PACKAGE_DIR="$WORKSPACE_ROOT_DIR/Package"
DOCS_DIR="$PACKAGE_DIR/docs"
# The path should look like this: /Package/sdk/rtc/android/ip-messaging/releases/0.8.3.b54-deadbee/twilio-rtc-ip-messaging.tar.bz2
PLATFORM_NAME="android"
PRODUCT_NAME="ip-messaging"
POD_NAME="TwilioIPMessagingClient"
PLATFORM_PATH="$PACKAGE_DIR/sdk/rtc/${PLATFORM_NAME}"
PRODUCT_PATH="$PLATFORM_PATH/${PRODUCT_NAME}"
RELEASE_VERSION_PATH="$PRODUCT_PATH/releases"
ARTIFACT_NAME="twilio-rtc-${PRODUCT_NAME}.tar.bz2"

if [ ! -d "$PACKAGE_DIR" ]; then
	echo "Error: Couldn't find \"Package\" folder"
	exit 1
fi

rm -rf "${PACKAGE_DIR}/sdk"

# move the tarball and docs folder to the release directory
if [ ! -d "$DOCS_DIR" ]; then
	echo "Error: Couldn't find \"docs\" folder"
	exit 1
fi
if [ ! -f "$PACKAGE_DIR/$CI_TARBALL_NAME" ]; then
	echo "Error: Couldn't find the tarball"
	exit 1
fi
if [ ! -f "${PACKAGE_DIR}/${POD_NAME}.podspec" ]; then
	echo "Error: Couldn't find the podspec"
	exit 1
fi

if [ -z "${RELEASE_VERSION_PATH}" -o -z "${RELEASE_VERSION}" ]; then
	echo "Error: incorrect path variables"
	exit 1
fi
if [ ! -d "${RELEASE_VERSION_PATH}/${RELEASE_VERSION}" ]; then
	mkdir "${RELEASE_VERSION_PATH}/${RELEASE_VERSION}"
fi
mkdir -p "${RELEASE_VERSION_PATH}/${RELEASE_VERSION}/docs"
mv "${DOCS_DIR}" "${RELEASE_VERSION_PATH}/${RELEASE_VERSION}"
mv "${PACKAGE_DIR}/${CI_TARBALL_NAME}" "${RELEASE_VERSION_PATH}/${RELEASE_VERSION}/$ARTIFACT_NAME"
cp "${PACKAGE_DIR}/${POD_NAME}.podspec" "${RELEASE_VERSION_PATH}/${RELEASE_VERSION}/${POD_NAME}.podspec"

if [ -z "${PLATFORM_PATH}" -o -z "${PUBLIC_VERSION}" ]; then
	echo "Error: incorrect path variables"
	exit 1
fi

# creating symlinks to comply with the schema
rm -f "${PRODUCT_PATH}/${PUBLIC_VERSION}"
ln -s "${RELEASE_VERSION_PATH}/${RELEASE_VERSION}" "${PRODUCT_PATH}/${PUBLIC_VERSION}"
rm -f "${PRODUCT_PATH}/latest"
ln -s "${PRODUCT_PATH}/${PUBLIC_VERSION}" "${PRODUCT_PATH}/latest"

if [ ! -d "${PLATFORM_PATH}/${PUBLIC_VERSION}" ]; then
	mkdir "${PLATFORM_PATH}/${PUBLIC_VERSION}"
fi

if [ ! -d "${PLATFORM_PATH}/latest" ]; then
	mkdir "${PLATFORM_PATH}/latest"
fi

rm -f "${PLATFORM_PATH}/${PUBLIC_VERSION}/${ARTIFACT_NAME}"
ln -s "${PRODUCT_PATH}/${PUBLIC_VERSION}/${ARTIFACT_NAME}" "${PLATFORM_PATH}/${PUBLIC_VERSION}/${ARTIFACT_NAME}"
rm -f "${PLATFORM_PATH}/latest/${ARTIFACT_NAME}"
ln -s "${PLATFORM_PATH}/${PUBLIC_VERSION}/${ARTIFACT_NAME}" "${PLATFORM_PATH}/latest/${ARTIFACT_NAME}"



popd >/dev/null
