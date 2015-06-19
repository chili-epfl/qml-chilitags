TEMPLATE = app

QT += qml quick multimedia widgets
CONFIG += debug

QT += 3dcore 3drenderer 3dquick 3dinput qml quick

SOURCES += src/main.cpp

RESOURCES += qml.qrc

android {
    target.path = /libs/armeabi-v7a
    export(target.path)
    INSTALLS += target
    export(INSTALLS)

    #In order to debug custum plugin, we need to copy the library in the build dir so that gdb canm load the symbols
    #http://visualgdb.com/gdbreference/commands/set_solib-search-path (I cannot set it in QTC, it is overwritter...)

    ANDROID_EXTRA_LIBS = \
        /home/chili/opencv/platforms/build-android/install/sdk/native/libs/armeabi-v7a/libopencv_core.so \
        /home/chili/opencv/platforms/build-android/install/sdk/native/libs/armeabi-v7a/libopencv_imgproc.so \
        /home/chili/opencv/platforms/build-android/install/sdk/native/libs/armeabi-v7a/libopencv_imgcodecs.so \
        /home/chili/opencv/platforms/build-android/install/sdk/native/libs/armeabi-v7a/libopencv_flann.so \
        /home/chili/opencv/platforms/build-android/install/sdk/native/libs/armeabi-v7a/libopencv_ml.so \
        /home/chili/opencv/platforms/build-android/install/sdk/native/libs/armeabi-v7a/libopencv_features2d.so \
        /home/chili/opencv/platforms/build-android/install/sdk/native/libs/armeabi-v7a/libopencv_calib3d.so \
        /home/chili/opencv/platforms/build-android/install/sdk/native/libs/armeabi-v7a/libopencv_video.so \
        /home/chili/opencv/platforms/build-android/install/sdk/native/libs/armeabi-v7a/libopencv_videoio.so \
        /home/chili/opencv/platforms/build-android/install/sdk/native/libs/armeabi-v7a/libopencv_highgui.so \
        /home/chili/chilitags/build-android/install/lib/libchilitags.so \
        $$[QT_INSTALL_QML]/Chilitags/libchilitagsplugin.so \

    ANDROID_PERMISSIONS += \
        android.permission.CAMERA

    ANDROID_FEATURES += \
        android.hardware.camera
}
!android{
    INCLUDEPATH += /home/chili/opencv/build-linux/install/include
    INCLUDEPATH += /home/chili/chilitags/build-linux/install/include
    LIBS += -L/home/chili/opencv/build-linux/install/lib
    LIBS += -L/home/chili/chilitags/build-linux/install/lib
    LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc
    LIBS += -lchilitags
}

DISTFILES += \
    android/android-libtemplates.so-deployment-settings.json \
    android/AndroidManifest.xml \
    android/libtemplates.so \
    android/res/values/libs.xml \
    android/build.gradle \
    android/Makefile

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android


