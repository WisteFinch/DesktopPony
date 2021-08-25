/**
 * @file ui/plugin/uipluginobjeditor.h
 * @brief 界面-插件-插件对象编辑器
 * @author WisteFinch
 * @date 2021.8.22
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

#ifndef UIPLUGINOBJEDITOR_H
#define UIPLUGINOBJEDITOR_H

#include "ui/plugin/editor/uipluginobjmetadataeditor.h"
#include "plugin/pluginobject.h"
#include "data/text.h"
#include "data/style.h"
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QLayout>
#include <QScrollArea>
#include <QCloseEvent>
#include <QMessageBox>

namespace Ui
{
class UiPluginObjEditor;
}

class UiPluginObjEditor : public QWidget
{
    Q_OBJECT
public:
    explicit UiPluginObjEditor(QWidget *parent = nullptr);
    ~UiPluginObjEditor();
    bool m_on_delete = false;   ///< 正在删除
    bool m_edited = false;  ///< 已编辑

    enum SIG_BACK_TYPE {
        closed = 0x00000000,///< 关闭
        edited = 0x00000001,///< 已编辑
    };  ///< 返回信号类型

    /**
     * @brief 初始化
     * @details 初始化界面内容
     * @param 文本指针
     * @param 样式指针
     * @param 插件对象指针
     */
    void init(Text *ptrText, Style *ptrStyle, PluginObject *ptrObj);

    /**
     * @brief 保存改动
     */
    void save();

    /**
     * @brief 设置启用编辑
     * @param 开关
     */
    void setEidtEnabled(bool s = true);

signals:
    void sigBack(SIG_BACK_TYPE);///< 信号：返回

private:
    Ui::UiPluginObjEditor *ui;
    Text *m_p_text = nullptr;   ///< 文本
    Style *m_p_style = nullptr; ///< 样式
    PluginObject *m_p_obj = nullptr;   ///< 插件对象
    PluginObject *m_p_orig_obj = nullptr;   ///< 原插件对象
    PluginObject *m_p_edited_obj = nullptr; ///< 编辑过的插件对象

    // 部件部分
    QWidget *ui_tools_widget = nullptr; ///< 工具部件
    QLabel *ui_tools_status = nullptr;  ///< 工具状态
    QPushButton *ui_tools_edit = nullptr;   ///< 启用编辑
    QPushButton *ui_tools_save = nullptr;   ///< 保存
    QPushButton *ui_tools_discard = nullptr;///< 舍弃

    QScrollArea *ui_scrollarea = nullptr;   ///< 滚动区域
    QWidget *ui_widget = nullptr;   ///< 滚动区域部件
    UiPluginObjMetadataEditor *ui_metadata = nullptr;   ///< 元数据

    // 布局部分
    QVBoxLayout *ui_layout_main = nullptr;  ///< 主界面布局
    QHBoxLayout *ui_layout_tools = nullptr; ///< 工具布局
    QVBoxLayout *ui_layout_scrollarea = nullptr;///< 滚动区域布局
    QVBoxLayout *ui_layout_list = nullptr;  ///< 元素列表布局

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
     * @brief 初始化值
     */
    void initValue();

    /**
     * @brief 清理信号槽
     */
    void clearConnect();
private slots:
    /**
     * @brief 槽：舍弃修改
     */
    void slotDiscard();

protected:
    virtual void closeEvent(QCloseEvent *ev);
};

#endif // UIPLUGINOBJEDITOR_H
