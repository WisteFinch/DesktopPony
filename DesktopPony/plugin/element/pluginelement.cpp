#include "pluginelement.h"

PluginElement::PluginElement()
{

}

PluginElement::~PluginElement()
{
    if(this->m_p_metadata != nullptr) {
        delete this->m_p_metadata;
        this->m_p_metadata = nullptr;
    }
    if(this->m_p_exc_list != nullptr) {
        delete this->m_p_exc_list;
        this->m_p_exc_list = nullptr;
    }
}

PLUGIN_EXC_LIST *PluginElement::read(QString filePath, QString dirPath)
{
    // 清理
    if(this->m_p_metadata != nullptr) {
        delete this->m_p_metadata;
    }
    if(this->m_p_exc_list != nullptr) {
        delete this->m_p_exc_list;
    }
    this->m_p_metadata = new PluginElementMetadata;
    this->m_p_exc_list = new PLUGIN_EXC_LIST;

    // 读取元素头文件
    QFile head(filePath);
    if(head.open(QIODevice::ReadOnly)) {// 文件是否打开:是
        QByteArray allData = head.readAll();
        head.close();

        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
        if(json_error.error != QJsonParseError::NoError) {
            // 异常：错误006-插件元素头文件JSON无法读取
            this->m_p_metadata->uuid16 = Tools::creatUuid16();
            this->m_p_metadata->type = element_type_null;
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_006;
            d.element_uuid = this->m_p_metadata->uuid16;
            this->m_p_exc_list->first.append(d);
            return this->m_p_exc_list;
        }

        QJsonObject rootObj = jsonDoc.object();
        PLUGIN_EXC_LIST excList = *read(rootObj, filePath, dirPath, false); // 读取元素头文件
        this->m_p_exc_list->first.append(excList.first);
        this->m_p_exc_list->second.append(excList.second);
    } else { // 文件是否打开:否
        // 异常：错误006-插件元素头文件无法读取
        this->m_p_metadata->uuid16 = Tools::creatUuid16();
        this->m_p_metadata->type = element_type_null;
        PluginExceptionData d;
        d.e = PLUGIN_EXC_ERR_005;
        d.element_uuid = this->m_p_metadata->uuid16;
        this->m_p_exc_list->first.append(d);
        return this->m_p_exc_list;
    }

    QVector<PluginExceptionData>::iterator iter;
    for(iter = this->m_p_exc_list->first.begin(); iter != this->m_p_exc_list->first.end(); iter++) {
        iter->element_uuid = this->m_p_metadata->uuid16;
    }
    for(iter = this->m_p_exc_list->second.begin(); iter != this->m_p_exc_list->second.end(); iter++) {
        iter->element_uuid = this->m_p_metadata->uuid16;
    }
    return this->m_p_exc_list;
}

void PluginElement::readMetadata(QJsonObject metadataObj)
{
    if(this->m_p_metadata != nullptr) {
        delete this->m_p_metadata;
    }
    this->m_p_metadata = new PluginElementMetadata;
    this->m_p_metadata->uuid16 = metadataObj.value("uuid").toString();
    this->m_p_metadata->orig_uuid16 = metadataObj.value("uuid").toString();
    if(this->m_p_metadata->uuid16.isEmpty()) {
        this->m_p_metadata->uuid16 = Tools::creatUuid16();
        // 异常：警告006-插件元素头文件元数据缺少uuid
        PluginExceptionData d;
        d.e = PLUGIN_EXC_WARN_006;
        d.element_uuid = this->m_p_metadata->uuid16;
        this->m_p_exc_list->second.append(d);
    }
    this->m_p_metadata->id = metadataObj.value("id").toString();
    if(this->m_p_metadata->id.isEmpty()) {
        // 异常：错误008-插件元素头文件元数据缺少标识符
        PluginExceptionData d;
        d.e = PLUGIN_EXC_ERR_008;
        d.element_uuid = this->m_p_metadata->uuid16;
        this->m_p_exc_list->first.append(d);
    }
    this->m_p_metadata->caption = metadataObj.value("caption").toString();
    this->m_p_metadata->description = metadataObj.value("description").toString();
    this->m_p_metadata->icon = metadataObj.value("icon").toString();
    PluginElementTypeName type;
    this->m_p_metadata->type = type.getType(metadataObj.value("type").toString());
}
