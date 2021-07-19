#include "pluginelement.h"

PluginElement::PluginElement()
{

}

void PluginElement::read(QString path)
{
    // 读取元素头文件
    QFile head(path);
    if(head.open(QIODevice::ReadOnly)) {// 文件是否打开:是
        QByteArray allData = head.readAll();
        head.close();

        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
        if(json_error.error != QJsonParseError::NoError) {
            //>>>>>>>>>>>>>错误:JSON<<<<<<<<<<<<<
        }

        QJsonObject rootObj = jsonDoc.object();
        read(rootObj);
    } else { // 文件是否打开:否
        //>>>>>>>>>>>>错误：文件无法打开<<<<<<<<<<<<<<<
    }
}

void PluginElement::readMetadata(QJsonObject metadataObj)
{
    PluginElementMetadata metadata;
    metadata.id = metadataObj.value("id").toString();
    metadata.uuid16 = metadataObj.value("uuid").toString();
    if(metadata.uuid16.isEmpty()) {
        metadata.uuid16 = Tools::creatUuid16();
    }
    metadata.caption = metadataObj.value("caption").toString();
    metadata.description = metadataObj.value("description").toString();
    PluginElementTypeName type;
    metadata.type = type.getType(metadataObj.value("type").toString());
    this->m_metadata = metadata;
}
