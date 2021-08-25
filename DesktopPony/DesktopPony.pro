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
    data/config.cpp \
    data/localisation.cpp \
    data/style.cpp \
    data/text.cpp \
    file/filecharacter.cpp \
    file/filetasks.cpp \
    plugin/data/pluginelementconfigdata.cpp \
    plugin/data/pluginelementlocalisationdata.cpp \
    plugin/element/pluginelementconfig.cpp \
    plugin/element/pluginelementevent.cpp \
    plugin/element/pluginelementlibrary.cpp \
    plugin/element/pluginelementlocalisation.cpp \
    plugin/element/pluginelement.cpp \
    plugin/element/pluginelementmodel.cpp \
    plugin/element/pluginelementresources.cpp \
    plugin/element/pluginelementstyle.cpp \
    plugin/pluginmanager.cpp \
    plugin/pluginobject.cpp \
    main.cpp \
    desktoppony.cpp \
    plugin/pluginsharedefinition.cpp \
    scriptinterpreter/core/lexer/lexer.cpp \
    scriptinterpreter/core/parser/parser.cpp \
    scriptinterpreter/core/parser/parsernode.cpp \
    scriptinterpreter/core/public/scriptexceptiondata.cpp \
    scriptinterpreter/core/public/scriptvariabledata.cpp \
    scriptinterpreter/core/scriptexception.cpp \
    scriptinterpreter/core/scriptinterpreter.cpp \
    scriptinterpreter/core/scriptlimit.cpp \
    scriptinterpreter/core/scriptvariable.cpp \
    scriptinterpreter/scriptinterpretershell.cpp \
    tools.cpp \
    ui/character/uicharacterpage.cpp \
    ui/config/uiconfigitemcard.cpp \
    ui/config/uiconfigpage.cpp \
    ui/info/uiinfopage.cpp \
    ui/plugin/editor/uipluginobjeditor.cpp \
    ui/plugin/editor/uipluginobjmetadataeditor.cpp \
    ui/plugin/uipluginfilter.cpp \
    ui/plugin/uipluginlistcard.cpp \
    ui/plugin/uipluginpage.cpp \
    ui/tools/animationstackedwidget.cpp \
    ui/tools/hline.cpp \
    ui/tools/vline.cpp \
    ui/uimainpanel.cpp

HEADERS += \
    data/config.h \
    data/localisation.h \
    data/style.h \
    data/text.h \
    desktoppony.h \
    file/filecharacter.h \
    file/fileevent.h \
    plugin/data/pluginelementconfigdata.h \
    plugin/data/pluginelementlocalisationdata.h \
    plugin/element/pluginelementconfig.h \
    plugin/element/pluginelementevent.h \
    plugin/element/pluginelementlibrary.h \
    plugin/element/pluginelementlocalisation.h \
    plugin/element/pluginelement.h \
    plugin/element/pluginelementmodel.h \
    plugin/element/pluginelementresources.h \
    plugin/element/pluginelementstyle.h \
    plugin/pluginmanager.h \
    plugin/pluginobject.h \
    metadata.h \
    plugin/pluginsharedefinition.h \
    scriptinterpreter/core/lexer/lexer.h \
    scriptinterpreter/core/parser/parser.h \
    scriptinterpreter/core/parser/parsernode.h \
    scriptinterpreter/core/public/scriptexceptiondata.h \
    scriptinterpreter/core/public/scriptvariabledata.h \
    scriptinterpreter/core/public/sharedefinition.h \
    scriptinterpreter/core/scriptexception.h \
    scriptinterpreter/core/scriptinterpreter.h \
    scriptinterpreter/core/scriptlimit.h \
    scriptinterpreter/core/scriptvariable.h \
    scriptinterpreter/scriptinterpretershell.h \
    tools.h \
    ui/character/uicharacterpage.h \
    ui/config/uiconfigitemcard.h \
    ui/config/uiconfigpage.h \
    ui/info/uiinfopage.h \
    ui/plugin/editor/uipluginobjeditor.h \
    ui/plugin/editor/uipluginobjmetadataeditor.h \
    ui/plugin/uipluginfilter.h \
    ui/plugin/uipluginlistcard.h \
    ui/plugin/uipluginpage.h \
    ui/tools/animationstackedwidget.h \
    ui/tools/hline.h \
    ui/tools/vline.h \
    ui/uimainpanel.h

FORMS += \
    ui/plugin/editor/uipluginobjeditor.ui \
    ui/uimainpanel.ui

RESOURCES += \
    image.qrc \
    data.qrc \
    plugin.qrc

VERSION = 0.0.0.1
RC_ICONS = img/icon.ico
RC_FILE += winappinfo.rc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
