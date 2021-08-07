#include "uiinfopage.h"

UiInfoPage::UiInfoPage(QWidget *parent) :
    QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground);
}

UiInfoPage::~UiInfoPage()
{
    clearConnect();
}

void UiInfoPage::init(Config *ptrconf, Text *ptrText)
{
    this->m_p_conf = ptrconf;
    this->m_p_text = ptrText;
    ptrconf = nullptr;
    ptrText = nullptr;

    initWidget();
    initObjectName();
    initProperty();
    initContent();
    initConnect();
}

void UiInfoPage::initWidget()
{
    this->ui_info_page_layout_main = new QVBoxLayout;
    this->ui_info_page_layout_main->setSpacing(0);
    this->ui_info_page_layout_main->setMargin(0);
    this->ui_info_page_scrollarea = new QScrollArea;
    this->ui_info_page_scrollarea->setWidgetResizable(true);
    this->ui_info_page_layout_main->addWidget(this->ui_info_page_scrollarea);
    this->ui_info_page_scrollarea_basis = new QWidget;
    this->ui_info_page_scrollarea->setWidget(this->ui_info_page_scrollarea_basis);
    this->ui_info_page_layout_scrollarea_basis = new QVBoxLayout;
    this->ui_info_page_layout_scrollarea_basis->setSpacing(0);
    this->ui_info_page_layout_scrollarea_basis->setContentsMargins(0, 0, 2, 0);
    this->ui_info_page_scrollarea_basis->setLayout(this->ui_info_page_layout_scrollarea_basis);
    this->ui_info_page_scrollarea_widget = new QWidget;
    this->ui_info_page_layout_scrollarea_basis->addWidget(this->ui_info_page_scrollarea_widget);
    this->ui_info_page_layout_scrollarea = new QVBoxLayout;
    this->ui_info_page_layout_scrollarea->setSpacing(5);
    this->ui_info_page_layout_scrollarea->setMargin(5);
    this->ui_info_page_scrollarea_widget->setLayout(this->ui_info_page_layout_scrollarea);

    // 关于
    this->ui_info_page_layout_about = new QHBoxLayout;
    this->ui_info_page_layout_about_icon = new QVBoxLayout;
    this->ui_info_page_layout_about->addLayout(this->ui_info_page_layout_about_icon);
    this->ui_info_page_about_icon = new QLabel;
    this->ui_info_page_about_icon->setScaledContents(true);
    this->ui_info_page_layout_about_icon->addWidget(this->ui_info_page_about_icon);
    this->ui_info_page_layout_about_icon->addStretch();
    this->ui_info_page_about = new QLabel;
    this->ui_info_page_about->setWordWrap(true);
    this->ui_info_page_layout_about->addWidget(this->ui_info_page_about);
    this->ui_info_page_layout_scrollarea->addLayout(this->ui_info_page_layout_about);
    // 项
    this->ui_info_page_layout_scrollarea->addWidget(new HLine);
    this->ui_info_page_layout_log = new QHBoxLayout;
    this->ui_info_page_layout_log->setMargin(0);
    this->ui_info_page_layout_log_text = new QVBoxLayout;
    this->ui_info_page_layout_log_text->setSpacing(0);
    this->ui_info_page_layout_log_text->setMargin(0);
    this->ui_info_page_layout_log->addLayout(this->ui_info_page_layout_log_text);
    this->ui_info_page_log_caption = new QLabel;
    this->ui_info_page_layout_log_text->addWidget(this->ui_info_page_log_caption);
    this->ui_info_page_log_desc = new QLabel;
    this->ui_info_page_log_desc->setWordWrap(true);
    this->ui_info_page_layout_log_text->addWidget(this->ui_info_page_log_desc);
    this->ui_info_page_log_view = new QPushButton;
    this->ui_info_page_layout_log->addWidget(this->ui_info_page_log_view);

    this->ui_info_page_layout_version = new QHBoxLayout;
    this->ui_info_page_layout_version->setMargin(0);
    this->ui_info_page_layout_version_text = new QVBoxLayout;
    this->ui_info_page_layout_version_text->setSpacing(0);
    this->ui_info_page_layout_version_text->setMargin(0);
    this->ui_info_page_layout_version->addLayout(this->ui_info_page_layout_version_text);
    this->ui_info_page_version_caption = new QLabel;
    this->ui_info_page_layout_version_text->addWidget(this->ui_info_page_version_caption);
    this->ui_info_page_version_desc = new QLabel;
    this->ui_info_page_version_desc->setWordWrap(true);
    this->ui_info_page_layout_version_text->addWidget(this->ui_info_page_version_desc);
    this->ui_info_page_version_check_update = new QPushButton;
    this->ui_info_page_layout_version->addWidget(this->ui_info_page_version_check_update);

    this->ui_info_page_layout_repo = new QHBoxLayout;
    this->ui_info_page_layout_repo->setMargin(0);
    this->ui_info_page_layout_repo_text = new QVBoxLayout;
    this->ui_info_page_layout_repo_text->setSpacing(0);
    this->ui_info_page_layout_repo_text->setMargin(0);
    this->ui_info_page_layout_repo->addLayout(this->ui_info_page_layout_repo_text);
    this->ui_info_page_repo_caption = new QLabel;
    this->ui_info_page_layout_repo_text->addWidget(this->ui_info_page_repo_caption);
    this->ui_info_page_repo_desc = new QLabel;
    this->ui_info_page_repo_desc->setWordWrap(true);
    this->ui_info_page_layout_repo_text->addWidget(this->ui_info_page_repo_desc);
    this->ui_info_page_github_link = new QPushButton;
    this->ui_info_page_layout_repo->addWidget(this->ui_info_page_github_link);

    this->ui_info_page_layout_doc = new QHBoxLayout;
    this->ui_info_page_layout_doc->setMargin(0);
    this->ui_info_page_layout_doc_text = new QVBoxLayout;
    this->ui_info_page_layout_doc_text->setSpacing(0);
    this->ui_info_page_layout_doc_text->setMargin(0);
    this->ui_info_page_layout_doc->addLayout(this->ui_info_page_layout_doc_text);
    this->ui_info_page_doc_caption = new QLabel;
    this->ui_info_page_layout_doc_text->addWidget(this->ui_info_page_doc_caption);
    this->ui_info_page_doc_desc = new QLabel;
    this->ui_info_page_doc_desc->setWordWrap(true);
    this->ui_info_page_layout_doc_text->addWidget(this->ui_info_page_doc_desc);
    this->ui_info_page_doc_link = new QPushButton;
    this->ui_info_page_layout_doc->addWidget(this->ui_info_page_doc_link);

    this->ui_info_page_layout_scrollarea->addLayout(this->ui_info_page_layout_log);
    this->ui_info_page_layout_scrollarea->addLayout(this->ui_info_page_layout_version);
    this->ui_info_page_layout_scrollarea->addLayout(this->ui_info_page_layout_repo);
    this->ui_info_page_layout_scrollarea->addLayout(this->ui_info_page_layout_doc);
    // 应用信息
    this->ui_info_page_layout_scrollarea->addWidget(new HLine);
    this->ui_info_page_layout_appinfo = new QGridLayout;
    this->ui_info_page_copyright_caption = new QLabel;
    this->ui_info_page_layout_appinfo->addWidget(this->ui_info_page_copyright_caption, 0, 0, 1, 1);
    this->ui_info_page_copyright = new QLabel;
    this->ui_info_page_copyright->setWordWrap(true);
    this->ui_info_page_layout_appinfo->addWidget(this->ui_info_page_copyright, 0, 1, 1, 4);
    this->ui_info_page_author_caption = new QLabel;
    this->ui_info_page_layout_appinfo->addWidget(this->ui_info_page_author_caption, 1, 0, 1, 1);
    this->ui_info_page_author = new QLabel;
    this->ui_info_page_author->setWordWrap(true);
    this->ui_info_page_layout_appinfo->addWidget(this->ui_info_page_author, 1, 1, 1, 4);
    this->ui_info_page_license_caption = new QLabel;
    this->ui_info_page_layout_appinfo->addWidget(this->ui_info_page_license_caption, 2, 0, 1, 1);
    this->ui_info_page_license = new QLabel;
    this->ui_info_page_license->setWordWrap(true);
    this->ui_info_page_layout_appinfo->addWidget(this->ui_info_page_license, 2, 1, 1, 3);
    this->ui_info_page_license_link = new QPushButton;
    this->ui_info_page_layout_appinfo->addWidget(this->ui_info_page_license_link, 2, 4, 1, 1);
    this->ui_info_page_layout_scrollarea->addLayout(this->ui_info_page_layout_appinfo);

    this->ui_info_page_layout_scrollarea->addStretch();
    this->setLayout(this->ui_info_page_layout_main);
}

void UiInfoPage::initObjectName()
{
    this->setObjectName("ui_info_page");

    // 部件部分
    this->ui_info_page_scrollarea->setObjectName("ui_info_page_scrollarea");
    this->ui_info_page_scrollarea_widget->setObjectName("ui_info_page_scrollarea_widget");
    this->ui_info_page_scrollarea_basis->setObjectName("ui_info_page_scrollarea_basis");
    this->ui_info_page_about_icon->setObjectName("ui_info_page_about_icon");
    this->ui_info_page_about->setObjectName("ui_info_page_about");
    this->ui_info_page_log_caption->setObjectName("ui_info_page_log_caption");
    this->ui_info_page_log_desc->setObjectName("ui_info_page_log_desc");
    this->ui_info_page_log_view->setObjectName("ui_info_page_log_view");
    this->ui_info_page_version_caption->setObjectName("ui_info_page_version_caption");
    this->ui_info_page_version_desc->setObjectName("ui_info_page_version_desc");
    this->ui_info_page_version_check_update->setObjectName("ui_info_page_version_check_update");
    this->ui_info_page_repo_caption->setObjectName("ui_info_page_repo_caption");
    this->ui_info_page_repo_desc->setObjectName("ui_info_page_repo_desc");
    this->ui_info_page_github_link->setObjectName("ui_info_page_github_link");
    this->ui_info_page_doc_caption->setObjectName("ui_info_page_doc_caption");
    this->ui_info_page_doc_desc->setObjectName("ui_info_page_doc_desc");
    this->ui_info_page_doc_link->setObjectName("ui_info_page_doc_link");
    this->ui_info_page_copyright_caption->setObjectName("ui_info_page_copyright_caption");
    this->ui_info_page_copyright->setObjectName("ui_info_page_page_copyright");
    this->ui_info_page_author_caption->setObjectName("ui_info_page_author_caption");
    this->ui_info_page_author->setObjectName("ui_info_page_author");
    this->ui_info_page_license_caption->setObjectName("ui_info_page_license_caption");
    this->ui_info_page_license->setObjectName("ui_info_page_license");
    this->ui_info_page_license_link->setObjectName("ui_info_page_license_link");

    // 布局部分
    this->ui_info_page_layout_main->setObjectName("ui_info_page_layout_main");
    this->ui_info_page_layout_scrollarea->setObjectName("ui_info_page_layout_scrollarea");
    this->ui_info_page_layout_scrollarea_basis->setObjectName("ui_info_page_layout_scrollarea_basis");
    this->ui_info_page_layout_about->setObjectName("ui_info_page_layout_about");
    this->ui_info_page_layout_about_icon->setObjectName("ui_info_page_layout_about_icon");
    this->ui_info_page_layout_log->setObjectName("ui_info_page_layout_log");
    this->ui_info_page_layout_log_text->setObjectName("ui_info_page_layout_log_text");
    this->ui_info_page_layout_version->setObjectName("ui_info_page_layout_version");
    this->ui_info_page_layout_version_text->setObjectName("ui_info_page_layout_version_text");
    this->ui_info_page_layout_repo->setObjectName("ui_info_page_layout_repo");
    this->ui_info_page_layout_repo_text->setObjectName("ui_info_page_layout_repo_text");
    this->ui_info_page_layout_doc->setObjectName("ui_info_page_layout_doc");
    this->ui_info_page_layout_doc_text->setObjectName("ui_info_page_layout_doc_text");
    this->ui_info_page_layout_appinfo->setObjectName("ui_info_page_layout_appinfo");
}

void UiInfoPage::initProperty()
{
    this->setProperty("category", "page");
    // 部件部分
    this->ui_info_page_scrollarea->setProperty("category", "plugin_info_scrollarea");
    this->ui_info_page_scrollarea_widget->setProperty("category", "plugin_info_scrollarea_widget");
    this->ui_info_page_scrollarea_basis->setProperty("category", "plugin_info_scrollarea_basis_widget");
    this->ui_info_page_about_icon->setProperty("category", "plugin_info_icon_label");
    this->ui_info_page_about->setProperty("category", "plugin_info_desc_label");
    this->ui_info_page_log_caption->setProperty("category", "plugin_info_item_caption_label");
    this->ui_info_page_log_desc->setProperty("category", "plugin_info_item_desc_label");
    this->ui_info_page_log_view->setProperty("category", "plugin_info_button");
    this->ui_info_page_version_caption->setProperty("category", "plugin_info_item_caption_label");
    this->ui_info_page_version_desc->setProperty("category", "plugin_info_item_desc_label");
    this->ui_info_page_version_check_update->setProperty("category", "plugin_info_button");
    this->ui_info_page_repo_caption->setProperty("category", "plugin_info_item_caption_label");
    this->ui_info_page_repo_desc->setProperty("category", "plugin_info_item_desc_label");
    this->ui_info_page_github_link->setProperty("category", "plugin_info_button");
    this->ui_info_page_doc_caption->setProperty("category", "plugin_info_item_caption_label");
    this->ui_info_page_doc_desc->setProperty("category", "plugin_info_item_desc_label");
    this->ui_info_page_doc_link->setProperty("category", "plugin_info_button");
    this->ui_info_page_copyright_caption->setProperty("category", "plugin_info_appinfo_caption_label");
    this->ui_info_page_copyright->setProperty("category", "plugin_info_appinfo_text_label");
    this->ui_info_page_author_caption->setProperty("category", "plugin_info_appinfo_caption_label");
    this->ui_info_page_author->setProperty("category", "plugin_info_appinfo_text_label");
    this->ui_info_page_license_caption->setProperty("category", "plugin_info_appinfo_caption_label");
    this->ui_info_page_license->setProperty("category", "plugin_info_appinfo_text_label");
    this->ui_info_page_license_link->setProperty("category", "plugin_info_button");
}

void UiInfoPage::initContent()
{
    // 关于
    this->ui_info_page_about_icon->setPixmap(QPixmap(STR_IMG_PATH + "icon.svg"));
    this->ui_info_page_about->setText(this->m_p_text->getLoc(this->ui_info_page_about->objectName() + "_txt"));
    // 项
    this->ui_info_page_log_caption->setText(this->m_p_text->getLoc(this->ui_info_page_log_caption->objectName() + "_txt"));
    this->ui_info_page_log_desc->setText(this->m_p_text->getLoc(this->ui_info_page_log_desc->objectName() + "_txt"));
    this->ui_info_page_log_view->setIcon(QIcon(STR_IMG_PATH + "right.svg"));
    this->ui_info_page_log_view->setToolTip(this->m_p_text->getLoc(this->ui_info_page_log_view->objectName() + "_tip"));
    this->ui_info_page_version_caption->setText(this->m_p_text->getLoc(this->ui_info_page_version_caption->objectName() + "_txt"));
    this->ui_info_page_repo_caption->setText(this->m_p_text->getLoc(this->ui_info_page_repo_caption->objectName() + "_txt"));
    this->ui_info_page_repo_desc->setText(this->m_p_text->getLoc(this->ui_info_page_repo_desc->objectName() + "_txt"));
    this->ui_info_page_github_link->setIcon(QIcon(STR_IMG_PATH + "github.svg"));
    this->ui_info_page_github_link->setToolTip(this->m_p_text->getLoc(this->ui_info_page_github_link->objectName() + "_tip"));
    this->ui_info_page_doc_caption->setText(this->m_p_text->getLoc(this->ui_info_page_doc_caption->objectName() + "_txt"));
    this->ui_info_page_doc_desc->setText(this->m_p_text->getLoc(this->ui_info_page_doc_desc->objectName() + "_txt"));
    this->ui_info_page_doc_link->setIcon(QIcon(STR_IMG_PATH + "doc.svg"));
    this->ui_info_page_doc_link->setToolTip(this->m_p_text->getLoc(this->ui_info_page_doc_link->objectName() + "_tip"));
    // 程序信息
    this->ui_info_page_copyright_caption->setText(this->m_p_text->getLoc(this->ui_info_page_copyright_caption->objectName() + "_txt"));
    this->ui_info_page_copyright->setText(STR_APP_COPYRIGHT);
    this->ui_info_page_author_caption->setText(this->m_p_text->getLoc(this->ui_info_page_author_caption->objectName() + "_txt"));
    this->ui_info_page_author->setText(STR_APP_AUTHOR);
    this->ui_info_page_license_caption->setText(this->m_p_text->getLoc(this->ui_info_page_license_caption->objectName() + "_txt"));
    this->ui_info_page_license->setText(STR_APP_LICENSE);
    this->ui_info_page_license_link->setIcon(QIcon(STR_IMG_PATH + "link.svg"));
    this->ui_info_page_license_link->setToolTip(this->m_p_text->getLoc(this->ui_info_page_license_link->objectName() + "_tip"));
}

void UiInfoPage::initConnect()
{
    connect(this->ui_info_page_github_link, &QPushButton::clicked, this, [ = ] {QDesktopServices::openUrl(QUrl(STR_APP_GITHUB_URL));});
    connect(this->ui_info_page_license_link, &QPushButton::clicked, this, [ = ] {QDesktopServices::openUrl(QUrl(STR_APP_LICENSE_URL));});
}

void UiInfoPage::clearConnect()
{
    disconnect(this->ui_info_page_github_link, &QPushButton::clicked, this, nullptr);
    disconnect(this->ui_info_page_license_link, &QPushButton::clicked, this, nullptr);
}
