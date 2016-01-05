# Twilio IP Messaging SDK for Android #
 
## To build the RTDSDK project ##

* Run `git submodule update --init` to get `twilio-jni` depedency.
* Run `./RTDSDK/Scripts/combine-dependencies.sh` to get twilio-common-android library, RTD libraries and their dependencies.
* Run `ndk-build` in `./RTDSDK/jni` directory to build the shared library.


## To run the demo app ##

`ip-messaging-demo-android` currently depends on twilio-ip-messaging-android, twilio-common-android and Google Play Services library.

* twilio-ip-messaging-android library : you can copy them from the public distribution. To create it locally run `/tools/mksdk.sh -includetestapp`and copy them from generated /output folder. Easiest way will be to copy everything under /output/twilio-ip-messaging-android/libs folder.
* twilio-common-android library : Get the latest artifact from http://nexus.corp.twilio.com/content/repositories/artifact-staging/com/twilio/sdk/twilio-common-android/ and copy the jar under /libs
* Google Play Services library : Follow the instructions detailed under https://developers.google.com/android/guides/setup

After adding the demo app dependencies you will need to provide a valid `AUTH_PHP_SCRIPT` string for the variable in LoginActivity. 

## Troubleshooting tips ##

* To see the crash dumb run `adb logcat | $ANDROID_NDK_ROOT/ndk-stack -sym $PROJECT_HOME/android-ip-messaging-api/RTDSDK/obj/local/armeabi-v7a`
* Enable debug level log `TwilioIPMessagingSDK.setLogLevel(android.util.Log.DEBUG);`
