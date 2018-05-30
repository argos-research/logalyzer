TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    event.cpp \
    Stage.cpp \
    CapMap.cpp \
    Logalyzer.cpp \
    L4Thread.cpp \
    L4Gate.cpp \
    L4IRQ.cpp \
    L4Object.cpp \
    L4Task.cpp

HEADERS += \
    event.h \
    Stage.h \
    CapMap.h \
    Logalyzer.h \
    CapabilityInfo.h \
    L4Object.h \
    L4Thread.h \
    L4Gate.h \
    L4IRQ.h \
    L4Task.h
