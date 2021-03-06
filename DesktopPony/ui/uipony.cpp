#include "uipony.h"
#include "ui_uipony.h"

UIPony::UIPony(QWidget *parent) :
    QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool),
    ui(new Ui::UIPony)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

UIPony::~UIPony()
{
    delete ui;
}

void UIPony::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        //左键拖动窗体
        relativePos = this->pos() - event->globalPos();
    }
    if(event->button() == Qt::RightButton) {
        //右键打开菜单
        menuPony->exec(QCursor::pos());
    }
}
void UIPony::mouseMoveEvent(QMouseEvent *event)
{
    //移动窗体
    this->move(event->globalPos() + relativePos);
}

void UIPony::initSystemTray()
{
}

void UIPony::ponyPosWrite()
{
}

void UIPony::slotImagesChange()
{

}
void UIPony::slotEventHandle()
{

}

void UIPony::initMenu()
{


}
void UIPony::slotMenuQuit()
{

}
void UIPony::slotMenuSettings()
{


}
void UIPony::slotMenuInformaiton()
{

}
//连接信号
void UIPony::setConnect()
{

}
//设置布局
void UIPony::setItemsLayout()
{
    this->ponyLayout->addWidget(this->ponyLabel);
    this->layout->addLayout(ponyLayout);
    this->layout->addWidget(debugLabel);

    this->setLayout(layout);
}
//初始化组件
void UIPony::initItems()
{
    ponyLabel->setPixmap(this->imageProcessing->getFrame());
    debugLabel->setText("debug");
}
//显示
void UIPony::display()
{
    this->show();
}

void UIPony::initThis(Config *tempConfig, Limit *tempLimit, QSS *tempQSS, Localisation *tempLocalisation, ImageProcessing *tempImageProcessing)
{
    config = tempConfig;
    limit = tempLimit;
    qss = tempQSS;
    localisation = tempLocalisation;
    imageProcessing = tempImageProcessing;
    tempConfig = nullptr;
    tempLimit = nullptr;
    tempQSS = nullptr;
    tempLocalisation = nullptr;
    tempImageProcessing = nullptr;

//设置窗体
    this->setWindowTitle(localisation->get("uiSettingsTitle"));
    this->setFixedSize(this->width, this->height);
//设置布局
    setItemsLayout();
//初始化组件
    initItems();
//信号槽
    setConnect();
}
