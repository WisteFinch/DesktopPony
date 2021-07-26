#include "pluginelementconfig.h"

PluginElementConfig::PluginElementConfig()
{

}

PluginElementConfig::~PluginElementConfig()
{
    delete this->m_p_data;
}

PLUGIN_EXC_LIST *PluginElementConfig::read(QJsonObject obj, QString path, bool flag)
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
    this->m_p_metadata->file_path = path;

    //读取内容
    QJsonArray contentArray = obj.value("content").toArray();

    for(int i = 0; i < contentArray.count(); i++) {
        PluginElementConfigData::Item *item = new PluginElementConfigData::Item;
        this->m_p_data->item_list->append(item);

        QJsonObject itemObj = contentArray.at(i).toObject();

        // 读取uuid
        item->uuid16 = itemObj.value("uuid").toString();
        item->orig_uuid16 = itemObj.value("uuid").toString();
        if(item->uuid16.isEmpty()) {// uuid为空
            // 生成uuid
            do {
                item->uuid16 = Tools::creatUuid16();
            } while(this->m_p_data->item_uuid_index->contains(item->uuid16));
        }
        if(this->m_p_data->item_uuid_index->contains(item->uuid16)) {   //uuid冲突
            // 生成新uuid
            do {
                item->uuid16 = Tools::creatUuid16();
            } while(this->m_p_data->item_uuid_index->contains(item->uuid16));
            // 异常：警告007-uuid冲突
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_007;
            d.group_uuid = item->uuid16;
            this->m_p_exc_list->append(d);
        }

        item->id = itemObj.value("id").toString();
        if(item->id.isEmpty()) {
            // 异常：错误700-配置元素：组缺少标识符
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_700;
            d.group_uuid = item->uuid16;
            this->m_p_exc_list->append(d);
        }
        item->caption = itemObj.value("caption").toString();
        item->desc = itemObj.value("description").toString();
        item->type = this->m_p_data->getType(itemObj.value("type").toString());
        item->hidden = itemObj.value("hidden").toBool();
        item->reset = itemObj.value("reset").toBool();
        QString _default = itemObj.value("default").toString();
        QString rangeFrom = itemObj.value("range_from").toString();
        QString rangeTo = itemObj.value("range_to").toString();
        QJsonArray selectList = itemObj.value("select_list").toArray();
        if(item->type == PluginElementConfigData::Config_TYPE::config_type_integer) {
            item->_default = _default.toInt();
            item->range_from = rangeFrom.toInt();
            item->range_to = rangeTo.toInt();
        } else if(item->type == PluginElementConfigData::Config_TYPE::config_type_real) {
            item->_default = _default.toDouble();
            item->range_from = rangeFrom.toDouble();
            item->range_to = rangeTo.toDouble();
        } else if(item->type == PluginElementConfigData::Config_TYPE::config_type_bool) {
            item->_default = _default.toLower() == "true";
        } else if(item->type == PluginElementConfigData::Config_TYPE::config_type_string) {
            item->_default = _default;
        } else if(item->type == PluginElementConfigData::Config_TYPE::config_type_select) {
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
                    } while(this->m_p_data->select_item_uuid_index->contains(item->uuid16));
                }
                if(this->m_p_data->select_item_uuid_index->contains(si.uuid16)) {   //uuid冲突
                    // 生成新uuid
                    do {
                        si.uuid16 = Tools::creatUuid16();
                    } while(this->m_p_data->select_item_uuid_index->contains(si.uuid16));
                    // 异常：警告007-uuid冲突
                    PluginExceptionData d;
                    d.e = PLUGIN_EXC_ERR_007;
                    d.group_uuid = item->uuid16;
                    d.item_uuid = si.uuid16;
                    this->m_p_exc_list->append(d);
                }

                //加入索引
                PluginElementConfigData::SelectItemIndex index;
                index.item_uuid = item->uuid16;
                index.s = j;
                this->m_p_data->select_item_uuid_index->insert(si.uuid16, index);

                si.id = sObj.value("id").toString();
                if(si.id.isEmpty()) {
                    // 异常：错误702-配置元素：下拉项缺少标识符
                    PluginExceptionData d;
                    d.e = PLUGIN_EXC_ERR_702;
                    d.group_uuid = item->uuid16;
                    d.item_uuid = si.uuid16;
                    this->m_p_exc_list->append(d);
                }
                si.caption = sObj.value("caption").toString();

                item->select.append(si);
            }
        } else {
            // 异常：错误701-配置元素：组少项类型
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_701;
            d.group_uuid = item->uuid16;
            this->m_p_exc_list->append(d);
        }


        // 加入索引
        this->m_p_data->item_uuid_index->insert(item->uuid16, i);
    }

    PLUGIN_EXC_LIST::iterator iter;
    for(iter = this->m_p_exc_list->begin(); iter != this->m_p_exc_list->end(); iter++) {
        iter->element_uuid = this->m_p_metadata->uuid16;
    }
    return this->m_p_exc_list;
}
