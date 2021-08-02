/**
 * @file plugin/data/pluginelementlocalisationdata.h
 * @brief 插件-数据-本地化数据
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

#ifndef PLUGINELEMENTLOCALISATIONDATA_H
#define PLUGINELEMENTLOCALISATIONDATA_H

#include "../pluginsharedefinition.h"

/**
 * @brief 本地化元素数据
 * @details 存放本地化元素数据
 */
class PluginElementLocalisationData
{
public:
    PluginElementLocalisationData();
    ~PluginElementLocalisationData();

    struct Item {
        QString key;///< 键
        QString value;  ///< 值
        QString uuid16; ///< 组16位唯一标识符
        bool isErr = false; ///< 存在错误
    };  ///< 项
    typedef QVector<Item> TABLE;   ///< 类型定义：本地化对照表
    struct Lang {
        QString lang;   ///< 语种
        TABLE *table = new TABLE;   ///< 对照表
        QString uuid16; ///< 组16位唯一标识符
        QString orig_uuid16;///< 原16位唯一标识符
        bool isErr = false; ///< 存在错误
        ~Lang()
        {
            delete this->table;
        }
    };  ///< 语种
    struct ItemIndex {
        bool t; ///< 公有'true'/私有'false'
        QString lang_uuid;  ///< 语种uuid
        uint s; ///< 下标
    };
    struct LangIndex {
        bool t; ///< 公有'true'/私有'false'
        uint s; ///< 下标
    };

    QMap<QString, LangIndex> *lang_uuid_index = new QMap<QString, LangIndex>;  ///< 语种uuid索引
    QMap<QPair<QString, bool>, LangIndex> *lang_name_index = new QMap<QPair<QString, bool>, LangIndex>;  ///< 语种名称索引
    QMap<QString, ItemIndex> *item_uuid_index = new QMap<QString, ItemIndex>;  ///< 项索uuid引
    QVector<Lang *> *_public = nullptr; ///< 公有
    QVector<Lang *> *_private = nullptr; ///< 私有
};

#endif // PLUGINELEMENTLOCALISATIONDATA_H
