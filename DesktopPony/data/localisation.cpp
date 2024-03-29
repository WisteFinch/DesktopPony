#include "localisation.h"

Localisation::Localisation()
{
}

Localisation::~Localisation()
{
    clear();
}

void Localisation::clear()
{
    if(this->m_p_public != nullptr) {
        delete this->m_p_public;
        this->m_p_public = nullptr;
    }
    if(this->m_p_private != nullptr) {
        qDeleteAll(this->m_p_private->begin(), this->m_p_private->end());
        this->m_p_private->clear();
        delete this->m_p_private;
        this->m_p_private = nullptr;
    }
}

void Localisation::init(PTRFUNC_GET_ELEMENT_PAIR_LIST ptrfunc1, PTRFUNC_GET_DEISABLED_PLUGINS ptrfunc2)
{
    this->m_ptrfunc_get_element_pair_list = ptrfunc1;
    this->m_ptrfunc_get_disabled_plugins = ptrfunc2;
}

void Localisation::setLanguage(QString lang)
{
    this->m_s_language = lang;
}

QString Localisation::get(QString key)
{
    if(this->m_p_public->contains(key)) {
        return this->m_p_public->value(key).str;
    } else {
        return key;
    }
}

Localisation::Element Localisation::getDebugData(QString key)
{
    if(this->m_p_public->contains(key)) {
        return this->m_p_public->value(key);
    } else {
        Element e;
        e.str = "[NULL]" + key;
        return e;
    }
}


QString Localisation::getPriv(QString uuid, QString key)
{
    if(this->m_p_private->contains(uuid)) {
        if(this->m_p_private->value(uuid)->contains(key)) {
            return this->m_p_private->value(uuid)->value(key).str;
        }
    }
    return key;
}

Localisation::Element Localisation::getPrivDebugData(QString uuid, QString key)
{
    if(this->m_p_private->contains(uuid)) {
        if(this->m_p_private->value(uuid)->contains(key)) {
            return this->m_p_private->value(uuid)->value(key);
        }
    }
    Element e;
    e.str = "[NULL]" + key;
    return e;
}

void Localisation::creatIndex()
{
    //清理
    clear();
    m_p_public = new TABLE;
    m_p_private = new QMap<QString, TABLE *>;

    ELEMENT_PAIR_LIST *rootList = this->m_ptrfunc_get_element_pair_list(PLUGIN_ELEMENT_TYPE::element_type_localisation);
    QList<QVariant> disabledList = this->m_ptrfunc_get_disabled_plugins();
    if(rootList != nullptr) {
        ELEMENT_PAIR_LIST::Iterator rootIter = rootList->begin();
        while(rootIter != rootList->end()) {
            if(!disabledList.contains(rootIter->first->m_p_metadata->obj_id)) {
                PluginElementLocalisation *element = static_cast<PluginElementLocalisation *>(rootIter->first);
                PluginElementLocalisationData *data = element->m_p_data;
                // 读取公有
                QPair<QString, bool> key = QPair<QString, bool>(this->m_s_language, true);
                bool isContain = data->lang_name_index->contains(key);
                if(isContain) { // 判断语种是否存在
                    uint sub = data->lang_name_index->value(key).s;
                    {
                        if(!data->_public->isEmpty()) {
                            // 获取对照表
                            PluginElementLocalisationData::TABLE *tbl = data->_public->at(sub)->table;
                            PluginElementLocalisationData::TABLE::Iterator tblIter = tbl->begin();
                            // 加入列表
                            while(tblIter != tbl->end()) {
                                if(!tblIter->isErr) {
                                    Element e;
                                    e.str = tblIter->value;
                                    e.index = data->item_uuid_index->value(tblIter->uuid16).s;
                                    e.uuid_plugin = rootIter->second;
                                    e.uuid_element = element->m_p_metadata->uuid16;
                                    this->m_p_public->insert(tblIter->key, e);
                                    tblIter++;
                                }
                            }
                        }
                    }
                }
                // 读取私有
                key = QPair<QString, bool>(this->m_s_language, false);
                isContain = data->lang_name_index->contains(key);
                if(isContain) {
                    uint sub = data->lang_name_index->value(key).s;
                    {
                        if(!data->_private->isEmpty()) {
                            PluginElementLocalisationData::TABLE *tbl = data->_private->at(sub)->table;
                            PluginElementLocalisationData::TABLE::Iterator tblIter = tbl->begin();
                            if(!this->m_p_private->contains(rootIter->second)) {
                                this->m_p_private->insert(rootIter->second, new TABLE);
                            }
                            TABLE *privateTbl = this->m_p_private->value(rootIter->second);
                            while(tblIter != tbl->end()) {
                                if(!tblIter->isErr) {
                                    Element e;
                                    e.str = tblIter->value;
                                    e.index = data->item_uuid_index->value(tblIter->uuid16).s;
                                    e.uuid_plugin = rootIter->second;
                                    e.uuid_element = element->m_p_metadata->uuid16;
                                    privateTbl->insert(tblIter->key, e);
                                }
                                tblIter++;
                            }
                            privateTbl = nullptr;
                        }
                    }
                }
                data = nullptr;
            }
            rootIter++;
        }
    }
}
