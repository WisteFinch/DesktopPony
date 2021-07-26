#include "pluginelementlocalisationdata.h"

PluginElementLocalisationData::PluginElementLocalisationData()
{

}

PluginElementLocalisationData::~PluginElementLocalisationData()
{
    delete this->lang_uuid_index;
    delete this->lang_name_index;
    delete this->item_uuid_index;
    while(!this->_public->isEmpty()) {
        delete this->_public->last();
        this->_public->pop_back();
    }
    while(!this->_private->isEmpty()) {
        delete this->_private->last();
        this->_private->pop_back();
    }
}
