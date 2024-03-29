#include "pluginmanager.h"

PluginManager::PluginManager()
{

}

PluginManager::~PluginManager()
{
    clear();
}

void PluginManager::refreshList()
{
    // 清理
    clear();
    this->m_p_plugin_obj_list = new OBJECT_LIST;
    this->m_p_plugin_obj_uuid_index = new OBJECT_UUID_INDEX;
    this->m_p_plugin_obj_type_index = new OBJECT_TYPE_INDEX;
    this->m_p_plugin_element_type_index = new ELEMENT_TYPE_INDEX;
    this->m_p_plugin_exc_list = new PLUGIN_EXC_LIST;
    this->m_p_plugin_element_config_name_index = new ELEMENT_CONFIG_NAME_INDEX;
    this->m_p_lang_list = new QSet<QString>;

    // 获取系统插件路径
    QDir sysRoot(STR_SYSTEM_PLUGIN_PATH);
    sysRoot.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks);
    sysRoot.setSorting(QDir::Name);
    QFileInfoList sysList = sysRoot.entryInfoList();
    // 遍历文件夹列表
    for (int i = 0; i < sysList.size(); ++i) {
        QFileInfo dir = sysList.at(i);
        // 跳过错误的路径
        if(dir.fileName() == "." || dir.fileName() == "..") {
            continue;
        }
        // 读取插件对象
        PluginObject *obj = new  PluginObject();
        this->m_p_plugin_obj_list->append(obj);
        PLUGIN_EXC_LIST excList =  *obj->readHead(sysList.at(i).filePath(), true, this->m_p_plugin_element_config_name_index, this->m_p_lang_list);
        this->m_p_plugin_exc_list->first.append(excList.first);
        this->m_p_plugin_exc_list->second.append(excList.second);
    }


    // 获取用户插件路径
    QDir usrRoot(STR_PLUGIN_PATH);
    usrRoot.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks);
    usrRoot.setSorting(QDir::Name);
    QFileInfoList usrList = usrRoot.entryInfoList();
    // 遍历文件夹列表
    for (int i = 0; i < usrList.size(); ++i) {
        QFileInfo dir = usrList.at(i);
        // 跳过错误的路径
        if(dir.fileName() == "." || dir.fileName() == "..") {
            continue;
        }
        // 读取插件对象
        PluginObject *obj = new  PluginObject();
        this->m_p_plugin_obj_list->append(obj);
        PLUGIN_EXC_LIST excList = *obj->readHead(usrList.at(i).filePath(), false, this->m_p_plugin_element_config_name_index, this->m_p_lang_list);
        this->m_p_plugin_exc_list->first.append(excList.first);
        this->m_p_plugin_exc_list->second.append(excList.second);
    }

    // 创建索引
    // 遍历对象列表
    OBJECT_LIST::iterator objIterator;
    for(objIterator = this->m_p_plugin_obj_list->begin(); objIterator != this->m_p_plugin_obj_list->end(); objIterator++) {
        PluginObject *obj = *objIterator;

        // 对象uuid去重
        if(this->m_p_plugin_obj_uuid_index->contains(obj->m_p_metadata->uuid)) {
            // 生成新uuid
            do {
                obj->m_p_metadata->uuid = Tools::creatUuid();
            } while(this->m_p_plugin_obj_uuid_index->contains(obj->m_p_metadata->uuid));
            if(!obj->m_p_metadata->orig_uuid.isEmpty()) {
                // 异常：警告007-uuid冲突
                PluginExceptionData d;
                d.e = PLUGIN_EXC_ERR_007;
                d.object_uuid = obj->m_p_metadata->uuid;
                this->m_p_plugin_exc_list->second.append(d);
            }
        }

        // 加入对象uuid索引
        this->m_p_plugin_obj_uuid_index->insert(obj->m_p_metadata->uuid, obj);
        PLUGIN_ELEMENT_LIST *elementList = obj->m_p_element_list;
        // 遍历元素列表
        PLUGIN_ELEMENT_LIST::iterator elementIterator;
        for(elementIterator = elementList->begin(); elementIterator != elementList->end(); elementIterator++) {
            PluginElement *element = *elementIterator;
            PLUGIN_ELEMENT_TYPE type = element->m_p_metadata->type;
            // 加入对象包含的元素类型索引
            if(!this->m_p_plugin_obj_type_index->contains(type)) {
                this->m_p_plugin_obj_type_index->insert(type, new QVector<QString>);
            }
            QVector<QString> *uuidVector = this->m_p_plugin_obj_type_index->value(element->m_p_metadata->type);
            uuidVector->append(obj->m_p_metadata->uuid);
            // 加入元素类型索引
            if(!this->m_p_plugin_element_type_index->contains(type)) {
                this->m_p_plugin_element_type_index->insert(type, new QVector<QPair<PluginElement *, QString>>);
            }
            ELEMENT_PAIR_LIST *pairVector = this->m_p_plugin_element_type_index->value(element->m_p_metadata->type);
            pairVector->append(QPair<PluginElement *, QString>(element, obj->m_p_metadata->uuid));

            element = nullptr;
        }
        obj = nullptr;
    }
}

void PluginManager::clear()
{
    // 清理异常
    if(this->m_p_plugin_exc_list != nullptr) {
        delete this->m_p_plugin_exc_list;
        this->m_p_plugin_exc_list = nullptr;
    }
    // 清理对象uuid索引
    if(this->m_p_plugin_obj_uuid_index != nullptr) {
        OBJECT_UUID_INDEX::iterator iter = this->m_p_plugin_obj_uuid_index->begin();
        while(iter != this->m_p_plugin_obj_uuid_index->end()) {
            iter.value() = nullptr;
            iter++;
        }
        this->m_p_plugin_obj_uuid_index->clear();
        delete this->m_p_plugin_obj_uuid_index;
        this->m_p_plugin_obj_uuid_index = nullptr;
    }
    // 清理对象包含的元素类型索引
    if(this->m_p_plugin_obj_type_index != nullptr) {
        OBJECT_TYPE_INDEX::iterator iter = this->m_p_plugin_obj_type_index->begin();
        while(iter != this->m_p_plugin_obj_type_index->end()) {
            delete iter.value();
            iter++;
        }
        this->m_p_plugin_obj_type_index->clear();
        delete this->m_p_plugin_obj_type_index;
        this->m_p_plugin_obj_type_index = nullptr;
    }
    // 清理元素类型索引
    if(this->m_p_plugin_element_type_index != nullptr) {
        ELEMENT_TYPE_INDEX::iterator rootIter = this->m_p_plugin_element_type_index->begin();
        while(rootIter != this->m_p_plugin_element_type_index->end()) {
            ELEMENT_PAIR_LIST::iterator subIter = rootIter.value()->begin();
            while(subIter != rootIter.value()->end()) {
                subIter->first = nullptr;
                subIter++;
            }
            delete rootIter.value();
            rootIter++;
        }
        this->m_p_plugin_element_type_index->clear();
        delete this->m_p_plugin_element_type_index;
        this->m_p_plugin_element_type_index = nullptr;
    }
    // 清理插件对象列表
    if(this->m_p_plugin_element_config_name_index != nullptr) {
        delete this->m_p_plugin_element_config_name_index;
        this->m_p_plugin_element_config_name_index = nullptr;
    }
    if(this->m_p_plugin_obj_list != nullptr) {
        qDeleteAll(this->m_p_plugin_obj_list->begin(), this->m_p_plugin_obj_list->end());
        this->m_p_plugin_obj_list->clear();
        delete this->m_p_plugin_obj_list;
        this->m_p_plugin_obj_list = nullptr;
    }
    if(this->m_p_lang_list != nullptr) {
        delete this->m_p_lang_list;
        this->m_p_lang_list = nullptr;
    }
}

ELEMENT_PAIR_LIST *PluginManager::getElementPairList(PLUGIN_ELEMENT_TYPE type)
{
    return this->m_p_plugin_element_type_index->value(type);
}

QSet<QString> *PluginManager::getLangList()
{
    return this->m_p_lang_list;
}

