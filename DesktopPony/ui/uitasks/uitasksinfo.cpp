#include "uitasksinfo.h"
#include "ui_uitasksinfo.h"

UITasksInfo::UITasksInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UITasksInfo)
{
    ui->setupUi(this);

    this->m_layout_main = new QVBoxLayout;

    this->m_scrollarea_info = new QScrollArea;
    this->m_widget_info = new QWidget;
    this->m_layout_info = new QVBoxLayout;

    this->m_layout_group_name = new QHBoxLayout;
    this->m_label_group_name = new QLabel;
    this->m_text_group_name = new QTextEdit;

    this->m_layout_group_path = new QHBoxLayout;
    this->m_label_group_path = new QLabel;
    this->m_text_group_path = new QTextEdit;

    this->m_layout_author = new QHBoxLayout;
    this->m_label_author = new QLabel;
    this->m_text_author = new QTextEdit;

    this->m_layout_desc = new QHBoxLayout;
    this->m_label_desc = new QLabel;
    this->m_text_desc = new QTextEdit;

    this->m_layout_standard = new QHBoxLayout;
    this->m_label_standard = new QLabel;
    this->m_text_standard = new QTextEdit;

    this->m_layout_group_trigger = new QHBoxLayout;
    this->m_label_group_trigger = new QLabel;
    this->m_text_group_trigger = new QTextEdit;

    this->m_layout_group_option = new QHBoxLayout;
    this->m_label_group_option = new QLabel;
    this->m_text_group_option = new QTextEdit;

    this->m_layout_events = new QHBoxLayout;
    this->m_label_events = new QLabel;
    this->m_text_events = new QTextEdit;

    //setAttribute(Qt::WA_ShowModal, true);    //设置模态
}

UITasksInfo::~UITasksInfo()
{
    delete this->m_layout_group_name;
    delete this->m_label_group_name;
    delete this->m_text_group_name;

    delete this->m_layout_group_path;
    delete this->m_label_group_path;
    delete this->m_text_group_path;

    delete this->m_layout_author;
    delete this->m_label_author;
    delete this->m_text_author;

    delete this->m_layout_desc;
    delete this->m_label_desc;
    delete this->m_text_desc;

    delete this->m_layout_standard;
    delete this->m_label_standard;
    delete this->m_text_standard;

    delete this->m_layout_group_trigger;
    delete this->m_label_group_trigger;
    delete this->m_text_group_trigger;

    delete this->m_layout_group_option;
    delete this->m_label_group_option;
    delete this->m_text_group_option;

    delete this->m_layout_events;
    delete this->m_label_events;
    delete this->m_text_events;

    delete this->m_layout_info;
    delete this->m_widget_info;
    delete this->m_scrollarea_info;

    delete this->m_layout_main;

    delete ui;

}

void UITasksInfo::init(Localisation *tmpLocalisation, FileTasks::STaskdata *tmpTaskdata, QImage bg, QSize size)
{
    this->m_p_localisation = tmpLocalisation;
    this->m_p_data = tmpTaskdata;
    tmpLocalisation = nullptr;
    tmpTaskdata = nullptr;

    this->width = size.width();
    this->height = size.height();

    this->background = bg;
    //=========================test============================

        QFile *a = new QFile;
        a->setFileName(":/css/default.css");
        a->open(QFile::ReadOnly);
        QString s = a->readAll();
        this->setStyleSheet(s);
        a->close();
        delete a;

    //=======================test end=========================

    this->setFixedSize(this->width, this->height);
    setBackGround();    //设置背景
    initObjectName();   //初始化Obj名

    this->m_layout_main->setMargin(0);

    this->m_widget_info->setLayout(this->m_layout_info);
    this->m_scrollarea_info->setWidget(this->m_widget_info);
    this->m_scrollarea_info->setWidgetResizable(true);
    this->m_layout_main->addWidget(this->m_scrollarea_info);

    this->m_layout_group_name->addWidget(this->m_label_group_name);
    this->m_label_group_name->setFixedWidth(100);
    this->m_text_group_name->setReadOnly(true);
    this->m_layout_group_name->addWidget(this->m_text_group_name);
    this->m_layout_info->addLayout(this->m_layout_group_name);

    this->m_layout_group_path->addWidget(this->m_label_group_path);
    this->m_label_group_path->setFixedWidth(100);
    this->m_text_group_path->setReadOnly(true);
    this->m_layout_group_path->addWidget(this->m_text_group_path);
    this->m_layout_info->addLayout(this->m_layout_group_path);

    this->m_layout_author->addWidget(this->m_label_author);
    this->m_label_author->setFixedWidth(100);
    this->m_text_author->setReadOnly(true);
    this->m_layout_author->addWidget(this->m_text_author);
    this->m_layout_info->addLayout(this->m_layout_author);

    this->m_layout_desc->addWidget(this->m_label_desc);
    this->m_label_desc->setFixedWidth(100);
    this->m_text_desc->setReadOnly(true);
    this->m_layout_desc->addWidget(this->m_text_desc);
    this->m_layout_info->addLayout(this->m_layout_desc);

    this->m_layout_standard->addWidget(this->m_label_standard);
    this->m_label_standard->setFixedWidth(100);
    this->m_text_standard->setReadOnly(true);
    this->m_layout_standard->addWidget(this->m_text_standard);
    this->m_layout_info->addLayout(this->m_layout_standard);

    this->m_layout_group_trigger->addWidget(this->m_label_group_trigger);
    this->m_label_group_trigger->setFixedWidth(100);
    this->m_text_group_trigger->setReadOnly(true);
    this->m_layout_group_trigger->addWidget(this->m_text_group_trigger);
    this->m_layout_info->addLayout(this->m_layout_group_trigger);

    this->m_layout_group_option->addWidget(this->m_label_group_option);
    this->m_label_group_option->setFixedWidth(100);
    this->m_text_group_option->setReadOnly(true);
    this->m_layout_group_option->addWidget(this->m_text_group_option);
    this->m_layout_info->addLayout(this->m_layout_group_option);

    this->m_layout_events->addWidget(this->m_label_events);
    this->m_label_events->setFixedWidth(100);
    this->m_text_events->setReadOnly(true);
    this->m_layout_events->addWidget(this->m_text_events);
    this->m_layout_info->addLayout(this->m_layout_events);

    this->m_layout_info->addStretch();

    this->setLayout(this->m_layout_main);

    initText();
}

void UITasksInfo::setBackGround()
{
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(QPixmap::fromImage(this->background)));
    this->setPalette(palette);
}

void UITasksInfo::initObjectName()
{
    this->setObjectName("uitasksinfo_widget");

    this->m_layout_main->setObjectName("uitasksinfo_layout_main");

    this->m_layout_info->setObjectName("uitasksinfo_layout_info");
    this->m_widget_info->setObjectName("uitasksinfo_widget_info");
    this->m_scrollarea_info->setObjectName("uitasksinfo_scrollarea_info");

    this->m_layout_group_name->setObjectName("uitasksinfo_layout_group_name");
    this->m_label_group_name->setObjectName("uitasksinfo_label_group_name");
    this->m_text_group_name->setObjectName("uitasksinfo_text_group_name");

    this->m_layout_group_path->setObjectName("uitasksinfo_layout_group_path");
    this->m_label_group_path->setObjectName("uitasksinfo_label_group_path");
    this->m_text_group_path->setObjectName("uitasksinfo_text_group_path");

    this->m_layout_author->setObjectName("uitasksinfo_layout_author");
    this->m_label_author->setObjectName("uitasksinfo_label_author");
    this->m_text_author->setObjectName("uitasksinfo_text_author");

    this->m_layout_desc->setObjectName("uitasksinfo_layout_desc");
    this->m_label_desc->setObjectName("uitasksinfo_label_desc");
    this->m_text_desc->setObjectName("uitasksinfo_text_desc");

    this->m_layout_standard->setObjectName("uitasksinfo_layout_standard");
    this->m_label_standard->setObjectName("uitasksinfo_label_standard");
    this->m_text_standard->setObjectName("uitasksinfo_text_standard");

    this->m_layout_group_trigger->setObjectName("uitasksinfo_layout_group_trigger");
    this->m_label_group_trigger->setObjectName("uitasksinfo_label_group_trigger");
    this->m_text_group_trigger->setObjectName("uitasksinfo_text_group_trigger");

    this->m_layout_group_option->setObjectName("uitasksinfo_layout_group_option");
    this->m_label_group_option->setObjectName("uitasksinfo_label_group_option");
    this->m_text_group_option->setObjectName("uitasksinfo_text_group_option");

    this->m_layout_events->setObjectName("uitasksinfo_layout_events");
    this->m_label_events->setObjectName("uitasksinfo_label_events");
    this->m_text_events->setObjectName("uitasksinfo_text_events");
}

void UITasksInfo::initText()
{
    this->setWindowTitle(this->m_p_localisation->get(this->objectName()));

    this->m_label_group_name->setText(this->m_p_localisation->get(this->m_label_group_name->objectName()));
    this->m_label_group_path->setText(this->m_p_localisation->get(this->m_label_group_path->objectName()));
    this->m_label_author->setText(this->m_p_localisation->get(this->m_label_author->objectName()));
    this->m_label_desc->setText(this->m_p_localisation->get(this->m_label_desc->objectName()));
    this->m_label_standard->setText(this->m_p_localisation->get(this->m_label_standard->objectName()));
    this->m_label_group_trigger->setText(this->m_p_localisation->get(this->m_label_group_trigger->objectName()));
    this->m_label_group_option->setText(this->m_p_localisation->get(this->m_label_group_option->objectName()));
    this->m_label_events->setText(this->m_p_localisation->get(this->m_label_events->objectName()));

    this->m_text_group_name->setText(this->m_p_data->groupName);
    this->m_text_group_path->setText(this->m_p_data->groupPath);
    this->m_text_author->setText(this->m_p_data->author);
    this->m_text_desc->setText(this->m_p_data->desc);
    this->m_text_standard->setText(this->m_p_data->standard);
    this->m_text_group_trigger->setText(this->m_p_data->groupTrigger);
    this->m_text_group_option->setText(this->m_p_data->groupOption);
    QString str;
    QStringList keys = this->m_p_data->events.keys();
    for(int i = 0; i < keys.size(); i++)
    {
        FileTasks::STaskdata::STaskEvent *event = this->m_p_data->events.value(keys.at(i));
        str.append("{\n\tname: " + event->name + ";\n");
        str.append("\tpath: " + event->path + ";\n");
        str.append("\ttrigger: {" + event->trigger + "};\n");
        str.append("\toption: {" + event->option + "};\n};\n");
    }
    this->m_text_events->setText(this->m_p_data->groupOption);
}
