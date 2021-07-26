#include "style.h"

Style::Style()
{

}

void Style::init(PTRFUNC_GET_ELEMENT_PAIR_LIST ptrfunc)
{
    this->ptrfun_get_element_pair_list = ptrfunc;
}

QString Style::getQSS()
{
    return this->qss;
}

void Style::setStyleName(QString n)
{
    this->m_s_style_name = n;
}

void Style::refreshStyle()
{
    ELEMENT_PAIR_LIST *rootList = this->ptrfun_get_element_pair_list(PLUGIN_ELEMENT_TYPE::element_type_style);
    ELEMENT_PAIR_LIST::Iterator rootIter;
    if(rootList != nullptr) {
        for(rootIter = rootList->begin(); rootIter != rootList->end(); rootIter++) {
            PluginElementStyle *element = static_cast<PluginElementStyle *>(rootIter->first);
            PluginElementStyleData *data = element->m_p_data;
            if(data->style_name == this->m_s_style_name && !data->isErr) {
                this->qss = data->qss;
            }
        }
    }
}
