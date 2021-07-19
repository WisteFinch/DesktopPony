#include "pluginmanager.h"

PluginManager::PluginManager()
{

}

PluginManager::~PluginManager()
{
    clearObjectList();
}

void PluginManager::refreshList()
{
    // 清理
    clearObjectList();
    this->m_p_plugin_obj_list = new OBJECT_LIST;
    this->m_p_plugin_obj_uuid_index = new OBJECT_UUID_INDEX;
    this->m_p_plugin_obj_type_index = new OBJECT_TYPE_INDEX;
    this->m_p_plugin_element_type_index = new ELEMENT_TYPE_INDEX;

    // 获取系统插件路径
    QDir sysRoot(STR_SYSTEM_PLUGIN_PATH);
    sysRoot.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks);
    sysRoot.setSorting(QDir::Name);
    QFileInfoList sysList = sysRoot.entryInfoList();
    // 遍历文件夹列表
    for (int i = 0; i < sysList.size(); ++i) {
        QFileInfo dir = sysList.at(i);
        if(dir.fileName() == "." || dir.fileName() == "..") { // 跳过错误的路径
            continue;
        }
        // 读取插件对象
        PluginObject *obj = new  PluginObject();
        this->m_p_plugin_obj_list->append(obj);
        QVector<PLUGIN_ERROR> error = obj->readHead(sysList.at(i).filePath(), true);
        if(!error.isEmpty()) {
            if(error.first() == ERROR_001 || error.first() == ERROR_002) {
                // 消息
            }
        }
    }


    // 获取用户插件路径
    QDir usrRoot(STR_PLUGIN_PATH);
    usrRoot.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks);
    usrRoot.setSorting(QDir::Name);
    QFileInfoList usrList = usrRoot.entryInfoList();
    // 遍历文件夹列表
    for (int i = 0; i < usrList.size(); ++i) {
        QFileInfo dir = usrList.at(i);
        if(dir.fileName() == "." || dir.fileName() == "..") { // 跳过错误的路径
            continue;
        }
        // 读取插件对象
        PluginObject *obj = new  PluginObject();
        this->m_p_plugin_obj_list->append(obj);
        QVector<PLUGIN_ERROR> error = obj->readHead(usrList.at(i).filePath(), false);
        if(!error.isEmpty()) {
            if(error.first() == ERROR_001 || error.first() == ERROR_002) {
                // 消息
            }
        }
    }


    creatIndex();
}

void PluginManager::clearObjectList()
{
    if(this->m_p_plugin_obj_uuid_index != nullptr) {
        QList<PluginObject *> vl = this->m_p_plugin_obj_uuid_index->values();
        while(!vl.isEmpty()) {
            vl.last() = nullptr;
            vl.pop_back();
        }
        delete this->m_p_plugin_obj_uuid_index;
        this->m_p_plugin_obj_uuid_index = nullptr;
    }

    if(this->m_p_plugin_obj_type_index != nullptr) {
        QList<QVector<QString>*> vl = this->m_p_plugin_obj_type_index->values();
        while(!vl.isEmpty()) {
            delete vl.last();
            vl.pop_back();
        }
        delete this->m_p_plugin_obj_type_index;
        this->m_p_plugin_obj_type_index = nullptr;
    }

    if(this->m_p_plugin_element_type_index != nullptr) {
        QList<ELEMENT_PAIR_LIST *> vl = this->m_p_plugin_element_type_index->values();
        while(!vl.isEmpty()) {
            ELEMENT_PAIR_LIST *v = vl.last();
            while(!v->isEmpty()) {
                v->last().first = nullptr;
                v->pop_back();
            }
            delete v;
            vl.pop_back();
        }
        delete this->m_p_plugin_element_type_index;
        this->m_p_plugin_element_type_index = nullptr;
    }

    if(this->m_p_plugin_obj_list != nullptr) {
        while(!this->m_p_plugin_obj_list->isEmpty()) {
            delete this->m_p_plugin_obj_list->last();
            this->m_p_plugin_obj_list->pop_back();
        }
        delete this->m_p_plugin_obj_list;
        this->m_p_plugin_obj_list = nullptr;
    }
}

void PluginManager::creatIndex()
{
    OBJECT_LIST::iterator objIterator;
    PluginObject *obj = nullptr;
    for(objIterator = this->m_p_plugin_obj_list->begin(); objIterator != this->m_p_plugin_obj_list->end(); objIterator++) {
        obj = *objIterator;
        this->m_p_plugin_obj_uuid_index->insert(obj->m_p_metadata->uuid, obj);
        PLUGIN_ELEMENT_LIST *elementList = obj->m_p_element_list;
        PLUGIN_ELEMENT_LIST::iterator elementIterator;
        for(elementIterator = elementList->begin(); elementIterator != elementList->end(); elementIterator++) {
            PluginElement *element = *elementIterator;
            PLUGIN_ELEMENT_TYPE type = element->m_metadata.type;

            if(!this->m_p_plugin_obj_type_index->contains(type)) {
                this->m_p_plugin_obj_type_index->insert(type, new QVector<QString>);
            }
            QVector<QString> *uuidVector = this->m_p_plugin_obj_type_index->value(element->m_metadata.type);
            uuidVector->append(obj->m_p_metadata->uuid);

            if(!this->m_p_plugin_element_type_index->contains(type)) {
                this->m_p_plugin_element_type_index->insert(type, new QVector<QPair<PluginElement *, QString>>);
            }
            ELEMENT_PAIR_LIST *pairVector = this->m_p_plugin_element_type_index->value(element->m_metadata.type);
            pairVector->append(QPair<PluginElement *, QString>(element, obj->m_p_metadata->uuid));
        }
        obj = nullptr;
    }
}

ELEMENT_PAIR_LIST *PluginManager::getElementPairList(PLUGIN_ELEMENT_TYPE type)
{
    return this->m_p_plugin_element_type_index->value(type);
}

