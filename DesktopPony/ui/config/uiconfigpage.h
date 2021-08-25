/**
 * @file ui/config/uiconfigpage.h
 * @brief 界面-配置-配置页
 * @author WisteFinch
 * @date 2021.8.8
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

#ifndef UICONFIGPAGE_H
#define UICONFIGPAGE_H

#include "ui/config/uiconfigitemcard.h"
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

/**
 * @brief 配置页
 * @details 列出程序配置项
 */
class UiConfigPage : public QWidget
{
    Q_OBJECT

public:
    explicit UiConfigPage(QWidget *parent = nullptr);
    ~UiConfigPage();

    /**
     * @brief 初始化
     * @details 初始化界面内容
     * @param 配置指针
     * @param 文本指针
     * @param 插件管理指针
     * @param 函数指针：获取配置
     */
    void init(Config *ptrconf, Text *ptrText, PluginManager *ptrPluginManager, Config::PTRFUNC_GET_CONFIG ptrfuncGetConf);

    bool m_value_changed = false;
signals:
    void sigShowRestart();  ///< 信号：显示重启按钮

private:
    Config *m_p_conf = nullptr; ///< 配置
    Text *m_p_text = nullptr;   ///< 文本
    PluginManager *m_p_plugin = nullptr;///< 插件
    Config::PTRFUNC_GET_CONFIG m_ptrfunc_get_conf;///< 函数指针：获取配置获取配置
    QVector<UiConfigItemCard *> *m_p_items = nullptr;   ///< 项列表

    // 部件部分
    QScrollArea *ui_scrollarea = nullptr;   ///< 滚动区域
    QWidget *ui_scrollarea_widget = nullptr;///< 滚动区域部件

    // 布局部分
    QVBoxLayout *ui_layout_main = nullptr;  ///< 主界面布局
    QVBoxLayout *ui_layout_list = nullptr;///< 列表布局


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
     * @brief 加入项
     */
    void addItem(QMap<QString, QVector<QString>*>::iterator indexIter, QMap<QString, Config::Item *> *list);

    /**
     * @brief 清除列表
     */
    void clearList();

    /**
     * @brief 刷新列表
     */
    void refreshList();
};

#endif // UICONFIGPAGE_H
