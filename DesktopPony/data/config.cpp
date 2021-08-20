#include "config.h"

Config::Config()
{
}

Config::~Config()
{
    clear();
    this->m_p_lang_list = nullptr;
}

void Config::init(PTRFUNC_GET_ELEMENT_PAIR_LIST ptrfunc, QSet<QString> *langList)
{
    this->m_ptrfunc_get_element_pair_list = ptrfunc;
    this->m_p_lang_list = langList;
    langList = nullptr;
}

void Config::clear()
{
    if(this->m_p_config_data != nullptr) {
        qDeleteAll(this->m_p_config_data->begin(), this->m_p_config_data->end());
        this->m_p_config_data->clear();
        delete this->m_p_config_data;
    }
    if(this->m_p_element_index != nullptr) {
        QMap<QPair<QString, QString>, PluginElementConfig *>::iterator iter = this->m_p_element_index->begin();
        while(iter != this->m_p_element_index->end()) {
            iter.value() = nullptr;
            iter++;
        }
        delete this->m_p_element_index;
        this->m_p_element_index = nullptr;
    }
    if(this->m_p_category_index != nullptr) {
        qDeleteAll(this->m_p_category_index->begin(), this->m_p_category_index->end());
        delete this->m_p_category_index;
        this->m_p_category_index = nullptr;
    }
    if(this->m_p_json_obj != nullptr) {
        delete this->m_p_json_obj;
        this->m_p_json_obj = nullptr;
    }

    if(this->m_p_sys_item_info_lang != nullptr) {
        delete this->m_p_sys_item_info_lang;
        this->m_p_sys_item_info_lang = nullptr;
    }
    if(this->m_p_sys_item_info_style != nullptr) {
        delete this->m_p_sys_item_info_style;
        this->m_p_sys_item_info_style = nullptr;
    }
    if(this->m_p_sys_item_info_character != nullptr) {
        delete this->m_p_sys_item_info_character;
        this->m_p_sys_item_info_character = nullptr;
    }
    if(this->m_p_sys_item_info_debug != nullptr) {
        delete this->m_p_sys_item_info_debug;
        this->m_p_sys_item_info_debug = nullptr;
    }
    if(this->m_p_sys_item_info_fav_plugin != nullptr) {
        delete this->m_p_sys_item_info_fav_plugin;
        this->m_p_sys_item_info_fav_plugin = nullptr;
    }
    if(this->m_p_sys_item_info_disabled_plugin != nullptr) {
        delete this->m_p_sys_item_info_disabled_plugin;
        this->m_p_sys_item_info_disabled_plugin = nullptr;
    }
}

void Config::creatIndex()
{
    // 清理
    clear();
    initSystemItem();
    this->m_p_config_data = new QMap<QString, Item *>;
    this->m_p_element_index = new QMap<QPair<QString, QString>, PluginElementConfig *>;
    this->m_p_category_index = new QMap<QString, QVector<QString>*>;

    // 系统配置
    this->m_p_category_index->insert(QStringLiteral("system"), new QVector<QString>);
    // 语言
    Item *langItem = new Item;
    langItem->info = this->m_p_sys_item_info_lang;
    langItem->v = this->m_p_sys_item_info_lang->_default;
    this->m_p_config_data->insert(this->m_p_sys_item_info_lang->config_name, langItem);
    this->m_p_category_index->value(QStringLiteral("system"))->append(langItem->info->config_name);
    // 样式
    Item *styleItem = new Item;
    styleItem->info = this->m_p_sys_item_info_style;
    styleItem->v = this->m_p_sys_item_info_style->_default;
    this->m_p_config_data->insert(this->m_p_sys_item_info_style->config_name, styleItem);
    this->m_p_category_index->value(QStringLiteral("system"))->append(styleItem->info->config_name);
    // 调试
    Item *debugItem = new Item;
    debugItem->info = this->m_p_sys_item_info_debug;
    debugItem->v = this->m_p_sys_item_info_debug->_default;
    this->m_p_config_data->insert(this->m_p_sys_item_info_debug->config_name, debugItem);
    this->m_p_category_index->value(QStringLiteral("system"))->append(debugItem->info->config_name);
    // 收藏的插件
    Item *favPluginItem = new Item;
    favPluginItem->info = this->m_p_sys_item_info_fav_plugin;
    favPluginItem->v = this->m_p_sys_item_info_fav_plugin->_default;
    this->m_p_config_data->insert(this->m_p_sys_item_info_fav_plugin->config_name, favPluginItem);
    this->m_p_category_index->value(QStringLiteral("system"))->append(favPluginItem->info->config_name);
    // 禁用的插件
    Item *disabledPluginItem = new Item;
    disabledPluginItem->info = this->m_p_sys_item_info_disabled_plugin;
    disabledPluginItem->v = this->m_p_sys_item_info_disabled_plugin->_default;
    this->m_p_config_data->insert(this->m_p_sys_item_info_disabled_plugin->config_name, disabledPluginItem);
    this->m_p_category_index->value(QStringLiteral("system"))->append(disabledPluginItem->info->config_name);



    // 插件配置
    ELEMENT_PAIR_LIST *rootList = this->m_ptrfunc_get_element_pair_list(PLUGIN_ELEMENT_TYPE::element_type_config);
    ELEMENT_PAIR_LIST::Iterator rootIter = rootList->begin();
    while(rootIter != rootList->end()) {
        PluginElementConfig *element = static_cast<PluginElementConfig *>(rootIter->first);
        QVector<PluginElementConfigData::Item> *list = element->m_p_data->item_list;

        // 加入配置元素索引
        this->m_p_element_index->insert(QPair<QString, QString>(rootIter->first->m_p_metadata->obj_uuid, rootIter->first->m_p_metadata->uuid16), element);

        // 读取配置项
        QVector<PluginElementConfigData::Item>::iterator listIter = list->begin();
        while(listIter != list->end()) {
            if(!listIter->isErr) {
                Item *item = new Item;
                item->info = listIter;
                item->v = listIter->_default;
                item->obj_uuid = rootIter->first->m_p_metadata->obj_uuid;
                item->element_uuid = rootIter->first->m_p_metadata->uuid16;
                item->group_uuid = listIter->uuid16;
                this->m_p_config_data->insert(item->info->config_name, item);
                if(!this->m_p_category_index->contains(item->info->category)) {
                    this->m_p_category_index->insert(item->info->category, new QVector<QString>);
                }
                this->m_p_category_index->value(item->info->category)->append(item->info->config_name);
            }
            listIter++;
        }
        rootIter++;
    }
}

void Config::load()
{
    if(this->m_p_config_data == nullptr) {
        creatIndex();
    }

    // 读取配置文件
    QFile head(STR_CONFIG_PATH);
    if(head.open(QIODevice::ReadOnly)) {// 文件是否打开:是
        QByteArray allData = head.readAll();
        head.close();

        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
        if(json_error.error != QJsonParseError::NoError) {
            // 异常：无法读取配置文件
            makeJsonObj();
        }

        QJsonObject rootObj = jsonDoc.object();
        if(this->m_p_json_obj != nullptr) {
            delete this->m_p_json_obj;
        }
        this->m_p_json_obj = new QJsonObject(rootObj);

        QStringList keyList = rootObj.keys();
        for(int i = 0; i < keyList.count(); i++) {
            if(!this->m_p_config_data->contains(keyList.at(i))) {
                continue;
            }
            Item *item = this->m_p_config_data->value(keyList.at(i));
            if(item->info->type == PluginElementConfigData::config_type_list) {
                QJsonArray array = rootObj.value(keyList.at(i)).toArray();
                QSet<QString> set;
                for(int k = 0; k < array.count(); k++) {
                    set.insert(array.at(k).toString());
                }
                QList<QVariant> list;
                QSet<QString>::iterator iter = set.begin();
                while(iter != set.end()) {
                    list.append(*iter);
                    iter++;
                }
                item->v.setValue(list);
            } else {
                QVariant v = rootObj.value(keyList.at(i)).toVariant();
                if(item->info->type == PluginElementConfigData::config_type_integer) {
                    item->v.setValue(v.toInt());
                } else if(item->info->type == PluginElementConfigData::config_type_real) {
                    item->v.setValue(v.toDouble());
                } else if(item->info->type == PluginElementConfigData::config_type_bool) {
                    item->v.setValue(v.toBool());
                } else if(item->info->type == PluginElementConfigData::config_type_string) {
                    item->v.setValue(v.toString());
                } else if(item->info->type == PluginElementConfigData::config_type_select) {
                    item->v.setValue(v.toString());
                }
            }
        }
    } else {
        // 异常：无法读取配置文件
        makeJsonObj();
    }
}

void Config::save()
{
    if(this->m_p_json_obj == nullptr) {
        makeJsonObj();
    }

    QMap<QString, Item *>::iterator iter = this->m_p_config_data->begin();
    while(iter != this->m_p_config_data->end()) {
        this->m_p_json_obj->remove(iter.value()->info->config_name);
        this->m_p_json_obj->insert(iter.value()->info->config_name, QJsonValue::fromVariant(iter.value()->v));
        iter++;
    }

    QFile head(STR_CONFIG_PATH);
    if(head.exists()) {
        if(!head.remove()) {
            // 异常：移除失败
        }
    }
    if(head.open(QIODevice::WriteOnly)) {
        head.write(QJsonDocument(*this->m_p_json_obj).toJson());
        head.close();
    } else {
        // 异常：保存失败
    }
}

void Config::makeJsonObj()
{
    if(this->m_p_config_data == nullptr) {
        // 生成索引
        creatIndex();
    }

    if(this->m_p_json_obj != nullptr) {
        delete this->m_p_json_obj;
    }
    this->m_p_json_obj = new QJsonObject();

    QMap<QString, Item *>::iterator iter = this->m_p_config_data->begin();
    while(iter != this->m_p_config_data->end()) {
        this->m_p_json_obj->insert(iter.value()->info->config_name, QJsonValue::fromVariant(iter.value()->v));
        iter++;
    }
}

QVariant Config::get(QString key)
{
    if(this->m_p_config_data->contains(key)) {
        return this->m_p_config_data->value(key)->v;
    } else {
        QVariant v;
        return v;
    }
}

void Config::set(QString key, QVariant value)
{
    if(this->m_p_config_data->contains(key)) {
        Item *item = this->m_p_config_data->value(key);
        if(item->info->type == PluginElementConfigData::config_type_integer && value.type() == QVariant::Int) {
            item->v = value;
        } else if(item->info->type == PluginElementConfigData::config_type_real && value.type() == QVariant::Double) {
            item->v = value;
        } else if(item->info->type == PluginElementConfigData::config_type_bool && value.type() == QVariant::Bool) {
            item->v = value;
        } else if(item->info->type == PluginElementConfigData::config_type_string && value.type() == QVariant::String) {
            item->v = value;
        } else if(item->info->type == PluginElementConfigData::config_type_select && value.type() == QVariant::String) {
            item->v = value;
        } else if(item->info->type == PluginElementConfigData::config_type_list && value.type() == QVariant::List) {
            item->v = value;
        } else {
            // 异常：类型不符
        }
    } else {
        // 异常：键不存在
    }
}

PluginElementConfigData::Item Config::getItem(QString objUuid, QString elementUuid, QString itemUuid)
{
    PluginElementConfigData::Item item;
    if(this->m_p_element_index->contains(QPair<QString, QString>(objUuid, elementUuid))) {
        PluginElementConfigData *data = (this->m_p_element_index->value(QPair<QString, QString>(objUuid, elementUuid)))->m_p_data;
        if(data->item_uuid_index->contains(itemUuid)) {
            item = data->item_list->at(data->item_uuid_index->value(itemUuid));
        }
    }
    return item;
}

QMap<QString, Config::Item *> *Config::getList()
{
    return this->m_p_config_data;
}

QMap<QString, QVector<QString>*> *Config::getCategoryIndex()
{
    return this->m_p_category_index;
}

void Config::initSystemItem()
{
    this->m_p_sys_item_info_lang = new PluginElementConfigData::Item;
    this->m_p_sys_item_info_style = new PluginElementConfigData::Item;
    this->m_p_sys_item_info_character = new PluginElementConfigData::Item;
    this->m_p_sys_item_info_debug = new PluginElementConfigData::Item;
    this->m_p_sys_item_info_fav_plugin = new PluginElementConfigData::Item;
    this->m_p_sys_item_info_disabled_plugin = new PluginElementConfigData::Item;
    // 语种
    this->m_p_sys_item_info_lang->id = QStringLiteral("sys_lang");
    this->m_p_sys_item_info_lang->caption = QStringLiteral("§[loc_pub:conf_item_sys_lang_cap]");
    this->m_p_sys_item_info_lang->desc = QStringLiteral("§[loc_pub:conf_item_sys_lang_desc]");
    this->m_p_sys_item_info_lang->config_name = QStringLiteral("sys_lang");
    this->m_p_sys_item_info_lang->category = QStringLiteral("system");
    this->m_p_sys_item_info_lang->type = PluginElementConfigData::config_type_select;
    this->m_p_sys_item_info_lang->_default.setValue(QStringLiteral("zh-hans"));
    this->m_p_sys_item_info_lang->restart = true;
    if(this->m_p_lang_list != nullptr) {
        QSet<QString>::iterator iter = this->m_p_lang_list->begin();
        while(iter != this->m_p_lang_list->end()) {
            PluginElementConfigData::Item::Entry entry;
            entry.id = *iter;
            entry.name = QStringLiteral("§[loc_pub:lang_") + *iter + QStringLiteral("]");
            this->m_p_sys_item_info_lang->list.append(entry);
            iter++;
        }
    }

    // 样式
    this->m_p_sys_item_info_style->id = QStringLiteral("sys_style");
    this->m_p_sys_item_info_style->caption = QStringLiteral("§[loc_pub:conf_item_sys_style_cap]");
    this->m_p_sys_item_info_style->desc = QStringLiteral("§[loc_pub:conf_item_sys_style_desc]");
    this->m_p_sys_item_info_style->config_name = QStringLiteral("sys_style");
    this->m_p_sys_item_info_style->category = QStringLiteral("system");
    this->m_p_sys_item_info_style->type = PluginElementConfigData::config_type_select;
    this->m_p_sys_item_info_style->_default.setValue(QStringLiteral("default"));
    this->m_p_sys_item_info_style->restart = true;
    ELEMENT_PAIR_LIST *rootList = this->m_ptrfunc_get_element_pair_list(PLUGIN_ELEMENT_TYPE::element_type_style);
    ELEMENT_PAIR_LIST::Iterator rootIter;
    if(rootList != nullptr) {
        rootIter = rootList->begin();
        while(rootIter != rootList->end()) {
            PluginElementStyle *element = static_cast<PluginElementStyle *>(rootIter->first);
            PluginElementStyleData *data = element->m_p_data;

            PluginElementConfigData::Item::Entry entry;
            entry.id = data->style_name;
            entry.obj_uuid = element->m_p_metadata->obj_uuid;
            entry.name = QStringLiteral("§[loc_priv:style_") + data->style_name + QStringLiteral("]");
            this->m_p_sys_item_info_style->list.append(entry);
            rootIter++;
        }
    }

    // 角色

    // 调试
    this->m_p_sys_item_info_debug->id = QStringLiteral("sys_debug");
    this->m_p_sys_item_info_debug->caption = QStringLiteral("§[loc_pub:conf_item_sys_debug_cap]");
    this->m_p_sys_item_info_debug->desc = QStringLiteral("§[loc_pub:conf_item_sys_debug_desc]");
    this->m_p_sys_item_info_debug->config_name = QStringLiteral("sys_debug");
    this->m_p_sys_item_info_debug->category = QStringLiteral("system");
    this->m_p_sys_item_info_debug->type = PluginElementConfigData::config_type_bool;
    this->m_p_sys_item_info_debug->_default.setValue(false);
    this->m_p_sys_item_info_debug->restart = true;

    // 收藏的插件
    this->m_p_sys_item_info_fav_plugin->id = QStringLiteral("sys_fav_plugin");
    this->m_p_sys_item_info_fav_plugin->caption = QStringLiteral("§[loc_pub:conf_item_sys_fav_plugin_cap]");
    this->m_p_sys_item_info_fav_plugin->desc = QStringLiteral("§[loc_pub:conf_item_fav_plugin_desc]");
    this->m_p_sys_item_info_fav_plugin->config_name = QStringLiteral("sys_fav_plugin");
    this->m_p_sys_item_info_fav_plugin->category = QStringLiteral("system");
    this->m_p_sys_item_info_fav_plugin->type = PluginElementConfigData::config_type_list;
    this->m_p_sys_item_info_fav_plugin->hidden = true;
    this->m_p_sys_item_info_fav_plugin->read_only = true;

    // 禁用的插件
    this->m_p_sys_item_info_disabled_plugin->id = QStringLiteral("sys_disabled_plugin");
    this->m_p_sys_item_info_disabled_plugin->caption = QStringLiteral("§[loc_pub:conf_item_sys_disabled_plugin_cap]");
    this->m_p_sys_item_info_disabled_plugin->desc = QStringLiteral("§[loc_pub:conf_item_disabled_plugin_desc]");
    this->m_p_sys_item_info_disabled_plugin->config_name = QStringLiteral("sys_disabled_plugin");
    this->m_p_sys_item_info_disabled_plugin->category = QStringLiteral("system");
    this->m_p_sys_item_info_disabled_plugin->type = PluginElementConfigData::config_type_list;
    this->m_p_sys_item_info_disabled_plugin->hidden = true;
    this->m_p_sys_item_info_disabled_plugin->read_only = true;
}

