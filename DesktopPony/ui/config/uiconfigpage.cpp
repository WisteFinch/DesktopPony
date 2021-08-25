#include "uiconfigpage.h"

UiConfigPage::UiConfigPage(QWidget *parent) :
    QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground);
}

UiConfigPage::~UiConfigPage()
{
    this->m_p_conf = nullptr;
    this->m_p_text = nullptr;
    this->m_p_plugin = nullptr;
    clearConnect();
    clearList();
}

void UiConfigPage::init(Config *ptrconf, Text *ptrText, PluginManager *ptrPluginManager, Config::PTRFUNC_GET_CONFIG ptrfuncGetConf)
{
    this->m_p_conf = ptrconf;
    this->m_p_text = ptrText;
    this->m_p_plugin = ptrPluginManager;
    this->m_ptrfunc_get_conf = ptrfuncGetConf;
    ptrconf = nullptr;
    ptrText = nullptr;
    ptrPluginManager = nullptr;

    initWidget();
    initObjectName();
    initProperty();

    initContent();
    initConnect();

    refreshList();
}

void UiConfigPage::initWidget()
{
    this->ui_layout_main = new QVBoxLayout;
    this->ui_layout_main->setSpacing(0);
    this->ui_layout_main->setMargin(0);
    this->ui_scrollarea = new QScrollArea;
    this->ui_scrollarea->setWidgetResizable(true);
    this->ui_layout_main->addWidget(this->ui_scrollarea);
    this->ui_scrollarea_widget = new QWidget;
    this->ui_layout_list = new QVBoxLayout;
    this->ui_layout_list->setSpacing(5);
    this->ui_layout_list->setContentsMargins(0, 0, 2, 0);
    this->ui_scrollarea_widget->setLayout(this->ui_layout_list);
    this->ui_scrollarea->setWidget(this->ui_scrollarea_widget);

    this->setLayout(this->ui_layout_main);
}

void UiConfigPage::initObjectName()
{
    this->setObjectName(QStringLiteral("ui_conf_page"));
    // 部件部分
    this->ui_scrollarea->setObjectName(QStringLiteral("ui_conf_page_scrollarea"));
    this->ui_scrollarea_widget->setObjectName(QStringLiteral("ui_conf_page_scrollarea_widget"));
    // 布局
    this->ui_scrollarea->setObjectName(QStringLiteral("ui_conf_page_scrollarea"));
    this->ui_layout_list->setObjectName(QStringLiteral("ui_conf_page_scrollarea"));
}

void UiConfigPage::initProperty()
{
    this->setProperty("category", QStringLiteral("page"));
    // 部件部分
    this->ui_scrollarea->setProperty("category", QStringLiteral("conf_page_scrollarea"));
    this->ui_scrollarea_widget->setProperty("category", QStringLiteral("conf_page_widget"));
}

void UiConfigPage::initContent()
{

}


void UiConfigPage::initConnect()
{

}

void UiConfigPage::clearConnect()
{

}

void UiConfigPage::creatList()
{
    this->m_p_items = new QVector<UiConfigItemCard *>;
    QMap<QString, Config::Item *> *list =  this->m_p_conf->getList();
    QMap<QString, QVector<QString>*> *index = this->m_p_conf->getCategoryIndex();
    QMap<QString, QVector<QString>*>::iterator indexIter = index->begin();
    while(indexIter != index->end()) {
        if(indexIter.key() == QStringLiteral("system")) {
            addItem(indexIter, list);
            break;
        }
        indexIter++;
    }
    indexIter = index->begin();
    while(indexIter != index->end()) {
        if(indexIter.key() != QStringLiteral("system")) {
            addItem(indexIter, list);
        }
        indexIter++;
    }
    this->ui_layout_list->addStretch();
}

void UiConfigPage::addItem(QMap<QString, QVector<QString>*>::iterator indexIter, QMap<QString, Config::Item *> *list)
{
    bool containItem = false;
    QVBoxLayout *categoryItemLayout = new QVBoxLayout;
    categoryItemLayout->setContentsMargins(5, 0, 5, 0);

    for(int i = 0; i < indexIter.value()->count(); i++) {
        Config::Item *item = list->value(indexIter.value()->at(i));
        if(item->info->type == PluginElementConfigData::config_type_null || (item->info->hidden && ! this->m_p_conf->get(QStringLiteral("sys_debug")).toBool())) {
            continue;
        }
        UiConfigItemCard *itemCard = new UiConfigItemCard;
        itemCard->init(this->m_p_text, item, this->m_ptrfunc_get_conf);
        this->m_p_items->append(itemCard);
        categoryItemLayout->addWidget(itemCard);
        connect(itemCard, &UiConfigItemCard::sigValueChanged, this, [ = ] {this->m_value_changed = true;});
        connect(itemCard, &UiConfigItemCard::sigShowRestart, this, [ = ] {emit sigShowRestart();});
        containItem = true;
    }

    if(containItem) {
        QWidget *categoryWidget = new QWidget;
        QVBoxLayout *categoryLayout = new QVBoxLayout;
        categoryLayout->setMargin(5);
        categoryLayout->setSpacing(5);
        QLabel *categoryCaption = new QLabel;
        categoryLayout->addWidget(categoryCaption);
        categoryLayout->addLayout(categoryItemLayout);
        categoryWidget->setLayout(categoryLayout);

        categoryWidget->setObjectName(QStringLiteral("ui_conf_page_category_widget"));
        categoryLayout->setObjectName(QStringLiteral("ui_conf_page_category_layout"));
        categoryItemLayout->setObjectName(QStringLiteral("ui_conf_page_category_item_layout"));
        categoryCaption->setObjectName(QStringLiteral("ui_conf_page_category_caption"));

        categoryWidget->setProperty("category", QStringLiteral("conf_page_category_widget"));
        categoryCaption->setProperty("category", QStringLiteral("conf_page_category_label"));

        categoryCaption->setText(this->m_p_text->getLoc("config_category_" + indexIter.key()));

        this->ui_layout_list->addWidget(categoryWidget);
    } else {
        delete categoryItemLayout;
    }
}

void UiConfigPage::clearList()
{
    if(this->m_p_items != nullptr) {
        while(!this->m_p_items->isEmpty()) {
            disconnect(this->m_p_items->last(), &UiConfigItemCard::sigValueChanged, this, nullptr);
            disconnect(this->m_p_items->last(), &UiConfigItemCard::sigShowRestart, this, nullptr);
            this->ui_layout_list->removeWidget(this->m_p_items->last());
            this->m_p_items->last()->setParent(nullptr);
            delete this->m_p_items->last();
            this->m_p_items->pop_back();
        }
        delete this->m_p_items;
        this->m_p_items = nullptr;
    }
}

void UiConfigPage::refreshList()
{
    clearList();
    creatList();
}
