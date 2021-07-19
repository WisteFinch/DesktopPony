#include "tools.h"

Tools::Tools()
{

}

QString Tools::creatUuid()
{
    QUuid id = QUuid::createUuid();
    return id.toString().remove('{').remove('}').remove('-');
}

QString Tools::creatUuid16()
{
    return creatUuid().right(16);
}

void Tools::getScreenSize()
{

    QDesktopWidget *desktop = QApplication::desktop();
    screenSizeX = desktop->width();
    screenSizeY = desktop->height();
}
