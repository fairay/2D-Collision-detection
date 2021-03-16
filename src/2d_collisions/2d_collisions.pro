QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp
LIBS += -fopenmp

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algorithms/base_tree.cpp \
    algorithms/bin_tree.cpp \
    algorithms/brute_force.cpp \
    algorithms/dynamic_quad_tree.cpp \
    algorithms/hexa_tree.cpp \
    algorithms/quad_tree.cpp \
    algorithms/rect_tree.cpp \
    ball/ball.cpp \
    main.cpp \
    mainwindow.cpp \
    scene/scene.cpp \
    scene/timer_.cpp

HEADERS += \
    algorithms/base_tree.h \
    algorithms/rect_tree.h \
    algorithms/thread_manager.h \
    ball/ball.h \
    mainwindow.h \
    scene/scene.h \
    scene/timer_.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
