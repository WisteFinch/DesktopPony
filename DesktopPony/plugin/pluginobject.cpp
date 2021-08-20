#include "pluginobject.h"

PluginObject::PluginObject()
{

}

PluginObject::~PluginObject()
{
    clear();
}

PLUGIN_EXC_LIST *PluginObject::readHead(QString path, bool isSystem, ELEMENT_CONFIG_NAME_INDEX *configNameIndex, QSet<QString> *langList)
{
    // 清理
    clear();
    this->m_p_element_list = new PLUGIN_ELEMENT_LIST;
    this->m_p_exc_list = new PLUGIN_EXC_LIST;
    this->m_p_metadata = new PluginObjectMetadata;
    this->m_p_element_uuid_index = new QMap<QString, uint>;

    this->m_p_metadata->is_system = isSystem;

    // 读取头文件并转换为jsonObj
    QFile head(QDir(path).filePath(STR_PLUGIN_HEAD_PATH));
    if(head.open(QIODevice::ReadOnly)) {// 文件是否打开:是
        QByteArray allData = head.readAll();
        head.close();

        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
        if(json_error.error != QJsonParseError::NoError) {
            // 异常：错误002-插件对象头文件JSON无法读取
            this->m_p_metadata->uuid = Tools::creatUuid();
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_002;
            d.object_uuid = this->m_p_metadata->uuid;
            this->m_p_exc_list->first.append(d);
            return this->m_p_exc_list;
        }

        QJsonObject rootObj = jsonDoc.object();

        // 读取元数据
        if(rootObj.contains("metadata")) {  // 元数据是否存在:是
            QJsonObject metadataObj = rootObj.value("metadata").toObject();
            this->m_p_metadata->uuid = metadataObj.value("uuid").toString();
            this->m_p_metadata->orig_uuid = metadataObj.value("uuid").toString();
            this->m_p_metadata->id = metadataObj.value("id").toString();
            this->m_p_metadata->caption = metadataObj.value("caption").toString();
            this->m_p_metadata->description = metadataObj.value("description").toString();
            this->m_p_metadata->icon = metadataObj.value("icon").toString();
            this->m_p_metadata->author =  metadataObj.value("author").toString();
            this->m_p_metadata->version =  metadataObj.value("version").toString();
            this->m_p_metadata->remote_url =  metadataObj.value("remote_url").toString();
            this->m_p_metadata->file_path = QDir(path).filePath(STR_PLUGIN_HEAD_PATH);
            this->m_p_metadata->dir_path = path;
            if(!this->m_p_metadata->remote_url.isEmpty()) {
                this->m_p_metadata->is_remote = true;
            }
            //元数据检查
            metadataCheck();
        } else {// 元数据是否存在:否
            this->m_p_metadata->uuid = Tools::creatUuid();
            // 异常：错误003-插件对象头文件缺少元数据
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_003;
            d.object_uuid = this->m_p_metadata->uuid;
            this->m_p_exc_list->first.append(d);
        }

        // 读取元素
        if(rootObj.contains("elements")) {
            // 获取元素列表
            QJsonArray elementsArray = rootObj.value("elements").toArray();
            for(int i = 0; i < elementsArray.size(); i++) {
                QJsonObject obj = elementsArray.at(i).toObject();
                QString p = QDir(path).filePath(obj.value("path").toString());
                if(p.isEmpty()) {
                    continue;
                }
                PluginElement *e = readElement(p, configNameIndex, langList);  // 读取元素

                // 元素uuid去重
                if(this->m_p_element_uuid_index->contains(e->m_p_metadata->uuid16)) {
                    // 生成新uuid
                    do {
                        e->m_p_metadata->uuid16 = Tools::creatUuid16();
                    } while(this->m_p_element_uuid_index->contains(e->m_p_metadata->uuid16));
                    if(!e->m_p_metadata->orig_uuid16.isEmpty()) {
                        // 异常：警告007-uuid冲突
                        PluginExceptionData d;
                        d.e = PLUGIN_EXC_ERR_007;
                        d.element_uuid = e->m_p_metadata->uuid16;
                        this->m_p_exc_list->second.append(d);
                    }
                }

                //加入索引
                this->m_p_element_uuid_index->insert(e->m_p_metadata->uuid16, i);

                this->m_p_element_list->append(e);
            }
        }
    } else {// 文件是否打开:否
        // 异常：错误001-插件对象头文件无法读取
        this->m_p_metadata->uuid = Tools::creatUuid();
        PluginExceptionData d;
        d.e = PLUGIN_EXC_ERR_001;
        d.object_uuid = this->m_p_metadata->uuid;
        this->m_p_exc_list->first.append(d);
        return this->m_p_exc_list;
    }

    QVector<PluginExceptionData>::iterator iter;
    for(iter = this->m_p_exc_list->first.begin(); iter != this->m_p_exc_list->first.end(); iter++) {
        iter->object_uuid = this->m_p_metadata->uuid;
    }
    for(iter = this->m_p_exc_list->second.begin(); iter != this->m_p_exc_list->second.end(); iter++) {
        iter->object_uuid = this->m_p_metadata->uuid;
    }
    return this->m_p_exc_list;
}

void PluginObject::metadataCheck()
{
    if(this->m_p_metadata->id.isEmpty()) {
        // 异常：错误004-插件对象头文件元数据缺少标识符
        PluginExceptionData d;
        d.e = PLUGIN_EXC_ERR_004;
        d.object_uuid = this->m_p_metadata->uuid;
        this->m_p_exc_list->first.append(d);
    }
    if(this->m_p_metadata->caption.isEmpty()) {
        // 异常：警告001-插件对象头文件元数据缺少名称
        PluginExceptionData d;
        d.e = PLUGIN_EXC_WARN_001;
        d.object_uuid = this->m_p_metadata->uuid;
        this->m_p_exc_list->second.append(d);
    }
    if(this->m_p_metadata->description.isEmpty()) {
        // 异常：警告002-插件对象头文件元数据缺少介绍
        PluginExceptionData d;
        d.e = PLUGIN_EXC_WARN_002;
        d.object_uuid = this->m_p_metadata->uuid;
        this->m_p_exc_list->second.append(d);
    }
    if(this->m_p_metadata->author.isEmpty()) {
        // 异常：警告003-插件对象头文件元数据缺少作者
        PluginExceptionData d;
        d.e = PLUGIN_EXC_WARN_003;
        d.object_uuid = this->m_p_metadata->uuid;
        this->m_p_exc_list->second.append(d);
    }
    if(this->m_p_metadata->version.isEmpty()) {
        // 异常：警告004-插件对象头文件元数据缺少版本
        PluginExceptionData d;
        d.e = PLUGIN_EXC_WARN_004;
        d.object_uuid = this->m_p_metadata->uuid;
        this->m_p_exc_list->second.append(d);
    }
    if(this->m_p_metadata->uuid.isEmpty()) {
        // 创建uuid
        this->m_p_metadata->uuid = Tools::creatUuid();
        // 异常：警告005-插件对象头文件元数据缺少uuid
        PluginExceptionData d;
        d.e = PLUGIN_EXC_WARN_005;
        d.object_uuid = this->m_p_metadata->uuid;
        this->m_p_exc_list->second.append(d);
    }
}

PluginElement *PluginObject::readElement(QString path, ELEMENT_CONFIG_NAME_INDEX *configNameIndex, QSet<QString> *langList)
{
    // 读取元素头文件并转换为jsonObj
    QFile head(path);
    if(head.open(QIODevice::ReadOnly)) {// 文件是否打开:是
        QByteArray allData = head.readAll();
        head.close();

        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
        if(json_error.error != QJsonParseError::NoError) {
            // 异常：错误006-插件元素头文件JSON无法读取
            PluginElement *e = new PluginElement();
            e->m_p_metadata = new PluginElementMetadata;
            e->m_p_metadata->type = element_type_null;
            e->m_p_metadata->uuid16 = Tools::creatUuid16();
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_006;
            d.element_uuid = e->m_p_metadata->uuid16;
            this->m_p_exc_list->first.append(d);
            return e;
        }

        // 获取元素头文件元数据
        QJsonObject rootObj = jsonDoc.object();
        if(!rootObj.contains("metadata")) {
            // 异常：错误007-插件元素头文件缺少元数据
            PluginElement *e = new PluginElement();
            e->m_p_metadata = new PluginElementMetadata;
            e->m_p_metadata->type = element_type_null;
            e->m_p_metadata->uuid16 = Tools::creatUuid16();
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_007;
            d.element_uuid = e->m_p_metadata->uuid16;
            this->m_p_exc_list->first.append(d);
            return e;
        }
        QJsonObject metadataObj = rootObj.value("metadata").toObject();

        // 获取元素类型
        PLUGIN_ELEMENT_TYPE type = this->m_plugin_type_name.getType(metadataObj.value("type").toString());
        PluginElement *e;

        QString dirPath = QFileInfo(path).path();
        // 元素类型为本地化
        if(type == element_type_localisation) {
            e = new PluginElementLocalisation(langList);
            PLUGIN_EXC_LIST excList = *e->read(rootObj, path, dirPath, true);
            this->m_p_exc_list->first.append(excList.first);
            this->m_p_exc_list->second.append(excList.second);
            this->m_p_metadata->has_localisation = true;
        }
        // 元素为样式
        else if(type == element_type_style) {
            e = new PluginElementStyle();
            PLUGIN_EXC_LIST excList = *e->read(rootObj, path, dirPath, true);
            this->m_p_exc_list->first.append(excList.first);
            this->m_p_exc_list->second.append(excList.second);
            this->m_p_metadata->has_style = true;
        }
        // 元素为模型
        else if(type == element_type_model) {
            e = new PluginElementModel();
            PLUGIN_EXC_LIST excList = *e->read(rootObj, path, dirPath, true);
            this->m_p_exc_list->first.append(excList.first);
            this->m_p_exc_list->second.append(excList.second);
            this->m_p_metadata->has_model = true;
        }
        // 元素为配置
        else if(type == element_type_config) {
            e = new PluginElementConfig(configNameIndex);
            PLUGIN_EXC_LIST excList = *e->read(rootObj, path, dirPath, true);
            this->m_p_exc_list->first.append(excList.first);
            this->m_p_exc_list->second.append(excList.second);
            this->m_p_metadata->has_config = true;
        } else {
            // 异常：错误009-插件元素类型不存在
            e = new PluginElement();
            e->m_p_metadata = new PluginElementMetadata;
            e->m_p_metadata->type = element_type_null;
            e->m_p_metadata->uuid16 = Tools::creatUuid16();
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_009;
            d.element_uuid = e->m_p_metadata->uuid16;
            this->m_p_exc_list->first.append(d);
        }
        e->m_p_metadata->obj_uuid = this->m_p_metadata->uuid;
        return e;
    } else {// 文件是否打开:否
        // 异常：错误005-插件元素头文件无法读取
        PluginElement *e = new PluginElement();
        e->m_p_metadata = new PluginElementMetadata;
        e->m_p_metadata->type = element_type_null;
        e->m_p_metadata->uuid16 = Tools::creatUuid16();
        PluginExceptionData d;
        d.e = PLUGIN_EXC_ERR_005;
        d.element_uuid = e->m_p_metadata->uuid16;
        this->m_p_exc_list->first.append(d);
        return e;
    }
}
void PluginObject::clear()
{
    if(this->m_p_exc_list != nullptr) {
        delete this->m_p_exc_list;
        this->m_p_exc_list = nullptr;
    }
    if(this->m_p_element_list != nullptr) {
        qDeleteAll(this->m_p_element_list->begin(), this->m_p_element_list->end());
        this->m_p_element_list->clear();
        delete this->m_p_element_list;
        this->m_p_element_list = nullptr;
    }
    if(this->m_p_element_list != nullptr) {
        delete this->m_p_element_list;
        this->m_p_element_list = nullptr;
    }
    if(this->m_p_metadata != nullptr) {
        delete this->m_p_metadata;
        this->m_p_metadata = nullptr;
    }
    if(this->m_p_element_uuid_index != nullptr) {
        delete this->m_p_element_uuid_index;
        this->m_p_element_uuid_index = nullptr;
    }
}
