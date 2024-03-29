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
#include <QSet>

/**
 * @brief 插件管理
 * @details 存放、管理所有插件数据
 */
class PluginManager
{
public:
    PluginManager();
    ~PluginManager();

    /**
     * @brief 刷新插件对象列表
     * @details 重新从文件中读取插件
     */
    void refreshList();

    /**
     * @brief 清理
     * @details 清理内存中原有的插件数据
     */
    void clear();

    /**
     * @brief 获取元素组合列表
     * @details 返回元素和对象uuid列表，供数据类使用
     * @param 元素类型
     * @return 元素组合列表
     */
    ELEMENT_PAIR_LIST *getElementPairList(PLUGIN_ELEMENT_TYPE type);

    /**
     * @brief 获取语种列表
     * @return 语种列表
     */
    QSet<QString> *getLangList();

    OBJECT_LIST *m_p_plugin_obj_list = nullptr;  ///< 插件对象列表
    OBJECT_UUID_INDEX *m_p_plugin_obj_uuid_index = nullptr;   ///< 对象uuid索引
    OBJECT_TYPE_INDEX *m_p_plugin_obj_type_index = nullptr;///< 对象包含的元素类型索引
    ELEMENT_TYPE_INDEX *m_p_plugin_element_type_index = nullptr;  ///< 元素类型索引
    ELEMENT_CONFIG_NAME_INDEX *m_p_plugin_element_config_name_index = nullptr;  ///< 配置元素名称索引
    QSet<QString> *m_p_lang_list = nullptr; ///< 语种列表

    PLUGIN_EXC_LIST *m_p_plugin_exc_list = nullptr; ///< 插件异常列表
};

#endif // PLUGINMANAGER_H
