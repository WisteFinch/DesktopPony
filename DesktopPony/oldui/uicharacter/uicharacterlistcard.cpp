#include "uicharacterlistcard.h"

UICharacterListCard::UICharacterListCard(QWidget *parent) :
    QWidget(parent)
{
    this->m_p_layout_main = new QHBoxLayout;
    this->m_p_label_status = new QLabel;
    this->m_p_label_image = new QLabel;
    this->m_p_label_name = new QLabel;
}

UICharacterListCard::~UICharacterListCard()
{
    delete this->m_p_label_name;
    delete this->m_p_label_status;
    delete this->m_p_label_image;
    delete this->m_p_layout_main;
}

void UICharacterListCard::init(QString name, QString path)
{
    this->m_str_name = name;
    this->m_str_path = path;
    this->setFixedHeight(50);
    this->m_p_layout_main->setSpacing(0);
    this->m_p_layout_main->setMargin(0);
    this->m_p_label_status->setMaximumWidth(5);
    this->m_p_label_name->setWordWrap(true);
    this->m_p_layout_main->addWidget(this->m_p_label_status);
    this->m_p_label_image->setFixedSize(this->height(), this->height());
    this->m_p_layout_main->addWidget(this->m_p_label_image);
    this->m_p_layout_main->addWidget(this->m_p_label_name);
    this->m_p_layout_main->addStretch();
    this->setLayout(this->m_p_layout_main);
    this->setObjectName("uicharacter_list_card_widget");
    this->m_p_label_status->setObjectName("uicharacter_list_card_label_status");
    this->m_p_label_image->setObjectName("uicharacter_list_card_label_image");
    this->m_p_label_name->setObjectName("uicharacter_list_card_label_name");

    this->m_p_label_name->setText(name);
    setStatus(NORMAL);
}

void UICharacterListCard::setStatus(Status flag)
{
    this->m_e_status = flag;
    this->m_p_label_status->clear();
    QPalette p;
    switch (flag)
    {
    case NORMAL:
        p.setColor(QPalette::Background, QColor(38, 157, 206, 0));
        break;
    case HOVER:
        p.setColor(QPalette::Background, QColor(38, 157, 206, 50));
        break;
    case SELECTED:
        p.setColor(QPalette::Background, QColor(38, 157, 206, 150));
        break;
    case PRESS:
        p.setColor(QPalette::Background, QColor(38, 157, 206, 250));
        break;
    }
    this->m_p_label_status->setAutoFillBackground(true);
    this->m_p_label_status->setPalette(p);
}

void UICharacterListCard::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && this->m_e_status != SELECTED)
    {
        this->m_b_mouse_press = true;
        setStatus(PRESS);
    }
}
void UICharacterListCard::mouseReleaseEvent(QMouseEvent *event)
{
    if(this->m_b_mouse_press&&this->rect().contains(event->pos()))
    {
        this->m_b_mouse_press = false;
        emit clicked(this->m_str_name);
    }
}

void UICharacterListCard::enterEvent(QEvent *)
{
    if(this->m_e_status != SELECTED)
        setStatus(HOVER);
}

void UICharacterListCard::leaveEvent(QEvent *)
{
    if(this->m_e_status != SELECTED)
        setStatus(NORMAL);
}
