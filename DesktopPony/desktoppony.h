/**
 * @file desktoppony.h
 * @brief 桌面小马主类
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

#ifndef DESKTOPPONY_H
#define DESKTOPPONY_H

#include "ui/uimainpanel.h"
#include "tools.h"
#include "data/config.h"
#include "data/localisation.h"
#include "data/style.h"
#include "data/text.h"
#include "plugin/pluginmanager.h"
#include "metadata.h"
#include <QObject>

/**
 * @brief 主控类
 * @details 管理程序所有资源
 */
class DesktopPony : public QObject
{
    Q_OBJECT
public:
    DesktopPony();
    ~DesktopPony();

    /**
     * @brief 启动
     */
    void start();

    /**
     * @brief 初始化数据
     */
    void initData();

    /**
     * @brief 清理数据
     */
    void clearData();

    /**
     * @brief 初始化ui
     */
    void initUi();

    /**
     * @brief 清理Ui
     */
    void clearUi();

    /**
     * @brief 初始化信号槽
     */
    void initConnect();

    /**
     * @brief 清理信号槽
     */
    void clearConnect();

    /**
     * @brief 重新载入数据
     */
    void slotReloadData();

private:
    // Ui部分
    UiMainPanel *m_p_ui_main_panel = nullptr;

    // 数据部分
    PluginManager *m_p_plugin_manager = nullptr;

    Config *m_p_config = nullptr;
    Localisation *m_p_localisation = nullptr;
    Style *m_p_style = nullptr;
    Text *m_p_text = nullptr;

    PTRFUNC_GET_ELEMENT_PAIR_LIST m_ptrfun_get_element_pair_list = nullptr;
};

#endif // DESKTOPPONY_H
