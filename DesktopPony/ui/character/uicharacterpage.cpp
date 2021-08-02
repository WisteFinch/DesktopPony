#include "uicharacterpage.h"
#include "ui_uicharacterpage.h"

UiCharacterPage::UiCharacterPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UiCharacterPage)
{
    ui->setupUi(this);
}

UiCharacterPage::~UiCharacterPage()
{
    delete ui;
}
