#include "uicharacterpage.h"

UiCharacterPage::UiCharacterPage(QWidget *parent) :
    QWidget(parent)
{
    this->setAttribute(Qt::WA_StyledBackground);

    this->setProperty("category", "page");
}

UiCharacterPage::~UiCharacterPage()
{
}
