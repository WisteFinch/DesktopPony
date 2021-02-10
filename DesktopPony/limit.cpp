#include "limit.h"

Limit::Limit()
{

}

void Limit::getScreenSize()
{

    QDesktopWidget *desktop = QApplication::desktop();
    screenSizeX = desktop->width();
    screenSizeY = desktop->height();
}
