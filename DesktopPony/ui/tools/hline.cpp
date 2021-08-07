#include "hline.h"
#include "ui_hline.h"

HLine::HLine(QWidget *parent) :
    QFrame(parent)
{
    this->setObjectName("h_line");
    this->setProperty("category", "tools");
}

HLine::~HLine()
{
}
