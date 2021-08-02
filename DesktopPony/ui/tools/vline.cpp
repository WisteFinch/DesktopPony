#include "vline.h"
#include "ui_vline.h"

VLine::VLine(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::VLine)
{
    ui->setupUi(this);
    this->setObjectName("v_line");
    this->setProperty("category", "tools");
}

VLine::~VLine()
{
    delete ui;
}
