#include "pluginelementlocalisation.h"

PluginElementLocalisation::PluginElementLocalisation()
{

}

PluginElementLocalisation::~PluginElementLocalisation()
{

}

void PluginElementLocalisation::read(QJsonObject obj)
{
    if(this->m_p_data != nullptr) {
        delete this->m_p_data;
    }
    this->m_p_data = new PluginElementLocalisationData;

    //读取元数据
    readMetadata(obj.value("metadata").toObject());

    //读取内容
    QJsonObject contentObj = obj.value("content").toObject();
    this->m_p_data->global = readLang(contentObj.value("global").toArray()); //读取全局数据
    this->m_p_data->local = readLang(contentObj.value("local").toArray());  //读取局部数据
}

PluginElementLocalisationData::LANG PluginElementLocalisation::readLang(QJsonArray array)
{
    PluginElementLocalisationData::LANG lang;
    for(int i = 0; i < array.count(); i++) {
        QJsonObject langObj = array.at(i).toObject();
        QString langName = langObj.value("lang").toString();
        QJsonArray tableObj = langObj.value("table").toArray();
        PluginElementLocalisationData::TABLE table;
        for(int j = 0; j < tableObj.count(); j++) {
            QJsonObject e = tableObj.at(j).toObject();
            table.insert(e.value("key").toString(), QPair<QString, int>(e.value("value").toString(), j + 1));
        }
        lang.insert(langName, table);
    }
    return lang;
}
