/**
 * @file ui/info/uiinfopage.h
 * @brief 界面-信息-信息页
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

#ifndef UIINFOPAGE_H
#define UIINFOPAGE_H

#include "ui/tools/hline.h"
#include "data/config.h"
#include "data/text.h"
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QDesktopServices>
#include <QUrl>

/**
 * @brief 信息页
 * @details 列出程序信息
 */
class UiInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit UiInfoPage(QWidget *parent = nullptr);
    ~UiInfoPage();

    /**
     * @brief 初始化
     * @details 初始化界面内容
     * @param 配置指针
     * @param 文本指针
     */
    void init(Config *ptrconf, Text *ptrText);

private:
    Config *m_p_conf = nullptr; ///< 配置
    Text *m_p_text = nullptr;   ///< 文本

    // 部件部分
    QScrollArea *ui_scrollarea = nullptr;   ///< 滚动区域
    QWidget *ui_scrollarea_widget = nullptr;///< 滚动区域显示部件
    QWidget *ui_scrollarea_basis = nullptr;///< 滚动区域基础部件
    QLabel *ui_about_icon = nullptr;  ///< 关于-图标
    QLabel *ui_about = nullptr;   ///< 关于
    QLabel *ui_log_caption = nullptr; ///< 日志-标题
    QLabel *ui_log_desc = nullptr;///< 日志-描述
    QPushButton *ui_log_view = nullptr;   ///< 查看日志
    QLabel *ui_version_caption = nullptr; ///< 版本-标题
    QLabel *ui_version_desc = nullptr;///< 版本-描述
    QPushButton *ui_version_check_update = nullptr;   ///< 检查更新
    QLabel *ui_repo_caption = nullptr;///< 仓库
    QLabel *ui_repo_desc = nullptr;   ///< 仓库
    QPushButton *ui_github_link = nullptr;///< 打开GitHub
    QLabel *ui_doc_caption = nullptr; ///< GitHub
    QLabel *ui_doc_desc = nullptr;///< GitHub
    QPushButton *ui_doc_link = nullptr;   ///< 打开GitHub
    QLabel *ui_copyright_caption = nullptr;   ///< 版权-标题
    QLabel *ui_copyright = nullptr;   ///< 版权
    QLabel *ui_author_caption = nullptr;  ///< 作者-标题
    QLabel *ui_author = nullptr;  ///< 作者
    QLabel *ui_license_caption = nullptr; ///< 许可证-标题
    QLabel *ui_license = nullptr; ///< 许可证
    QPushButton *ui_license_link = nullptr;///< 许可证链接

    // 布局部分
    QVBoxLayout *ui_layout_main = nullptr;  ///< 主界面布局
    QVBoxLayout *ui_layout_scrollarea = nullptr;  ///< 滚动区域布局
    QVBoxLayout *ui_layout_scrollarea_basis = nullptr;  ///< 滚动区域基础布局
    QHBoxLayout *ui_layout_about = nullptr; ///< 关于布局
    QHBoxLayout *ui_layout_log = nullptr; ///< 日志布局
    QVBoxLayout *ui_layout_log_text = nullptr;///< 日志-文本布局
    QHBoxLayout *ui_layout_version = nullptr; ///< 版本布局
    QVBoxLayout *ui_layout_version_text = nullptr;///< 版本-文本布局
    QHBoxLayout *ui_layout_repo = nullptr;///< 仓库布局
    QVBoxLayout *ui_layout_repo_text = nullptr;   ///< 仓库-文本布局
    QHBoxLayout *ui_layout_doc = nullptr; ///< 文档布局
    QVBoxLayout *ui_layout_doc_text = nullptr;///< 文档-文本布局
    QHBoxLayout *ui_layout_copyright = nullptr;   ///< 版权部件
    QHBoxLayout *ui_layout_author = nullptr;  ///< 版权部件
    QHBoxLayout *ui_layout_license = nullptr; ///< 许可证部件

    QGridLayout *ui_layout_appinfo = nullptr;


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

#endif // UIINFOPAGE_H
