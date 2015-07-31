qml-chilitags
=============

QMLChilitags is a QML wrapper for the fiducial marker tracker Chilitags. It works on desktop and Android.

The following is required for QMLChilitags to work:

  - Ubuntu `14.04`
  - Qt `5.5`
  - OpenCV `3.0.0-beta` (**Note:** If you checked out OpenCV master branch, change `cv::ITERATIVE` to `cv::SOLVEPNP_ITERATIVE` in `Chilitags3D.cpp` when building Chilitags. At the time of writing, this API change in OpenCV is not yet in a tag, so it is not included in the Chilitags source.)

QML API
-------

The QML API consists of the following:

>  - **ChilitagsDetection** A wrapper around chilitags. The class implements the `QAbstractVideoFilter`, hence it receives a frame from a VideoOutput and processes it.
>   -**ChilitagsObject** An object define by the tag number. Chilitags are added to the `chiliobjects` property of ChiliDetection 
>   -**ChiliThread** Class running the detection in a separated thread. Implements `QVideoFilterRunnable`.


Desktop Build
-------------

1. Build and install OpenCV `3.0.0-beta` tag:

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
  make install **optional**
  ```

2. Build and install `chilitags` from [https://github.com/chili-epfl/chilitags](https://github.com/chili-epfl/chilitags):

  ```
  mkdir build-desktop
  cd build-desktop
  cmake .. -DCMAKE_INSTALL_PREFIX=/usr
  make -j 5
  make install **optional**
  ```

3. Build and install qt  (http://qt-project.org/wiki/Building_Qt_5_from_Git):
	For making the compilation faster, skip the following modelus in the configure step
	
	```
	-skip qttranslations -skip qtwebkit -skip qtserialport -skip qtwebkit-examples  -skip qtwebengine 	

	```

4. Build and install qt creator (http://wiki.qt.io/Building-Qt-Creator-from-Git):
	

5. Open qml-chilitags in Qt creator

  Now the Chilitags QML plugin is installed alongside Qt's QML plugins and can be used similar to any other plugin.


Android Build
-------------

In addition to the OS, Qt and OpenCV requirements, you need:

  - Android SDK and Android API 14 (Android version 4.0)

  - Android NDK r10d

These instructions assume `armv7-a` target architecture. For other architectures, adapt the instructions to your liking.


1. Build and install OpenCV `3.0.0-alpha` tag:

  ```
  git checkout 3.0.0-alpha
  cd platforms
  mkdir build-android
  cd build-android
  cmake ../.. \
      -DANDROID_NDK=[android-ndk-dir] \
      -DCMAKE_TOOLCHAIN_FILE=../android/android.toolchain.cmake \
      -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.9 \
      -DCMAKE_INSTALL_PREFIX=[absolute-path-build-android]/install \
      -DANDROID_NDK_LAYOUT=RELEASE \
      -DANDROID_ABI="armeabi-v7a"
   ```

  At this point, enable `BUILD_SHARED_LIBS`.

  OpenCV components other than `calib3d`, `core`, `features2d`, `flann`, `highgui`, `imgcodecs`, `imgproc`, `video` and `videoio` are not needed and can be disabled. Leave these options untouched if you are unsure what to do.

  The options can be accessed with `ccmake ../..` after running `cmake`.

  ```
  make -j 5
  make install
  ```

2. Build and install `chilitags` from [https://github.com/chili-epfl/chilitags](https://github.com/chili-epfl/chilitags):

  Set up the following environment variables:
  ```
  export ANDROID_NATIVE_API_LEVEL=14
  export ANDROID_ABI=armeabi-v7a
   
  ```
  
  ```
  mkdir build-android
  cd build-android
  cmake .. \
      -DCMAKE_TOOLCHAIN_FILE=[opencv-dir]/platforms/android/android.toolchain.cmake \
      -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.9 \
      -DCMAKE_INSTALL_PREFIX=[absolute-path-build-android]/install \
      -DANDROID_NDK_LAYOUT=RELEASE \
      -DOpenCV_DIR=opencv-build-android-directory 

  ```

  At this point, disable `ANDROID_INSTALL_LIBRARIES` and `WITH_JNI_BINDINGS` since you won't be using Chilitags inside an Android project directly.

  ```
  make -j 5
  make install 
  ```
The previous two steps can be customized in for the CortexA15.A7 of Tablet Samsung SM-P600 by replacing the `ANDROID_ABI="armeabi-v7a with CortexA15"` and substitute the file  [opencv-dir]/cmake/OpenCVCompilerOptions.cmake and [opencv-dir/platforms/android/android.toolchain.cmake] with the ones from [https://github.com/lorenzolightsgdwarf/opencv-3.00-CortexA15-config]


3. Build and install `qt` from (http://qt-project.org/wiki/Qt5ForAndroidBuilding):

  ```
  cd qt 5
  mkdir qt-build-android && cd qt-build-android
  ../configure -xplatform android-g++ -developer-build -opensource -nomake tests -nomake examples -android-sdk [android-sdk-dir]  -android-ndk [android-ndk-r10d-dir] -android-toolchain-version 4.9  -android-ndk-host linux-x86_64 -android-ndk-platform android-19 -android-arch armeabi-v7a -confirm-license -no-warnings-are-errors -skip qtactiveqt -skip qttranslations -skip qtwebkit -skip qtserialport -skip qtwebkit-examples -skip qtdoc -skip qtrepotools -skip qtqa -skip qtsystems -skip qtdocgallery -skip qtpim -skip qtwayland -skip qtx11extras -skip qtmacextras -skip qtwinextras -skip qtenginio -skip webchannel
  make
  ```

To customize for CortexA15.A7 of Tablet Samsung SM-P600, enabling neon and hard float-abi:
  
   ```
   	gedit qt5/qtbase/mkspecs/android-g++/qmake.conf
	Search for `equals(ANDROID_TARGET_ARCH, armeabi-v7a)`, comment and substitute it with 
	QMAKE_CFLAGS = -Wno-psabi -march=armv7ve -mtune=cortex-a15.cortex-a7  -mcpu=cortex-a15 -mfpu=neon-vfpv4 -mhard-float -mfloat-abi=hard -  D_NDK_MATH_NO_SOFTFP=1  -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums -DANDROID -Wa,--noexecstack -fno-builtin-memmove  -ftree-vectorize -ftree-vectorizer-verbose=1 -Wno-error=cast-align
	search for line `QMAKE_LFLAGS = --sysroot=$$ANDROID_PLATFORM_ROOT_PATH` and add -Wl,--no-warn-mismatch -lm_hard
	Search for `QMAKE_LIBS_PRIVATE`, remove -lm and add -lm_hard

   ```

4. Using qml-chilitags in Qt creator:

  Remember to add 'make install' after the step 'make' in the Build Steps.
  
	In order to debug the pluging when used in a 3rd qml application, you need to make a symbolic link to the plugin library (.so) in the build dir of the application so that gdb canm load the symbols [http://visualgdb.com/gdbreference/commands/set_solib-search-path] (I cannot set it in QTC, it is overwritter...) 

QT Creator doesn't recognize the pluging
----------------------------------------
With reference to [http://doc.qt.io/qt-5/qtqml-modules-qmldir.html#writing-a-qmltypes-file]

Usually plugins get installed in [qt-build]/qtbase/qml/Plugin-name
For some obscure reason Qt doesn't create the plugins.qmltype that provides the plugin description.
In order to solve it, you have to manualy create the file using the tool `qmlplugindump` in [qt-built]/qtbase/bin

For example, 
```
qmlplugindump Chilitags 1.0 [qt-build]/qtbase/qml/Chilitags > [qt-build]/qtbase/qml/Chilitags/plugin.qmltypes
```

Running Samples
---------------

See [samples/README.md](samples/README.md).

