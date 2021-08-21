#include "hline.h"

HLine::HLine(QWidget *parent) :
    QFrame(parent)
{
    this->setObjectName(QStringLiteral("h_line"));
    this->setProperty("category", QStringLiteral("tools"));
}

HLine::~HLine()
{
}
