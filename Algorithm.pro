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


LIBS += -L$$PWD/Libs/ -lhalcon
LIBS += -lhalcon \
        -lhalconcpp \
        -lhdevenginecpp

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/Libs/ -lopencv_world4110
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/Libs/ -lopencv_world4110d

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include


win32: LIBS += -L$$PWD/Libs/ -lnpymath

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include


win32: LIBS += -L$$PWD/Libs/ -lpython39

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/Libs/python39.lib
