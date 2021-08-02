/**
 * @file ui/plugin/uipluginlistcard.h
 * @brief 界面-插件-插件列表卡片
 * @author WisteFinch
 * @date 2021.7.29
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

#ifndef UIPLUGINLISTCARD_H
#define UIPLUGINLISTCARD_H

#include "data/text.h"
#include "plugin/pluginobject.h"
#include <QWidget>
#include <QLabel>
#include <QVector>
#include <QLayout>
#include <QPixmap>
#include <QDir>
#include <QVector>
#include <QMouseEvent>

namespace Ui
{
class UiPluginListCard;
}

/**
 * @brief 插件列表卡片
 * @details 位于插件页的插件列表，显示插件概要
 */
class UiPluginListCard : public QWidget
{
    Q_OBJECT

public:
    explicit UiPluginListCard(QWidget *parent = nullptr);
    ~UiPluginListCard();

    qint32 m_index; ///< 序号

    /**
     * @brief 初始化
     * @details 初始化界面内容
     * @param 配置指针
     * @param 文本指针
     */
    void init(Text *ptrText, PluginObject *ptrObj, qint32 index);

    /**
     * @brief 设置选定
     * @param 是否选定
     */
    void setSelected(bool flag = true);

private:
    Ui::UiPluginListCard *ui;

    Text *m_p_text = nullptr;   ///< 文本
    PluginObject *m_p_obj = nullptr;///< 插件对象

    // 部件部分
    QLabel *ui_plugin_list_card_icon = nullptr; ///< 图标
    QLabel *ui_plugin_list_card_caption = nullptr;  ///< 名称
    QLabel *ui_plugin_list_card_author = nullptr;   ///< 作者
    QLabel *ui_plugin_list_card_version = nullptr;  ///< 版本
    QLabel *ui_plugin_list_card_desc = nullptr; ///< 介绍
    QVector<QLabel *> *ui_plugin_list_card_elements = nullptr;  ///< 元素列表
    // 部件部分
    QHBoxLayout *ui_plugin_list_card_layout_main = nullptr; ///< 主布局
    QVBoxLayout *ui_plugin_list_card_layout_content = nullptr;  ///< 内容布局
    QHBoxLayout *ui_plugin_list_card_layout_title = nullptr;///< 标题布局
    QHBoxLayout *ui_plugin_list_card_layout_elements = nullptr; ///< 元素布局

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
     * @brief 初始化元素图标
     */
    void initElementsIcon();

    /**
     * @brief 获取元素图标
     * @return 图标标签
     */
    QLabel *getElementIcon();

signals:
    void clicked(qint32);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *ev);
};

#endif // UIPLUGINLISTCARD_H
