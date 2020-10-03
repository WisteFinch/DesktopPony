#-------------------------------------------------
#
# Project created by QtCreator 2019-04-06T17:01:21
# Author: WiFi_Sunglow
#
#-------------------------------------------------

QT       += core gui svg xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DesktopPony
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    file/config.cpp \
    file/filecharacter.cpp \
	file/localisation.cpp \
    file/filetasks.cpp \
    imageprocessing/accessories.cpp \
    imageprocessing/imageprocessing.cpp \
    main.cpp \
    desktoppony.cpp \
    limit.cpp \
    qss.cpp \
    scriptinterpreter/scriptinterpreter.cpp \
    ui/uicharacter/uicharacter.cpp \
    ui/uicharacter/uicharacterlistcard.cpp \
    ui/uipony.cpp \
    ui/uisettings.cpp \
    ui/uitasks/uitasks.cpp \
    ui/uitasks/uitaskseventcard.cpp \
    ui/uitasks/uitasksinfo.cpp \
    ui/uitasks/uitaskslistcard.cpp \
    ui/uitasks/uitasksstandardlistcard.cpp \
    scriptinterpreter/lexer/lexer.cpp \
    scriptinterpreter/parser/parser.cpp \
    scriptinterpreter/parser/parsernode.cpp

HEADERS += \
    desktoppony.h \
    file/config.h \
    file/filecharacter.h \
    file/filetasks.h \
	file/localisation.h \
    imageprocessing/accessories.h \
    imageprocessing/imageprocessing.h \
    imageprocessing/imagestruct.h \
    limit.h \
    metadata.h \
    qss.h \
    scriptinterpreter/limit.h \
    scriptinterpreter/scriptinterpreter.h \
    ui/uicharacter/uicharacter.h \
    ui/uicharacter/uicharacterlistcard.h \
    ui/uipony.h \
    ui/uisettings.h \
    ui/uitasks/uitasks.h \
    ui/uitasks/uitaskseventcard.h \
    ui/uitasks/uitasksinfo.h \
    ui/uitasks/uitaskslistcard.h \
    ui/uitasks/uitasksstandardlistcard.h \
    scriptinterpreter/lexer/lexer.h \
    scriptinterpreter/parser/parser.h \
    scriptinterpreter/parser/parsernode.h

FORMS += \
    ui/uicharacter/uicharacter.ui \
    ui/uipony.ui \
    ui/uisettings.ui \
    ui/uitasks/uitasks.ui \
    ui/uitasks/uitasksinfo.ui

RESOURCES += \
    image.qrc \
    localisation.qrc \
    qss.qrc \
    data.qrc

RC_FILE += resource.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
