TEMPLATE = lib
TARGET = chilitagsplugin

CONFIG += qt plugin c++11 nostrip
CONFIG -= android_install

QT += qml quick

qtHaveModule(3d){
    message("Qt3D found, adding 3d components.")
    QT += 3d
    DEFINES += QT_3D_LIB    #QMake should be adding this but it doesn't...

    HEADERS += \
        src/MatrixTransform3D.h

    SOURCES += \
        src/MatrixTransform3D.cpp
}
else{
    message("Qt3D not found, skipping 3d components.")
}

QMAKE_CXXFLAGS -= -O2
QMAKE_CXXFLAGS_RELEASE -= -O2

QMAKE_CXXFLAGS += -O3
QMAKE_CXXFLAGS_RELEASE += -O3

TARGET = $$qtLibraryTarget($$TARGET)
uri = Chilitags

HEADERS += \
    src/MatrixTransform.h \
    src/ChilitagsDetection.h \
    src/ChilitagsObject.h \
    src/ChilitagsPlugin.h

SOURCES += \
    src/MatrixTransform.cpp \
    src/ChilitagsDetection.cpp  \
    src/ChilitagsObject.cpp \
    src/ChilitagsPlugin.cpp

LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc
LIBS += -lchilitags

android {

    #Enable automatic NEON vectorization
    QMAKE_CXXFLAGS -= -mfpu=vfp
    QMAKE_CXXFLAGS_RELEASE -= -mfpu=vfp
    QMAKE_CXXFLAGS += -mfpu=neon -ftree-vectorize -ftree-vectorizer-verbose=1 -mfloat-abi=softfp
    QMAKE_CXXFLAGS_RELEASE += -mfpu=neon -ftree-vectorize -ftree-vectorizer-verbose=1 -mfloat-abi=softfp

    INCLUDEPATH += $(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/include
    INCLUDEPATH += $(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/share/opencv/sdk/native/jni/include
    LIBS += -L$(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/lib
    LIBS += -L$(ANDROID_STANDALONE_TOOLCHAIN)/sysroot/usr/share/opencv/sdk/native/libs/armeabi-v7a/
}

OTHER_FILES += qmldir #chilitags.types

#Install plugin library, qmldir and types
qmldir.files = qmldir
#types.files = chilitags.types
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    #types.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir #types
}

