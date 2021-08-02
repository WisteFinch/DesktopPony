#include "pluginelementlocalisationdata.h"

PluginElementLocalisationData::PluginElementLocalisationData()
{

}

PluginElementLocalisationData::~PluginElementLocalisationData()
{
    delete this->lang_uuid_index;
    delete this->lang_name_index;
    delete this->item_uuid_index;
    qDeleteAll(this->_public->begin(), this->_public->end());
    delete this->_public;
    qDeleteAll(this->_private->begin(), this->_private->end());
    delete this->_private;
}
