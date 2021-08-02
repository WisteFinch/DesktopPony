#include "hline.h"
#include "ui_hline.h"

HLine::HLine(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HLine)
{
    ui->setupUi(this);
    this->setObjectName("h_line");
    this->setProperty("category", "tools");
}

HLine::~HLine()
{
    delete ui;
}
