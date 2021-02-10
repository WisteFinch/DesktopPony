/**
 * @file ui/uisettings.h
 * @brief 界面-设置
 * @author WisteFinch
 * @date 2020.12.13
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

#ifndef UISETTINGS_H
#define UISETTINGS_H

#include "file/localisation.h"
#include "file/config.h"
#include "file/filecharacter.h"
#include "file/fileevent.h"
#include "ui/uicharacter/uicharacter.h"
#include "ui/uitasks/uitasks.h"
#include <QWidget>
#include <QCloseEvent>
#include <QLayout>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QDesktopServices>
#include <QUrl>
#include <QTime>
#include <QLineEdit>
#include <QRegExpValidator>
#include <QRandomGenerator>
#include <QPainter>
#include <QProxyStyle>

namespace Ui {
class UISettings;
}

class UISettings : public QWidget
{
    Q_OBJECT

public:
    explicit UISettings(QWidget *parent = nullptr);
    ~UISettings();

    //黄金比例:D
    const int width = 600;
    const int height = 370;
    QImage background;

    /**
     * @brief 关闭事件
     * @details 重写事件，不退出程序
     * @param 事件
     */
    void closeEvent(QCloseEvent *event);

    /**
     * @brief 初始化
     * @param 本地化指针
     * @param 配置文件指针
     */
    void init(Localisation *tmpLocalisation, Config *tmpConfig, FileCharacter *tmpFileCharacter, FileTasks *tmpFileTasks);

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
     * @brief 刷新数值
     */
    void refreshValue();

    /**
     * @brief 保存数值
     */
    void saveValue();

    /**
     * @brief 水平线
     * @param object名
     * @param parent
     * @return 水平线
     */
    QFrame *horizontalLine(QString objectName = "horzontal_line", QWidget *parent=nullptr);

    /**
     * @brief 设置背景
     */
    void setBackGround();

public slots:

    /**
     * @brief 槽：打开“选择角色”
     */
    void slotShowUICharacter();

    /**
     * @brief 槽：角色选择-返回
     */
    void slotUICharacterBack(QString name);

    /**
     * @brief 槽：打开“任务”
     */
    void slotShowUITasks();

    /**
     * @brief 槽：“任务”返回
     */
    void slotUITasksBack(QString str);

    /**
     * @brief 槽：切换标签至“角色”
     */
    void slotChangeTabCharacter();
    /**
     * @brief 槽：切换标签至“常规”
     */
    void slotChangeTabGeneral();
    /**
     * @brief 槽：切换标签至“信息”
     */
    void slotChangeTabInfo();

    /**
     * @brief 槽：打开外链“介绍”
     */
    void slotOpenUrlDesc();
    /**
     * @brief 槽：打开外链“文档”
     */
    void slotOpenUrlDoc();


private:
    Ui::UISettings *ui;

    QHBoxLayout *m_layout_main;///<主界面布局
    QVBoxLayout *m_layout_tab;///<标签布局

    QStackedWidget *m_stackedwidget_main;///<选项卡

    //第一页：角色
    QPushButton *m_button_tab_character;///<“角色”标签按钮
    QScrollArea *m_scrollarea_character;///<“角色”滚动区域
    QVBoxLayout *m_layout_character;///<“角色”布局
    QWidget *m_widget_character;///<“角色”界面
    QHBoxLayout *m_layout_character_switch;///<“选择角色”布局
    QVBoxLayout *m_layout_character_switch_label;///<“选择角色-标签”布局
    QLabel *m_label_character_switch_title;///<“选择角色-标题”标签
    QLabel *m_label_character_switch_content;///<“选择角色-内容”标签
    QPushButton *m_button_character_switch;///<“选择角色”按钮
    QHBoxLayout *m_layout_tasks;///<“任务”布局
    QVBoxLayout *m_layout_tasks_label;///<“任务-标签”布局
    QLabel *m_label_tasks_title;///<“任务-标题”标签
    QLabel *m_label_tasks_content;///<“任务-内容”标签
    QPushButton *m_button_tasks;///<“任务”按钮

    //第二页：常规
    QPushButton *m_button_tab_general;///<“常规”标签按钮
    QScrollArea *m_scrollarea_general;///<“常规”滚动区域
    QVBoxLayout *m_layout_general;///<“常规”布局
    QWidget *m_widget_general;///<“常规”界面
    QHBoxLayout *m_layout_general_option_character_size;///<“选项-角色大小”布局
    QVBoxLayout *m_layout_general_option_character_size_label;///<“选项-角色大小-标签”布局
    QLabel *m_label_general_option_character_size_title;///<“选项-角色大小-标题”标签
    QLabel *m_label_general_option_character_size_content;///<“选项-角色大小-内容”标签
    QLineEdit *m_lineedit_general_option_character_size;///<“选项-角色大小”输入框
    QHBoxLayout *m_layout_general_constant_gravitational_acceleration;///<“常量-重力加速度”布局
    QVBoxLayout *m_layout_general_constant_gravitational_acceleration_label;///<“常量-重力加速度-标签”布局
    QLabel *m_label_general_constant_gravitational_acceleration_title;///<“常量-重力加速度-标题”标签
    QLabel *m_label_general_constant_gravitational_acceleration_content;///<“常量-重力加速度-内容”标签
    QLineEdit *m_lineedit_general_constant_gravitational_acceleration;///<“常量-重力加速度”输入框
    QHBoxLayout *m_layout_general_constant_obstruction;///<“常量-阻力”布局
    QVBoxLayout *m_layout_general_constant_obstruction_label;///<“常量-阻力-标签”布局
    QLabel *m_label_general_constant_obstruction_title;///<“常量-阻力”标签
    QLabel *m_label_general_constant_obstruction_content;///<“常量-阻力”标签
    QLineEdit *m_lineedit_general_constant_obstruction;///<“常量-阻力”输入框


    //第三页：信息
    QPushButton *m_button_tab_info;///<“信息”标签按钮
    QScrollArea *m_scrollarea_info;///<“信息”滚动区域
    QVBoxLayout *m_layout_info;///<“信息”布局
    QWidget *m_widget_info;///<“信息”界面
    QHBoxLayout *m_layout_info_desc;///<“介绍”布局
    QVBoxLayout *m_layout_info_desc_label;///<<“介绍-标签”布局
    QLabel *m_label_info_desc_title;///<“介绍-标题”标签
    QLabel *m_label_info_desc_content;///<“介绍-内容”标签
    QPushButton *m_button_info_desc;///<“介绍”按钮
    QHBoxLayout *m_layout_info_version;///<“版本”布局
    QVBoxLayout *m_layout_info_version_label;///<<“版本-标签”布局
    QLabel *m_label_info_version_title;///<“版本-标题”标签
    QLabel *m_label_info_version_content;///<“版本-内容”标签
    QPushButton *m_button_info_update;///< “版本：检查更新”按钮
    QHBoxLayout *m_layout_info_doc;///<“文档”布局
    QVBoxLayout *m_layout_info_doc_label;///<<“文档-标签”布局
    QLabel *m_label_info_doc_title;///<“文档-标题”标签
    QLabel *m_label_info_doc_content;///<“文档-内容”标签
    QPushButton *m_button_info_doc;///< “文档”按钮
    QHBoxLayout *m_layout_info_log;///<“日志”布局
    QVBoxLayout *m_layout_info_log_label;///<<“日志-标签”布局
    QLabel *m_label_info_log_title;///<“日志-标题”标签
    QLabel *m_label_info_log_content;///<“日志-内容”标签
    QPushButton *m_button_info_log;///< “日志”按钮
    QLabel *m_label_info_copyright;///<“版权”标签

    Localisation *m_p_localisation;
    Config *m_p_config;
    FileCharacter *m_p_file_character;
    FileTasks *m_p_file_tasks;
    UICharacter *m_p_uicharacter = nullptr;
    UITasks *m_p_uitasks = nullptr;

};

#endif // UISETTINGS_H
