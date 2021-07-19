#include "pluginobject.h"

PluginObject::PluginObject()
{

}

PluginObject::~PluginObject()
{
    clearElementList();
}

QVector<PLUGIN_ERROR> PluginObject::readHead(QString path, bool isSystem)
{
    // 清理
    clearElementList();
    this->m_p_element_list = new PLUGIN_ELEMENT_LIST;
    this->m_p_error = new QVector<PLUGIN_ERROR>;
    this->m_p_metadata = new PluginObjectMetadata;

    this->m_p_metadata->is_system = isSystem;

    // 读取头文件并转换为jsonObj
    QFile head(QDir(path).filePath(STR_PLUGIN_HEAD_PATH));
    if(head.open(QIODevice::ReadOnly)) {// 文件是否打开:是
        QByteArray allData = head.readAll();
        head.close();

        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
        if(json_error.error != QJsonParseError::NoError) {
            this->m_p_error->append(ERROR_002);
            return *this->m_p_error;
        }

        QJsonObject rootObj = jsonDoc.object();

        // 读取元数据
        if(rootObj.contains("metadata")) {  // 元数据是否存在:是
            QJsonObject metadataObj = rootObj.value("metadata").toObject();
            this->m_p_metadata->id = metadataObj.value("id").toString();
            if(this->m_p_metadata->id.isEmpty()) {
                this->m_p_error->append(ERROR_004);
                return *this->m_p_error;
            }
            this->m_p_metadata->uuid = metadataObj.value("uuid").toString();
            if(this->m_p_metadata->uuid.isEmpty()) {
                this->m_p_metadata->uuid = Tools::creatUuid();
            }
            this->m_p_metadata->caption = metadataObj.value("caption").toString();
            this->m_p_metadata->description = metadataObj.value("description").toString();
            this->m_p_metadata->author =  metadataObj.value("author").toString();
            this->m_p_metadata->version =  metadataObj.value("version").toString();
            this->m_p_metadata->remote_url =  metadataObj.value("remote_url").toString();
            if(!this->m_p_metadata->remote_url.isEmpty()) {
                this->m_p_metadata->is_remote = true;
            }
            metadataCheck();//元数据检查
        } else {    // 元数据是否存在:是
            this->m_p_error->append(ERROR_003);
            return *this->m_p_error;
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
                PluginElement *e = readElement(p);  // 读取元素

                //==========================读取错误信息BEGIN=============================
                if(e == nullptr) {

                }
                //==========================读取错误信息END=============================

                this->m_p_element_list->append(e);
            }
        }
    } else {// 文件是否打开:否
        this->m_p_error->append(ERROR_001);
        return *this->m_p_error;
    }
    return *this->m_p_error;
}

void PluginObject::metadataCheck()
{
    if(this->m_p_metadata->id.isEmpty()) {
        this->m_p_error->append(ERROR_004);
    }
    if(this->m_p_metadata->caption.isEmpty()) {
        this->m_p_error->append(WARN_001);
    }
    if(this->m_p_metadata->description.isEmpty()) {
        this->m_p_error->append(WARN_002);
    }
    if(this->m_p_metadata->author.isEmpty()) {
        this->m_p_error->append(WARN_003);
    }
    if(this->m_p_metadata->version.isEmpty()) {
        this->m_p_error->append(WARN_004);
    }
}

PluginElement *PluginObject::readElement(QString path)
{
    PluginElement *e = nullptr;

    // 读取元素头文件并转换为jsonObj
    QFile head(path);
    if(head.open(QIODevice::ReadOnly)) {// 文件是否打开:是
        QByteArray allData = head.readAll();
        head.close();

        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));
        if(json_error.error != QJsonParseError::NoError) {
            //>>>>>>>>>>>>>错误:JSON<<<<<<<<<<<<<
        }

        // 获取元素头文件元数据
        QJsonObject rootObj = jsonDoc.object();
        QJsonObject metadataObj = rootObj.value("metadata").toObject();
        //>>>>>>>>>>>>错误:元数据不存在<<<<<<<<<<<<<<<<<
        // 获取元素类型
        PLUGIN_ELEMENT_TYPE type = this->m_plugin_type_name.getType(metadataObj.value("type").toString());

        // 元素类型为本地化
        if(type == type_localisation) {
            e = new PluginElementLocalisation();
            e->read(rootObj);
        }
        // 元素为模型
        else if(type == type_model) {
            e = new PluginElementModel();
            e->read(rootObj);
        } else {    // 文件是否打开:否
            //>>>>>>>>>>错误:文件无法打开<<<<<<<<<<<<<<
        }
    }
    return e;
}
void PluginObject::clearElementList()
{
    if(this->m_p_element_list != nullptr) {
        while (!this->m_p_element_list->isEmpty()) {
            delete this->m_p_element_list->last();
            this->m_p_element_list->pop_back();
        }
    }
    delete this->m_p_element_list;
    delete this->m_p_error;
    delete this->m_p_metadata;
}
