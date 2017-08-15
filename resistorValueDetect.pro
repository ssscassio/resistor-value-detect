QT       += core
QT       -= gui

TARGET = OpenCV-Ex1
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

win32 {
    INCLUDEPATH += C:/opencv-build-2.4.13/install/include

    LIBS += -LC:/opencv-build-2.4.13/install/x86/mingw/lib \
        -llibopencv_core2413 \
        -llibopencv_highgui2413 \
        -llibopencv_imgproc2413 \
        -llibopencv_features2d2413 \
        -llibopencv_calib3d2413 \
        -llibopencv_ml2413 \
        -llibopencv_features2d2413
}

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += opencv

}
