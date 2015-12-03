# Twilio IP Messaging SDK for Android #
 
## How to build the RTDSDK project ##

* Run `git submodule update --init` to get `twilio-jni` depedency.
* Run `./RTDSDK/Script/combine-dependencies.sh` to get twilio-common-android library, RTD libraries and their dependencies.
* Run `ndk-build` in `./RTDSDK/jni` directory to build the shared library.



## How to run the demo app ##

`ip-messaging-demo-android` currently depedns on twilio-ip-messaging-android, twilio-common-android and Google Play Services library. 

* twilio-ip-messaging-android library : you can copy them from the public distribution. To create it locally run `/tools/mksdk.sh`and copy them from generated /output folder. Easiest way will be to copy everything under /output/twilio-ip-messaging-android/libs folder.
* twilio-common-android library : You need to get the artifacts from http://nexus.corp.twilio.com/content/repositories/artifact-staging/com/twilio/sdk/twilio-common-android/0.1.4/ and copy them under /libs. Extrarct the content of twilio-rtc-ip-messaging.tar.bz2, copy .jar under /libs and *.so under /libs/armeabi-v7a. 

* Google Play Services library : https://developers.google.com/android/guides/setup


## Troubleshooting tips ##

* To see the crash dumb run `adb logcat | $ANDROID_NDK_ROOT/ndk-stack -sym $PROJECT_HOME/android-ip-messaging-api/RTDSDK/obj/local/armeabi-v7a`
