TEMPLATE = lib
TARGET = chilitagsplugin

CONFIG += qt plugin c++11 nostrip debug
CONFIG -= android_install

QT += qml quick multimedia


TARGET = $$qtLibraryTarget($$TARGET)
uri = Chilitags

HEADERS += \
    src/ChilitagsDetection.h \
    src/ChilitagsThread.h \
    src/ChilitagsObject.h \
    src/ChilitagsPlugin.h

SOURCES += \
    src/ChilitagsDetection.cpp  \
    src/ChilitagsThread.cpp \
    src/ChilitagsObject.cpp \
    src/ChilitagsPlugin.cpp

LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc
LIBS += -lchilitags

android {
    #INCLUDEPATH += $(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/include
    INCLUDEPATH += /home/chili/opencv/platforms/build-android/install/sdk/native/jni/include
    INCLUDEPATH += /home/chili/chilitags/build-android/install/include

    #LIBS += -L$(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/lib
    LIBS += -L/home/chili/opencv/platforms/build-android/install/sdk/native/libs/armeabi-v7a/
    LIBS += -L/home/chili/chilitags/build-android/install/lib

}
!android{
    INCLUDEPATH += /home/chili/opencv/build-linux/install/include
    INCLUDEPATH += /home/chili/chilitags/build-linux/install/include
    LIBS += -L/home/chili/opencv/build-linux/install/lib
    LIBS += -L/home/chili/chilitags/build-linux/install/lib
}

OTHER_FILES += qmldir #chilitags.types

#Install plugin library, qmldir and types
qmldir.files = qmldir
#types.files = chilitags.types

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
qmldir.path = $$installPath
#types.path = $$installPath
target.path = $$installPath

INSTALLS += target qmldir #types


