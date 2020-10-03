#include "uitasks.h"
#include "ui_uitasks.h"

UITasks::UITasks(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UITasks)
{
    ui->setupUi(this);

    this->m_layout_main = new QHBoxLayout();

    this->m_layout_button = new QVBoxLayout();
    this->m_button_info = new QPushButton();
    this->m_button_switch = new QPushButton();
    this->m_button_refresh = new QPushButton();
    this->m_button_exit = new QPushButton();

    this->m_scrollarea_tasks = new QScrollArea();
    this->m_widget_tasks = new QWidget();
    this->m_layout_tasks = new QVBoxLayout();
    this->m_scrollarea_standards = new QScrollArea();
    this->m_widget_standards = new QWidget();
    this->m_layout_standards = new QVBoxLayout();

    this->m_list_card = new QList<UITasksListCard*>;
    this->m_list_standard_card = new QList<UITasksStandardListCard*>;

    setAttribute(Qt::WA_ShowModal, true);    //设置模态
}

UITasks::~UITasks()
{
    doClearList();
    doClearStandardList();

    delete this->m_layout_tasks;
    delete this->m_widget_tasks;
    delete this->m_scrollarea_tasks;
    delete this->m_layout_standards;
    delete this->m_widget_standards;
    delete this->m_scrollarea_standards;
    delete this->m_button_info;
    delete this->m_button_switch;
    delete this->m_button_refresh;
    delete this->m_button_exit;
    delete this->m_layout_button;
    delete this->m_layout_main;
    delete ui;
}

void UITasks::closeEvent(QCloseEvent *event)
{
    event->ignore();
    emit signalBack(nullptr);
}

void UITasks::init(Localisation *tmpLocalisation, Config *tmpConfig, FileTasks *tmpTasks, QImage bg, QPoint pos, QSize size)
{
    this->m_p_localisation = tmpLocalisation;
    this->m_p_config = tmpConfig;
    this->m_p_tasks = tmpTasks;
    tmpLocalisation = nullptr;
    tmpConfig = nullptr;
    tmpTasks = nullptr;
    this->width = size.width();
    this->height = size.height();

    this->background = bg;
    //=========================test============================

        QFile *a = new QFile;
        a->setFileName("a.css");
        a->open(QFile::ReadOnly);
        QString s = a->readAll();
        this->setStyleSheet(s);
        a->close();
        delete a;

    //=======================test end=========================

    this->setFixedSize(this->width, this->height);
    setBackGround();    //设置背景
    initObjectName();   //初始化Obj名

    this->m_layout_main->setMargin(5);

    this->m_layout_standards->setSpacing(0);
    this->m_widget_standards->setLayout(this->m_layout_standards);
    this->m_layout_standards->setMargin(0);
    this->m_scrollarea_standards->setWidget(this->m_widget_standards);
    this->m_scrollarea_standards->setWidgetResizable(true);
    this->m_scrollarea_standards->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->m_scrollarea_standards->setFixedWidth(85);
    this->m_layout_button->addWidget(this->m_scrollarea_standards);

    this->m_layout_button->addStretch();
    this->m_layout_button->addWidget(this->m_button_info);
    this->m_layout_button->addWidget(this->m_button_switch);
    this->m_layout_button->addWidget(this->m_button_refresh);
    this->m_layout_button->addSpacing(5);
    this->m_layout_button->addWidget(this->m_button_exit);
    this->m_layout_button->setSpacing(0);
    this->m_layout_main->addLayout(this->m_layout_button);

    this->m_layout_tasks->setSpacing(0);
    this->m_widget_tasks->setLayout(this->m_layout_tasks);
    this->m_layout_tasks->setMargin(0);
    this->m_scrollarea_tasks->setWidget(this->m_widget_tasks);
    this->m_scrollarea_tasks->setWidgetResizable(true);
    this->m_layout_main->addWidget(this->m_scrollarea_tasks);

    this->m_layout_main->setSpacing(0);
    this->setLayout(this->m_layout_main);

    initText(); //初始化文本
    initConnect();  //初始化连接
    this->m_p_selected_task = nullptr;
    this->m_str_selected_standard = nullptr;
    doLoadStandardList();   //读取标准列表
    doLoadList();   //读取列表
    //slotListChange(this->m_list_task->currentItem());

    this->move(pos);
}

void UITasks::initObjectName()
{
    this->setObjectName("uitasks_widget");

    this->m_layout_main->setObjectName("uitasks_layout_main");
    this->m_scrollarea_tasks->setObjectName("uitasks_scrollarea_tasks");
    this->m_widget_tasks->setObjectName("uitasks_widget_tasks");
    this->m_layout_tasks->setObjectName("uitasks_layout_tasks");
    this->m_scrollarea_standards->setObjectName("uitasks_scrollarea_standards");
    this->m_widget_standards->setObjectName("uitasks_widget_standards");
    this->m_layout_standards->setObjectName("uitasks_layout_standards");
    this->m_layout_button->setObjectName("uitasks_layout_button");
    this->m_button_info->setObjectName("uitasks_button_info");
    this->m_button_switch->setObjectName("uitasks_button_switch");
    this->m_button_refresh->setObjectName("uitasks_button_refresh");
    this->m_button_exit->setObjectName("uitasks_button_exit");
}

void UITasks::initText()
{
    this->setWindowTitle(this->m_p_localisation->get(this->objectName()));

    this->m_button_info->setText(this->m_p_localisation->get(this->m_button_info->objectName()));
    this->m_button_switch->setText(this->m_p_localisation->get(this->m_button_switch->objectName()));
    this->m_button_refresh->setText(this->m_p_localisation->get(this->m_button_refresh->objectName()));
    this->m_button_exit->setText(this->m_p_localisation->get(this->m_button_exit->objectName()));

}

void UITasks::initConnect()
{
    //connect(this->m_list_task, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(slotListChange(QListWidgetItem*)));
    //connect(this->m_button_info, SIGNAL(clicked()), this, SLOT(slotAddtasks()));
    //connect(this->m_button_delete, SIGNAL(clicked()), this, SLOT(slotDeletetasks()));
    //connect(this->m_button_refresh, SIGNAL(clicked()), this, SLOT(slotRefreshList()));

    connect(this->m_button_info, SIGNAL(clicked()), this, SLOT(slotShowInfo()));
    connect(this->m_button_refresh, SIGNAL(clicked()), this, SLOT(slotRefreshList()));
    connect(this->m_button_exit, SIGNAL(clicked()), this, SLOT(slotExit()));
}

void UITasks::setBackGround()
{
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(QPixmap::fromImage(this->background)));
    this->setPalette(palette);
}

void UITasks::doLoadStandardList()
{
    doClearStandardList();
    QStringList keys = this->m_p_tasks->data->keys();
    for(int i = 0; i < keys.size(); i++)
    {
        UITasksStandardListCard *card = new UITasksStandardListCard;
        card->init(keys.at(i));
        this->m_list_standard_card->push_back(card);
        this->m_layout_standards->addWidget(card);
        connect(card, SIGNAL(clicked(QString)), this, SLOT(slotStandardListSelected(QString)));
    }
    this->m_layout_standards->addStretch();

    if(this->m_list_standard_card->size() != 0)
        this->slotStandardListSelected(this->m_list_standard_card->at(0)->m_str_name);
}

void UITasks::doLoadList()
{
    doClearList();

    QStringList keys = this->m_p_tasks->data->value(this->m_str_selected_standard)->keys();
    QMap<QString, FileTasks::STaskdata*> *p = this->m_p_tasks->data->value(this->m_str_selected_standard);
    for(int i = 0; i < keys.size(); i++)
    {
        UITasksListCard *card = new UITasksListCard;
        card->init(p->value(keys.at(i)));
        this->m_list_card->push_back(card);
        this->m_layout_tasks->addWidget(card);
        connect(card, SIGNAL(clicked(FileTasks::STaskdata*)), this, SLOT(slotListSelected(FileTasks::STaskdata*)));
    }
    this->m_layout_tasks->addStretch();

    if(keys.size() != 0)
        this->slotListSelected(p->value(keys.at(0)));
}

void UITasks::doClearList()
{
    QLayoutItem *c;
    while ((c = this->m_layout_tasks->takeAt(0)) != nullptr)
    {
           //setParent为NULL，防止删除之后界面不消失
           if(c->widget())
           {
               c->widget()->setParent(nullptr);
           }

           delete c;
   }
    for(int i = 0; i < this->m_list_card->size(); i++)
    {
        disconnect(this->m_list_card->at(i), SIGNAL(clicked(FileTasks::STaskdata*)), this, SLOT(slotListSelected(FileTasks::STaskdata*)));
        this->m_list_card->at(i)->setParent(nullptr);
        this->m_layout_tasks->removeWidget(this->m_list_card->at(i));
        delete this->m_list_card->at(i);
    }
    delete this->m_list_card;
    this->m_list_card = new QList<UITasksListCard*>;
}

void UITasks::doClearStandardList()
{
    QLayoutItem *c;
    while ((c = this->m_layout_standards->takeAt(0)) != nullptr)
    {
           //setParent为NULL，防止删除之后界面不消失
           if(c->widget())
           {
               c->widget()->setParent(nullptr);
           }

           delete c;
   }
    for(int i = 0; i < this->m_list_standard_card->size(); i++)
    {
        disconnect(this->m_list_standard_card->at(i), SIGNAL(clicked(QString)), this, SLOT(slotStandardListSelected(QString)));
        this->m_list_standard_card->at(i)->setParent(nullptr);
        this->m_layout_standards->removeWidget(this->m_list_standard_card->at(i));
        delete this->m_list_standard_card->at(i);
    }
    delete this->m_list_standard_card;
    this->m_list_standard_card = new QList<UITasksStandardListCard*>;
}


/**********Public slots**********/

void UITasks::slotExit()
{
    emit signalBack(nullptr);
}

void UITasks::slotRefreshList()
{
    this->m_p_selected_task = nullptr;
    this->m_p_tasks->clear();
    this->m_p_tasks->readAll();
    doLoadStandardList();
    doLoadList();
}

void UITasks::slotListSelected(FileTasks::STaskdata *p)
{
    this->m_p_selected_task = p;
    for(int i = 0; i < this->m_list_card->size(); i++)
    {
        if(this->m_list_card->at(i)->m_p_taskdata->groupName == p->groupName)
        {
            this->m_list_card->at(i)->setStatus(UITasksListCard::SELECTED);
        }else
        {
            if(this->m_list_card->at(i)->m_e_status == UITasksListCard::SELECTED)
                this->m_list_card->at(i)->setStatus(UITasksListCard::NORMAL);
        }
    }
}


void UITasks::slotStandardListSelected(QString str)
{
    this->m_str_selected_standard = str;
    for(int i = 0; i < this->m_list_standard_card->size(); i++)
    {
        if(this->m_list_standard_card->at(i)->m_str_name == str)
        {
            this->m_list_standard_card->at(i)->setStatus(UITasksStandardListCard::SELECTED);
        }else
        {
            if(this->m_list_standard_card->at(i)->m_e_status == UITasksStandardListCard::SELECTED)
                this->m_list_standard_card->at(i)->setStatus(UITasksStandardListCard::NORMAL);
        }
    }

    doLoadList();
}

void UITasks::slotShowInfo()
{
    UITasksInfo *i = new UITasksInfo;
    i->init(this->m_p_localisation, this->m_p_selected_task, this->background, this->size());
    i->show();
}

