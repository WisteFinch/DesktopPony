#include "localisation.h"

Localisation::Localisation()
{
}

void Localisation::clear()
{
    if(this->m_p_global != nullptr) {
        delete this->m_p_global;
        this->m_p_global = nullptr;
    }
    if(this->m_p_local != nullptr) {
        QList<TABLE *> l = this->m_p_local->values();
        while(!l.isEmpty()) {
            delete l.last();
            l.pop_back();
        }
        delete this->m_p_local;
        this->m_p_local = nullptr;
    }
}

void Localisation::init(PTRFUNC_GET_ELEMENT_PAIR_LIST ptrfunc)
{
    //清理
    clear();
    m_p_global = new TABLE;
    m_p_local = new QMap<QString, TABLE *>;

    this->ptrfun_get_element_pair_list = ptrfunc;
}

void Localisation::setLanguage(QString lang)
{
    this->m_s_language = lang;
}

QString Localisation::get(QString key)
{
    if(this->m_p_global->contains(key)) {
        return this->m_p_global->value(key).s;
    } else {
        return key;
    }
}

Localisation::Element Localisation::getDebugData(QString key)
{
    if(this->m_p_global->contains(key)) {
        return this->m_p_global->value(key);
    } else {
        Element e;
        e.s = key;
        return e;
    }
}


QString Localisation::getLocal(QString uuid, QString key)
{
    if(this->m_p_local->contains(uuid)) {
        if(this->m_p_local->value(uuid)->contains(key)) {
            return this->m_p_local->value(uuid)->value(key).s;
        }
    }
    return key;
}

Localisation::Element Localisation::getLocalDebugData(QString uuid, QString key)
{
    if(this->m_p_local->contains(uuid)) {
        if(this->m_p_local->value(uuid)->contains(key)) {
            return this->m_p_local->value(uuid)->value(key);
        }
    }
    Element e;
    e.s = key;
    return e;
}

void Localisation::creatIndex()
{
    ELEMENT_PAIR_LIST *rootList = this->ptrfun_get_element_pair_list(type_localisation);
    ELEMENT_PAIR_LIST::Iterator rootIter;
    for(rootIter = rootList->begin(); rootIter != rootList->end(); rootIter++) {
        PluginElementLocalisation *element = static_cast<PluginElementLocalisation *>(rootIter->first);
        PluginElementLocalisationData *data = element->m_p_data;
        {
            PluginElementLocalisationData::TABLE tbl = data->global.value(this->m_s_language);
            PluginElementLocalisationData::TABLE::Iterator tblIter = tbl.begin();
            while(tblIter != tbl.end()) {
                Element e;
                e.s = tblIter.value().first;
                e.index = tblIter.value().second;
                e.uuid_plugin = rootIter->second;
                e.uuid_element = element->m_metadata.uuid16;
                this->m_p_global->insert(tblIter.key(), e);
                tblIter++;
            }
        }
        {
            PluginElementLocalisationData::TABLE tbl = data->local.value(this->m_s_language);
            PluginElementLocalisationData::TABLE::Iterator tblIter = tbl.begin();
            if(!this->m_p_local->contains(rootIter->second)) {
                this->m_p_local->insert(rootIter->second, new TABLE);
            }
            TABLE *localTbl = this->m_p_local->value(rootIter->second);
            while(tblIter != tbl.end()) {
                Element e;
                e.s = tblIter.value().first;
                e.index = tblIter.value().second;
                e.uuid_plugin = rootIter->second;
                e.uuid_element = element->m_metadata.uuid16;
                localTbl->insert(tblIter.key(), e);
                tblIter++;
            }
            localTbl = nullptr;
        }
        data = nullptr;
    }
}
