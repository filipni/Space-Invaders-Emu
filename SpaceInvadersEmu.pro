TEMPLATE = app
TARGET = SpaceInvadersEmu

QT = core gui widgets

SOURCES += \
    main.cpp \
    emulator.cpp \
    cpu.cpp \
    flagregister.cpp

HEADERS += \
    emulator.h \
    cpu.h \
    flagregister.h
