#include "pluginelementmodel.h"

PluginElementModel::PluginElementModel()
{

}

PluginElementModel::~PluginElementModel()
{

}

void PluginElementModel::read(QJsonObject obj)
{
    if(this->m_p_data != nullptr) {
        delete this->m_p_data;
    }
    this->m_p_data = new PluginElementModelData;

    //读取元数据
    readMetadata(obj.value("metadata").toObject());

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

    PluginElementModelData::IMG_CATEGORIES imgCategories;
    for(int i = 0; i < imgCategoryArray.count(); i++) {
        imgCategories.append(imgCategoryArray.at(i).toString());
    }
    this->m_p_data->img_categories = imgCategories;

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
}
