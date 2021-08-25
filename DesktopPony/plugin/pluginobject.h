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
#include "element/pluginelementstyle.h"
#include "element/pluginelementconfig.h"
#include "element/pluginelementresources.h"
#include "element/pluginelementlibrary.h"
#include "tools.h"
#include <QFile>
#include <QDir>
#include <QString>
#include <QVector>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonArray>
#include <QSet>

/**
 * @brief 插件对象
 * @details 存放、管理单个插件数据
 */
class PluginObject
{
public:
    PluginObject();
    ~PluginObject();

    /**
     * @brief 读取头文件
     * @param 路径
     * @param 配置元素名称索引
     * @param 语种列表
     * @return 错误信息
     */
    PLUGIN_EXC_LIST *readHead(QString path, bool isSystem, ELEMENT_CONFIG_NAME_INDEX *configNameIndex, QSet<QString> *langList);

    /**
     * @brief 元数据检查
     */
    void metadataCheck();

    /**
     * @brief 读取元素
     * @param 元素头文件路径
     * @param 配置元素名称索引
     * @param 语种列表
     * @return 元素
     */
    PluginElement *readElement(QString path, ELEMENT_CONFIG_NAME_INDEX *configNameIndex, QSet<QString> *langList);

    /**
     * @brief 清理
     */
    void clear();

    PLUGIN_EXC_LIST *m_p_exc_list = nullptr;   ///< 异常列表
    PluginObjectMetadata *m_p_metadata = nullptr;   ///< 元数据
    PLUGIN_ELEMENT_LIST *m_p_element_list = nullptr;///< 元素列表
    QMap<QString, uint> *m_p_element_uuid_index = nullptr;  ///< 元素uuid索引
    PluginObject *m_p_edited_obj = nullptr; ///< 编辑过的对象
private:
    PluginElementTypeName m_plugin_type_name;
};

#endif // PLUGINOBJECT_H
