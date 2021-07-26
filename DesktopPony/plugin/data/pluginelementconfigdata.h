#ifndef PLUGINELEMENTCONFIGDATA_H
#define PLUGINELEMENTCONFIGDATA_H

#include "../pluginsharedefinition.h"

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
            QString caption;///< 名称
            QString uuid16; ///< 项16位唯一标识符
            QString orig_uuid16;///< 原16位唯一标识符
        };
        QString id; ///< 标识符
        QString caption;///< 名称
        QString desc;   ///< 介绍
        QString uuid16;   ///< 组16位唯一标识符
        QString orig_uuid16;///< 原16位唯一标识符
        Config_TYPE type = config_type_null;///< 类型
        bool hidden = false;///< 设置中隐藏
        bool reset = false; ///< 启动时重置
        QVariant _default;  ///< 默认值
        QVariant range_from;///< 起始范围
        QVariant range_to;  ///< 终止范围
        QVector<SelectItem> select; ///< 下拉框
    };  ///< 组

    struct SelectItemIndex {
        QString item_uuid;  ///< 组uuid
        uint s; ///< 下标
    };

    QVector<Item *> *item_list = new QVector<Item *>; ///< 组列表
    QMap<QString, uint> *item_uuid_index = new QMap<QString, uint>; ///< 组uuid索引
    QMap<QString, SelectItemIndex> *select_item_uuid_index = new QMap<QString, SelectItemIndex>; ///< 下拉项uuid索引

    QString getTypeName(Config_TYPE t);
    Config_TYPE getType(QString s);
};

#endif // PLUGINELEMENTCONFIGDATA_H
