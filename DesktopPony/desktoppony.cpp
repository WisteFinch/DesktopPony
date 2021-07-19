#include "desktoppony.h"

DesktopPony::DesktopPony()
{
    fileTasks = new FileTasks;
    config = new Config;
    fileCharacter = new FileCharacter;
    qss = new QSS;
    uiSettings = new UISettings;
    uiPony = new UIPony;
    imageProcessing = new ImageProcessing;

    this->m_p_tools = new Tools;
    this->m_p_plugin_manager = new PluginManager;
    this->m_p_localisation = new Localisation;
}

void DesktopPony::start()
{
    // 读取插件信息
    this->m_p_plugin_manager->refreshList();

    // 初始化数据
    this->m_p_localisation->init(std::bind(&PluginManager::getElementPairList, this->m_p_plugin_manager, std::placeholders::_1));

    //imageProcessing->init("character");

    //读取文件
    config->read();

    // 创建本地化文本索引
    this->m_p_localisation->setLanguage("zh-hans"); // <临时
    this->m_p_localisation->creatIndex();

    //初始化指针
    //uiSettings->initThis(config, limit, qss, text);
    //uiPony->initThis(config, limit, qss, text, imageProcessing);
    /*
        if(!textFlag) {
            QMessageBox::warning(uiSettings, "错误", "无法读取文本", QMessageBox::Ok);
        }
    */
    //连接信号
    //initConnect();
    //uiPony->display();
    //uiSettings->display();

    fileTasks->readAll();

    uiSettings->init(this->m_p_localisation, config, fileCharacter, fileTasks);
    uiSettings->show();
}

void DesktopPony::initConnect()
{
    connect(uiSettings, SIGNAL(signalBack(int)), this, SLOT(slotSettings(int)));
}
//“设置”返回信号
void DesktopPony::slotSettings(int solt)
{
    //if(solt == UISettings::closeAction){uiSettings->display();}
}

