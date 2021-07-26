#include "pluginelementmodel.h"

PluginElementModel::PluginElementModel()
{

}

PluginElementModel::~PluginElementModel()
{
    delete this->m_p_metadata;
    delete this->m_p_exc_list;
    delete this->m_p_data;
}

PLUGIN_EXC_LIST *PluginElementModel::read(QJsonObject obj, QString path, bool flag)
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
    this->m_p_data = new PluginElementModelData;

    //读取元数据
    readMetadata(obj.value("metadata").toObject());

    this->m_p_metadata->file_path = path;

    //读取内容
    QJsonObject contentObj = obj.value("content").toObject();
    QJsonArray layersArray = contentObj.value("layers").toArray();//读取图层数据
    QJsonArray imgCategoryArray = contentObj.value("img_categories").toArray();//读取图像类别数据
    QJsonArray unitsArray = contentObj.value("units").toArray();//读取部件数据

    PluginElementModelData::LAYERS layers;
    for(int i = 0; i < layersArray.count(); i++) {
        layers.append(layersArray.at(i).toString());
    }
    this->m_p_data->layers = layers;
    if(layers.isEmpty()) {
        // 异常：警告600-模型元素：缺少图层
        PluginExceptionData d;
        d.e = PLUGIN_EXC_WARN_600;
        this->m_p_exc_list->append(d);
    }

    PluginElementModelData::IMG_CATEGORIES imgCategories;
    for(int i = 0; i < imgCategoryArray.count(); i++) {
        imgCategories.append(imgCategoryArray.at(i).toString());
    }
    this->m_p_data->img_categories = imgCategories;
    if(imgCategories.isEmpty()) {
        // 异常：警告601-模型元素：缺少图像类别
        PluginExceptionData d;
        d.e = PLUGIN_EXC_WARN_601;
        this->m_p_exc_list->append(d);
    }

    PluginElementModelData::Units units;
    for(int i = 0; i < unitsArray.count(); i++) {
        QJsonObject unitObj = unitsArray.at(i).toObject();
        QJsonArray unitLayersArray = unitObj.value("layers").toArray();
        PluginElementModelData::Unit unit;
        unit.name = unitObj.value("name").toString();
        for(int j = 0; j < unitLayersArray.count(); j++) {
            unit.layers.append(unitLayersArray.at(j).toString());
        }
        units.append(unit);
    }
    this->m_p_data->units = units;
    if(units.isEmpty()) {
        // 异常：警告602-模型元素：缺少部件
        PluginExceptionData d;
        d.e = PLUGIN_EXC_WARN_602;
        this->m_p_exc_list->append(d);
    }

    PLUGIN_EXC_LIST::iterator iter;
    for(iter = this->m_p_exc_list->begin(); iter != this->m_p_exc_list->end(); iter++) {
        iter->element_uuid = this->m_p_metadata->uuid16;
    }
    return this->m_p_exc_list;
}
