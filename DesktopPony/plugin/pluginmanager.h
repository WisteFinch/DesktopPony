/**
 * @file plugin/pluginmanager.h
 * @brief 插件-管理
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

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "metadata.h"
#include "pluginsharedefinition.h"
#include "pluginobject.h"
#include <QVector>
#include <QFile>
#include <QDir>
#include <QJsonObject>

class PluginManager
{
public:
    PluginManager();
    ~PluginManager();

    /**
     * @brief 刷新插件对象列表
     */
    void refreshList();

    /**
     * @brief 清理对象列表
     */
    void clearObjectList();

    /**
     * @brief 获取元素组合列表
     * @param 元素类型
     * @return 元素组合列表
     */
    ELEMENT_PAIR_LIST *getElementPairList(PLUGIN_ELEMENT_TYPE type);

    OBJECT_LIST *m_p_plugin_obj_list = nullptr;  ///< 插件对象列表
    OBJECT_UUID_INDEX *m_p_plugin_obj_uuid_index = nullptr;   ///< 对象uuid索引
    OBJECT_TYPE_INDEX *m_p_plugin_obj_type_index = nullptr;///< 对象包含的元素类型索引
    ELEMENT_TYPE_INDEX *m_p_plugin_element_type_index = nullptr;  ///< 元素类型索引

    PLUGIN_EXC_LIST *m_p_plugin_exc_list = nullptr; ///< 插件异常列表
};

#endif // PLUGINMANAGER_H
