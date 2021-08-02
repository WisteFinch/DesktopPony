/**
 * @file plugin/element/pluginelementconfig.h
 * @brief 插件-元素-配置
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

#ifndef PLUGINELEMENTCONFIG_H
#define PLUGINELEMENTCONFIG_H

#include "pluginelement.h"
#include "../data/pluginelementconfigdata.h"

/**
 * @brief 配置元素
 * @details 存放、管理单个配置数据
 */
class PluginElementConfig : public PluginElement
{
public:
    PluginElementConfig(ELEMENT_CONFIG_NAME_INDEX *configNameIndex);
    ~PluginElementConfig();

    /**
     * @brief 读取元素
     * @param 元素JSON对象
     * @param 元素头文件路径
     * @param 元素头文件文件夹路径
     * @param 是否进行清理
     * @return 异常列表
     */
    PLUGIN_EXC_LIST *read(QJsonObject obj, QString filePath, QString dirPath, bool flag = true);

    PluginElementConfigData *m_p_data = nullptr;  ///< 配置数据

    ELEMENT_CONFIG_NAME_INDEX *m_p_config_name_index = nullptr;
};

#endif // PLUGINELEMENTCONFIG_H
