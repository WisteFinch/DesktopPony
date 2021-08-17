#include "uimainpanel.h"
#include "ui_uimainpanel.h"

UiMainPanel::UiMainPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiMainPanel)
{
    ui->setupUi(this);
}

UiMainPanel::~UiMainPanel()
{
    this->m_p_conf = nullptr;
    this->m_p_style = nullptr;
    this->m_p_text = nullptr;
    this->m_p_plugin = nullptr;
    clearConnect();
    delete ui;
}

void UiMainPanel::closeEvent(QCloseEvent *event)
{
    // 忽略事件
    event->ignore();
    // 保存配置
    this->m_p_conf->save();
    // 测试：退出程序
    qApp->quit();
}

void UiMainPanel::init(Config *ptrconf, Style *ptrStyle, Text *ptrText, PluginManager *ptrPluginManager, Config::PTRFUNC_GET_CONFIG ptrGetConf)
{
    this->m_p_conf = ptrconf;
    this->m_p_style = ptrStyle;
    this->m_p_text = ptrText;
    this->m_p_plugin = ptrPluginManager;
    this->m_ptrfunc_get_conf = ptrGetConf;
    ptrconf = nullptr;
    ptrStyle = nullptr;
    ptrText = nullptr;
    ptrPluginManager = nullptr;

    this->setAttribute(Qt::WA_StyledBackground);
    initWidget();   // 初始化部件
    initObjectName();   // 初始化对象名
    initProperty(); // 初始化属性
    initContent();  // 初始化内容
    initConnect();  // 初始化信号槽
    slotChangeTab(1);   // 设置标签页为插件（临时）
    this->setStyleSheet(this->m_p_style->getQSS()); // 设置样式
}

void UiMainPanel::initWidget()
{
    this->ui_main_panel_layout_main = new QHBoxLayout;
    this->ui_main_panel_layout_main->setSpacing(5);
    this->setLayout(this->ui_main_panel_layout_main);
    // 选项卡
    this->ui_main_panel_layout_tab = new QVBoxLayout;
    this->ui_main_panel_layout_tab->setSpacing(0);
    this->ui_main_panel_tab_character = new QPushButton;
    this->ui_main_panel_tab_plugin = new QPushButton;
    this->ui_main_panel_tab_config = new QPushButton;
    this->ui_main_panel_tab_info = new QPushButton;
    this->ui_main_panel_logo = new QLabel;
    this->ui_main_panel_version = new QLabel;
    this->ui_main_panel_layout_tab->addWidget(this->ui_main_panel_tab_character);
    this->ui_main_panel_layout_tab->addWidget(this->ui_main_panel_tab_plugin);
    this->ui_main_panel_layout_tab->addWidget(this->ui_main_panel_tab_config);
    this->ui_main_panel_layout_tab->addWidget(this->ui_main_panel_tab_info);
    this->ui_main_panel_layout_tab->addStretch();
    this->ui_main_panel_layout_tab->addWidget(this->ui_main_panel_logo);
    this->ui_main_panel_layout_tab->addWidget(this->ui_main_panel_version);
    this->ui_main_panel_layout_main->addLayout(this->ui_main_panel_layout_tab);
    // 页
    this->ui_main_panel_pages = new AnimationStackedWidget;
    this->ui_main_panel_layout_main->addWidget(this->ui_main_panel_pages);
    this->ui_character_page = new UiCharacterPage;
    //this->ui_character_page->init(this->m_p_conf, this->m_p_text);
    this->ui_main_panel_pages->addWidget(this->ui_character_page);
    this->ui_plugin_page = new UiPluginPage;
    this->ui_plugin_page->init(this->m_p_conf, this->m_p_text, this->m_p_plugin);
    this->ui_main_panel_pages->addWidget(this->ui_plugin_page);
    this->ui_config_page = new UiConfigPage;
    this->ui_config_page->init(this->m_p_conf, this->m_p_text, this->m_p_plugin, this->m_ptrfunc_get_conf);
    this->ui_main_panel_pages->addWidget(this->ui_config_page);
    this->ui_info_page = new UiInfoPage;
    this->ui_info_page->init(this->m_p_conf, this->m_p_text);
    this->ui_main_panel_pages->addWidget(this->ui_info_page);
}

void UiMainPanel::initObjectName()
{
    this->setObjectName("ui_main_panel");
    // Widget部分
    this->ui_main_panel_pages->setObjectName("ui_main_panel_pages");
    this->ui_main_panel_tab_character->setObjectName("ui_main_panel_tab_character");
    this->ui_main_panel_tab_plugin->setObjectName("ui_main_panel_tab_plugin");
    this->ui_main_panel_tab_config->setObjectName("ui_main_panel_tab_config");
    this->ui_main_panel_tab_info->setObjectName("ui_main_panel_tab_info");
    this->ui_main_panel_logo->setObjectName("ui_main_panel_logo");
    this->ui_main_panel_version->setObjectName("ui_main_panel_version");

    // Layout部分
    this->ui_main_panel_layout_main->setObjectName("ui_main_panel_layout_main");
    this->ui_main_panel_layout_tab->setObjectName("ui_main_panel_layout_tab");
}

void UiMainPanel::initProperty()
{
    this->setProperty("category", "frame");
    // Widget部分
    this->ui_main_panel_pages->setProperty("category", "main_panel_stacked_widget");
    this->ui_main_panel_tab_character->setProperty("category", "main_panel_tab_buttton");
    this->ui_main_panel_tab_plugin->setProperty("category", "main_panel_tab_buttton");
    this->ui_main_panel_tab_config->setProperty("category", "main_panel_tab_buttton");
    this->ui_main_panel_tab_info->setProperty("category", "main_panel_tab_buttton");
    this->ui_main_panel_logo->setProperty("category", "main_panel_tab_label");
    this->ui_main_panel_version->setProperty("category", "main_panel_tab_label");
}

void UiMainPanel::initContent()
{
    this->setWindowTitle(this->m_p_text->getLoc(this->objectName() + "_tit"));

    this->ui_main_panel_tab_character->setText(this->m_p_text->getLoc(this->ui_main_panel_tab_character->objectName() + "_cap"));
    this->ui_main_panel_tab_plugin->setText(this->m_p_text->getLoc(this->ui_main_panel_tab_plugin->objectName() + "_cap"));
    this->ui_main_panel_tab_config->setText(this->m_p_text->getLoc(this->ui_main_panel_tab_config->objectName() + "_cap"));
    this->ui_main_panel_tab_info->setText(this->m_p_text->getLoc(this->ui_main_panel_tab_info->objectName() + "_cap"));

    this->ui_main_panel_version->setText(STR_APP_VERSION);
    this->ui_main_panel_version->setAlignment(Qt::AlignCenter);
}

void UiMainPanel::initConnect()
{
    // 标签页部分
    connect(this->ui_main_panel_tab_character, &QPushButton::clicked, this, [ = ] {slotChangeTab(0);});
    connect(this->ui_main_panel_tab_plugin, &QPushButton::clicked, this, [ = ] {slotChangeTab(1);});
    connect(this->ui_main_panel_tab_config, &QPushButton::clicked, this, [ = ] {slotChangeTab(2);});
    connect(this->ui_main_panel_tab_info, &QPushButton::clicked, this, [ = ] {slotChangeTab(3);});

    // 重启
    connect(this->ui_plugin_page, &UiPluginPage::sigReloadData, this, [ = ] {emit sigRestart();});
    connect(this->ui_config_page, &UiConfigPage::sigRestart, this, [ = ] {emit sigRestart();});
}

void UiMainPanel::clearConnect()
{
    // 标签页部分
    disconnect(this->ui_main_panel_tab_character, &QPushButton::clicked, this, nullptr);
    disconnect(this->ui_main_panel_tab_plugin, &QPushButton::clicked, this, nullptr);
    disconnect(this->ui_main_panel_tab_config, &QPushButton::clicked, this, nullptr);
    disconnect(this->ui_main_panel_tab_info, &QPushButton::clicked, this, nullptr);

    // 重启
    disconnect(this->ui_plugin_page, &UiPluginPage::sigReloadData, this, nullptr);
    disconnect(this->ui_config_page, &UiConfigPage::sigRestart, this, nullptr);
}

void UiMainPanel::slotChangeTab(int index)
{
    qint32 origIndex = this->ui_main_panel_pages->currentIndex();
    // 刷新页
    bool flag = this->ui_main_panel_pages->slotSetIndex(index);
    if(flag) {
        if(origIndex == 2 && this->ui_config_page->m_value_changed) {
            this->m_p_conf->save();
        }

        // 设置当前页序号
        this->m_page_index = index;
        // 重置属性
        this->ui_main_panel_tab_character->setProperty("selected", false);
        this->ui_main_panel_tab_plugin->setProperty("selected", false);
        this->ui_main_panel_tab_config->setProperty("selected", false);
        this->ui_main_panel_tab_info->setProperty("selected", false);
        // 指定属性
        switch(index) {
        case 0:
            this->ui_main_panel_tab_character->setProperty("selected", true);
            break;
        case 1:
            this->ui_main_panel_tab_plugin->setProperty("selected", true);
            break;
        case 2:
            this->ui_main_panel_tab_config->setProperty("selected", true);
            this->ui_config_page->m_value_changed = false;
            break;
        case 3:
            this->ui_main_panel_tab_info->setProperty("selected", true);
            break;
        }
        // 刷新样式
        this->ui_main_panel_tab_character->setStyle(QApplication::style());
        this->ui_main_panel_tab_plugin->setStyle(QApplication::style());
        this->ui_main_panel_tab_config->setStyle(QApplication::style());
        this->ui_main_panel_tab_info->setStyle(QApplication::style());
    }
}
