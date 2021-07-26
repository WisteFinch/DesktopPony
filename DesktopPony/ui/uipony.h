/**
 * @file ui/uipony.h
 * @brief 界面-小马
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

#ifndef UIPONY_H
#define UIPONY_H

#include "data/localisation.h"
#include "data/style.h"
#include "imageprocessing/imageprocessing.h"
#include "tools.h"
#include "file/config.h"
#include <QWidget>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPoint>
#include <QPixmap>
#include <QLabel>
#include <QTimer>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include <QPointer>

namespace Ui
{
class UIPony;
}

class UIPony : public QWidget
{
    Q_OBJECT

public:
    explicit UIPony(QWidget *parent = nullptr);
    ~UIPony();
    void init(Localisation *tempLocalisation, Style *tmpStyle, Config *tempConfig, Tools *tempTools, ImageProcessing *tempImageProcessing);
    //组件初始化
    void initItems();
    //应用布局
    void setItemsLayout();
    //连接信号
    void setConnect();
    void initMenu();
    void initSystemTray();
    void ponyPosWrite();
    void display();

    //黄金比例:D
    const int width = 600;
    const int height = 370;

    int imagesNumber;
    int eventID;
    int eventIDOld;
    int spareTime;
    int posX, posY;

private slots:
    void slotImagesChange();
    void slotEventHandle();
    void slotMenuQuit();
    void slotMenuSettings();
    void slotMenuInformaiton();
    //void slotSettings(QString str);

private:
    Ui::UIPony *ui;


    QSystemTrayIcon *systemTray = new QSystemTrayIcon(this);

    QTimer *timerImagesChange = new QTimer;
    QTimer *timerEventHandle = new QTimer;

    QMenu *menuPony = new QMenu;
    QAction *actionQuit = new QAction;
    QAction *actionSettings = new QAction;
    QAction *actionInformation = new QAction;

    QLabel *ponyLabel = new QLabel;

    QVBoxLayout *layout = new QVBoxLayout;

    QHBoxLayout *ponyLayout = new QHBoxLayout;

    QLabel *debugLabel = new QLabel;

    Config *config;
    Tools *tools;
    ImageProcessing *imageProcessing;

    Style *m_p_style = nullptr;
    Localisation *m_p_localisation = nullptr;

protected:
    QPoint relativePos;
//   void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
};

#endif // UIPONY_H
