#include "pluginelementlocalisation.h"

PluginElementLocalisation::PluginElementLocalisation(QSet<QString> *langList)
{
    this->m_p_lang_list = langList;
}

PluginElementLocalisation::~PluginElementLocalisation()
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
}

PLUGIN_EXC_LIST *PluginElementLocalisation::read(QJsonObject obj, QString filePath, QString dirPath, bool flag)
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
    this->m_p_data = new PluginElementLocalisationData;

    //读取元数据
    readMetadata(obj.value("metadata").toObject());
    this->m_p_metadata->file_path = filePath;
    this->m_p_metadata->dir_path = dirPath;

    //读取内容
    QJsonObject contentObj = obj.value("content").toObject();
    this->m_p_data->_public = readLang(contentObj.value("public").toArray(), true); //读取公有数据
    this->m_p_data->_private = readLang(contentObj.value("private").toArray(), false);  //读取私有数据

    PLUGIN_EXC_LIST::iterator iter;
    for(iter = this->m_p_exc_list->begin(); iter != this->m_p_exc_list->end(); iter++) {
        iter->element_uuid = this->m_p_metadata->uuid16;
    }
    return this->m_p_exc_list;
}

QVector<PluginElementLocalisationData::Lang *> *PluginElementLocalisation::readLang(QJsonArray array, bool isPublic)
{
    QVector<PluginElementLocalisationData::Lang *> *v = new QVector<PluginElementLocalisationData::Lang *>;
    for(int i = 0; i < array.count(); i++) {
        PluginElementLocalisationData::Lang *lang = new PluginElementLocalisationData::Lang;
        // 读取语种
        QJsonObject langObj = array.at(i).toObject();
        lang->uuid16 = langObj.value("uuid").toString();
        lang->orig_uuid16 = langObj.value("uuid").toString();
        if(lang->uuid16.isEmpty()) {// uuid为空
            // 生成uuid
            do {
                lang->uuid16 = Tools::creatUuid16();
            } while(this->m_p_data->lang_uuid_index->contains(lang->uuid16));
        }
        if(this->m_p_data->lang_uuid_index->contains(lang->uuid16)) {   //uuid冲突
            // 生成新uuid
            do {
                lang->uuid16 = Tools::creatUuid16();
            } while(this->m_p_data->lang_uuid_index->contains(lang->uuid16));
            // 异常：警告007-uuid冲突
            PluginExceptionData d;
            d.e = PLUGIN_EXC_WARN_007;
            d.group_uuid = lang->uuid16;
            this->m_p_exc_list->append(d);
        }

        // 读取语种
        lang->lang = langObj.value("lang").toString();
        this->m_p_lang_list->insert(lang->lang);
        if(lang->lang.isEmpty()) {
            // 异常：错误-100-本地化元素：缺少语种名称
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_100;
            d.group_uuid = lang->uuid16;
            this->m_p_exc_list->append(d);
            lang->isErr = true;
        }

        // 读取对照表
        QJsonObject tableObj = langObj.value("table").toObject();
        QJsonObject::iterator tblIter = tableObj.begin();
        int j = 0;
        while(tblIter != tableObj.end()) {
            PluginElementLocalisationData::Item item;
            // 生成uuid
            do {
                item.uuid16 = Tools::creatUuid16();
            } while(this->m_p_data->item_uuid_index->contains(item.uuid16));

            item.key = tblIter.key();
            item.value = tblIter.value().toString();

            // 加入索引
            PluginElementLocalisationData::ItemIndex itemIndex;
            itemIndex.lang_uuid = lang->uuid16;
            itemIndex.t = isPublic;
            itemIndex.s = j;
            this->m_p_data->item_uuid_index->insert(item.uuid16, itemIndex);

            lang->table->append(item);

            tblIter++;
            j++;
        }
        v->append(lang);

        // 加入索引
        PluginElementLocalisationData::LangIndex langIndex;
        langIndex.t = isPublic;
        langIndex.s = i;
        this->m_p_data->lang_name_index->insert(QPair<QString, bool>(lang->lang, isPublic), langIndex);
        this->m_p_data->lang_uuid_index->insert(lang->uuid16, langIndex);
    }
    return v;
}
