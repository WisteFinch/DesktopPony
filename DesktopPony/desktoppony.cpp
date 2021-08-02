#include "desktoppony.h"

DesktopPony::DesktopPony()
{
}

DesktopPony::~DesktopPony()
{
    clearConnect();
    clearUi();
    clearData();
}

void DesktopPony::start()
{
    initData();
    initUi();
    initConnect();
    this->m_p_ui_main_panel->show();
}

void DesktopPony::initData()
{
    this->m_p_plugin_manager = new PluginManager;
    this->m_p_config = new Config;
    this->m_p_localisation = new Localisation;
    this->m_p_style = new Style;
    this->m_p_text = new Text;

    // 读取插件信息
    this->m_p_plugin_manager->refreshList();

    // 初始化数据
    this->m_ptrfun_get_element_pair_list = std::bind(&PluginManager::getElementPairList, this->m_p_plugin_manager, std::placeholders::_1);
    this->m_p_config->init(this->m_ptrfun_get_element_pair_list);
    this->m_p_localisation->init(this->m_ptrfun_get_element_pair_list);
    this->m_p_style->init(this->m_ptrfun_get_element_pair_list);
    this->m_p_text->init(this->m_p_localisation);

    //读取配置文件
    this->m_p_config->load();

    // 创建本地化文本索引
    this->m_p_localisation->setLanguage("zh-hans"); // 临时
    this->m_p_localisation->creatIndex();

    // 设置样式
    this->m_p_style->setStyleName("default");   // 临时
    this->m_p_style->refreshStyle();
}

void DesktopPony::clearData()
{
    this->m_ptrfun_get_element_pair_list = nullptr;
    delete this->m_p_text;
    delete this->m_p_config;
    delete this->m_p_localisation;
    delete this->m_p_style;
    delete this->m_p_plugin_manager;
}

void DesktopPony::initUi()
{
    this->m_p_ui_main_panel = new UiMainPanel;
    this->m_p_ui_main_panel->init(this->m_p_config, this->m_p_style, this->m_p_text, this->m_p_plugin_manager);
}

void DesktopPony::clearUi()
{
    delete this->m_p_ui_main_panel;
}

void DesktopPony::initConnect()
{
    // 重载数据
    connect(this->m_p_ui_main_panel, &UiMainPanel::sigReloadData, this, &DesktopPony::slotReloadData);
}

void DesktopPony::clearConnect()
{
    // 重载数据
    disconnect(this->m_p_ui_main_panel, &UiMainPanel::sigReloadData, this, &DesktopPony::slotReloadData);
}

void DesktopPony::slotReloadData()
{
    clearConnect();
    clearUi();
    clearData();
    start();
}
