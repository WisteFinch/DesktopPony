#include "vline.h"

VLine::VLine(QWidget *parent) :
    QFrame(parent)
{
    this->setObjectName(QStringLiteral("v_line"));
    this->setProperty("category", QStringLiteral("tools"));
}

VLine::~VLine()
{
}
