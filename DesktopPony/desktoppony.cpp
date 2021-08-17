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
    this->m_ptrfunc_get_element_pair_list = std::bind(&PluginManager::getElementPairList, this->m_p_plugin_manager, std::placeholders::_1);
    this->m_p_config->init(this->m_ptrfunc_get_element_pair_list, this->m_p_plugin_manager->getLangList());
    this->m_p_localisation->init(this->m_ptrfunc_get_element_pair_list);
    this->m_p_style->init(this->m_ptrfunc_get_element_pair_list);
    this->m_p_text->init(this->m_p_localisation);

    //读取配置文件
    this->m_p_config->load();
    this->m_ptrfunc_get_conf = std::bind(&Config::get, this->m_p_config, std::placeholders::_1);

    // 创建本地化文本索引
    this->m_p_localisation->setLanguage(this->m_p_config->get("sys_lang").toString());
    this->m_p_localisation->creatIndex();

    // 设置样式
    this->m_p_style->setStyleName(this->m_p_config->get("sys_style").toString());
    this->m_p_style->refreshStyle();
}

void DesktopPony::clearData()
{
    this->m_ptrfunc_get_element_pair_list = nullptr;
    delete this->m_p_text;
    delete this->m_p_config;
    delete this->m_p_localisation;
    delete this->m_p_style;
    delete this->m_p_plugin_manager;
}

void DesktopPony::initUi()
{
    this->m_p_ui_main_panel = new UiMainPanel;
    this->m_p_ui_main_panel->init(this->m_p_config, this->m_p_style, this->m_p_text, this->m_p_plugin_manager, this->m_ptrfunc_get_conf);
}

void DesktopPony::clearUi()
{
    this->m_p_ui_main_panel->deleteLater();
}

void DesktopPony::initConnect()
{
    // 重载数据
    connect(this->m_p_ui_main_panel, &UiMainPanel::sigRestart, this, &DesktopPony::slotReloadData);
}

void DesktopPony::clearConnect()
{
    // 重载数据
    disconnect(this->m_p_ui_main_panel, &UiMainPanel::sigRestart, this, &DesktopPony::slotReloadData);
}

void DesktopPony::slotReloadData()
{
    this->m_p_config->save();
    clearConnect();
    clearUi();
    clearData();
    start();
}
