/**
 * @file plugin/data/pluginelementconfigdata.h
 * @brief 插件-数据-配置数据
 * @author WisteFinch
 * @date 2021.7.18
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

#ifndef PLUGINELEMENTCONFIGDATA_H
#define PLUGINELEMENTCONFIGDATA_H

#include "../pluginsharedefinition.h"

/**
 * @brief 配置元素数据
 * @details 存放配置元素数据
 */
class PluginElementConfigData
{
public:
    PluginElementConfigData();
    ~PluginElementConfigData();

    enum Config_TYPE {
        config_type_null,   ///< 无
        config_type_integer,///< 整型
        config_type_real,   ///< 实数型
        config_type_bool,   ///< 布尔型
        config_type_string, ///< 字符串型
        config_type_select  ///< 下拉框型
    };  ///< 配置类型

    QVector<QPair<Config_TYPE, QString>> type_vector = {
        {config_type_null, "null"},
        {config_type_integer, "integar"},
        {config_type_real, "real"},
        {config_type_bool, "bool"},
        {config_type_string, "string"},
        {config_type_select, "select"}
    };  ///< 配置类型名称

    struct Item {
        struct SelectItem {
            QString id; ///< 标识符
            QString name;   ///< 名称
            QString uuid16; ///< 项16位唯一标识符
            QString orig_uuid16;///< 原项16位唯一标识符
            QString obj_uuid;   ///< 对象uuid
        };
        QString id; ///< 标识符
        QString config_name;///< 配置名称
        QString caption;///< 名称
        QString desc;   ///< 介绍
        QString uuid16;   ///< 组16位唯一标识符
        QString orig_uuid16;///< 原16位唯一标识符
        Config_TYPE type = config_type_null;///< 类型
        bool hidden = false;///< 设置中隐藏
        bool reset = false; ///< 启动时重置
        bool read_only = false; ///< 只读
        QVariant _default;  ///< 默认值
        QVariant range_from;///< 起始范围
        QVariant range_to;  ///< 终止范围
        QVector<SelectItem> select; ///< 下拉框
        bool isErr = false; ///< 存在错误
        QString category = "general";///< 类别
        bool restart = false;   ///< 请求重启程序
    };  ///< 组

    struct SelectItemIndex {
        QString item_uuid;  ///< 组uuid
        uint s; ///< 下标
    };

    QVector<Item> *item_list = new QVector<Item>; ///< 组列表
    QMap<QString, uint> *item_uuid_index = new QMap<QString, uint>; ///< 组uuid索引
    QMap<QString, SelectItemIndex> *select_item_uuid_index = new QMap<QString, SelectItemIndex>; ///< 下拉项uuid索引

    QString getTypeName(Config_TYPE t);
    Config_TYPE getType(QString s);
};

#endif // PLUGINELEMENTCONFIGDATA_H
