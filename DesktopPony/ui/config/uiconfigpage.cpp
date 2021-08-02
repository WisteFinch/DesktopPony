#include "uiconfigpage.h"
#include "ui_uiconfigpage.h"

UiConfigPage::UiConfigPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiConfigPage)
{
    ui->setupUi(this);
}

UiConfigPage::~UiConfigPage()
{
    delete ui;
}
