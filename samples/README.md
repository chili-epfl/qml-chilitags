qml-chilitags Samples
---------------------

These samples demonstrate the usage of qml-chilitags. To build a sample:

  - Build and install qml-chilitags itself, see [../README.md](../README.md)
  - Go inside a sample's directory and follow the instructions below

QtCreator Build for Desktop and Android
---------------------------------------

Open the project in Qt Creator.

Fix the include and libs path in the .pro file according to the install directories of Opencv and Chilitag

To debug qml-chilitags on Android, make a symbolic link to the build directory 

Example libchilitagsplugin.so in build-detection-Android_for_armeabi_v7a_GCC_4_9_Qt_5_4_3-Debug

Note about Android camera
-----------------------
The implementation of the Qml Camera doesn't allow to set the viewfinder resolution on Android. The default resolution the highest preview resolution available that has the same aspect ratio of the imageCapture resolution [qt-multimedia/src/plugins/android/src/mediacapture/qandroidcamerasession.cpp]. This means that chiltags will work on a 8Mpixel image! Temporary solution is to modify the 
qandroidcamerasession.cpp to pick a preview resolution equal to the imageCapture one [https://github.com/lorenzolightsgdwarf/qtmultimedia-qandroidcamerasession/commit/5c00fcd0dd94152a6f3815aef433e34632e2759c]