#include "vline.h"
#include "ui_vline.h"

VLine::VLine(QWidget *parent) :
    QFrame(parent)
{
    this->setObjectName("v_line");
    this->setProperty("category", "tools");
}

VLine::~VLine()
{
}
