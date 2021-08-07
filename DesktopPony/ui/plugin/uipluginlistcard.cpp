#include "uipluginlistcard.h"
#include "ui_uipluginlistcard.h"

UiPluginListCard::UiPluginListCard(QWidget *parent) :
    QWidget(parent)
{
}

UiPluginListCard::~UiPluginListCard()
{
    this->m_p_text = nullptr;
    this->m_p_obj = nullptr;

    while(!this->ui_plugin_list_card_elements->isEmpty()) {
        delete this->ui_plugin_list_card_elements->last();
        this->ui_plugin_list_card_elements->pop_back();
    }
    delete this->ui_plugin_list_card_elements;
}

void UiPluginListCard::init(Text *ptrText, PluginObject *ptrObj, qint32 index)
{
    this->m_p_text = ptrText;
    this->m_p_obj = ptrObj;
    ptrText = nullptr;
    ptrObj = nullptr;

    this->m_index = index;

    this->setAttribute(Qt::WA_StyledBackground);
    initWidget();
    initObjectName();
    initProperty();
    initContent();
    initConnect();
    initElementsIcon();
}

void UiPluginListCard::setSelected(bool flag)
{
    if(flag) {
        this->setProperty("selected", true);
    } else {
        this->setProperty("selected", false);
    }
    this->setStyle(QApplication::style());
}

void UiPluginListCard::initWidget()
{
    this->ui_plugin_list_card_layout_main = new QHBoxLayout;
    this->ui_plugin_list_card_layout_main->setSpacing(5);
    this->ui_plugin_list_card_layout_main->setMargin(5);
    // 图标
    this->ui_plugin_list_card_icon = new QLabel;
    this->ui_plugin_list_card_icon->setScaledContents(true);
    this->ui_plugin_list_card_layout_main->addWidget(this->ui_plugin_list_card_icon);
    // 标题
    this->ui_plugin_list_card_layout_content = new QVBoxLayout;
    this->ui_plugin_list_card_layout_content->setSpacing(0);
    this->ui_plugin_list_card_layout_title = new QHBoxLayout;
    this->ui_plugin_list_card_layout_title->setSpacing(5);
    this->ui_plugin_list_card_caption = new QLabel;
    this->ui_plugin_list_card_author = new QLabel;
    this->ui_plugin_list_card_version = new QLabel;
    this->ui_plugin_list_card_layout_title->addWidget(this->ui_plugin_list_card_caption);
    this->ui_plugin_list_card_layout_title->addWidget(this->ui_plugin_list_card_author);
    this->ui_plugin_list_card_layout_title->addWidget(this->ui_plugin_list_card_version);
    this->ui_plugin_list_card_layout_title->addStretch();
    this->ui_plugin_list_card_layout_content->addLayout(this->ui_plugin_list_card_layout_title);
    // 描述
    this->ui_plugin_list_card_desc = new QLabel;
    this->ui_plugin_list_card_layout_content->addWidget(this->ui_plugin_list_card_desc);
    this->ui_plugin_list_card_layout_content->addStretch();
    // 元素
    this->ui_plugin_list_card_layout_elements = new QHBoxLayout;
    this->ui_plugin_list_card_layout_elements->setSpacing(0);

    this->ui_plugin_list_card_layout_content->addLayout(this->ui_plugin_list_card_layout_elements);

    this->ui_plugin_list_card_layout_main->addLayout(this->ui_plugin_list_card_layout_content);
    this->ui_plugin_list_card_layout_main->addStretch();
    this->setLayout(this->ui_plugin_list_card_layout_main);
}

void UiPluginListCard::initObjectName()
{
    this->setObjectName("ui_plugin_list_card");
    // 部件部分
    this->ui_plugin_list_card_icon->setObjectName("ui_plugin_list_card_icon");
    this->ui_plugin_list_card_caption->setObjectName("ui_plugin_list_card_caption");
    this->ui_plugin_list_card_author->setObjectName("ui_plugin_list_card_author");
    this->ui_plugin_list_card_version->setObjectName("ui_plugin_list_card_version");
    this->ui_plugin_list_card_desc->setObjectName("ui_plugin_list_card_desc");
    // 布局部分
    this->ui_plugin_list_card_layout_main->setObjectName("ui_plugin_list_card_layout_main");
    this->ui_plugin_list_card_layout_content->setObjectName("ui_plugin_list_card_layout_content");
    this->ui_plugin_list_card_layout_title->setObjectName("ui_plugin_list_card_layout_title");
    this->ui_plugin_list_card_layout_elements->setObjectName("ui_plugin_list_card_layout_elements");
}

void UiPluginListCard::initProperty()
{
    this->setProperty("category", "plugin_list_card");
    this->setProperty("selected", false);
    // 部件部分
    this->ui_plugin_list_card_icon->setProperty("category", "plugin_list_card_label");
    this->ui_plugin_list_card_caption->setProperty("category", "plugin_list_card_label");
    this->ui_plugin_list_card_author->setProperty("category", "plugin_list_card_label");
    this->ui_plugin_list_card_version->setProperty("category", "plugin_list_card_label");
    this->ui_plugin_list_card_desc->setProperty("category", "plugin_list_card_label");
}

void UiPluginListCard::initContent()
{
    QString objUuid = this->m_p_obj->m_p_metadata->uuid;
    this->ui_plugin_list_card_icon->setPixmap(QPixmap(QDir(this->m_p_obj->m_p_metadata->dir_path).filePath(this->m_p_obj->m_p_metadata->icon)));
    this->ui_plugin_list_card_caption->setText(this->m_p_text->get(this->m_p_obj->m_p_metadata->caption, objUuid));
    this->ui_plugin_list_card_author->setText(this->m_p_text->get(this->m_p_obj->m_p_metadata->author, objUuid));
    this->ui_plugin_list_card_version->setText(this->m_p_text->get(this->m_p_obj->m_p_metadata->version, objUuid));
    this->ui_plugin_list_card_desc->setText(this->m_p_text->get(this->m_p_obj->m_p_metadata->description, objUuid));
}

void UiPluginListCard::initConnect()
{

}

void UiPluginListCard::clearConnect()
{

}

void UiPluginListCard::initElementsIcon()
{
    this->ui_plugin_list_card_elements = new QVector<QLabel *>;
    if(this->m_p_obj->m_p_metadata->has_localisation) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + "localisation.svg"));
        l->setToolTip(this->m_p_text->getLoc("plugin_localisation"));
        this->ui_plugin_list_card_elements->append(l);
        this->ui_plugin_list_card_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_style) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + "style.svg"));
        l->setToolTip(this->m_p_text->getLoc("plugin_style"));
        this->ui_plugin_list_card_elements->append(l);
        this->ui_plugin_list_card_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_event) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + "script.svg"));
        l->setToolTip(this->m_p_text->getLoc("plugin_event"));
        this->ui_plugin_list_card_elements->append(l);
        this->ui_plugin_list_card_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_action) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + "action.svg"));
        l->setToolTip(this->m_p_text->getLoc("plugin_action"));
        this->ui_plugin_list_card_elements->append(l);
        this->ui_plugin_list_card_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_accessories) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + "accessories.svg"));
        l->setToolTip(this->m_p_text->getLoc("plugin_accessories"));
        this->ui_plugin_list_card_elements->append(l);
        this->ui_plugin_list_card_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_model) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + "model.svg"));
        l->setToolTip(this->m_p_text->getLoc("plugin_model"));
        this->ui_plugin_list_card_elements->append(l);
        this->ui_plugin_list_card_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_config) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + "config.svg"));
        l->setToolTip(this->m_p_text->getLoc("plugin_config"));
        this->ui_plugin_list_card_elements->append(l);
        this->ui_plugin_list_card_layout_elements->addWidget(l);
    }
    this->ui_plugin_list_card_layout_elements->addStretch();
}

QLabel *UiPluginListCard::getElementIcon()
{
    QLabel *l = new QLabel;
    l->setObjectName("ui_plugin_list_card_element_icon");
    l->setProperty("category", "plugin_list_card_element_icon_label");
    l->setScaledContents(true);
    return l;
}

void UiPluginListCard::mouseReleaseEvent(QMouseEvent *ev)
{
    ev->ignore();
    emit clicked(this->m_index);
}
