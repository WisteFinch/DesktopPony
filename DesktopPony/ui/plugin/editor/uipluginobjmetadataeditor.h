/**
 * @file ui/plugin/uipluginobjmetadataeditor.h
 * @brief 界面-插件-插件对象元数据编辑器
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

#ifndef UIPLUGINOBJMETADATAEDITOR_H
#define UIPLUGINOBJMETADATAEDITOR_H

#include "plugin/pluginobject.h"
#include "tools.h"
#include "data/text.h"
#include "data/style.h"
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QLayout>
#include <QPushButton>

class UiPluginObjMetadataEditor : public QWidget
{
    Q_OBJECT
public:
    explicit UiPluginObjMetadataEditor(QWidget *parent = nullptr);
    ~UiPluginObjMetadataEditor();

    /**
     * @brief 初始化
     * @details 初始化界面内容
     * @param 文本指针
     * @param 样式指针
     * @param 插件对象元数据指针
     */
    void init(Text *ptrText, Style *ptrStyle, PluginObjectMetadata *ptrMetadata);

    /**
     * @brief 设置启用编辑
     * @param 开关
     */
    void setEidtEnabled(bool s = true);

    /**
     * @brief 初始化值
     */
    void initValue();

private:
    Text *m_p_text = nullptr;   ///< 文本
    Style *m_p_style = nullptr; ///< 样式
    PluginObjectMetadata *m_p_metadata = nullptr; ///< 元数据

    // 部件部分
    QLabel *ui_title = nullptr; ///< 标题
    QLabel *ui_id = nullptr;    ///< 标识符标签
    QLineEdit *ui_id_lineedit = nullptr;///< 标识符编辑
    QLabel *ui_uuid = nullptr;  ///< uuid标签
    QLineEdit *ui_uuid_lineedit = nullptr;  ///< uuid编辑
    QPushButton *ui_uuid_button = nullptr;  ///< uuid生成
    QLabel *ui_caption = nullptr;   ///< 名称标签
    QLineEdit *ui_caption_lineedit = nullptr;   ///< 名称编辑
    QLabel *ui_desc = nullptr;  ///< 描述标签
    QTextEdit *ui_desc_textedit = nullptr;  ///< 描述编辑
    QLabel *ui_author = nullptr;///< 作者标签
    QLineEdit *ui_author_lineedit = nullptr;///< 作者编辑
    QLabel *ui_version = nullptr;   ///< 版本标签
    QLineEdit *ui_version_lineedit = nullptr;   ///< 版本编辑
    QLabel *ui_url = nullptr;   ///< 更新地址标签
    QLineEdit *ui_url_lineedit = nullptr;   ///< 更新地址编辑

    // 布局部分
    QVBoxLayout *ui_layout_main = nullptr;  ///< 主界面布局
    QHBoxLayout *ui_layout_title = nullptr; ///< 标题布局
    QVBoxLayout *ui_layout_content = nullptr;   ///< 内容布局
    QHBoxLayout *ui_layout_icon = nullptr;  ///< 图标布局
    QHBoxLayout *ui_layout_id = nullptr;///< 标识符布局
    QHBoxLayout *ui_layout_uuid = nullptr;  ///< uuid布局
    QHBoxLayout *ui_layout_caption = nullptr;   ///< 名称布局
    QHBoxLayout *ui_layout_desc = nullptr;  ///< 描述布局
    QHBoxLayout *ui_layout_author = nullptr;///< 内容布局
    QHBoxLayout *ui_layout_version = nullptr;   ///< 版本布局
    QHBoxLayout *ui_layout_url = nullptr;   ///< 更新地址布局


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

#endif // UIPLUGINOBJMETADATAEDITOR_H
