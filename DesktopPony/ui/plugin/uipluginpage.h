/**
 * @file ui/plugin/uipluginpage.h
 * @brief 界面-插件-插件页
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

#ifndef UIPLUGINPAGE_H
#define UIPLUGINPAGE_H

#include "ui/plugin/uipluginlistcard.h"
#include "ui/plugin/uipluginfilter.h"
#include "ui/tools/vline.h"
#include "ui/tools/hline.h"
#include "data/config.h"
#include "data/text.h"
#include "plugin/pluginmanager.h"
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QScrollArea>
#include <QPixmap>
#include <QAction>
#include <QVector>
#include <QMessageBox>
#include <QSet>

/**
 * @brief 插件页
 * @details 列出插件，提供插件编辑入口
 */
class UiPluginPage : public QWidget
{
    Q_OBJECT

public:
    explicit UiPluginPage(QWidget *parent = nullptr);
    ~UiPluginPage();

    /**
     * @brief 初始化
     * @details 初始化界面内容
     * @param 配置指针
     * @param 文本指针
     * @param 插件管理指针
     */
    void init(Config *ptrconf, Text *ptrText, PluginManager *ptrPluginManager);

    UiPluginFilter *ui_plugin_page_filter = nullptr;///< 筛选器

signals:
    void sigReloadData();   ///< 信号：重载数据

private:
    Config *m_p_conf = nullptr; ///< 配置
    Text *m_p_text = nullptr;   ///< 文本
    PluginManager *m_p_plugin = nullptr;///< 插件

    qint32 m_list_index = -1;   ///< 列表序号

    QVector<UiPluginListCard *> *m_p_cards = nullptr; ///< 卡片
    QVector<UiPluginListCard *> *m_p_sorted_cards = nullptr; ///< 排序过的卡片
    QVector<UiPluginListCard *> *m_p_filtered_cards = nullptr;  ///< 筛选过的卡片

    // 部件部分
    QLineEdit *ui_plugin_page_search = nullptr; ///< 搜索框
    QPushButton *ui_plugin_page_search_filters = nullptr;   ///< 过滤器
    QScrollArea *ui_plugin_page_scrollarea = nullptr;   ///< 滚动区域
    QWidget *ui_plugin_page_list = nullptr; ///< 列表界面
    QPushButton *ui_plugin_page_tools_reload = nullptr; ///< 重新读取
    QPushButton *ui_plugin_page_tools_addnew = nullptr; ///< 新建
    QPushButton *ui_plugin_page_tools_delete = nullptr; ///< 删除
    QPushButton *ui_plugin_page_tools_enable = nullptr; ///< 启用
    QPushButton *ui_plugin_page_tools_disable = nullptr;///< 禁用
    QPushButton *ui_plugin_page_tools_star = nullptr;   ///< 收藏

    // 布局部分
    QVBoxLayout *ui_plugin_page_layout_main = nullptr;  ///< 主界面布局
    QHBoxLayout *ui_plugin_page_layout_search = nullptr;///< 搜索布局
    QVBoxLayout *ui_plugin_page_layout_list = nullptr;  ///< 列表布局
    QHBoxLayout *ui_plugin_page_layout_tools = nullptr; ///< 工具布局

    QAction *ui_plugin_page_search_icon = nullptr; ///< 搜索图标

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

    /**
     * @brief 生成列表
     */
    void creatList();

    /**
     * @brief 清除列表
     */
    void clearList();

    /**
     * @brief 生成筛选过的列表
     */
    void creatFilteredList();

    /**
     * @brief 清除筛选过的列表
     */
    void clearFilteredList();

    /**
     * @brief 刷新除筛选过的列表
     */
    void refreshFilteredList();

    /**
     * @brief 刷新列表
     */
    void refreshList();

    /**
     * @brief 列表筛选
     * @param 卡片
     * @return 是否筛选
     */
    bool filter(UiPluginListCard *card);

    /**
     * @brief 列表排序
     */
    void sortList();

private slots:
    /**
     * @brief 槽：列表序号改变
     */
    void slotListIndexChange(qint32 index = -1);

    /**
     * @brief 槽：重载列表
     */
    void slotReolad();

    /**
     * @brief 槽：设置收藏
     */
    void slotSetFav();

    /**
     * @brief 槽：设置启用
     * @param 是否启用
     */
    void slotSetEnable(bool enable);
};

#endif // UIPLUGINPAGE_H
