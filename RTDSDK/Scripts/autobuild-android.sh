source `dirname $0`/env.sh
rm -rf temp/Build
pushd `dirname $0`/..

VERSION_INFO=""
if [ ! -z "${RELEASE_VERSION}" ]; then
  CI_BUILD_VERSION=${RELEASE_VERSION}
fi
if [ ! -z "${CI_BUILD_VERSION}" ]; then
  VERSION=${CI_BUILD_VERSION}
  BUILD=${BUILD_NUMBER}
  VERSION_INFO="TS_SDK_VERSION=${VERSION} TS_SDK_BUILD_NUMBER=${BUILD}"
fi


popd
