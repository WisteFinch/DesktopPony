/**
 * @file data/style.h
 * @brief 数据-样式
 * @author WisteFinch
 * @date 2021.7.22
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

#ifndef STYLE_H
#define STYLE_H

#include "plugin/pluginsharedefinition.h"
#include "plugin/element/pluginelementstyle.h"
#include <QString>
#include <QMap>

/**
 * @brief 样式类
 * @details 为程序提供外观数据
 */
class Style
{
public:
    Style();

    /**
     * @brief 初始化
     * @param "插件元素组合列表"函数指针
     */
    void init(PTRFUNC_GET_ELEMENT_PAIR_LIST ptrfunc);

    /**
     * @brief 获取全局文本
     * @param 键
     * @return 值
     */
    QString getQSS();

    /**
     * @brief 设置样式名称
     * @param n
     */
    void setStyleName(QString n);

    /**
     * @brief 刷新样式数据
     */
    void refreshStyle();

private:
    PTRFUNC_GET_ELEMENT_PAIR_LIST ptrfun_get_element_pair_list = nullptr;   ///< "插件元素组合列表"函数指针
    QString qss;///< Qt样式表
    QString m_s_style_name = "default"; ///< 当前样式名称
};

#endif // QSS_H
