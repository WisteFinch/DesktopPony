/**
 * @file plugin/element/pluginelement.h
 * @brief 插件-元素
 * @author WisteFinch
 * @date 2021.6.8
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

#ifndef PLUGINELEMENT_H
#define PLUGINELEMENT_H

#include "../pluginsharedefinition.h"
#include "tools.h"
#include <QJsonObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QFile>

class PluginElement
{
public:
    PluginElement();
    virtual ~PluginElement() {};

    /**
     * @brief 读取元素
     * @param 元素JSON对象
     */
    virtual void read(QJsonObject obj) = 0;
    /**
     * @brief 读取元素
     * @param 元素头文件路径
     */
    void read(QString path);

    /**
     * @brief 读取元数据
     * @param JSON对象
     * @return 插件元素元数据
     */
    void readMetadata(QJsonObject metadataObj);

    QString m_head_path; ///< 元素头文件路径
    PluginElementMetadata m_metadata;   ///< 插件元素元数据

};

#endif // PLUGINELEMENT_H
