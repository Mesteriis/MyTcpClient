QT       += core gui network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    guicore.cpp \
    qcustomplot.cpp \
    swcore.cpp

HEADERS += \
    guicore.hpp \
    qcustomplot.hpp \
    swcore.hpp

FORMS += \
    guicore.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc


macx:ICON = $${PWD}/client.icns

VERSION = 1.0.0

QMAKE_TARGET_COMPANY = "Shade Inc"
QMAKE_TARGET_PRODUCT = MyTcpClient
QMAKE_TARGET_DESCRIPTION = Simpli Tcp Client graf
QMAKE_TARGET_COPYRIGHT = AVM
