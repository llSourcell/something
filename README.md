To build the project:

1. Run `git submodule update --init` to get `twilio-jni` depedency.
2. Run `./RTDSDK/Script/combine-dependencies.sh` to get RTD libraries and their dependencies.
3. Run `ndk-build` in `./RTDSDK/jni` directory to build the shared library.



To see the crash dumb run `adb logcat | $ANDROID_NDK_ROOT/ndk-stack -sym $PROJECT_HOME/android-ip-messaging-api/RTDSDK/obj/local/armeabi-v7a`
