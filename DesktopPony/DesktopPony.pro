#-------------------------------------------------
#
# Project created by QtCreator 2019-04-06T17:01:21
# Author: WisteFinch
#
# MIT License
# Copyright (c) 2019-2021 WisteFinch
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
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
    file/fileqss.cpp \
	file/localisation.cpp \
    file/filetasks.cpp \
    imageprocessing/accessories.cpp \
    imageprocessing/imageprocessing.cpp \
    main.cpp \
    desktoppony.cpp \
    limit.cpp \
    scriptinterpreter/scriptexception.cpp \
    scriptinterpreter/scriptinterpreter.cpp \
    scriptinterpreter/scriptlimit.cpp \
    scriptinterpreter/scriptvariable.cpp \
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
    file/fileevent.h \
    file/fileqss.h \
	file/localisation.h \
    imageprocessing/accessories.h \
    imageprocessing/imageprocessing.h \
    imageprocessing/imagestruct.h \
    limit.h \
    metadata.h \
    scriptinterpreter/scriptexception.h \
    scriptinterpreter/scriptinterpreter.h \
    scriptinterpreter/scriptlimit.h \
    scriptinterpreter/scriptvariable.h \
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
    css.qrc \
    image.qrc \
    localisation.qrc \
    data.qrc

VERSION = 0.0.0.1
RC_ICONS = img/icon.ico
RC_FILE += winappinfo.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
