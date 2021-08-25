#include "uipluginobjeditor.h"
#include "ui_uipluginobjeditor.h"

UiPluginObjEditor::UiPluginObjEditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiPluginObjEditor)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
}

UiPluginObjEditor::~UiPluginObjEditor()
{
    clearConnect();
    this->m_p_text = nullptr;
    this->m_p_orig_obj = nullptr;
    this->m_p_style = nullptr;
    delete ui;
}

void UiPluginObjEditor::init(Text *ptrText, Style *ptrStyle, PluginObject *ptrObj)
{
    this->m_p_text = ptrText;
    this->m_p_orig_obj = ptrObj;
    this->m_p_style = ptrStyle;
    ptrText = nullptr;
    ptrObj = nullptr;
    ptrStyle = nullptr;

    this->setAttribute(Qt::WA_StyledBackground);
    initWidget();
    initObjectName();
    initProperty();
    initContent();
    initConnect();
    initValue();

    this->setStyleSheet(this->m_p_style->getQSS());

    setEidtEnabled(false);
}

void UiPluginObjEditor::closeEvent(QCloseEvent *event)
{
    if(this->m_edited) {
        QMessageBox::StandardButton b;
        if(this->m_on_delete) {
            b = QMessageBox::warning(this, this->m_p_text->getLoc(QStringLiteral("ui_plugin_obj_editor_msgbox_save_tit")), this->m_p_text->getLoc(QStringLiteral("ui_plugin_obj_editor_msgbox_save_txt")), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        } else {
            b = QMessageBox::warning(this, this->m_p_text->getLoc(QStringLiteral("ui_plugin_obj_editor_msgbox_save_tit")), this->m_p_text->getLoc(QStringLiteral("ui_plugin_obj_editor_msgbox_save_txt")), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
        }
        if(b == QMessageBox::Yes) {
            save();
            event->accept();
            emit sigBack(SIG_BACK_TYPE(edited | closed));
        } else if(b == QMessageBox::No) {
            event->accept();
            emit sigBack(closed);
        } else {
            event->ignore();
        }
    } else {
        emit sigBack(closed);
    }
}

void UiPluginObjEditor::initWidget()
{
    this->ui_layout_main = new QVBoxLayout;
    this->ui_layout_main->setSpacing(0);
    this->ui_layout_main->setMargin(0);

    this->ui_tools_widget = new QWidget;
    this->ui_layout_main->addWidget(this->ui_tools_widget);
    this->ui_layout_tools = new QHBoxLayout;
    this->ui_layout_tools->setMargin(0);
    this->ui_layout_tools->setSpacing(0);
    this->ui_tools_widget->setLayout(this->ui_layout_tools);
    this->ui_tools_edit = new QPushButton;
    this->ui_tools_save = new QPushButton;
    this->ui_tools_discard = new QPushButton;
    this->ui_tools_status = new QLabel;
    this->ui_layout_tools->addWidget(this->ui_tools_edit);
    this->ui_layout_tools->addWidget(this->ui_tools_save);
    this->ui_layout_tools->addWidget(this->ui_tools_discard);
    this->ui_layout_tools->addWidget(this->ui_tools_status);
    this->ui_layout_tools->addStretch();

    this->ui_scrollarea = new QScrollArea;
    this->ui_scrollarea->setWidgetResizable(true);
    this->ui_layout_main->addWidget(this->ui_scrollarea);
    this->ui_widget = new QWidget;
    this->ui_scrollarea->setWidget(this->ui_widget);
    this->ui_layout_scrollarea = new QVBoxLayout;
    this->ui_layout_scrollarea->setMargin(5);
    this->ui_layout_scrollarea->setSpacing(5);
    this->ui_widget->setLayout(this->ui_layout_scrollarea);
    this->ui_metadata = new UiPluginObjMetadataEditor;
    this->ui_metadata->init(this->m_p_text, this->m_p_style, this->m_p_orig_obj->m_p_metadata);
    this->ui_layout_scrollarea->addWidget(this->ui_metadata);
    this->ui_layout_list = new QVBoxLayout;
    this->ui_layout_list->setMargin(0);
    this->ui_layout_list->setSpacing(5);
    this->ui_layout_scrollarea->addLayout(this->ui_layout_list);
    this->ui_layout_scrollarea->addStretch();

    this->setLayout(this->ui_layout_main);
}

void UiPluginObjEditor::initObjectName()
{
    this->setObjectName(QStringLiteral("ui_plugin_obj_editor"));
    // 部件部分
    this->ui_tools_widget->setObjectName(QStringLiteral("ui_plugin_obj_editor_tools_widget"));
    this->ui_tools_edit->setObjectName(QStringLiteral("ui_plugin_obj_editor_tools_edit"));
    this->ui_tools_save->setObjectName(QStringLiteral("ui_plugin_obj_editor_tools_save"));
    this->ui_tools_discard->setObjectName(QStringLiteral("ui_plugin_obj_editor_tools_discard"));
    this->ui_tools_status->setObjectName(QStringLiteral("ui_plugin_obj_editor_tools_status"));
    this->ui_scrollarea->setObjectName(QStringLiteral("ui_plugin_obj_editor_scrollarea"));
    this->ui_widget->setObjectName(QStringLiteral("ui_plugin_obj_editor_widget"));
    // 布局部分
    this->ui_layout_main->setObjectName(QStringLiteral("ui_plugin_obj_editor_layout_main"));
    this->ui_layout_tools->setObjectName(QStringLiteral("ui_plugin_obj_editor_layout_tools"));
    this->ui_layout_scrollarea->setObjectName(QStringLiteral("ui_plugin_obj_editor_layout_scrollarea"));
    this->ui_layout_list->setObjectName(QStringLiteral("ui_plugin_obj_editor_layout_list"));
}

void UiPluginObjEditor::initProperty()
{
    this->setProperty("category", QStringLiteral("frame"));
    // 部件部分
    this->ui_tools_widget->setProperty("category", QStringLiteral("plugin_obj_editor_tools_widget"));
    this->ui_tools_edit->setProperty("category", QStringLiteral("plugin_obj_editor_tools_button"));
    this->ui_tools_save->setProperty("category", QStringLiteral("plugin_obj_editor_tools_button"));
    this->ui_tools_discard->setProperty("category", QStringLiteral("plugin_obj_editor_tools_button"));
    this->ui_tools_status->setProperty("category", QStringLiteral("plugin_obj_editor_tools_label"));
    this->ui_scrollarea->setProperty("category", QStringLiteral("plugin_obj_editor_scrollarea"));
    this->ui_widget->setProperty("category", QStringLiteral("plugin_obj_editor_widget"));
}

void UiPluginObjEditor::initContent()
{
    this->ui_tools_edit->setIcon(QIcon(STR_IMG_PATH + QStringLiteral("edit.svg")));
    this->ui_tools_edit->setToolTip(this->m_p_text->getLoc(this->ui_tools_edit->objectName() + QStringLiteral("_tip")));
    this->ui_tools_save->setIcon(QIcon(STR_IMG_PATH + QStringLiteral("save.svg")));
    this->ui_tools_save->setToolTip(this->m_p_text->getLoc(this->ui_tools_save->objectName() + QStringLiteral("_tip")));
    this->ui_tools_discard->setIcon(QIcon(STR_IMG_PATH + QStringLiteral("fork.svg")));
    this->ui_tools_discard->setToolTip(this->m_p_text->getLoc(this->ui_tools_discard->objectName() + QStringLiteral("_tip")));
}

void UiPluginObjEditor::initConnect()
{
    connect(this->ui_tools_edit, &QPushButton::clicked, this, [ = ] {this->m_edited = true; this->setEidtEnabled();});
    connect(this->ui_tools_discard, &QPushButton::clicked, this, &UiPluginObjEditor::slotDiscard);
}

void UiPluginObjEditor::clearConnect()
{
    disconnect(this->ui_tools_edit, &QPushButton::clicked, this, nullptr);
    disconnect(this->ui_tools_discard, &QPushButton::clicked, this, &UiPluginObjEditor::slotDiscard);
}

void UiPluginObjEditor::initValue()
{
    this->ui_metadata->initValue();
}

void UiPluginObjEditor::save()
{

}

void UiPluginObjEditor::slotDiscard()
{
    QMessageBox::StandardButton b = QMessageBox::warning(this, this->m_p_text->getLoc(QStringLiteral("ui_plugin_obj_editor_msgbox_discard_tit")), this->m_p_text->getLoc(QStringLiteral("ui_plugin_obj_editor_msgbox_discard_txt")), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if(b == QMessageBox::Yes) {
        initValue();
        setEidtEnabled(false);
    }
}

void UiPluginObjEditor::setEidtEnabled(bool s)
{
    if(s) {
        QList<QVariant> v = {0, this->m_p_text->get(this->m_p_orig_obj->m_p_metadata->caption, this->m_p_orig_obj->m_p_metadata->uuid)};
        this->setWindowTitle(this->m_p_text->getLoc(this->objectName() + QStringLiteral("_tit_edited"), v));
        this->ui_tools_status->setText(this->m_p_text->getLoc(this->ui_tools_status->objectName() + QStringLiteral("_txt_edit_mode")));
        this->ui_tools_edit->setEnabled(false);
    } else {
        QList<QVariant> v = {0, this->m_p_text->get(this->m_p_orig_obj->m_p_metadata->caption, this->m_p_orig_obj->m_p_metadata->uuid)};
        this->setWindowTitle(this->m_p_text->getLoc(this->objectName() + QStringLiteral("_tit"), v));
        if(this->m_p_orig_obj->m_p_metadata->is_system) {
            this->ui_tools_edit->setEnabled(false);
            this->ui_tools_status->setText(this->m_p_text->getLoc(this->ui_tools_status->objectName() + QStringLiteral("_txt_sys")));
        } else {
            this->ui_tools_edit->setEnabled(true);
            this->ui_tools_status->setText(this->m_p_text->getLoc(this->ui_tools_status->objectName() + QStringLiteral("_txt_browse_mode")));
        }
    }
    this->ui_tools_save->setEnabled(s);
    this->ui_tools_discard->setEnabled(s);

    this->ui_metadata->setEidtEnabled(s);
}
