#include "uitaskslistcard.h"

UITasksListCard::UITasksListCard(QWidget *parent) :
    QWidget(parent)
{
    this->m_p_layout_main = new QHBoxLayout;
    this->m_p_label_status = new QLabel;
    this->m_p_label_image = new QLabel;
    this->m_p_label_name = new QLabel;

    this->m_p_taskdata = nullptr;
}

UITasksListCard::~UITasksListCard()
{
    delete this->m_p_label_name;
    delete this->m_p_label_status;
    delete this->m_p_label_image;
    delete this->m_p_layout_main;
}

void UITasksListCard::init(FileTasks::STaskdata *p)
{
    this->m_p_taskdata = p;
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
    this->setObjectName("uitasks_list_card_widget");
    this->m_p_label_status->setObjectName("uitasks_list_card_label_status");
    this->m_p_label_image->setObjectName("uitasks_list_card_label_image");
    this->m_p_label_name->setObjectName("uitasks_list_card_label_name");

    this->m_p_label_name->setText(this->m_p_taskdata->groupName);
    setStatus(NORMAL);
}

void UITasksListCard::setStatus(Status flag)
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

void UITasksListCard::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && this->m_e_status != SELECTED)
    {
        this->m_b_mouse_press = true;
        setStatus(PRESS);
    }
}
void UITasksListCard::mouseReleaseEvent(QMouseEvent *event)
{
    if(this->m_b_mouse_press&&this->rect().contains(event->pos()))
    {
        this->m_b_mouse_press = false;
        emit clicked(this->m_p_taskdata);
    }
}

void UITasksListCard::enterEvent(QEvent *)
{
    if(this->m_e_status != SELECTED)
        setStatus(HOVER);
}

void UITasksListCard::leaveEvent(QEvent *)
{
    if(this->m_e_status != SELECTED)
        setStatus(NORMAL);
}
