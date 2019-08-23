QT += quick
QT += testlib
QT += network


CONFIG += c++11


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    AppMain.hpp \
    AppEnums.hpp \
    AppDefines.hpp \
    Model/Model.hpp \
    Controller/MainController.hpp \
    Controller/FarmActions.hpp \
    APIs/AutoFarmerJNI.hpp \
    APIs/AutoFarmerAPIsWraper.hpp \
#    MultiThreads/CheckCurrSrcWorker.hpp
    Controller/ScreenAnalysis.h

SOURCES += \
        main.cpp \
    AppMain.cpp \
    Model/Model.cpp \
    Controller/MainController.cpp \
    Controller/FarmActions.cpp \
    APIs/AutoFarmerJNI.cpp \
    APIs/AutoFarmerAPIsWraper.cpp \
#    MultiThreads/CheckCurrSrcWorker.cpp
    Controller/ScreenAnalysis.cpp

INCLUDEPATH += $$PWD/Model \
               $$PWD/Controller\
                $$PWD/APIs \
                $$PWD/MultiThreads

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.


android {
    QT += androidextras

    DEFINES += APIS_DEBUG_MODE

    INCLUDEPATH += $$PWD/AutoFarmer.API/Android/include

    LIBS += \
        -L"$$PWD/AutoFarmer.API/Android/libs/$$ANDROID_TARGET_ARCH"\
        -lAutoFarmerAPI\
        -llibtiff\
        -llibjpeg\
        -llibjasper\
        -llibpng\
        -lIlmImf\
        -ltbb\
        -lopencv_core\
        -lopencv_androidcamera\
        -lopencv_flann\
        -lopencv_imgproc\
        -lopencv_highgui\
        -lopencv_features2d\
        -lopencv_calib3d\
        -lopencv_ml\
        -lopencv_objdetect\
        -lopencv_video\
        -lopencv_contrib\
        -lopencv_photo\
        -lopencv_java\
        -lopencv_legacy\
        -lopencv_ocl\
        -lopencv_stitching\
        -lopencv_superres\
        -lopencv_ts\
        -lopencv_videostab



    ANDROID_EXTRA_LIBS += \
        $$PWD/AutoFarmer.API/Android/libs/$$ANDROID_TARGET_ARCH/libcrypto.so \
        $$PWD/AutoFarmer.API/Android/libs/$$ANDROID_TARGET_ARCH/libssl.so\
        $$PWD/AutoFarmer.API/Android/libs/$$ANDROID_TARGET_ARCH/libopencv_java.so

    ANDROID_PACKAGE_SOURCE_DIR=$$_PRO_FILE_PWD_/android-sources

    DISTFILES += android-sources/AndroidManifest.xml \
                android-sources/assets/images/
}


