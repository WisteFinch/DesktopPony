/**
 * @file plugin/sharedefinition.h
 * @brief 鎻掍欢-鍏敤瀹氫箟
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

class PluginManager;
class PluginObject;
class PluginElement;

/**
 * @brief 插件元素类型
 */
enum PLUGIN_ELEMENT_TYPE {
    type_null,
    type_localisation,
    type_css,
    type_event,
    type_action,
    type_accessories,
    type_model
};

/**
 * @brief 插件元素名称转换
 */

struct PluginElementTypeName {
    QVector<QPair<PLUGIN_ELEMENT_TYPE, QString>> m_d;
    PluginElementTypeName()
    {
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(type_null, "null"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(type_localisation, "localisation"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(type_css, "css"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(type_event, "event"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(type_action, "action"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(type_accessories, "accessories"));
        this->m_d.append(QPair<PLUGIN_ELEMENT_TYPE, QString>(type_model, "model"));
    }
    PLUGIN_ELEMENT_TYPE getType(QString s)
    {
        s = s.toLower();
        for(int i = 0 ; i < this->m_d.size(); i++) {
            if(this->m_d.at(i).second == s) {
                return this->m_d.at(i).first;
            }
        }
        return type_null;
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
 * @brief 插件错误
 */
enum PLUGIN_ERROR {
    ERROR_001,  ///< 插件头文件无法读取
    ERROR_002,  ///< 插件头文件JSON无法读取
    ERROR_003,  ///< 插件头文件缺少元数据
    ERROR_004,  ///< 插件头文件缺少标识符

    WARN_001,   ///< 插件头文件元数据缺少名称
    WARN_002,   ///< 插件头文件元数据缺少介绍
    WARN_003,   ///< 插件头文件元数据缺少作者
    WARN_004,   ///< 插件头文件元数据缺少版本
};

/**
 * @brief 插件对象元数据
 */
struct PluginObjectMetadata {
    QString id; ///< ID
    QString uuid;   ///< 唯一标识符
    QString caption; ///< 名称
    QString description; ///< 介绍
    QString author; ///< 作者
    QString version; /// < 版本
    QString remote_url; ///< 网络地址
    bool is_remote = false; ///< 是否来自网络
    bool is_system = false; ///< 是否为系统插件
};

/**
 * @brief 插件元素元数据
 */
struct PluginElementMetadata {
    QString id; ///< 标识符
    QString uuid16;   ///< 16位唯一标识符
    QString caption;///< 名称
    QString description;///< 介绍
    PLUGIN_ELEMENT_TYPE type = type_null;   ///< 元素类型
};

/**
 * @brief 插件元素-本地化数据
 */
struct PluginElementLocalisationData {
    typedef QMap<QString, QPair<QString, int>> TABLE;   ///< 类型定义：本地化对照表
    typedef QMap<QString, TABLE> LANG;  ///< 类型定义：语种
    LANG global; ///< 全局
    LANG local;  ///< 局部
};

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

typedef QVector<PluginElement *> PLUGIN_ELEMENT_LIST; ///< 类型定义：插件元素列表
typedef QVector<PluginObject *> OBJECT_LIST; ///< 类型定义：对象列表
typedef QMap<QString, PluginObject *> OBJECT_UUID_INDEX;  ///< 类型定义：对象uuid索引 <对象uuid, 对象指针>
typedef QMap<PLUGIN_ELEMENT_TYPE, QVector<QString>*> OBJECT_TYPE_INDEX;  ///< 类型定义：对象包含的元素类型索引 <元素类型, 对象uuid列表>
typedef QVector<QPair<PluginElement *, QString>> ELEMENT_PAIR_LIST;  ///< 元素组合列表
typedef QMap<PLUGIN_ELEMENT_TYPE, ELEMENT_PAIR_LIST *> ELEMENT_TYPE_INDEX; ///< 类型定义：元素类型索引 <元素类型, 元素组合列表>

typedef std::function<ELEMENT_PAIR_LIST*(PLUGIN_ELEMENT_TYPE)> PTRFUNC_GET_ELEMENT_PAIR_LIST;///< 类型定义：函数指针:获取元素组合列表

#endif // PLUGINSHAREDEFINITION_H
