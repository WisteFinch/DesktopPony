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

int Limit::widgetPosition(int x, int y)
{
    if(screenSizeX == 0 && screenSizeY == 0)
        getScreenSize();
    double horizontal = static_cast<double>(x) / static_cast<double>(screenSizeX);
    double vertical = static_cast<double>(y) / static_cast<double>(screenSizeY);
    if(horizontal >= widgetPositionHorizontal && x+widgetPositionDeviation1X+20 > 0 && x+widgetPositionDeviation4X+20 > 0)
    {
        if(vertical >= widgetPositionVertical)
            return 4;//右下
        else
            return 1;//右上
    }
    else
    {
        if(vertical >= widgetPositionVertical)
            return 3;//左下
        else
            return 2;//左上
    }
}


