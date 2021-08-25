#include "uipluginlistcard.h"

UiPluginListCard::UiPluginListCard(QWidget *parent) :
    QWidget(parent)
{
}

UiPluginListCard::~UiPluginListCard()
{
    if(this->m_p_editor != nullptr) {
        this->m_p_editor->m_on_delete = true;
        this->m_p_editor->close();
        this->m_p_editor = nullptr;
    }

    this->m_p_text = nullptr;
    this->m_p_obj = nullptr;
    this->m_p_style = nullptr;

    while(!this->ui_elements->isEmpty()) {
        delete this->ui_elements->last();
        this->ui_elements->pop_back();
    }
    delete this->ui_elements;
}

void UiPluginListCard::init(Text *ptrText, Style *ptrStyle, PluginObject *ptrObj, qint32 index, bool isFav, bool isDisabled)
{
    this->m_p_text = ptrText;
    this->m_p_style = ptrStyle;
    this->m_p_obj = ptrObj;
    this->m_is_fav = isFav;
    this->m_is_disabled = isDisabled;
    ptrText = nullptr;
    ptrStyle = nullptr;
    ptrObj = nullptr;

    this->m_index = index;

    this->setAttribute(Qt::WA_StyledBackground);
    initWidget();
    initObjectName();
    initProperty();
    initContent();
    initConnect();
    initElementsIcon();

    refreshStatus();
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
    this->ui_layout_main = new QHBoxLayout;
    this->ui_layout_main->setSpacing(5);
    this->ui_layout_main->setMargin(5);
    // 图标
    this->ui_icon = new QLabel;
    this->ui_icon->setScaledContents(true);
    this->ui_layout_main->addWidget(this->ui_icon);
    // 标题
    this->ui_layout_content = new QVBoxLayout;
    this->ui_layout_content->setSpacing(0);
    this->ui_layout_title = new QHBoxLayout;
    this->ui_layout_title->setSpacing(5);
    this->ui_caption = new QLabel;
    this->ui_author = new QLabel;
    this->ui_version = new QLabel;
    this->ui_layout_title->addWidget(this->ui_caption);
    this->ui_layout_title->addWidget(this->ui_author);
    this->ui_layout_title->addWidget(this->ui_version);
    this->ui_layout_title->addStretch();
    this->ui_layout_content->addLayout(this->ui_layout_title);
    // 描述
    this->ui_desc = new QLabel;
    this->ui_layout_content->addWidget(this->ui_desc);
    this->ui_layout_content->addStretch();
    // 元素
    this->ui_layout_elements = new QHBoxLayout;
    this->ui_layout_elements->setSpacing(0);
    this->ui_layout_content->addLayout(this->ui_layout_elements);

    this->ui_layout_main->addLayout(this->ui_layout_content);
    this->ui_layout_main->addStretch();

    this->ui_status = new QLabel;
    this->ui_status->setScaledContents(true);
    this->ui_layout_main->addWidget(this->ui_status);

    this->setLayout(this->ui_layout_main);
}

void UiPluginListCard::initObjectName()
{
    this->setObjectName(QStringLiteral("ui_plugin_list_card"));
    // 部件部分
    this->ui_icon->setObjectName(QStringLiteral("ui_plugin_list_card_icon"));
    this->ui_caption->setObjectName(QStringLiteral("ui_plugin_list_card_caption"));
    this->ui_author->setObjectName(QStringLiteral("ui_plugin_list_card_author"));
    this->ui_version->setObjectName(QStringLiteral("ui_plugin_list_card_version"));
    this->ui_desc->setObjectName(QStringLiteral("ui_plugin_list_card_desc"));
    this->ui_status->setObjectName(QStringLiteral("ui_plugin_list_card_status"));
    // 布局部分
    this->ui_layout_main->setObjectName(QStringLiteral("ui_plugin_list_card_layout_main"));
    this->ui_layout_content->setObjectName(QStringLiteral("ui_plugin_list_card_layout_content"));
    this->ui_layout_title->setObjectName(QStringLiteral("ui_plugin_list_card_layout_title"));
    this->ui_layout_elements->setObjectName(QStringLiteral("ui_plugin_list_card_layout_elements"));
}

void UiPluginListCard::initProperty()
{
    this->setProperty("category", QStringLiteral("plugin_list_card"));
    this->setProperty("selected", false);
    // 部件部分
    this->ui_icon->setProperty("category", QStringLiteral("plugin_list_card_label"));
    this->ui_caption->setProperty("category", QStringLiteral("plugin_list_card_label"));
    this->ui_author->setProperty("category", QStringLiteral("plugin_list_card_label"));
    this->ui_version->setProperty("category", QStringLiteral("plugin_list_card_label"));
    this->ui_desc->setProperty("category", QStringLiteral("plugin_list_card_label"));
    this->ui_status->setProperty("category", QStringLiteral("plugin_list_card_label"));
}

void UiPluginListCard::initContent()
{
    QString objUuid = this->m_p_obj->m_p_metadata->uuid;
    this->ui_icon->setPixmap(QPixmap(QDir(this->m_p_obj->m_p_metadata->dir_path).filePath(this->m_p_obj->m_p_metadata->icon)));
    this->ui_caption->setText(this->m_p_text->get(this->m_p_obj->m_p_metadata->caption, objUuid));
    this->ui_author->setText(this->m_p_text->get(this->m_p_obj->m_p_metadata->author, objUuid));
    this->ui_version->setText(this->m_p_text->get(this->m_p_obj->m_p_metadata->version, objUuid));
    this->ui_desc->setText(this->m_p_text->get(this->m_p_obj->m_p_metadata->description, objUuid));
}

void UiPluginListCard::initConnect()
{
    connect(this, &UiPluginListCard::doubleClicked, this, &UiPluginListCard::slotOpenEditor);
}

void UiPluginListCard::clearConnect()
{
    disconnect(this, &UiPluginListCard::doubleClicked, this, &UiPluginListCard::slotOpenEditor);
}

void UiPluginListCard::initElementsIcon()
{
    this->ui_elements = new QVector<QLabel *>;
    if(this->m_p_obj->m_p_metadata->has_localisation) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + QStringLiteral("localisation.svg")));
        l->setToolTip(this->m_p_text->getLoc(QStringLiteral("plugin_localisation")));
        this->ui_elements->append(l);
        this->ui_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_style) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + QStringLiteral("style.svg")));
        l->setToolTip(this->m_p_text->getLoc(QStringLiteral("plugin_style")));
        this->ui_elements->append(l);
        this->ui_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_event) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + QStringLiteral("script.svg")));
        l->setToolTip(this->m_p_text->getLoc(QStringLiteral("plugin_event")));
        this->ui_elements->append(l);
        this->ui_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_action) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + QStringLiteral("action.svg")));
        l->setToolTip(this->m_p_text->getLoc(QStringLiteral("plugin_action")));
        this->ui_elements->append(l);
        this->ui_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_accessories) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + QStringLiteral("accessories.svg")));
        l->setToolTip(this->m_p_text->getLoc("plugin_accessories"));
        this->ui_elements->append(l);
        this->ui_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_model) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + QStringLiteral("model.svg")));
        l->setToolTip(this->m_p_text->getLoc(QStringLiteral("plugin_model")));
        this->ui_elements->append(l);
        this->ui_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_config) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + QStringLiteral("config.svg")));
        l->setToolTip(this->m_p_text->getLoc(QStringLiteral("plugin_config")));
        this->ui_elements->append(l);
        this->ui_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_resources) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + QStringLiteral("file.svg")));
        l->setToolTip(this->m_p_text->getLoc(QStringLiteral("plugin_resources")));
        this->ui_elements->append(l);
        this->ui_layout_elements->addWidget(l);
    }
    if(this->m_p_obj->m_p_metadata->has_library) {
        QLabel *l = getElementIcon();
        l->setPixmap(QPixmap(STR_IMG_PATH + QStringLiteral("library.svg")));
        l->setToolTip(this->m_p_text->getLoc(QStringLiteral("plugin_library")));
        this->ui_elements->append(l);
        this->ui_layout_elements->addWidget(l);
    }
    this->ui_layout_elements->addStretch();
}

QLabel *UiPluginListCard::getElementIcon()
{
    QLabel *l = new QLabel;
    l->setObjectName(QStringLiteral("ui_plugin_list_card_element_icon"));
    l->setProperty("category", QStringLiteral("plugin_list_card_element_icon_label"));
    l->setScaledContents(true);
    return l;
}

void UiPluginListCard::mouseReleaseEvent(QMouseEvent *ev)
{
    ev->ignore();
    emit clicked(this->m_index);
}

QString UiPluginListCard::getAllText()
{
    QString txt;
    txt.append(this->ui_caption->text());
    txt.append(this->ui_author->text());
    txt.append(this->ui_desc->text());
    txt.append(this->ui_version->text());
    txt.append(this->m_p_obj->m_p_metadata->orig_uuid);
    txt.append(this->m_p_obj->m_p_metadata->uuid);
    return txt;
}

void UiPluginListCard::refreshStatus()
{
    QString tip;
    if(this->m_is_fav) {
        tip.append(this->m_p_text->getLoc(QStringLiteral("ui_plugin_page_list_card_status_fav_tip")));
    }
    if(this->m_is_disabled) {
        if(!tip.isEmpty()) {
            tip.append(QStringLiteral(" | "));
        }
        tip.append(this->m_p_text->getLoc(QStringLiteral("ui_plugin_page_list_card_status_disabled_tip")));
    }
    this->setToolTip(tip);
    this->setProperty("contains_errors", !this->m_p_obj->m_p_exc_list->first.isEmpty());
    this->setProperty("contains_warnings", !this->m_p_obj->m_p_exc_list->second.isEmpty());
    this->setProperty("is_fav", this->m_is_fav);
    this->setProperty("is_disabled", this->m_is_disabled);
    this->ui_status->clear();
    if(!this->m_p_obj->m_p_exc_list->first.isEmpty()) {
        this->ui_status->setPixmap(QPixmap(STR_IMG_PATH + QStringLiteral("error.svg")));
        if(!this->m_p_obj->m_p_exc_list->second.isEmpty()) {
            QList<QVariant> v = {0, this->m_p_obj->m_p_exc_list->first.count(), this->m_p_obj->m_p_exc_list->second.count()};
            this->ui_status->setToolTip(this->m_p_text->getLoc(QStringLiteral("plugin_contains_e&w"), v));
        } else {
            QList<QVariant> v = {0, this->m_p_obj->m_p_exc_list->first.count()};
            this->ui_status->setToolTip(this->m_p_text->getLoc(QStringLiteral("plugin_contains_e"), v));
        }
    } else if(!this->m_p_obj->m_p_exc_list->second.isEmpty()) {
        this->ui_status->setPixmap(QPixmap(STR_IMG_PATH + QStringLiteral("warning.svg")));
        QList<QVariant> v = {0, this->m_p_obj->m_p_exc_list->second.count()};
        this->ui_status->setToolTip(this->m_p_text->getLoc(QStringLiteral("plugin_contains_w"), v));
    }
    this->setStyle(QApplication::style());
}

void UiPluginListCard::mouseDoubleClickEvent(QMouseEvent *ev)
{
    ev->accept();
    emit doubleClicked();
}

void UiPluginListCard::slotOpenEditor()
{
    if(this->m_p_editor == nullptr) {
        this->m_p_editor = new UiPluginObjEditor();
        this->m_p_editor->init(this->m_p_text, this->m_p_style, this->m_p_obj);
        connect(this->m_p_editor, &UiPluginObjEditor::sigBack, this, &UiPluginListCard::slotEditorBack);
    }
    this->m_p_editor->show();
}

void UiPluginListCard::slotEditorBack(UiPluginObjEditor::SIG_BACK_TYPE sig)
{
    if((sig & UiPluginObjEditor::edited) == UiPluginObjEditor::edited) {
        emit sigEdited();
    }
    if((sig & UiPluginObjEditor::closed) == UiPluginObjEditor::closed) {
        disconnect(this->m_p_editor, &UiPluginObjEditor::sigBack, this, &UiPluginListCard::slotEditorBack);
        delete this->m_p_editor;
        this->m_p_editor = nullptr;
    }
}
