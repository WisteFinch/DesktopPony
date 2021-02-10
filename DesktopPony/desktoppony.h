/**
 * @file desktoppony.h
 * @brief 桌面小马主类
 * @author WisteFinch
 * @date 2020.11.29
 *
 * MIT License
 * Copyright (c) 2019-2021 WisteFinch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef DESKTOPPONY_H
#define DESKTOPPONY_H

#include "limit.h"
#include "imageprocessing/imageprocessing.h"
#include "ui/uisettings.h"
#include "file/config.h"
#include "file/fileevent.h"
#include "metadata.h"
#include "file/filecharacter.h"
#include "file/fileqss.h"
#include "ui/uipony.h"
#include "file/localisation.h"
#include <QObject>

//=============-=====test========================
#include "scriptinterpreter/scriptinterpreter.h"
//===============================================

class DesktopPony : QObject
{
    Q_OBJECT
public:
    DesktopPony();
    void start();
    void initConnect();

private:
    FileTasks *fileTasks;
    Config *config;
    Localisation *localisation;
    FileCharacter *fileCharacter;
    Limit *limit;
    QSS *qss;
    UISettings *uiSettings;
    UIPony *uiPony;
    ImageProcessing *imageProcessing;

private slots:
    void slotSettings(int);
};

#endif // DESKTOPPONY_H
