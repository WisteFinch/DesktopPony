#include "config.h"

Config::Config()
{

}

Config::~Config()
{
    clear();
}

void Config::init(PTRFUNC_GET_ELEMENT_PAIR_LIST ptrfunc)
{
    this->ptrfun_get_element_pair_list = ptrfunc;
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
    if(this->m_p_json_obj != nullptr) {
        delete this->m_p_json_obj;
        this->m_p_json_obj = nullptr;
    }
}

void Config::creatIndex()
{
    // 清理
    clear();
    this->m_p_config_data = new QMap<QString, Item *>;
    this->m_p_element_index = new QMap<QPair<QString, QString>, PluginElementConfig *>;

    ELEMENT_PAIR_LIST *rootList = this->ptrfun_get_element_pair_list(PLUGIN_ELEMENT_TYPE::element_type_config);
    ELEMENT_PAIR_LIST::Iterator rootIter;
    for(rootIter = rootList->begin(); rootIter != rootList->end(); rootIter++) {
        PluginElementConfig *element = static_cast<PluginElementConfig *>(rootIter->first);
        QVector<PluginElementConfigData::Item> *list = element->m_p_data->item_list;

        // 加入配置元素索引
        this->m_p_element_index->insert(QPair<QString, QString>(rootIter->first->m_p_metadata->obj_uuid, rootIter->first->m_p_metadata->uuid16), element);

        // 读取配置项
        QVector<PluginElementConfigData::Item>::iterator listIter = list->begin();
        while(listIter != list->end()) {
            if(!listIter->isErr) {
                Item *item = new Item;
                item->config_name = listIter->config_name;
                item->type = listIter->type;
                item->v = listIter->_default;
                item->obj_uuid = rootIter->first->m_p_metadata->obj_uuid;
                item->element_uuid = rootIter->first->m_p_metadata->uuid16;
                item->group_uuid = listIter->uuid16;
                this->m_p_config_data->insert(item->config_name, item);
            }
            listIter++;
        }
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
            QVariant v = rootObj.value(keyList.at(i)).toVariant();
            Item *item = this->m_p_config_data->value(keyList.at(i));
            if(item->type == PluginElementConfigData::config_type_integer) {
                item->v = v.toInt();
            } else if(item->type == PluginElementConfigData::config_type_real) {
                item->v = v.toDouble();
            } else if(item->type == PluginElementConfigData::config_type_bool) {
                item->v = v.toBool();
            } else if(item->type == PluginElementConfigData::config_type_string) {
                item->v = v.toString();
            } else if(item->type == PluginElementConfigData::config_type_select) {
                item->v = v.toString();
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
        this->m_p_json_obj->remove(iter.value()->config_name);
        this->m_p_json_obj->insert(iter.value()->config_name, QJsonValue::fromVariant(iter.value()->v));
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
        this->m_p_json_obj->insert(iter.value()->config_name, QJsonValue::fromVariant(iter.value()->v));
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
        if(item->type == PluginElementConfigData::config_type_integer && value.type() == QVariant::Int) {
            item->v = value;
        } else if(item->type == PluginElementConfigData::config_type_real && value.type() == QVariant::Double) {
            item->v = value;
        } else if(item->type == PluginElementConfigData::config_type_bool && value.type() == QVariant::Bool) {
            item->v = value;
        } else if(item->type == PluginElementConfigData::config_type_string && value.type() == QVariant::String) {
            item->v = value;
        } else if(item->type == PluginElementConfigData::config_type_select && value.type() == QVariant::String) {
            item->v = value;
        } else {
            // 异常：类型不符
        }
    } else {
        // 异常：键不存在
    }
}
