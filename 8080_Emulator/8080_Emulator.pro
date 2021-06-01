QT       += core gui
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/instructionWindow.cpp \
    src/main.cpp \
    src/mainWindow.cpp \
    src/cpu/cpu.cpp \
    src/emulator/emulator.cpp \
    src/flags/flags.cpp \
    src/gui/gui.cpp

HEADERS += \
    src/instructionWindow.h \
    src/mainWindow.h \
    src/cpu/cpu.h \
    src/emulator/emulator.h \
    src/flags/flags.h \
    src/gui/gui.h

FORMS += \
    src/instructionWindow.ui \
    src/mainWindow.ui

RC_ICONS = src/space_invader_icon.ico

RESOURCES = src/res.qrc
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
