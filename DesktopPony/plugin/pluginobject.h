/**
 * @file plugin/pluginobject.h
 * @brief 插件-对象
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

#ifndef PLUGINOBJECT_H
#define PLUGINOBJECT_H

#include "pluginsharedefinition.h"
#include "metadata.h"
#include "element/pluginelementlocalisation.h"
#include "element/pluginelementmodel.h"
#include "tools.h"
#include <QFile>
#include <QDir>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>

class PluginObject
{
public:
    PluginObject();
    ~PluginObject();

    /**
     * @brief 读取头文件 >>>>>>>>>>>进行中<<<<<<<<<<<<
     * @param 路径
     * @return 错误信息
     */
    QVector<PLUGIN_ERROR> readHead(QString path, bool isSystem);

    /**
     * @brief 元数据检查
     */
    void metadataCheck();

    /**
     * @brief 读取元素
     * @param 元素头文件路径
     * @return 元素
     */
    PluginElement *readElement(QString path);

    /**
     * @brief 清理元素列表 >>>>>>>>>>>未完成<<<<<<<<<<<<<<
     */
    void clearElementList();

    QVector<PLUGIN_ERROR> *m_p_error = nullptr;   ///< 错误信息
    PluginObjectMetadata *m_p_metadata = nullptr; ///< 元数据
    PLUGIN_ELEMENT_LIST *m_p_element_list = nullptr; ///< 元素列表
private:
    PluginElementTypeName m_plugin_type_name;
};

#endif // PLUGINOBJECT_H
