To build the project:

1. Run `git submodule update --init` to get the depedency
2. Run `ndk-build` to build the shared library.


To see the crash dumb run `adb logcat | $ANDROID_NDK_ROOT/ndk-stack -sym A$APROJECT_HOME/android-ip-messaging-api/RTDSDK/obj/local/armeabi-v7a`
