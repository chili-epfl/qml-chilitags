qml-chilitags
=============

QMLChilitags is a QML wrapper for the fiducial marker tracker Chilitags. It works on desktop and Android.

The following is required for QMLChilitags to work:

  - Ubuntu `14.04`
  - Qt `5.3.2`
  - OpenCV `3.0.0-alpha` (**Note:** If you checked out OpenCV master branch, change `cv::ITERATIVE` to `cv::SOLVEPNP_ITERATIVE` in `Chilitags3D.cpp` when building Chilitags. At the time of writing, this API change in OpenCV is not yet in a tag, so it is not included in the Chilitags source.)

QML API
-------

The QML API consists of the following:

>  - **sourceImage** : `QVariant` - Should contain the input camera image as a `cv::Mat`
>  - **tags** : `QVariantMap` - Output detected tag poses as a map from `QString` to `QMatrix4x4`
>  - **projectionMatrix** : `QMatrix4x4` - Output matrix that transforms from the camera frame to the screen frame
>  - **tagConfigurationFile** : `QString` - qrc file that contains the tag configuration, must begin with `:/`

To use `qml-chilitags`, present a new frame as a `cv::Mat` wrapped in a `QVariant` through the `sourceImage` property.

Desktop Build
-------------

1. Build and install OpenCV `3.0.0-alpha` tag:

  ```
  git checkout 3.0.0-alpha
  cd platforms
  mkdir build-desktop
  cd build-desktop
  cmake ../.. -DCMAKE_INSTALL_PREFIX=/usr
  ```

  At this point, OpenCV components other than `calib3d`, `core`, `features2d`, `flann`, `highgui`, `imgcodecs`, `imgproc`, `video` and `videoio` are not needed and can be disabled. Leave the options untouched if you are unsure what to do. **Note:** I had to disable `WITH_GSTREAMER` manually for it to compile without errors. It is not needed and can be disabled. These options can be accessed with `ccmake ../..`.

  ```
  make -j 5
  make install
  ```

2. Build and install `chilitags` from [https://github.com/chili-epfl/chilitags](https://github.com/chili-epfl/chilitags):

  ```
  mkdir build-desktop
  cd build-desktop
  cmake .. -DCMAKE_INSTALL_PREFIX=/usr
  make -j 5
  make install
  ```

3. **Optional**: Build and install qt3d from [https://qt.gitorious.org/qt/qt3d](https://qt.gitorious.org/qt/qt3d):

  This enables QML interaction between Qt3D and Chilitags.

  ```
  mkdir build-desktop
  cd build-desktop
  /qt/install/root/5.3/gcc_64/bin/qmake ..
  make -j 5
  make install
  ```

4. Build and install qml-chilitags:

  ```
  mkdir build-desktop
  cd build-desktop
  /qt/install/root/5.3/gcc_64/bin/qmake ..
  make -j 5
  make install
  ```

  Now the Chilitags QML plugin is installed alongside Qt's QML plugins and can be used similar to any other plugin.

Android Build
-------------

In addition to the OS, Qt and OpenCV requirements, you need:

  - Android SDK and Android API 14 (Android version 4.0)

  - Android NDK r9d

These instructions assume `armv7-a` target architecture. For other architectures, adapt the instructions to your liking.

1. Export a standalone NDK toolchain:

  ```
  cd /path-to-android-ndk-r9d
  ./build/tools/make-standalone-toolchain.sh \
      --platform=android-14 \
      --install-dir=/desired/path/to/android/standalone/toolchain \
      --toolchain=arm-linux-androideabi-4.8
  ```

  Be aware that if you don't have write access to `/desired/path/to/android/standalone/toolchain`, the script fails silently.

2. Set up the following environment variables:

  ```
  export ANDROID_HOME=/path/to/android/sdk/linux
  export ANDROID_SDK_ROOT=$ANDROID_HOME
  export ANDROID_SDK=$ANDROID_SDK_ROOT
  export ANDROID_NDK_ROOT=/path-to-android-ndk-r9d
  export ANDROID_NDK=$ANDROID_NDK_ROOT
  export ANDROID_NDK_STANDALONE_TOOLCHAIN=/path/to/android/standalone/toolchain
  export ANDROID_STANDALONE_TOOLCHAIN=$ANDROID_NDK_STANDALONE_TOOLCHAIN
  export ANDROID_ABI=armeabi-v7a
  export ANDROID_NATIVE_API_LEVEL=14
  export ANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.8
  ```

3. Build and install OpenCV `3.0.0-alpha` tag:

  ```
  git checkout 3.0.0-alpha
  cd platforms
  mkdir build-desktop
  cd build-desktop
  cmake ../.. \
      -DCMAKE_TOOLCHAIN_FILE=../android/android.toolchain.cmake \
      -DCMAKE_INSTALL_PREFIX=$ANDROID_STANDALONE_TOOLCHAIN/sysroot/usr/share/opencv/
  ```

  At this point, enable `BUILD_SHARED_LIBS`.

  OpenCV components other than `calib3d`, `core`, `features2d`, `flann`, `highgui`, `imgcodecs`, `imgproc`, `video` and `videoio` are not needed and can be disabled. Leave these options untouched if you are unsure what to do.

  The options can be accessed with `ccmake ../..` after running `cmake`.

  ```
  make -j 5
  make install
  ```

4. Set up the following environment variable:

  ```
  export OpenCV_DIR=${ANDROID_STANDALONE_TOOLCHAIN}/sysroot/usr/share/opencv/sdk/native/jni/
  ```

5. Build and install `chilitags` from [https://github.com/chili-epfl/chilitags](https://github.com/chili-epfl/chilitags):

  ```
  mkdir build-desktop
  cd build-desktop
  cmake .. \
      -DCMAKE_TOOLCHAIN_FILE=$OpenCV_DIR/android.toolchain.file
      -DCMAKE_INSTALL_PREFIX=$ANDROID_STANDALONE_TOOLCHAIN/sysroot/usr/
  ```

  At this point, disable `ANDROID_INSTALL_LIBRARIES` and `WITH_JNI_BINDINGS` since you won't be using Chilitags inside an Android project directly.

  ```
  make -j 5
  make install
  ```

6. **Optional**: Build and install `qt3d` from [https://qt.gitorious.org/qt/qt3d](https://qt.gitorious.org/qt/qt3d):

  This enables QML interaction between Qt3D and Chilitags.

  ```
  mkdir build-android
  cd build-android
  /qt/install/root/5.3/android_armv7/bin/qmake ..
  make -j 5
  make install
  ```

**Note:** `make install` will try to install libraries to `/libs/armeabiv7-a/`. This is a remnant from ndk-build and should be avoided. Make sure you **don't** execute `make install` with `sudo`.

7. Build and install qml-chilitags:

  ```
  mkdir build-android
  cd build-android
  /qt/install/root/5.3/android_armv7/bin/qmake ..
  make -j 5
  make install
  ```

  Now the Chilitags QML plugin is installed alongside Qt's QML plugins and can be used similar to any other plugin.

  Note that you still need to bundle the required OpenCV libraries with `ANDROID_EXTRA_LIBS` in the project that uses Chilitags. See [samples/detection/detection.pro](samples/detection/detection.pro) for an example.

Running Samples
---------------

See [samples/README.md](samples/README.md).

