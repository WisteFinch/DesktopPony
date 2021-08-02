/**
 * @file data/config.h
 * @brief 数据-配置
 * @author WisteFinch
 * @date 2021.7.26
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

#ifndef CONFIG_H
#define CONFIG_H

#include "plugin/element/pluginelementconfig.h"
#include "metadata.h"

/**
 * @brief 配置类
 * @details 读写配置
 */
class Config
{
public:
    Config();
    ~Config();

    struct Item {
        QString config_name;///< 配置名称
        QVariant v; ///< 值
        PluginElementConfigData::Config_TYPE type;  ///< 类型
        QString obj_uuid;   ///< 对象uuid
        QString element_uuid;   ///< 元素uuid
        QString group_uuid; ///< 组uuid
    };  ///< 配置项

    /**
     * @brief 初始化
     * @param "插件元素组合列表"函数指针
     */
    void init(PTRFUNC_GET_ELEMENT_PAIR_LIST ptrfunc);

    /**
     * @brief 清理
     */
    void clear();

    /**
     * @brief 创建配置元素索引
     */
    void creatIndex();

    /**
     * @brief 读取配置文件
     */
    void load();

    /**
     * @brief 保存配置文件
     */
    void save();

    /**
     * @brief 生成配置文件Json对象
     */
    void makeJsonObj();

    /**
     * @brief 获取
     * @param 键
     * @return 值
     */
    QVariant get(QString key);

    /**
     * @brief 设置
     * @param 键
     * @param 值
     */
    void set(QString key, QVariant value);

private:
    QMap<QString, Item *> *m_p_config_data = nullptr;  ///< 配置数据
    QMap<QPair<QString, QString>, PluginElementConfig *> *m_p_element_index = nullptr;///< 配置元素索引 <<对象uuid, 元素uuid>, 配置元素>
    PTRFUNC_GET_ELEMENT_PAIR_LIST ptrfun_get_element_pair_list = nullptr;   ///< "插件元素组合列表"函数指针
    QJsonObject *m_p_json_obj = nullptr;  ///< 配置文件json对象
};

#endif // CONFIG_H
