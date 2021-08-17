#include "pluginelementconfig.h"

PluginElementConfig::PluginElementConfig(ELEMENT_CONFIG_NAME_INDEX *configNameIndex)
{
    this->m_p_config_name_index = configNameIndex;
}

PluginElementConfig::~PluginElementConfig()
{
    if(this->m_p_exc_list != nullptr) {
        delete this->m_p_exc_list;
        this->m_p_exc_list = nullptr;
    }
    if(this->m_p_metadata != nullptr) {
        delete this->m_p_metadata;
        this->m_p_metadata = nullptr;
    }
    if(this->m_p_data != nullptr) {
        delete this->m_p_data;
        this->m_p_data = nullptr;
    }
    this->m_p_config_name_index = nullptr;
}

PLUGIN_EXC_LIST *PluginElementConfig::read(QJsonObject obj, QString filePath, QString dirPath, bool flag)
{
    // 清理
    if(this->m_p_exc_list != nullptr && flag) {
        if(flag) {
            delete this->m_p_exc_list;
            this->m_p_exc_list = new PLUGIN_EXC_LIST;
        }
    } else {
        this->m_p_exc_list = new PLUGIN_EXC_LIST;
    }
    if(this->m_p_data != nullptr) {
        delete this->m_p_data;
    }
    this->m_p_data = new PluginElementConfigData;

    //读取元数据
    readMetadata(obj.value("metadata").toObject());
    this->m_p_metadata->file_path = filePath;
    this->m_p_metadata->dir_path = dirPath;

    //读取内容
    QJsonArray contentArray = obj.value("content").toArray();

    for(int i = 0; i < contentArray.count(); i++) {
        PluginElementConfigData::Item item;

        QJsonObject itemObj = contentArray.at(i).toObject();

        // 读取uuid
        item.uuid16 = itemObj.value("uuid").toString();
        item.orig_uuid16 = itemObj.value("uuid").toString();
        if(item.uuid16.isEmpty()) {// uuid为空
            // 生成uuid
            do {
                item.uuid16 = Tools::creatUuid16();
            } while(this->m_p_data->item_uuid_index->contains(item.uuid16));
        }
        if(this->m_p_data->item_uuid_index->contains(item.uuid16)) {   //uuid冲突
            // 生成新uuid
            do {
                item.uuid16 = Tools::creatUuid16();
            } while(this->m_p_data->item_uuid_index->contains(item.uuid16));
            // 异常：警告007-uuid冲突
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_007;
            d.group_uuid = item.uuid16;
            this->m_p_exc_list->append(d);
        }

        item.id = itemObj.value("id").toString();
        if(item.id.isEmpty()) {
            // 异常：错误700-配置元素：组缺少标识符
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_700;
            d.group_uuid = item.uuid16;
            this->m_p_exc_list->append(d);
            item.isErr = true;
        }
        item.config_name = itemObj.value("config_name").toString();
        if(item.config_name.isEmpty()) {
            // 异常：错误702-配置元素：组缺少配置名称
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_702;
            d.group_uuid = item.uuid16;
            this->m_p_exc_list->append(d);
            item.isErr = true;
        }
        if(this->m_p_config_name_index->contains(item.config_name)) {
            // 异常：错误703-配置元素：组配置名称冲突
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_703;
            d.group_uuid = item.uuid16;
            this->m_p_exc_list->append(d);
            item.isErr = true;
        } else {
            UuidIndexItem index;
            index.obj_uuid = this->m_p_metadata->obj_uuid;
            index.element_uuid = this->m_p_metadata->uuid16;
            index.group_uuid = item.uuid16;
            this->m_p_config_name_index->insert(item.config_name, index);
        }
        item.caption = itemObj.value("caption").toString();
        item.desc = itemObj.value("description").toString();
        QString category = itemObj.value("category").toString();
        if(!category.isEmpty()) {
            item.category = category;
        }
        item.type = this->m_p_data->getType(itemObj.value("type").toString());
        item.hidden = itemObj.value("hidden").toBool();
        item.reset = itemObj.value("reset").toBool();
        item.read_only = itemObj.value("read_only").toBool();
        item.restart = itemObj.value("restart").toBool();
        if(item.type == PluginElementConfigData::Config_TYPE::config_type_integer) {
            item._default.setValue(itemObj.value("default").toInt());
            item.range_from.setValue(itemObj.value("range_from").toInt());
            item.range_to.setValue(itemObj.value("range_to").toInt());
        } else if(item.type == PluginElementConfigData::Config_TYPE::config_type_real) {
            item._default.setValue(itemObj.value("default").toDouble());
            item.range_from.setValue(itemObj.value("range_from").toDouble());
            item.range_to.setValue(itemObj.value("range_to").toDouble());
        } else if(item.type == PluginElementConfigData::Config_TYPE::config_type_bool) {
            item._default.setValue(itemObj.value("default").toBool());
        } else if(item.type == PluginElementConfigData::Config_TYPE::config_type_string) {
            item._default.setValue(itemObj.value("default").toString());
        } else if(item.type == PluginElementConfigData::Config_TYPE::config_type_select) {
            QJsonArray selectList = itemObj.value("select_list").toArray();
            for(int j = 0; j < selectList.count(); j++) {
                QJsonObject sObj = selectList.at(j).toObject();
                PluginElementConfigData::Item::SelectItem si;

                // 读取uuid
                si.uuid16 = sObj.value("uuid").toString();
                si.orig_uuid16 = sObj.value("uuid").toString();
                if(si.uuid16.isEmpty()) {// uuid为空
                    // 生成uuid
                    do {
                        si.uuid16 = Tools::creatUuid16();
                    } while(this->m_p_data->select_item_uuid_index->contains(item.uuid16));
                }
                if(this->m_p_data->select_item_uuid_index->contains(si.uuid16)) {   //uuid冲突
                    // 生成新uuid
                    do {
                        si.uuid16 = Tools::creatUuid16();
                    } while(this->m_p_data->select_item_uuid_index->contains(si.uuid16));
                    // 异常：警告007-uuid冲突
                    PluginExceptionData d;
                    d.e = PLUGIN_EXC_ERR_007;
                    d.group_uuid = item.uuid16;
                    d.item_uuid = si.uuid16;
                    this->m_p_exc_list->append(d);
                }

                //加入索引
                PluginElementConfigData::SelectItemIndex index;
                index.item_uuid = item.uuid16;
                index.s = j;
                this->m_p_data->select_item_uuid_index->insert(si.uuid16, index);

                si.id = sObj.value("id").toString();
                if(si.id.isEmpty()) {
                    // 异常：错误704-配置元素：下拉项缺少标识符
                    PluginExceptionData d;
                    d.e = PLUGIN_EXC_ERR_704;
                    d.group_uuid = item.uuid16;
                    d.item_uuid = si.uuid16;
                    this->m_p_exc_list->append(d);
                    item.isErr = true;
                }
                si.name = sObj.value("name").toString();
                si.obj_uuid = this->m_p_metadata->obj_uuid;

                item.select.append(si);
                item._default.setValue(itemObj.value("default").toString());
            }
        } else {
            // 异常：错误701-配置元素：组少项类型
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_701;
            d.group_uuid = item.uuid16;
            this->m_p_exc_list->append(d);
            item.isErr = true;
        }

        // 加入索引
        this->m_p_data->item_uuid_index->insert(item.uuid16, i);

        this->m_p_data->item_list->append(item);
    }

    PLUGIN_EXC_LIST::iterator iter;
    for(iter = this->m_p_exc_list->begin(); iter != this->m_p_exc_list->end(); iter++) {
        iter->element_uuid = this->m_p_metadata->uuid16;
    }
    return this->m_p_exc_list;
}
