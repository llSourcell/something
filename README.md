To build the project:

1. Run `git submodule update --init` to get `twilio-jni` depedency.
2. Run `ndk-build` to build the shared library.
3. Run `./Script/combine-dependencies.sh` to get RTD libraries and their dependencies.


To see the crash dumb run `adb logcat | $ANDROID_NDK_ROOT/ndk-stack -sym $PROJECT_HOME/android-ip-messaging-api/RTDSDK/obj/local/armeabi-v7a`
