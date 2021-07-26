#include "pluginelementconfigdata.h"

PluginElementConfigData::PluginElementConfigData()
{

}

PluginElementConfigData::~PluginElementConfigData()
{
    while(!this->item_list->isEmpty()) {
        delete this->item_list->last();
        this->item_list->pop_back();
    }
    delete this->item_list;
    delete this->item_uuid_index;
    delete this->select_item_uuid_index;
}

QString PluginElementConfigData::getTypeName(Config_TYPE t)
{
    for(int i = 0 ; i < this->type_vector.size(); i++) {
        if(this->type_vector.at(i).first == t) {
            return this->type_vector.at(i).second;
        }
    }
    return "null";
}

PluginElementConfigData::Config_TYPE PluginElementConfigData::getType(QString s)
{
    s = s.toLower();
    for(int i = 0 ; i < this->type_vector.size(); i++) {
        if(this->type_vector.at(i).second == s) {
            return this->type_vector.at(i).first;
        }
    }
    return config_type_null;
}
