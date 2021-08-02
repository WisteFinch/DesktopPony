#include "uitaskseventcard.h"

UITasksEventCard::UITasksEventCard()
{
    this->m_label_title = new QLabel;
    this->m_textedit_text = new QTextEdit;
    this->m_layout_main = new QHBoxLayout;
}

UITasksEventCard::~UITasksEventCard()
{
    delete this->m_label_title;
    delete this->m_textedit_text;
    delete this->m_layout_main;
}

void UITasksEventCard::init(QString title, QString text)
{
    this->m_label_title->setText("");
}
