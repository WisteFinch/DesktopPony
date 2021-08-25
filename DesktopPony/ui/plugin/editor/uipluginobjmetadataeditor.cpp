#include "uipluginobjmetadataeditor.h"

UiPluginObjMetadataEditor::UiPluginObjMetadataEditor(QWidget *parent) : QWidget(parent)
{

}

UiPluginObjMetadataEditor::~UiPluginObjMetadataEditor()
{
    clearConnect();
}

void UiPluginObjMetadataEditor::init(Text *ptrText, Style *ptrStyle, PluginObjectMetadata *ptrMetadata)
{
    this->m_p_text = ptrText;
    this->m_p_metadata = ptrMetadata;
    this->m_p_style = ptrStyle;
    ptrText = nullptr;
    ptrMetadata = nullptr;
    ptrStyle = nullptr;

    this->setAttribute(Qt::WA_StyledBackground);
    initWidget();
    initObjectName();
    initProperty();
    initContent();
    initConnect();
}

void UiPluginObjMetadataEditor::initWidget()
{
    this->ui_layout_main = new QVBoxLayout;
    this->ui_layout_main->setMargin(5);
    this->ui_layout_main->setSpacing(5);
    this->ui_layout_title = new QHBoxLayout;
    this->ui_layout_title->setMargin(0);
    this->ui_layout_main->addLayout(this->ui_layout_title);
    this->ui_title = new QLabel;
    this->ui_layout_main->addWidget(ui_title);
    this->ui_layout_title->addStretch();
    this->ui_layout_content = new QVBoxLayout;
    this->ui_layout_content->setContentsMargins(5, 0, 0, 0);
    this->ui_layout_content->setSpacing(5);
    this->ui_layout_main->addLayout(this->ui_layout_content);

    this->ui_layout_icon = new QHBoxLayout;
    this->ui_layout_icon->setMargin(0);
    this->ui_layout_content->addLayout(this->ui_layout_icon);

    this->ui_layout_id = new QHBoxLayout;
    this->ui_layout_id->setMargin(0);
    this->ui_id = new QLabel;
    this->ui_id_lineedit = new QLineEdit;
    this->ui_layout_id->addWidget(this->ui_id);
    this->ui_layout_id->addWidget(this->ui_id_lineedit);
    this->ui_layout_content->addLayout(this->ui_layout_id);

    this->ui_layout_uuid = new QHBoxLayout;
    this->ui_layout_uuid->setMargin(0);
    this->ui_uuid = new QLabel;
    this->ui_uuid_lineedit = new QLineEdit;
    this->ui_uuid_button = new QPushButton;
    this->ui_layout_uuid->addWidget(this->ui_uuid);
    this->ui_layout_uuid->addWidget(this->ui_uuid_lineedit);
    this->ui_layout_uuid->addWidget(this->ui_uuid_button);
    this->ui_layout_content->addLayout(this->ui_layout_uuid);

    this->ui_layout_caption = new QHBoxLayout;
    this->ui_layout_caption->setMargin(0);
    this->ui_caption = new QLabel;
    this->ui_caption_lineedit = new QLineEdit;
    this->ui_layout_caption->addWidget(this->ui_caption);
    this->ui_layout_caption->addWidget(this->ui_caption_lineedit);
    this->ui_layout_content->addLayout(this->ui_layout_caption);

    this->ui_layout_desc = new QHBoxLayout;
    this->ui_layout_desc->setMargin(0);
    this->ui_desc = new QLabel;
    this->ui_desc_textedit = new QTextEdit;
    this->ui_layout_desc->addWidget(this->ui_desc);
    this->ui_layout_desc->addWidget(this->ui_desc_textedit);
    this->ui_layout_content->addLayout(this->ui_layout_desc);

    this->ui_layout_author = new QHBoxLayout;
    this->ui_layout_author->setMargin(0);
    this->ui_author = new QLabel;
    this->ui_author_lineedit = new QLineEdit;
    this->ui_layout_author->addWidget(this->ui_author);
    this->ui_layout_author->addWidget(this->ui_author_lineedit);
    this->ui_layout_content->addLayout(this->ui_layout_author);

    this->ui_layout_version = new QHBoxLayout;
    this->ui_layout_version->setMargin(0);
    this->ui_version = new QLabel;
    this->ui_version_lineedit = new QLineEdit;
    this->ui_layout_version->addWidget(this->ui_version);
    this->ui_layout_version->addWidget(this->ui_version_lineedit);
    this->ui_layout_content->addLayout(this->ui_layout_version);

    this->ui_layout_url = new QHBoxLayout;
    this->ui_layout_url->setMargin(0);
    this->ui_url = new QLabel;
    this->ui_url_lineedit = new QLineEdit;
    this->ui_layout_url->addWidget(this->ui_url);
    this->ui_layout_url->addWidget(this->ui_url_lineedit);
    this->ui_layout_content->addLayout(this->ui_layout_url);

    this->setLayout(this->ui_layout_main);
}

void UiPluginObjMetadataEditor::initObjectName()
{
    this->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor"));

    // 部件部分
    this->ui_title->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_title"));
    this->ui_id->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_id"));
    this->ui_id_lineedit->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_id_lineedit"));
    this->ui_uuid->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_uuid"));
    this->ui_uuid_lineedit->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_uuid_lineedit"));
    this->ui_uuid_button->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_uuid_button"));
    this->ui_caption->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_caption"));
    this->ui_caption_lineedit->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_caption_lineedit"));
    this->ui_desc->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_desc"));
    this->ui_desc_textedit->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_desc_textedit"));
    this->ui_author->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_author"));
    this->ui_author_lineedit->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_author_lineedit"));
    this->ui_version->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_version"));
    this->ui_version_lineedit->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_version_lineedit"));
    this->ui_url->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_url"));
    this->ui_url_lineedit->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_url_lineedit"));

    // 布局部分
    this->ui_layout_main->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_layout_main"));
    this->ui_layout_title->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_layout_title"));
    this->ui_layout_content->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_layout_content"));
    this->ui_layout_icon->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_layout_icon"));
    this->ui_layout_id->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_layout_id"));
    this->ui_layout_uuid->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_layout_uuid"));
    this->ui_layout_caption->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_layout_caption"));
    this->ui_layout_desc->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_layout_desc"));
    this->ui_layout_author->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_layout_author"));
    this->ui_layout_version->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_layout_version"));
    this->ui_layout_url->setObjectName(QStringLiteral("ui_plugin_obj_metadata_editor_layout_url"));
}

void UiPluginObjMetadataEditor::initProperty()
{
    this->setProperty("category", QStringLiteral("plugin_obj_metadata_editor"));
    this->setProperty("selected", false);
    // 部件部分
    this->ui_id->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_label"));
    this->ui_id_lineedit->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_lineedit"));
    this->ui_uuid->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_label"));
    this->ui_uuid_lineedit->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_lineedit"));
    this->ui_uuid_button->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_button"));
    this->ui_caption->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_label"));
    this->ui_caption_lineedit->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_lineedit"));
    this->ui_desc->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_label"));
    this->ui_desc_textedit->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_textedit"));
    this->ui_author->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_label"));
    this->ui_author_lineedit->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_lineedit"));
    this->ui_version->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_label"));
    this->ui_version_lineedit->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_lineedit"));
    this->ui_url->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_label"));
    this->ui_url_lineedit->setProperty("category", QStringLiteral("plugin_obj_metadata_editor_lineedit"));
}

void UiPluginObjMetadataEditor::initContent()
{
    this->ui_title->setText(this->m_p_text->getLoc(this->ui_title->objectName() + QStringLiteral("_txt")));
    this->ui_id->setText(this->m_p_text->getLoc(this->ui_id->objectName() + QStringLiteral("_txt")));
    this->ui_uuid->setText(this->m_p_text->getLoc(this->ui_uuid->objectName() + QStringLiteral("_txt")));
    this->ui_uuid_button->setIcon(QIcon(STR_IMG_PATH + QStringLiteral("reload.svg")));
    this->ui_uuid_button->setToolTip(this->m_p_text->getLoc(this->ui_uuid_button->objectName() + QStringLiteral("_tip")));
    this->ui_caption->setText(this->m_p_text->getLoc(this->ui_caption->objectName() + QStringLiteral("_txt")));
    this->ui_desc->setText(this->m_p_text->getLoc(this->ui_desc->objectName() + QStringLiteral("_txt")));
    this->ui_author->setText(this->m_p_text->getLoc(this->ui_author->objectName() + QStringLiteral("_txt")));
    this->ui_version->setText(this->m_p_text->getLoc(this->ui_version->objectName() + QStringLiteral("_txt")));
    this->ui_url->setText(this->m_p_text->getLoc(this->ui_url->objectName() + QStringLiteral("_txt")));
    this->ui_url->setToolTip(this->m_p_text->getLoc(this->ui_url->objectName() + QStringLiteral("_tip")));
    this->ui_url_lineedit->setPlaceholderText(this->m_p_text->getLoc(this->ui_url_lineedit->objectName() + QStringLiteral("_tip")));
}

void UiPluginObjMetadataEditor::initValue()
{
    this->ui_id_lineedit->setText(this->m_p_metadata->id);
    this->ui_uuid_lineedit->setText(this->m_p_metadata->uuid);
    this->ui_caption_lineedit->setText(this->m_p_metadata->caption);
    this->ui_desc_textedit->setText(this->m_p_metadata->description);
    this->ui_author_lineedit->setText(this->m_p_metadata->author);
    this->ui_version_lineedit->setText(this->m_p_metadata->version);
    this->ui_url_lineedit->setText(this->m_p_metadata->remote_url);
}

void UiPluginObjMetadataEditor::initConnect()
{
    connect(this->ui_uuid_button, &QPushButton::clicked, this, [ = ] {this->ui_uuid_lineedit->setText(Tools::creatUuid());});
}

void UiPluginObjMetadataEditor::clearConnect()
{
    disconnect(this->ui_uuid_button, &QPushButton::clicked, this, nullptr);
}


void UiPluginObjMetadataEditor::setEidtEnabled(bool s)
{
    this->ui_id_lineedit->setEnabled(s);
    this->ui_uuid_lineedit->setEnabled(s);
    this->ui_uuid_button->setEnabled(s);
    this->ui_caption_lineedit->setEnabled(s);
    this->ui_desc_textedit->setEnabled(s);
    this->ui_author_lineedit->setEnabled(s);
    this->ui_version_lineedit->setEnabled(s);
    this->ui_url_lineedit->setEnabled(s);
}
