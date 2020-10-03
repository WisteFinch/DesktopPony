#include "desktoppony.h"

DesktopPony::DesktopPony()
{
    fileTasks = new FileTasks;
    config = new Config;
    localisation = new Localisation;
    fileCharacter = new FileCharacter;

    limit = new Limit;
    qss = new QSS;
    uiSettings = new UISettings;
    uiPony = new UIPony;
    imageProcessing = new ImageProcessing;
}

void DesktopPony::start()
{
    //imageProcessing->init("character");

    //读取文件
    config->read();

    bool textFlag = localisation->readLocalisation("zh-cn");

    //初始化指针
    //uiSettings->initThis(config, limit, qss, text);
    //uiPony->initThis(config, limit, qss, text, imageProcessing);

    if(!textFlag)
    {
        QMessageBox::warning(uiSettings, "错误", "无法读取文本", QMessageBox::Ok);
    }

    //连接信号
    //initConnect();
    //uiPony->display();
    //uiSettings->display();

    fileTasks->readAll();

    uiSettings->init(localisation, config, fileCharacter, fileTasks);
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

