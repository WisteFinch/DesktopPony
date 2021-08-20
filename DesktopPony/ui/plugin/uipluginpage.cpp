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
    clearFilteredList();
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
    this->ui_plugin_page_search->setClearButtonEnabled(true);
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

    this->ui_plugin_page_filter = new UiPluginFilter(this);

    this->setLayout(this->ui_plugin_page_layout_main);
}

void UiPluginPage::initObjectName()
{
    this->setObjectName(QStringLiteral("ui_plugin_page"));
    // 部件部分
    this->ui_plugin_page_search->setObjectName(QStringLiteral("ui_plugin_page_search"));
    this->ui_plugin_page_search_filters->setObjectName(QStringLiteral("ui_plugin_page_search_filters"));
    this->ui_plugin_page_scrollarea->setObjectName(QStringLiteral("ui_plugin_page_scrollarea"));
    this->ui_plugin_page_list->setObjectName(QStringLiteral("ui_plugin_page_list"));
    this->ui_plugin_page_tools_reload->setObjectName(QStringLiteral("ui_plugin_page_tools_reload"));
    this->ui_plugin_page_tools_addnew->setObjectName(QStringLiteral("ui_plugin_page_tools_addnew"));
    this->ui_plugin_page_tools_delete->setObjectName(QStringLiteral("ui_plugin_page_tools_delete"));
    this->ui_plugin_page_tools_enable->setObjectName(QStringLiteral("ui_plugin_page_tools_enable"));
    this->ui_plugin_page_tools_disable->setObjectName(QStringLiteral("ui_plugin_page_tools_disable"));
    this->ui_plugin_page_tools_star->setObjectName(QStringLiteral("ui_plugin_page_tools_star"));
    // 布局部分
    this->ui_plugin_page_layout_main->setObjectName(QStringLiteral("ui_plugin_page_layout_main"));
    this->ui_plugin_page_layout_search->setObjectName(QStringLiteral("ui_plugin_page_layout_search"));
    this->ui_plugin_page_layout_list->setObjectName(QStringLiteral("ui_plugin_page_layout_list"));
    this->ui_plugin_page_layout_tools->setObjectName(QStringLiteral("ui_plugin_page_layout_tools"));

    this->ui_plugin_page_search_icon->setObjectName(QStringLiteral("ui_plugin_page_search_icon"));
}

void UiPluginPage::initProperty()
{
    this->setProperty("category", QStringLiteral("page"));
    // 部件部分
    this->ui_plugin_page_search->setProperty("category", QStringLiteral("plugin_page_search_line_edit"));
    this->ui_plugin_page_search_filters->setProperty("category", QStringLiteral("plugin_page_search_button"));
    this->ui_plugin_page_scrollarea->setProperty("category", QStringLiteral("plugin_page_scrollarea"));
    this->ui_plugin_page_list->setProperty("category", QStringLiteral("plugin_page_list_widget"));
    this->ui_plugin_page_tools_reload->setProperty("category", QStringLiteral("plugin_page_tools_button"));
    this->ui_plugin_page_tools_addnew->setProperty("category", QStringLiteral("plugin_page_tools_button"));
    this->ui_plugin_page_tools_delete->setProperty("category", QStringLiteral("plugin_page_tools_button"));
    this->ui_plugin_page_tools_enable->setProperty("category", QStringLiteral("plugin_page_tools_button"));
    this->ui_plugin_page_tools_disable->setProperty("category", QStringLiteral("plugin_page_tools_button"));
    this->ui_plugin_page_tools_star->setProperty("category", QStringLiteral("plugin_page_tools_button"));
}

void UiPluginPage::initContent()
{
    this->ui_plugin_page_search_icon->setIcon(QIcon(STR_IMG_PATH + QStringLiteral("search.svg")));
    this->ui_plugin_page_search->setPlaceholderText(this->m_p_text->getLoc(this->ui_plugin_page_search->objectName() + QStringLiteral("_cap")));
    this->ui_plugin_page_search_filters->setIcon(QIcon(STR_IMG_PATH + QStringLiteral("filter.svg")));
    this->ui_plugin_page_search_filters->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_search_filters->objectName() + QStringLiteral("_tip")));
    this->ui_plugin_page_tools_reload->setIcon(QIcon(STR_IMG_PATH + QStringLiteral("reload.svg")));
    this->ui_plugin_page_tools_reload->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_tools_reload->objectName() + QStringLiteral("_tip")));
    this->ui_plugin_page_tools_addnew->setIcon(QIcon(STR_IMG_PATH + QStringLiteral("plus.svg")));
    this->ui_plugin_page_tools_addnew->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_tools_addnew->objectName() + QStringLiteral("_tip")));
    this->ui_plugin_page_tools_delete->setIcon(QIcon(STR_IMG_PATH + QStringLiteral("delete.svg")));
    this->ui_plugin_page_tools_delete->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_tools_delete->objectName() + QStringLiteral("_tip")));
    this->ui_plugin_page_tools_enable->setIcon(QIcon(STR_IMG_PATH + QStringLiteral("tick.svg")));
    this->ui_plugin_page_tools_enable->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_tools_enable->objectName() + QStringLiteral("_tip")));
    this->ui_plugin_page_tools_disable->setIcon(QIcon(STR_IMG_PATH + QStringLiteral("fork.svg")));
    this->ui_plugin_page_tools_disable->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_tools_disable->objectName() + QStringLiteral("_tip")));
    this->ui_plugin_page_tools_star->setIcon(QIcon(STR_IMG_PATH + QStringLiteral("star.svg")));
    this->ui_plugin_page_tools_star->setToolTip(this->m_p_text->getLoc(this->ui_plugin_page_tools_star->objectName() + QStringLiteral("_tip")));

    this->ui_plugin_page_filter->init(this->m_p_text, this->ui_plugin_page_search_filters);
}

void UiPluginPage::initConnect()
{
    // 重载数据
    connect(this->ui_plugin_page_tools_reload, &QPushButton::clicked, this, &UiPluginPage::slotReolad);
    connect(this->ui_plugin_page_search_filters, &QPushButton::clicked, this->ui_plugin_page_filter, &UiPluginFilter::showFilter);
    connect(this->ui_plugin_page_filter, &UiPluginFilter::sigFilterChanged, this, &UiPluginPage::refreshFilteredList);
    connect(this->ui_plugin_page_search, &QLineEdit::textEdited, this, &UiPluginPage::refreshFilteredList);
    connect(this->ui_plugin_page_tools_star, &QPushButton::clicked, this, &UiPluginPage::slotSetFav);
    connect(this->ui_plugin_page_tools_enable, &QPushButton::clicked, this, [ = ] {slotSetEnable(true);});
    connect(this->ui_plugin_page_tools_disable, &QPushButton::clicked, this, [ = ] {slotSetEnable(false);});

}

void UiPluginPage::clearConnect()
{
    // 重载数据
    disconnect(this->ui_plugin_page_tools_reload, &QPushButton::clicked, this, &UiPluginPage::slotReolad);
    disconnect(this->ui_plugin_page_search_filters, &QPushButton::clicked, this->ui_plugin_page_filter, &UiPluginFilter::showFilter);
    disconnect(this->ui_plugin_page_filter, &UiPluginFilter::sigFilterChanged, this, &UiPluginPage::refreshFilteredList);
    disconnect(this->ui_plugin_page_search, &QLineEdit::textEdited, this, &UiPluginPage::refreshFilteredList);
    disconnect(this->ui_plugin_page_tools_star, &QPushButton::clicked, this, &UiPluginPage::slotSetFav);
    disconnect(this->ui_plugin_page_tools_enable, &QPushButton::clicked, this, nullptr);
    disconnect(this->ui_plugin_page_tools_disable, &QPushButton::clicked, this, nullptr);
}

void UiPluginPage::refreshList()
{
    disconnect(this->ui_plugin_page_filter, &UiPluginFilter::sigFilterChanged, this, &UiPluginPage::refreshFilteredList);
    clearList();
    clearFilteredList();
    this->ui_plugin_page_filter->reset();
    this->ui_plugin_page_search->clear();
    creatList();
    creatFilteredList();
    connect(this->ui_plugin_page_filter, &UiPluginFilter::sigFilterChanged, this, &UiPluginPage::refreshFilteredList);
}

void UiPluginPage::creatList()
{
    this->m_p_cards = new QVector<UiPluginListCard *>;
    OBJECT_LIST::iterator iter = this->m_p_plugin->m_p_plugin_obj_list->begin();
    qint32 index = 0;
    QList<QVariant> favList = this->m_p_conf->get(QStringLiteral("sys_fav_plugin")).toList();
    QList<QVariant> disabledList = this->m_p_conf->get(QStringLiteral("sys_fav_plugin")).toList();
    while(iter != this->m_p_plugin->m_p_plugin_obj_list->end()) {
        UiPluginListCard *card = new UiPluginListCard;
        bool isFav = favList.contains((*iter)->m_p_metadata->id);
        bool isDisabled = disabledList.contains((*iter)->m_p_metadata->id);
        card->init(this->m_p_text, *iter, index++, isFav, isDisabled);
        connect(card, &UiPluginListCard::clicked, this, &UiPluginPage::slotListIndexChange);
        this->m_p_cards->append(card);
        iter++;
    }
}

void UiPluginPage::clearList()
{
    if(this->m_p_cards != nullptr) {
        while(!this->m_p_cards->isEmpty()) {
            this->m_p_cards->last()->deleteLater();
            this->m_p_cards->pop_back();
        }
        delete this->m_p_cards;
        this->m_p_cards = nullptr;
    }
}

void UiPluginPage::creatFilteredList()
{
    sortList();
    this->m_p_filtered_cards = new QVector<UiPluginListCard *>;
    QVector<UiPluginListCard *>::iterator iter = this->m_p_sorted_cards->begin();
    while(iter != this->m_p_sorted_cards->end()) {
        if(filter(*iter)) {
            this->m_p_filtered_cards->append(*iter);
            this->ui_plugin_page_layout_list->addWidget(*iter);
        }
        iter++;
    }
    this->ui_plugin_page_layout_list->addStretch();
    if(this->m_p_filtered_cards->isEmpty() && !this->ui_plugin_page_search->text().isEmpty()) {
        this->ui_plugin_page_search->setProperty("no_match", true);
    }
    this->ui_plugin_page_search->setStyle(QApplication::style());
}

void UiPluginPage::clearFilteredList()
{
    if(this->m_p_filtered_cards != nullptr) {
        QLayoutItem *item;
        while ((item = this->ui_plugin_page_layout_list->takeAt(0)) != nullptr) {
            if(item->widget()) {
                item->widget()->setParent(nullptr);
            }
        }
        this->m_p_filtered_cards->clear();
        delete this->m_p_filtered_cards;
        this->m_p_filtered_cards = nullptr;
    }
    this->ui_plugin_page_search->setProperty("no_match", false);
    if(this->m_p_sorted_cards != nullptr) {
        delete this->m_p_sorted_cards;
        this->m_p_sorted_cards = nullptr;
    }
}

void UiPluginPage::refreshFilteredList()
{
    clearFilteredList();
    creatFilteredList();
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

    QMessageBox::StandardButton b = QMessageBox::warning(this, this->m_p_text->getLoc(QStringLiteral("ui_plugin_page_msgbox_reload_tit")), this->m_p_text->getLoc(QStringLiteral("ui_plugin_page_msgbox_reload_txt")), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(b == QMessageBox::Yes) {
        emit sigReloadData();
    }
}

bool UiPluginPage::filter(UiPluginListCard *card)
{
    bool flag = false;
    PluginObject *obj = card->m_p_obj;
    if(this->ui_plugin_page_filter->m_filter_loc && obj->m_p_metadata->has_localisation) {
        flag = true;
    } else if(this->ui_plugin_page_filter->m_filter_style && obj->m_p_metadata->has_style) {
        flag = true;
    } else if(this->ui_plugin_page_filter->m_filter_event && obj->m_p_metadata->has_event) {
        flag = true;
    } else if(this->ui_plugin_page_filter->m_filter_action && obj->m_p_metadata->has_action) {
        flag = true;
    } else if(this->ui_plugin_page_filter->m_filter_acc && obj->m_p_metadata->has_accessories) {
        flag = true;
    } else if(this->ui_plugin_page_filter->m_filter_model && obj->m_p_metadata->has_model) {
        flag = true;
    } else if(this->ui_plugin_page_filter->m_filter_conf && obj->m_p_metadata->has_config) {
        flag = true;
    } else if(this->ui_plugin_page_filter->m_filter_null &&
              !obj->m_p_metadata->has_localisation &&
              !obj->m_p_metadata->has_style &&
              !obj->m_p_metadata->has_event &&
              !obj->m_p_metadata->has_action &&
              !obj->m_p_metadata->has_accessories &&
              !obj->m_p_metadata->has_model &&
              !obj->m_p_metadata->has_config) {
        flag = true;
    }
    if(flag) {
        QString txt = this->ui_plugin_page_search->text().simplified();
        if(!txt.isEmpty()) {
            QRegularExpression re(txt);
            QRegularExpressionMatchIterator i = re.globalMatch(card->getAllText());
            if(!i.hasNext()) {
                flag = false;
            }
        }
    }
    return flag;
}

void UiPluginPage::sortList()
{
    this->m_p_sorted_cards = new QVector<UiPluginListCard *>;
    QList<QVariant> favList = this->m_p_conf->get(QStringLiteral("sys_fav_plugin")).toList();
    QList<QVariant> disabledList = this->m_p_conf->get(QStringLiteral("sys_disabled_plugin")).toList();
    QVector<UiPluginListCard *> favCards;
    QVector<UiPluginListCard *> cards;
    QVector<UiPluginListCard *> disabledCards;
    QVector<UiPluginListCard *>::iterator iter = this->m_p_cards->begin();
    while(iter != this->m_p_cards->end()) {
        if(favList.contains((*iter)->m_p_obj->m_p_metadata->id)) {
            favCards.append(*iter);
        } else if(disabledList.contains((*iter)->m_p_obj->m_p_metadata->id)) {
            disabledCards.append(*iter);
        } else {
            cards.append(*iter);
        }
        iter++;
    }
    this->m_p_sorted_cards->append(favCards);
    this->m_p_sorted_cards->append(cards);
    this->m_p_sorted_cards->append(disabledCards);
}

void UiPluginPage::slotSetFav()
{
    if(this->m_list_index != -1) {
        UiPluginListCard *card = this->m_p_cards->at(this->m_list_index);
        bool flag = card->m_is_fav;
        card->m_is_fav = ! flag;
        card->refreshStatus();
        QList<QVariant> vl = this->m_p_conf->get(QStringLiteral("sys_fav_plugin")).toList();
        if(flag) {
            vl.removeAll(card->m_p_obj->m_p_metadata->id);
        } else {
            vl.append(card->m_p_obj->m_p_metadata->id);
        }
        this->m_p_conf->set(QStringLiteral("sys_fav_plugin"), vl);
        refreshFilteredList();
    }
}

void UiPluginPage::slotSetEnable(bool enable)
{
    if(this->m_list_index != -1) {
        UiPluginListCard *card = this->m_p_cards->at(this->m_list_index);
        card->m_is_disabled = !enable;
        card->refreshStatus();
        QList<QVariant> vl = this->m_p_conf->get(QStringLiteral("sys_disabled_plugin")).toList();
        if(enable) {
            vl.removeAll(card->m_p_obj->m_p_metadata->id);
        } else {
            vl.append(card->m_p_obj->m_p_metadata->id);
        }
        this->m_p_conf->set(QStringLiteral("sys_disabled_plugin"), vl);
        refreshFilteredList();
    }
}
