/**
 * @file plugin/sharedefinition.h
 * @brief 插件-公用定义
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

#ifndef PLUGINSHAREDEFINITION_H
#define PLUGINSHAREDEFINITION_H

#include <QString>
#include <QVector>
#include <QPair>
#include <QMap>
#include <QJsonObject>
#include <functional>
#include <QVariant>
#include <QSet>
#include <QHash>

class PluginManager;
class PluginObject;
class PluginElement;

/**
 * @brief 插件元素类型
 */
enum PLUGIN_ELEMENT_TYPE {
    element_type_null,
    element_type_localisation,
    element_type_style,
    element_type_event,
    element_type_action,
    element_type_accessories,
    element_type_model,
    element_type_config,
    element_type_resources,
    element_type_library
};

/**
 * @brief 插件元素名称转换
 */

struct PluginElementTypeName {
    QVector<QPair<PLUGIN_ELEMENT_TYPE, QString>> m_d;
    PluginElementTypeName()
    {
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(element_type_null, "null"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(element_type_localisation, "localisation"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(element_type_style, "style"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(element_type_event, "event"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(element_type_action, "action"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(element_type_accessories, "accessories"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(element_type_model, "model"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(element_type_config, "config"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(element_type_resources, "resources"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(element_type_resources, "library"));
    }
    PLUGIN_ELEMENT_TYPE getType(QString s)
    {
        s = s.toLower();
        for(int i = 0 ; i < this->m_d.size(); i++) {
            if(this->m_d.at(i).second == s) {
                return this->m_d.at(i).first;
            }
        }
        return element_type_null;
    }
    QString getName(PLUGIN_ELEMENT_TYPE t)
    {
        for(int i = 0 ; i < this->m_d.size(); i++) {
            if(this->m_d.at(i).first == t) {
                return this->m_d.at(i).second;
            }
        }
        return "null";
    }
};

/**
 * @brief 插件异常代号
 */
enum PLUGIN_EXCEPTION {
    PLUGIN_EXC_NULL,    ///< 无
    PLUGIN_EXC_ERR_001, ///< 插件对象头文件无法读取
    PLUGIN_EXC_ERR_002, ///< 插件对象头文件JSON无法读取
    PLUGIN_EXC_ERR_003, ///< 插件对象头文件缺少元数据
    PLUGIN_EXC_ERR_004, ///< 插件对象头文件元数据缺少标识符
    PLUGIN_EXC_ERR_005, ///< 插件元素头文件无法读取
    PLUGIN_EXC_ERR_006, ///< 插件元素头文件JSON无法读取
    PLUGIN_EXC_ERR_007, ///< 插件元素头文件缺少元数据
    PLUGIN_EXC_ERR_008, ///< 插件元素头文件元数据缺少标识符
    PLUGIN_EXC_ERR_009, ///< 插件元素类型不存在
    PLUGIN_EXC_ERR_100, ///< 本地化元素：缺少语种名称
    PLUGIN_EXC_ERR_200, ///< 样式元素：缺少样式名称
    PLUGIN_EXC_ERR_201, ///< 样式元素：qss文件无法读取
    PLUGIN_EXC_ERR_700, ///< 配置元素：组缺少标识符
    PLUGIN_EXC_ERR_701, ///< 配置元素：组少项类型
    PLUGIN_EXC_ERR_702, ///< 配置元素：组缺少配置名称
    PLUGIN_EXC_ERR_703, ///< 配置元素：组配置名称冲突
    PLUGIN_EXC_ERR_704, ///< 配置元素：选项列表缺少标识符

    PLUGIN_EXC_WARN_001,///< 插件对象头文件元数据缺少名称
    PLUGIN_EXC_WARN_002,///< 插件对象头文件元数据缺少介绍
    PLUGIN_EXC_WARN_003,///< 插件对象头文件元数据缺少作者
    PLUGIN_EXC_WARN_004,///< 插件对象头文件元数据缺少版本
    PLUGIN_EXC_WARN_005,///< 插件对象头文件元数据缺少uuid
    PLUGIN_EXC_WARN_006,///< 插件元素头文件元数据缺少uuid
    PLUGIN_EXC_WARN_007,///< uuid冲突
    PLUGIN_EXC_WARN_600,///< 模型元素：缺少图层
    PLUGIN_EXC_WARN_601,///< 模型元素：缺少图像类别
    PLUGIN_EXC_WARN_602,///< 模型元素：缺少部件
    PLUGIN_EXC_WARN_700,///< 配置元素：最小值大于最大值
};

/**
 * @brief 插件异常数据
 */
struct PluginExceptionData {
    PLUGIN_EXCEPTION e; ///< 异常代号
    QString object_uuid;///< 插件uuid
    QString element_uuid;   ///< 元素uuid
    QString group_uuid; ///< 组uuid
    QString item_uuid;  ///< 项uuid
};

typedef QPair<QVector<PluginExceptionData>, QVector<PluginExceptionData>> PLUGIN_EXC_LIST;   ///< 类型定义：插件异常列表<错误列表, 警告列表>

/**
 * @brief 插件对象元数据
 */
struct PluginObjectMetadata {
    QString id; ///< ID
    QString uuid;   ///< 唯一标识符
    QString orig_uuid;  ///< 原唯一标识符
    QString caption; ///< 名称
    QString description; ///< 介绍
    QString icon;   ///< 图标地址
    QString author; ///< 作者
    QString version; /// < 版本
    QString remote_url; ///< 网络地址
    QString file_path;  ///< 文件地址
    QString dir_path;   ///< 文件夹地址
    bool is_remote = false; ///< 是否来自网络
    bool is_system = false; ///< 是否为系统插件

    bool has_localisation = false;  ///< 拥有本地化元素
    bool has_style = false; ///< 拥有样式元素
    bool has_event = false; ///< 拥有事件元素
    bool has_action = false;///< 拥有动作元素
    bool has_accessories = false;   ///< 拥有配件元素
    bool has_model = false; ///< 拥有模型元素
    bool has_config = false;///< 拥有配置元素
    bool has_resources = false; ///< 拥有资源元素
    bool has_library = false;///< 拥有库元素
};

/**
 * @brief 插件元素元数据
 */
struct PluginElementMetadata {
    QString id; ///< 标识符
    QString obj_id;  ///< 父对象标识符
    QString uuid16; ///< 16位唯一标识符
    QString orig_uuid16;///< 原16位唯一标识符
    QString obj_uuid;   ///< 父对象uuid
    QString caption;///< 名称
    QString description;///< 介绍
    QString icon;   ///< 图标地址
    QString file_path;  ///< 文件地址
    QString dir_path;   ///< 文件夹地址
    PLUGIN_ELEMENT_TYPE type = element_type_null;   ///< 元素类型
};

/**
 * @brief 模型元素数据
 */
struct PluginElementModelData {
    typedef QStringList LAYERS;  ///< 类型定义：图层列表
    typedef QStringList IMG_CATEGORIES ;   ///< 类型定义：图像类别
    struct Unit {
        typedef QStringList LAYERS;  ///< 类型定义：图层
        QString name; ///< 部件名称
        LAYERS layers; ///< 包括的图层
    };  ///< 结构体：部件
    typedef QVector<Unit> Units;///< 类型定义：部件列表

    LAYERS layers;  ///< 图层列表
    IMG_CATEGORIES img_categories;   ///< 图像类别
    Units units;///< 部件列表
};

/**
 * @brief 样式元素数据
 */
struct PluginElementStyleData {
    QString style_name; ///< 样式名称
    QString qss_path;   ///< qss地址
    QByteArray qss;  ///< qss数据
    bool isErr = false; ///< 存在错误
};

/**
 * @brief uuid索引项
 */
struct UuidIndexItem {
    QString obj_uuid;
    QString element_uuid;
    QString group_uuid;
    QString item_uuid;
};

typedef QVector<PluginElement *> PLUGIN_ELEMENT_LIST; ///< 类型定义：插件元素列表
typedef QVector<PluginObject *> OBJECT_LIST; ///< 类型定义：对象列表
typedef QMap<QString, PluginObject *> OBJECT_UUID_INDEX;  ///< 类型定义：对象uuid索引 <对象uuid, 对象指针>
typedef QMap<PLUGIN_ELEMENT_TYPE, QVector<QString>*> OBJECT_TYPE_INDEX;  ///< 类型定义：对象包含的元素类型索引 <元素类型, 对象uuid列表>
typedef QVector<QPair<PluginElement *, QString>> ELEMENT_PAIR_LIST;  ///< 元素组合列表
typedef QMap<PLUGIN_ELEMENT_TYPE, ELEMENT_PAIR_LIST *> ELEMENT_TYPE_INDEX; ///< 类型定义：元素类型索引 <元素类型, 元素组合列表>
typedef QMap<QString, UuidIndexItem> ELEMENT_CONFIG_NAME_INDEX;   ///< 类型定义：配置元素名称索引

typedef std::function<ELEMENT_PAIR_LIST *(PLUGIN_ELEMENT_TYPE)> PTRFUNC_GET_ELEMENT_PAIR_LIST;   ///< 类型定义：函数指针:获取元素组合列表
typedef std::function<QList<QVariant>()> PTRFUNC_GET_DEISABLED_PLUGINS; ///< 类型定义：函数指针：获取禁用的插件

#endif // PLUGINSHAREDEFINITION_H
