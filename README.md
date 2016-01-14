# Twilio IP Messaging SDK for Android #
 
## Building RTDSDK library ##

1. Run `git submodule update --init` to get `twilio-jni` depedency.

1. Run `./RTDSDK/Scripts/combine-dependencies.sh` to get twilio-common-android library, RTD libraries and their dependencies.

1. Run `ndk-build` in `./RTDSDK/jni` directory to build the shared library.

1. Run `./tools/mksdk.sh` to build the SDK.

## Running the DemoApp ##

### Dependencies ###

* twilio-ip-messaging-android
* twilio-common-android
* Google Play Services Library

### Android Studio ###

#### Access Token ####

The `ACCESS_TOKEN_SERVICE_URL` property can be provided in one the following ways:

* Update the `ACCESS_TOKEN_SERVICE_URL` value in `$RTD_SDK/ip-messaging-demo-android/gradle.properties`

`Note:` Double quotes must be escaped for the following approaches:

* Pass the `ACCESS_TOKEN_SERVICE_URL` property when executing a gradle task. 
	* ex.`./gradlew -PACCESS_TOKEN_SERVICE_URL ="\"<ACCESS_TOKEN_SERVICE_URLL>\"" <task>`
	
	
* Update the default `ACCESS_TOKEN_SERVICE_URL` value in `$RTD_SDK/ip-messaging-demo-android/app/build.gradle`


#### Developer Builds ####

1. Run `ant clean debug` from `$RTD_SDK/RTDSDK/` to rebuild the jar.

1. Replace the `twilio-ip-messaging-android.jar` in  `apps\libs` with the newly created artifact by either copying or symlinking.

	* `cp -R $RTD_SDK/RTDSDK/bin/twilio-ip-messaging-android.jar $RTD_SDK/ip-messaging-demo-android/app/libs`

	* `ln -s $RTD_SDK/RTDSDK/bin/twilio-ip-messaging-android.jar $RTD_SDK/ip-messaging-demo-android/app/libs`

1. Provide a valid `ACCESS_TOKEN_SERVICE_URL`. See above.

1. Switch to `developer` from the `Build Variants` on the bottom left tab.

1. Run the app by clicking the green play button or execute `cd $RTD_SDK/ip-messaging-demo-android; ./gradlew installDeveloper`

### Eclipse ###

No. 

## Troubleshooting tips ##

* To see the crash dumb run `adb logcat | $ANDROID_NDK_ROOT/ndk-stack -sym $PROJECT_HOME/android-ip-messaging-api/RTDSDK/obj/local/armeabi-v7a`
* Enable debug level log `TwilioIPMessagingSDK.setLogLevel(android.util.Log.DEBUG);`
