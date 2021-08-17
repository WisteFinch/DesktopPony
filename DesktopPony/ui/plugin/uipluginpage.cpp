#include "uipluginpage.h"

UiPluginPage::UiPluginPage(QWidget *parent) :
    QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground);
}

UiPluginPage::~UiPluginPage()
{
    this->m_p_conf = nullptr;
    this->m_p_text = nullptr;
    this->m_p_plugin = nullptr;
    clearConnect();
    clearList();
}

void UiPluginPage::init(Config *ptrconf, Text *ptrText, PluginManager *ptrPluginManager)
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
    initContent();
    initConnect();

    refreshList();
}

void UiPluginPage::initWidget()
{
    this->ui_plugin_page_layout_main = new QVBoxLayout;
    this->ui_plugin_page_layout_main->setSpacing(5);
    this->ui_plugin_page_layout_main->setMargin(0);
    // 搜索
    this->ui_plugin_page_layout_search = new QHBoxLayout;
    this->ui_plugin_page_layout_search->setSpacing(0);
    this->ui_plugin_page_search_icon = new QAction;
    this->ui_plugin_page_search  = new QLineEdit;
    this->ui_plugin_page_search_filters = new QPushButton;
    this->ui_plugin_page_search->addAction(this->ui_plugin_page_search_icon, QLineEdit::LeadingPosition);
    this->ui_plugin_page_layout_search->addWidget(this->ui_plugin_page_search);
    this->ui_plugin_page_layout_search->addWidget(new VLine);
    this->ui_plugin_page_layout_search->addWidget(this->ui_plugin_page_search_filters);
    this->ui_plugin_page_layout_main->addLayout(ui_plugin_page_layout_search);
    // 列表
    this->ui_plugin_page_scrollarea = new QScrollArea;
    this->ui_plugin_page_scrollarea->setWidgetResizable(true);
    this->ui_plugin_page_list = new QWidget;
    this->ui_plugin_page_layout_list = new QVBoxLayout;
    this->ui_plugin_page_layout_list->setSpacing(3);
    this->ui_plugin_page_layout_list->setMargin(0);
    this->ui_plugin_page_layout_list->setContentsMargins(0, 0, 2, 0);
    this->ui_plugin_page_list->setLayout(this->ui_plugin_page_layout_list);
    this->ui_plugin_page_scrollarea->setWidget(this->ui_plugin_page_list);
    this->ui_plugin_page_layout_main->addWidget(this->ui_plugin_page_scrollarea);
    // 工具
    this->ui_plugin_page_layout_tools = new QHBoxLayout;
    this->ui_plugin_page_layout_tools->setSpacing(0);
    this->ui_plugin_page_layout_tools->setMargin(0);
    this->ui_plugin_page_tools_reload = new QPushButton;
    this->ui_plugin_page_tools_addnew = new QPushButton;
    this->ui_plugin_page_tools_delete = new QPushButton;
    this->ui_plugin_page_tools_enable = new QPushButton;
    this->ui_plugin_page_tools_disable = new QPushButton;
    this->ui_plugin_page_tools_star = new QPushButton;
    this->ui_plugin_page_layout_tools->addWidget(this->ui_plugin_page_tools_reload);
    this->ui_plugin_page_layout_tools->addWidget(new VLine);
    this->ui_plugin_page_layout_tools->addWidget(this->ui_plugin_page_tools_addnew);
    this->ui_plugin_page_layout_tools->addWidget(this->ui_plugin_page_tools_delete);
    this->ui_plugin_page_layout_tools->addWidget(new VLine);
    this->ui_plugin_page_layout_tools->addWidget(this->ui_plugin_page_tools_enable);
    this->ui_plugin_page_layout_tools->addWidget(this->ui_plugin_page_tools_disable);
    this->ui_plugin_page_layout_tools->addWidget(this->ui_plugin_page_tools_star);
    this->ui_plugin_page_layout_tools->addStretch();
    this->ui_plugin_page_layout_main->addLayout(this->ui_plugin_page_layout_tools);

    this->setLayout(this->ui_plugin_page_layout_main);
}

void UiPluginPage::initObjectName()
{
    this->setObjectName("ui_plugin_page");
    // 部件部分
    this->ui_plugin_page_search->setObjectName("ui_plugin_page_search");
    this->ui_plugin_page_search_filters->setObjectName("ui_plugin_page_search_filters");
    this->ui_plugin_page_scrollarea->setObjectName("ui_plugin_page_scrollarea");
    this->ui_plugin_page_list->setObjectName("ui_plugin_page_list");
    this->ui_plugin_page_tools_reload->setObjectName("ui_plugin_page_tools_reload");
    this->ui_plugin_page_tools_addnew->setObjectName("ui_plugin_page_tools_addnew");
    this->ui_plugin_page_tools_delete->setObjectName("ui_plugin_page_tools_delete");
    this->ui_plugin_page_tools_enable->setObjectName("ui_plugin_page_tools_enable");
    this->ui_plugin_page_tools_disable->setObjectName("ui_plugin_page_tools_disable");
    this->ui_plugin_page_tools_star->setObjectName("ui_plugin_page_tools_star");
    // 布局部分
    this->ui_plugin_page_layout_main->setObjectName("ui_plugin_page_layout_main");
    this->ui_plugin_page_layout_search->setObjectName("ui_plugin_page_layout_search");
    this->ui_plugin_page_layout_list->setObjectName("ui_plugin_page_layout_list");
    this->ui_plugin_page_layout_tools->setObjectName("ui_plugin_page_layout_tools");

    this->ui_plugin_page_search_icon->setObjectName("ui_plugin_page_search_icon");
}

void UiPluginPage::initProperty()
{
    this->setProperty("category", "page");
    // 部件部分
    this->ui_plugin_page_search->setProperty("category", "plugin_page_search_line_edit");
    this->ui_plugin_page_search_filters->setProperty("category", "plugin_page_search_button");
    this->ui_plugin_page_scrollarea->setProperty("category", "plugin_page_scrollarea");
    this->ui_plugin_page_list->setProperty("category", "plugin_page_list_widget");
    this->ui_plugin_page_tools_reload->setProperty("category", "plugin_page_tools_button");
    this->ui_plugin_page_tools_addnew->setProperty("category", "plugin_page_tools_button");
    this->ui_plugin_page_tools_delete->setProperty("category", "plugin_page_tools_button");
    this->ui_plugin_page_tools_enable->setProperty("category", "plugin_page_tools_button");
    this->ui_plugin_page_tools_disable->setProperty("category", "plugin_page_tools_button");
    this->ui_plugin_page_tools_star->setProperty("category", "plugin_page_tools_button");
}

void UiPluginPage::initContent()
{
    this->ui_plugin_page_search_icon->setIcon(QIcon(STR_IMG_PATH + "search.svg"));
    this->ui_plugin_page_search->setPlaceholderText(this->m_p_text->getLoc(this->ui_plugin_page_search->objectName() + "_cap"));
    this->ui_plugin_page_search_filters->setIcon(QIcon(STR_IMG_PATH + "filter.svg"));
    this->ui_plugin_page_search_filters->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_search_filters->objectName() + "_tip"));
    this->ui_plugin_page_tools_reload->setIcon(QIcon(STR_IMG_PATH + "reload.svg"));
    this->ui_plugin_page_tools_reload->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_tools_reload->objectName() + "_tip"));
    this->ui_plugin_page_tools_addnew->setIcon(QIcon(STR_IMG_PATH + "plus.svg"));
    this->ui_plugin_page_tools_addnew->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_tools_addnew->objectName() + "_tip"));
    this->ui_plugin_page_tools_delete->setIcon(QIcon(STR_IMG_PATH + "delete.svg"));
    this->ui_plugin_page_tools_delete->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_tools_delete->objectName() + "_tip"));
    this->ui_plugin_page_tools_enable->setIcon(QIcon(STR_IMG_PATH + "tick.svg"));
    this->ui_plugin_page_tools_enable->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_tools_enable->objectName() + "_tip"));
    this->ui_plugin_page_tools_disable->setIcon(QIcon(STR_IMG_PATH + "fork.svg"));
    this->ui_plugin_page_tools_disable->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_tools_disable->objectName() + "_tip"));
    this->ui_plugin_page_tools_star->setIcon(QIcon(STR_IMG_PATH + "star.svg"));
    this->ui_plugin_page_tools_star->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_tools_star->objectName() + "_tip"));
}

void UiPluginPage::initConnect()
{
    // 重载数据
    connect(this->ui_plugin_page_tools_reload, &QPushButton::clicked, this, &UiPluginPage::slotReolad);
}

void UiPluginPage::clearConnect()
{
    // 重载数据
    disconnect(this->ui_plugin_page_tools_reload, &QPushButton::clicked, this, &UiPluginPage::slotReolad);
}

void UiPluginPage::refreshList()
{
    clearList();
    creatList();
}

void UiPluginPage::creatList()
{
    this->m_p_cards = new QVector<UiPluginListCard *>;
    OBJECT_LIST::iterator iter = this->m_p_plugin->m_p_plugin_obj_list->begin();
    qint32 index = 0;
    while(iter != this->m_p_plugin->m_p_plugin_obj_list->end()) {
        UiPluginListCard *card = new UiPluginListCard;
        card->init(this->m_p_text, *iter, index);
        this->m_p_cards->append(card);
        this->ui_plugin_page_layout_list->addWidget(card);
        connect(card, &UiPluginListCard::clicked, this, &UiPluginPage::slotListIndexChange);
        iter++;
        index++;
    }
    this->ui_plugin_page_layout_list->addStretch();
}

void UiPluginPage::clearList()
{
    if(this->m_p_cards != nullptr) {
        while(!this->m_p_cards->isEmpty()) {
            disconnect(this->m_p_cards->last(), &UiPluginListCard::clicked, this, &UiPluginPage::slotListIndexChange);
            this->ui_plugin_page_layout_list->removeWidget(this->m_p_cards->last());
            this->m_p_cards->last()->setParent(nullptr);
            delete this->m_p_cards->last();
            this->m_p_cards->pop_back();
        }
        delete this->m_p_cards;
        this->m_p_cards = nullptr;
    }
}

void UiPluginPage::slotListIndexChange(qint32 index)
{
    if(this->m_list_index != -1 && this->m_list_index < this->m_p_cards->count()) {
        this->m_p_cards->at(this->m_list_index)->setSelected(false);
    }
    if(index != -1 && index < this->m_p_cards->count()) {
        this->m_p_cards->at(index)->setSelected();
        this->m_list_index = index;
    } else {
        this->m_list_index = -1;
    }
}

void UiPluginPage::slotReolad()
{

    QMessageBox::StandardButton b = QMessageBox::warning(this, this->m_p_text->getLoc("ui_plugin_page_msgbox_reload_tit"), this->m_p_text->getLoc("ui_plugin_page_msgbox_reload_txt"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(b == QMessageBox::Yes) {
        emit sigReloadData();
    }
}
