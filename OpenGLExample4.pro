QT += core
QT -= gui

CONFIG += c++11

TARGET = OpenGLExample4
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

LIBS += -l3te -lSDL2 -lSPM -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lGL -lGLU

QMAKE_CXXFLAGS = -std=c++11

SOURCES += \
    src/main.cpp \
    src/customframe.cpp \
    src/state.cpp \
    src/rawmap.cpp

HEADERS += \
    src/customframe.h \
    src/state.h \
    src/rawmap.h


DEFINES += QT_DEPRECATED_WARNINGS
