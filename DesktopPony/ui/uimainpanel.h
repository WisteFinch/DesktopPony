/**
 * @file ui/uimainpanel.h
 * @brief 界面-主面板
 * @author WisteFinch
 * @date 2021.7.27
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

#ifndef UIMAINPANEL_H
#define UIMAINPANEL_H

#include "ui/character/uicharacterpage.h"
#include "ui/plugin/uipluginpage.h"
#include "ui/config/uiconfigpage.h"
#include "ui/info/uiinfopage.h"
#include "data/config.h"
#include "data/style.h"
#include "data/text.h"
#include "plugin/pluginmanager.h"
#include <QWidget>
#include <QCloseEvent>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedLayout>

namespace Ui
{
class UiMainPanel;
}

/**
 * @brief 主界面
 * @details 程序主界面入口
 */
class UiMainPanel : public QWidget
{
    Q_OBJECT

public:
    explicit UiMainPanel(QWidget *parent = nullptr);
    ~UiMainPanel();

    /**
     * @brief 关闭事件
     * @details 重写关闭事件，不退出程序
     * @param 事件
     */
    void closeEvent(QCloseEvent *event);

    /**
     * @brief 初始化
     * @details 初始化界面内容
     * @param 配置指针
     * @param 样式指针
     * @param 文本指针
     */
    void init(Config *ptrconf, Style *ptrStyle, Text *ptrText, PluginManager *ptrPluginManager);

    /**
     * @brief 槽：切换标签
     */
    void slotChangeTab(int index);

signals:
    void sigReloadData();   ///< 信号：重载数据

private:
    Ui::UiMainPanel *ui;

    Config *m_p_conf = nullptr; ///< 配置
    Style *m_p_style = nullptr; ///< 样式
    Text *m_p_text = nullptr;   ///< 文本
    PluginManager *m_p_plugin = nullptr;///< 插件

    int m_page_index;   ///< 当前页序号

    // Widget部分
    QPushButton *ui_main_panel_tab_character = nullptr; ///< “角色”选项卡
    QPushButton *ui_main_panel_tab_plugin = nullptr;///< “插件”选项卡
    QPushButton *ui_main_panel_tab_config = nullptr;///< “配置”选项卡
    QPushButton *ui_main_panel_tab_info = nullptr;  ///< “信息”选项卡
    QLabel *ui_main_panel_logo = nullptr;   ///< 标志
    QLabel *ui_main_panel_version = nullptr;///< 版本

    // Layout部分
    QHBoxLayout *ui_main_panel_layout_main = nullptr;   ///< 主界面布局
    QVBoxLayout *ui_main_panel_layout_tab = nullptr;///< 选项卡布局
    QStackedLayout *ui_main_panel_layout_pages = nullptr;   ///< 页面布局

    // 页部分
    UiCharacterPage *ui_character_page = nullptr;   ///< 角色页
    UiPluginPage *ui_plugin_page = nullptr; ///< 插件页
    UiConfigPage *ui_config_page = nullptr; ///< 配置页
    UiInfoPage *ui_info_page = nullptr; ///< 信息页


    /**
     * @brief 初始化部件
     */
    void initWidget();

    /**
     * @brief 初始化对象名称
     */
    void initObjectName();

    /**
     * @brief 初始化属性
     */
    void initProperty();

    /**
     * @brief 初始化内容
     */
    void initContent();

    /**
     * @brief 初始化信号槽
     */
    void initConnect();

    /**
     * @brief 清理信号槽
     */
    void clearConnect();
};

#endif // UIMAINPANEL_H
