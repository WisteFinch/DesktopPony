/**
 * @file ui/uitasks/uitasks.h
 * @brief 界面-任务
 * @author WisteFinch
 * @date 2021.1.29
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

#ifndef UITASKS_H
#define UITASKS_H

#include "file/localisation.h"
#include "imageprocessing/imageprocessing.h"
#include "file/config.h"
#include "file/fileevent.h"
#include "ui/uitasks/uitaskslistcard.h"
#include "ui/uitasks/uitasksstandardlistcard.h"
#include "ui/uitasks/uitasksinfo.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QPixmap>
#include <QFile>
#include <QCloseEvent>
#include <QListWidget>
#include <QFileDialog>

namespace Ui {
class UITasks;
}

class UITasks : public QWidget
{
    Q_OBJECT

public:
    explicit UITasks(QWidget *parent = nullptr);
    ~UITasks();

    int width;
    int height;
    QImage background;

    void closeEvent(QCloseEvent *event);

    /**
     * @brief 初始化
     */
    void init(Localisation *tmpLocalisation, Config *tmpConfig, FileTasks *tmpTasks, QImage bg, QPoint pos, QSize size);

    /**
     * @brief 初始化文本
     */
    void initText();

    /**
     * @brief 初始化Object名
     */
    void initObjectName();

    /**
     * @brief 初始化连接
     */
    void initConnect();

    /**
     * @brief 设置背景
     */
    void setBackGround();

    /**
     * @brief 读取标准列表
     */
    void doLoadStandardList();

    /**
     * @brief 清除标准列表
     */
    void doClearStandardList();

    /**
     * @brief 读取任务列表
     */
    void doLoadList();

    /**
     * @brief 清除任务列表
     */
    void doClearList();

public slots:
    /**
     * @brief 槽：返回
     */
    void slotExit();

    /**
     * @brief 槽：刷新
     */
    void slotRefreshList();

    /**
     * @brief 槽：列表选定
     * @param 名称
     */
    void slotListSelected(FileTasks::STaskdata *p);

    /**
     * @brief 槽：标准列表改变
     */
    void slotStandardListSelected(QString);

    /**
     * @brief 槽：显示详细信息
     */
    void slotShowInfo();

signals:

    void signalBack(QString str);

private:
    Ui::UITasks *ui;

    QHBoxLayout *m_layout_main;///< 主界面布局
    QVBoxLayout *m_layout_button;///< 按钮布局
    QScrollArea *m_scrollarea_tasks;///< 任务列表
    QWidget *m_widget_tasks;///< 任务列表-界面
    QVBoxLayout *m_layout_tasks;///< 任务列表-布局
    QScrollArea *m_scrollarea_standards;///< 标准列表
    QWidget *m_widget_standards;///< 标准列表-界面
    QVBoxLayout *m_layout_standards;///< 标准列表-布局
    QList<UITasksListCard*> *m_list_card;///< 任务列表-卡片
    QList<UITasksStandardListCard*> *m_list_standard_card;///< 标准列表-卡片

    QPushButton *m_button_info;///<  详细信息按钮
    QPushButton *m_button_switch;///<  开关按钮
    QPushButton *m_button_refresh;///< 刷新按钮
    QPushButton *m_button_exit;///< 退出按钮

    FileTasks::STaskdata *m_p_selected_task;
    QString m_str_selected_standard;

    Localisation *m_p_localisation;
    Config *m_p_config;
    FileTasks *m_p_tasks;
};

#endif // UITASKS_H
