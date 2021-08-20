#include "pluginelementstyle.h"

PluginElementStyle::PluginElementStyle()
{

}

PluginElementStyle::~PluginElementStyle()
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

PLUGIN_EXC_LIST *PluginElementStyle::read(QJsonObject obj, QString filePath, QString dirPath, bool flag)
{
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
    this->m_p_data = new PluginElementStyleData;

    //读取元数据
    readMetadata(obj.value("metadata").toObject());
    this->m_p_metadata->file_path = filePath;
    this->m_p_metadata->dir_path = dirPath;

    //读取内容
    QJsonObject contentObj = obj.value("content").toObject();
    this->m_p_data->style_name = contentObj.value("style_name").toString();
    if(this->m_p_data->style_name.isEmpty()) {
        // 异常：错误200-样式元素：缺少样式名称
        PluginExceptionData d;
        d.e = PLUGIN_EXC_ERR_200;
        d.element_uuid = this->m_p_metadata->uuid16;
        this->m_p_exc_list->first.append(d);
        return this->m_p_exc_list;
    }
    this->m_p_data->qss_path = contentObj.value("qss_path").toString();
    if(!this->m_p_data->qss_path.isEmpty()) {
        QFile file(QDir(dirPath).filePath(this->m_p_data->qss_path));
        if(file.open(QIODevice::ReadOnly)) {// 文件是否打开:是
            this->m_p_data->qss = file.readAll();
        } else {
            // 异常：错误201-样式元素：qss文件无法读取
            PluginExceptionData d;
            d.e = PLUGIN_EXC_ERR_201;
            d.element_uuid = this->m_p_metadata->uuid16;
            this->m_p_exc_list->first.append(d);
            return this->m_p_exc_list;
        }
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
