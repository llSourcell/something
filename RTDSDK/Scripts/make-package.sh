source `dirname $0`/env.sh

TARBALL_NAME=twilio-rtc-ip-messaging-ios-debug.tar.bz
if [ ! -z "${CI_TARBALL_NAME}" ]; then
  TARBALL_NAME="${CI_TARBALL_NAME}"
fi

DERIVED_DATA_DIR=temp

pushd `dirname $0`/..

if [ ! -d Package ]; then
  mkdir Package
fi

rm -rf Package/temp
rm -rf Package/docs

Scripts/gen_appledoc.sh
mkdir -p Package/temp/Documentation
cp -a Documentation/html/* Package/temp/Documentation/

sed -e"s#PODSPEC_RELEASE_VERSION#${PODSPEC_RELEASE_VERSION}#g;s#RELEASE_ARCHIVE_URL#${RELEASE_ARCHIVE_URL}#g" TwilioIPMessagingClient.podspec > Package/TwilioIPMessagingClient.podspec

cp -a "${DERIVED_DATA_DIR}/Build/Products/${CONFIGURATION}-universal/TwilioIPMessagingClient.framework" Package/temp/TwilioIPMessagingClient.framework

cd Package/temp
tar cvj --exclude ".DS_Store" -f ../${TARBALL_NAME} *
mv Documentation ../docs
cd ..
rm -rf temp

popd
