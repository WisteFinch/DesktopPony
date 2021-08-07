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
    //clearConnect();
    //clearList();
}

void UiConfigPage::init(Config *ptrconf, Text *ptrText, PluginManager *ptrPluginManager)
{
    this->m_p_conf = ptrconf;
    this->m_p_text = ptrText;
    this->m_p_plugin = ptrPluginManager;
    ptrconf = nullptr;
    ptrText = nullptr;
    ptrPluginManager = nullptr;
    initWidget();
    initObjectName();
    initProperty();
    /*
    initContent();
    initConnect();

    refreshList();
    */
}

void UiConfigPage::initWidget()
{
    this->ui_conf_page_layout_main = new QVBoxLayout;
    this->ui_conf_page_layout_main->setSpacing(0);
    this->ui_conf_page_layout_main->setMargin(0);
    this->ui_conf_page_scrollarea = new QScrollArea;
    this->ui_conf_page_layout_main->addWidget(this->ui_conf_page_scrollarea);
    this->ui_conf_page_layout_list = new QVBoxLayout;
    this->ui_conf_page_layout_list->setSpacing(0);
    this->ui_conf_page_layout_list->setContentsMargins(0, 0, 2, 0);
    this->ui_conf_page_scrollarea->setLayout(this->ui_conf_page_layout_list);

    this->setLayout(this->ui_conf_page_layout_main);
}

void UiConfigPage::initObjectName()
{
    this->setObjectName("ui_conf_page");
    // 部件部分
    this->ui_conf_page_scrollarea->setObjectName("ui_conf_page_scrollarea");
    // 布局
    this->ui_conf_page_scrollarea->setObjectName("ui_conf_page_scrollarea");
    this->ui_conf_page_layout_list->setObjectName("ui_conf_page_scrollarea");
}

void UiConfigPage::initProperty()
{
    this->setProperty("category", "page");
    // 部件部分
    this->ui_conf_page_scrollarea->setProperty("category", "plugin_info_scrollarea");
}
