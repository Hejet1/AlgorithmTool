QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Algorithm.cpp \
    algorithmparam.cpp \
    main.cpp \
    mainwindow.cpp \
    my_hdevoperatorimpl.cpp \
    my_thread_impl.cpp \
    qhalconwindow.cpp \
    run.cpp \
    test.cpp \
    toolbox.cpp

HEADERS += \
    Algorithm.h \
    algorithmparam.h \
    mainwindow.h \
    my_hdevoperatorimpl.h \
    my_thread_impl.h \
    qhalconwindow.h \
    run.h \
    test.h \
    toolbox.h

FORMS += \
    algorithmparam.ui \
    mainwindow.ui \
    test.ui \
    toolbox.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QMAKE_PROJECT_DEPTH = 0


LIBS += -LC:/Users/Administrator/AppData/Local/Programs/MVTec/HALCON-22.05-Progress/lib/x64-win64/ -lhalcon
LIBS += -lhalcon \
        -lhalconcpp \
        -lhdevenginecpp

INCLUDEPATH += C:/Users/Administrator/AppData/Local/Programs/MVTec/HALCON-22.05-Progress/include
DEPENDPATH += C:/Users/Administrator/AppData/Local/Programs/MVTec/HALCON-22.05-Progress/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../OpenCV/opencv/build/x64/vc16/lib/ -lopencv_world4110
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../OpenCV/opencv/build/x64/vc16/lib/ -lopencv_world4110d

INCLUDEPATH += $$PWD/../../OpenCV/opencv/build/include
DEPENDPATH += $$PWD/../../OpenCV/opencv/build/include


win32:LIBS += -L$$PWD/../../ProgramData/Anaconda/envs/yolo/Lib/site-packages/numpy/core/lib/ -lnpymath
INCLUDEPATH += $$PWD/../../ProgramData/Anaconda/envs/yolo/Lib/site-packages/numpy/core/include
DEPENDPATH += $$PWD/../../ProgramData/Anaconda/envs/yolo/Lib/site-packages/numpy/core/include

win32:LIBS += -L$$PWD/../../ProgramData/Anaconda/envs/yolo/libs/ -lpython39
INCLUDEPATH += $$PWD/../../ProgramData/Anaconda/envs/yolo/include
DEPENDPATH += $$PWD/../../ProgramData/Anaconda/envs/yolo/include
