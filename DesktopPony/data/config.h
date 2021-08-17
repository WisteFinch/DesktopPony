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
#include "plugin/element/pluginelementstyle.h"
#include "plugin/element/pluginelementlocalisation.h"
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
        QVariant v; ///< 值
        QString obj_uuid;   ///< 对象uuid
        QString element_uuid;   ///< 元素uuid
        QString group_uuid; ///< 组uuid
        PluginElementConfigData::Item *info = nullptr;  ///< 信息
    };  ///< 配置项

    typedef std::function<QVariant(QString)> PTRFUNC_GET_CONFIG;///< 类型定义：函数指针:获取配置

    /**
     * @brief 初始化
     * @param "插件元素组合列表"函数指针
     * @param 语种列表
     */
    void init(PTRFUNC_GET_ELEMENT_PAIR_LIST ptrfunc, QSet<QString> *langList);

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

    /**
     * @brief 获取配置项信息
     * @param 对象uuid
     * @param 元素uuid
     * @param 项uuid
     * @return 配置项信息
     */
    PluginElementConfigData::Item getItem(QString objUuid, QString elementUuid, QString itemUuid);

    /**
     * @brief 获取配置项列表
     * @return 配置项列表
     */
    QMap<QString, Item *> *getList();

    /**
     * @brief 获取配置项类别索引
     * @return 配置项类别索引
     */
    QMap<QString, QVector<QString>*> *getCategoryIndex();

private:
    QMap<QString, Item *> *m_p_config_data = nullptr;  ///< 配置数据
    QMap<QString, QVector<QString>*> *m_p_category_index = nullptr;  ///< 配置项类别索引 <类别, <配置项id>>
    QMap<QPair<QString, QString>, PluginElementConfig *> *m_p_element_index = nullptr;///< 配置元素索引 <<对象uuid, 元素uuid>, 配置元素>
    PTRFUNC_GET_ELEMENT_PAIR_LIST m_ptrfunc_get_element_pair_list = nullptr;   ///< "插件元素组合列表"函数指针
    QJsonObject *m_p_json_obj = nullptr;  ///< 配置文件json对象
    QSet<QString> *m_p_lang_list = nullptr; ///< 语种列表

    PluginElementConfigData::Item *m_p_sys_item_info_lang = nullptr; ///< 系统配置项信息：语种
    PluginElementConfigData::Item *m_p_sys_item_info_style = nullptr;///< 系统配置项信息：样式
    PluginElementConfigData::Item *m_p_sys_item_info_character = nullptr;///< 系统配置项信息：角色
    PluginElementConfigData::Item *m_p_sys_item_info_debug = nullptr;///< 系统配置项信息：调试

    /**
     * @brief 初始化系统配置项信息
     */
    void initSystemItem();
};

#endif // CONFIG_H
