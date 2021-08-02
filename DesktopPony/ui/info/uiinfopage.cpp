#include "uiinfopage.h"
#include "ui_uiinfopage.h"

UiInfoPage::UiInfoPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiInfoPage)
{
    ui->setupUi(this);
}

UiInfoPage::~UiInfoPage()
{
    delete ui;
}
