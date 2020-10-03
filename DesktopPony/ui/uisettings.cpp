#include "uisettings.h"
#include "ui_uisettings.h"

UISettings::UISettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UISettings)
{
    ui->setupUi(this);

    this->m_layout_main = new QHBoxLayout();

    this->m_layout_tab = new QVBoxLayout();
    this->m_stackedwidget_main = new QStackedWidget();
    //第一页：角色
    this->m_button_tab_character = new QPushButton();
    this->m_scrollarea_character = new QScrollArea();
    this->m_layout_character = new QVBoxLayout();
    this->m_widget_character = new QWidget();
    this->m_layout_character_switch = new QHBoxLayout();    //“选择角色”
    this->m_layout_character_switch_label = new QVBoxLayout();
    this->m_label_character_switch_title = new QLabel();
    this->m_label_character_switch_content = new QLabel();
    this->m_button_character_switch = new QPushButton();
    this->m_layout_tasks = new QHBoxLayout();    //“任务”
    this->m_layout_tasks_label = new QVBoxLayout();
    this->m_label_tasks_title = new QLabel();
    this->m_label_tasks_content = new QLabel();
    this->m_button_tasks = new QPushButton();
    //第二页：常规
    this->m_button_tab_general = new QPushButton();
    this->m_scrollarea_general = new QScrollArea();
    this->m_layout_general = new QVBoxLayout();
    this->m_widget_general = new QWidget();
    this->m_layout_general_option_character_size = new QHBoxLayout();//“选项-角色大小”
    this->m_layout_general_option_character_size_label = new QVBoxLayout();
    this->m_label_general_option_character_size_title = new QLabel();
    this->m_label_general_option_character_size_content = new QLabel();
    this->m_lineedit_general_option_character_size = new QLineEdit();
    this->m_layout_general_constant_gravitational_acceleration = new QHBoxLayout();//“常量-重力加速度”
    this->m_layout_general_constant_gravitational_acceleration_label = new QVBoxLayout();
    this->m_label_general_constant_gravitational_acceleration_title = new QLabel();
    this->m_label_general_constant_gravitational_acceleration_content = new QLabel();
    this->m_lineedit_general_constant_gravitational_acceleration = new QLineEdit();
    this->m_layout_general_constant_obstruction = new QHBoxLayout();//“常量-阻力”
    this->m_layout_general_constant_obstruction_label = new QVBoxLayout();
    this->m_label_general_constant_obstruction_title = new QLabel();
    this->m_label_general_constant_obstruction_content = new QLabel();
    this->m_lineedit_general_constant_obstruction = new QLineEdit();
    //第三页：信息
    this->m_button_tab_info = new QPushButton();
    this->m_scrollarea_info = new QScrollArea();
    this->m_layout_info = new QVBoxLayout();
    this->m_widget_info = new QWidget();
    this->m_layout_info_desc = new QHBoxLayout();//“介绍”
    this->m_layout_info_desc_label = new QVBoxLayout();
    this->m_label_info_desc_title = new QLabel();
    this->m_label_info_desc_content = new QLabel();
    this->m_button_info_desc = new QPushButton();
    this->m_layout_info_version = new QHBoxLayout();//“版本”
    this->m_layout_info_version_label = new QVBoxLayout();
    this->m_label_info_version_title = new QLabel();
    this->m_label_info_version_content = new QLabel();
    this->m_button_info_update = new QPushButton();
    this->m_layout_info_doc = new QHBoxLayout();//“文档”
    this->m_layout_info_doc_label = new QVBoxLayout();
    this->m_label_info_doc_title = new QLabel();
    this->m_label_info_doc_content = new QLabel();
    this->m_button_info_doc = new QPushButton();
    this->m_layout_info_log = new QHBoxLayout();//“日志”
    this->m_layout_info_log_label = new QVBoxLayout();
    this->m_label_info_log_title = new QLabel();
    this->m_label_info_log_content = new QLabel();
    this->m_button_info_log = new QPushButton();
    this->m_label_info_copyright = new QLabel();//“版权”

}

UISettings::~UISettings()
{
    delete ui;
}

void UISettings::closeEvent(QCloseEvent *event)
{
    saveValue();
    //event->ignore();
    this->hide();

    qApp->quit();
    //emit signalBack(closeAction);

}

void UISettings::init(Localisation *tmpLocalisation, Config *tmpConfig, FileCharacter *tmpFileCharacter, FileTasks *tmpFileTasks)
{
    this->m_p_localisation = tmpLocalisation;
    this->m_p_config = tmpConfig;
    this->m_p_file_character = tmpFileCharacter;
    this->m_p_file_tasks = tmpFileTasks;
    tmpLocalisation = nullptr;
    tmpConfig = nullptr;
    tmpFileCharacter = nullptr;
    tmpFileTasks = nullptr;

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
    setBackGround();
    initObjectName();

    this->m_stackedwidget_main->addWidget(this->m_scrollarea_character);
    this->m_stackedwidget_main->addWidget(this->m_scrollarea_general);
    this->m_stackedwidget_main->addWidget(this->m_scrollarea_info);

    this->m_layout_main->setMargin(5);
    this->m_layout_main->addLayout(this->m_layout_tab);
    this->m_layout_main->addWidget(this->m_stackedwidget_main);
    this->m_layout_main->setSpacing(0);

    this->m_layout_tab->addWidget(this->m_button_tab_character);
    this->m_layout_tab->addWidget(this->m_button_tab_general);
    this->m_layout_tab->addWidget(this->m_button_tab_info);
    this->m_layout_tab->addStretch();
    this->m_layout_tab->setSpacing(0);

    //第一页：角色
    this->m_scrollarea_character->setFocusPolicy(Qt::NoFocus);
    this->m_scrollarea_character->setWidgetResizable(true);

    this->m_layout_character_switch_label->addWidget(this->m_label_character_switch_title);   //“选择角色”
    this->m_layout_character_switch_label->addWidget(this->m_label_character_switch_content);
    this->m_layout_character_switch_label->setSpacing(2);
    this->m_layout_character_switch->addLayout(this->m_layout_character_switch_label);
    this->m_button_character_switch->setFixedSize(30, 30);
    this->m_layout_character_switch->addWidget(this->m_button_character_switch);
    this->m_layout_character->addLayout(this->m_layout_character_switch);
    this->m_layout_character->addWidget(horizontalLine());
    this->m_layout_tasks_label->addWidget(this->m_label_tasks_title);   //“任务”
    this->m_layout_tasks_label->addWidget(this->m_label_tasks_content);
    this->m_layout_tasks_label->setSpacing(2);
    this->m_layout_tasks->addLayout(this->m_layout_tasks_label);
    this->m_button_tasks->setFixedSize(30, 30);
    this->m_layout_tasks->addWidget(this->m_button_tasks);
    this->m_layout_character->addLayout(this->m_layout_tasks);
    this->m_layout_character->addWidget(horizontalLine());

    this->m_layout_character->addStretch();
    this->m_layout_character->setSpacing(10);
    this->m_widget_character->setLayout(this->m_layout_character);
    this->m_scrollarea_character->setWidget(m_widget_character);
    //第二页：常规
    this->m_scrollarea_general->setFocusPolicy(Qt::NoFocus);
    this->m_scrollarea_general->setWidgetResizable(true);

    this->m_layout_general_option_character_size_label->addWidget(this->m_label_general_option_character_size_title);   //“选项-角色大小”
    this->m_layout_general_option_character_size_label->addWidget(this->m_label_general_option_character_size_content);
    this->m_layout_general_option_character_size_label->setSpacing(2);
    this->m_layout_general_option_character_size->addLayout(this->m_layout_general_option_character_size_label);
    this->m_lineedit_general_option_character_size->setFixedSize(100, 20);
    this->m_lineedit_general_option_character_size->setAlignment(Qt::AlignBottom);
    this->m_lineedit_general_option_character_size->setValidator(new QRegExpValidator(QRegExp("^[0-9]*[1-9][0-9]*$"), this->m_lineedit_general_option_character_size));
    this->m_layout_general_option_character_size->addWidget(this->m_lineedit_general_option_character_size);
    this->m_layout_general->addLayout(this->m_layout_general_option_character_size);
    this->m_layout_general->addWidget(horizontalLine());
    this->m_layout_general_constant_gravitational_acceleration_label->addWidget(this->m_label_general_constant_gravitational_acceleration_title);   //“常量-重力加速度”
    this->m_layout_general_constant_gravitational_acceleration_label->addWidget(this->m_label_general_constant_gravitational_acceleration_content);
    this->m_layout_general_constant_gravitational_acceleration_label->setSpacing(2);
    this->m_layout_general_constant_gravitational_acceleration->addLayout(this->m_layout_general_constant_gravitational_acceleration_label);
    this->m_lineedit_general_constant_gravitational_acceleration->setFixedSize(100, 20);
    this->m_lineedit_general_constant_gravitational_acceleration->setAlignment(Qt::AlignBottom);
    this->m_lineedit_general_constant_gravitational_acceleration->setValidator(new QRegExpValidator(QRegExp("^\\d+(\\.\\d+)?$"), this->m_lineedit_general_constant_gravitational_acceleration));
    this->m_layout_general_constant_gravitational_acceleration->addWidget(this->m_lineedit_general_constant_gravitational_acceleration);
    this->m_layout_general->addLayout(this->m_layout_general_constant_gravitational_acceleration);
    this->m_layout_general->addWidget(horizontalLine());
    this->m_layout_general_constant_obstruction_label->addWidget(this->m_label_general_constant_obstruction_title);   //“常量-阻力”
    this->m_layout_general_constant_obstruction_label->addWidget(this->m_label_general_constant_obstruction_content);
    this->m_layout_general_constant_obstruction_label->setSpacing(2);
    this->m_layout_general_constant_obstruction->addLayout(this->m_layout_general_constant_obstruction_label);
    this->m_lineedit_general_constant_obstruction->setFixedSize(100, 20);
    this->m_lineedit_general_constant_obstruction->setAlignment(Qt::AlignBottom);
    this->m_lineedit_general_constant_obstruction->setValidator(new QRegExpValidator(QRegExp("^\\d+(\\.\\d+)?$"), this->m_lineedit_general_constant_obstruction));
    this->m_layout_general_constant_obstruction->addWidget(this->m_lineedit_general_constant_obstruction);
    this->m_layout_general->addLayout(this->m_layout_general_constant_obstruction);

    this->m_layout_general->addStretch();
    this->m_layout_general->setSpacing(10);
    this->m_widget_general->setLayout(this->m_layout_general);
    this->m_scrollarea_general->setWidget(m_widget_general);
    //第三页：信息
    this->m_scrollarea_info->setFocusPolicy(Qt::NoFocus);
    this->m_scrollarea_info->setWidgetResizable(true);

    this->m_layout_info_desc_label->addWidget(this->m_label_info_desc_title);//“介绍”
    this->m_layout_info_desc_label->addWidget(this->m_label_info_desc_content);
    this->m_layout_info_desc_label->setSpacing(2);
    this->m_layout_info_desc->addLayout(this->m_layout_info_desc_label);
    this->m_button_info_desc->setFixedSize(30, 30);
    this->m_layout_info_desc->addWidget(this->m_button_info_desc);
    this->m_layout_info->addLayout(this->m_layout_info_desc);
    this->m_layout_info->addWidget(horizontalLine());
    this->m_layout_info_version_label->addWidget(this->m_label_info_version_title);//“版本”
    this->m_layout_info_version_label->addWidget(this->m_label_info_version_content);
    this->m_layout_info_version_label->setSpacing(2);
    this->m_layout_info_version->addLayout(this->m_layout_info_version_label);
    this->m_button_info_update->setFixedSize(30, 30);
    this->m_layout_info_version->addWidget(this->m_button_info_update);
    this->m_layout_info->addLayout(this->m_layout_info_version);
    this->m_layout_info->addWidget(horizontalLine());
    this->m_layout_info_doc_label->addWidget(this->m_label_info_doc_title);//“文档”
    this->m_layout_info_doc_label->addWidget(this->m_label_info_doc_content);
    this->m_layout_info_doc_label->setSpacing(2);
    this->m_layout_info_doc->addLayout(this->m_layout_info_doc_label);
    this->m_button_info_doc->setFixedSize(30, 30);
    this->m_layout_info_doc->addWidget(this->m_button_info_doc);
    this->m_layout_info->addLayout(this->m_layout_info_doc);
    this->m_layout_info->addWidget(horizontalLine());
    this->m_layout_info_log_label->addWidget(this->m_label_info_log_title);//“日志”
    this->m_layout_info_log_label->addWidget(this->m_label_info_log_content);
    this->m_layout_info_log_label->setSpacing(2);
    this->m_layout_info_log->addLayout(this->m_layout_info_log_label);
    this->m_button_info_log->setFixedSize(30, 30);
    this->m_layout_info_log->addWidget(this->m_button_info_log);
    this->m_layout_info->addLayout(this->m_layout_info_log);
    this->m_layout_info->addWidget(horizontalLine());
    this->m_layout_info->addWidget(this->m_label_info_copyright);//“版权”

    this->m_layout_info->addStretch();
    this->m_layout_info->setSpacing(10);
    this->m_widget_info->setLayout(this->m_layout_info);
    this->m_scrollarea_info->setWidget(m_widget_info);


    this->setLayout(this->m_layout_main);

    initText();
    refreshValue();
    initConnect();

    slotChangeTabCharacter();
}

void UISettings::initText()
{
    this->setWindowTitle(this->m_p_localisation->get(this->objectName()));
    //第一页：角色
    this->m_button_tab_character->setText(this->m_p_localisation->get(this->m_button_tab_character->objectName()));
    this->m_label_character_switch_title->setText(this->m_p_localisation->get(this->m_label_character_switch_title->objectName()) + this->m_p_config->characterName); //“选择角色”
    this->m_label_character_switch_content->setText(this->m_p_localisation->get(this->m_label_character_switch_content->objectName()));
    this->m_button_character_switch->setIcon(QIcon(QPixmap(":\\img\\next.png")));
    this->m_button_character_switch->setToolTip(this->m_p_localisation->get(this->m_button_character_switch->objectName()+"_tooltip"));
    this->m_label_tasks_title->setText(this->m_p_localisation->get(this->m_label_tasks_title->objectName())); //“任务”
    this->m_label_tasks_content->setText(this->m_p_localisation->get(this->m_label_tasks_content->objectName()));
    this->m_button_tasks->setIcon(QIcon(QPixmap(":\\img\\next.png")));
    this->m_button_tasks->setToolTip(this->m_p_localisation->get(this->m_button_tasks->objectName()+"_tooltip"));
    //第二页：常规
    this->m_button_tab_general->setText(this->m_p_localisation->get(this->m_button_tab_general->objectName()));
    this->m_label_general_option_character_size_title->setText(this->m_p_localisation->get(this->m_label_general_option_character_size_title->objectName())); //“选项-角色大小”
    this->m_label_general_option_character_size_content->setText(this->m_p_localisation->get(this->m_label_general_option_character_size_content->objectName()));
    this->m_label_general_constant_gravitational_acceleration_title->setText(this->m_p_localisation->get(this->m_label_general_constant_gravitational_acceleration_title->objectName())); //“常量-重力加速度”
    this->m_label_general_constant_gravitational_acceleration_content->setText(this->m_p_localisation->get(this->m_label_general_constant_gravitational_acceleration_content->objectName()));
    this->m_label_general_constant_obstruction_title->setText(this->m_p_localisation->get(this->m_label_general_constant_obstruction_title->objectName())); //“常量-阻力”
    this->m_label_general_constant_obstruction_content->setText(this->m_p_localisation->get(this->m_label_general_constant_obstruction_content->objectName()));
    //第三页：信息
    this->m_button_tab_info->setText(this->m_p_localisation->get(this->m_button_tab_info->objectName()));
    this->m_label_info_desc_title->setWordWrap(true);
    this->m_label_info_desc_title->setText(this->m_p_localisation->get(this->m_label_info_desc_title->objectName()));
    this->m_label_info_desc_content->setWordWrap(true);
    this->m_label_info_desc_content->setText(this->m_p_localisation->get(this->m_label_info_desc_content->objectName()));
    this->m_button_info_desc->setIcon(QIcon(QPixmap(":\\img\\linkout.png")));
    this->m_button_info_desc->setToolTip(this->m_p_localisation->get(this->m_button_info_desc->objectName()+"_tooltip"));
    this->m_label_info_version_title->setText(this->m_p_localisation->get(this->m_label_info_version_title->objectName()));
    this->m_label_info_version_content->setText(this->m_p_config->version);
    this->m_button_info_update->setIcon(QIcon(QPixmap(":\\img\\refresh.png")));
    this->m_button_info_update->setToolTip(this->m_p_localisation->get(this->m_button_info_update->objectName()+"_tooltip"));
    this->m_label_info_doc_title->setText(this->m_p_localisation->get(this->m_label_info_doc_title->objectName()));
    this->m_label_info_doc_content->setText(this->m_p_localisation->get(this->m_label_info_doc_content->objectName()));
    this->m_button_info_doc->setIcon(QIcon(QPixmap(":\\img\\linkout.png")));
    this->m_button_info_doc->setToolTip(this->m_p_localisation->get(this->m_button_info_doc->objectName()+"_tooltip"));
    this->m_button_info_log->setIcon(QIcon(QPixmap(":\\img\\next.png")));
    this->m_label_info_log_title->setText(this->m_p_localisation->get(this->m_label_info_log_title->objectName()));
    this->m_label_info_log_content->setText(this->m_p_localisation->get(this->m_label_info_log_content->objectName()));
    this->m_button_info_log->setToolTip(this->m_p_localisation->get(this->m_button_info_log->objectName()+"_tooltip"));
    this->m_label_info_copyright->setText("copyright");
}

void UISettings::initObjectName()
{
    this->setObjectName("uisettings_widget");

    this->m_layout_tab->setObjectName("uisettings_layout_tab");
    this->m_stackedwidget_main->setObjectName("uisettings_stackedwidget_main");
    //第一页：角色
    this->m_button_tab_character->setObjectName("uisettings_button_tab_character");
    this->m_scrollarea_character->setObjectName("uisettings_scrollarea_character");
    this->m_layout_character->setObjectName("uisettings_layout_character");
    this->m_widget_character->setObjectName("uisettings_widget_character");
    this->m_layout_character_switch->setObjectName("uisettings_layout_character_switch");   //“选择角色”
    this->m_layout_character_switch_label->setObjectName("uisettings_layout_character_switch_label");
    this->m_label_character_switch_title->setObjectName("uisettings_label_character_switch_title");
    this->m_label_character_switch_content->setObjectName("uisettings_label_character_switch_content");
    this->m_button_character_switch->setObjectName("uisettings_button_character_switch");
    this->m_layout_tasks->setObjectName("uisettings_layout_tasks");   //“任务”
    this->m_layout_tasks_label->setObjectName("uisettings_layout_tasks_label");
    this->m_label_tasks_title->setObjectName("uisettings_label_tasks_title");
    this->m_label_tasks_content->setObjectName("uisettings_label_tasks_content");
    this->m_button_tasks->setObjectName("uisettings_button_tasks");
    //第二页：常规
    this->m_button_tab_general->setObjectName("uisettings_button_tab_general");
    this->m_scrollarea_general->setObjectName("uisettings_scrollarea_general");
    this->m_layout_general->setObjectName("uisettings_layout_general");
    this->m_widget_general->setObjectName("uisettings_widget_general");
    this->m_layout_general_option_character_size->setObjectName("uisettings_layout_general_option_character_size");//“选项-角色大小”
    this->m_layout_general_option_character_size_label->setObjectName("uisettings_layout_general_option_character_size_label");
    this->m_label_general_option_character_size_title->setObjectName("uisettings_label_general_option_character_size_title");
    this->m_label_general_option_character_size_content->setObjectName("uisettings_label_general_option_character_size_content");
    this->m_lineedit_general_option_character_size->setObjectName("uisettings_lineedit_general_option_character_size");
    this->m_layout_general_constant_gravitational_acceleration->setObjectName("uisettings_layout_general_constant_gravitational_acceleration");//“常量-重力加速度”
    this->m_layout_general_constant_gravitational_acceleration_label->setObjectName("uisettings_layout_general_constant_gravitational_acceleration_label");
    this->m_label_general_constant_gravitational_acceleration_title->setObjectName("uisettings_label_general_constant_gravitational_acceleration_title");
    this->m_label_general_constant_gravitational_acceleration_content->setObjectName("uisettings_label_general_constant_gravitational_acceleration_content");
    this->m_lineedit_general_constant_gravitational_acceleration->setObjectName("uisettings_lineedit_general_constant_gravitational_acceleration");
    this->m_layout_general_constant_obstruction->setObjectName("uisettings_layout_general_constant_obstruction");//“常量-阻力”
    this->m_layout_general_constant_obstruction_label->setObjectName("uisettings_layout_general_constant_obstruction_label");
    this->m_label_general_constant_obstruction_title->setObjectName("uisettings_label_general_constant_obstruction_title");
    this->m_label_general_constant_obstruction_content->setObjectName("uisettings_label_general_constant_obstruction_content");
    this->m_lineedit_general_constant_obstruction->setObjectName("uisettings_lineedit_general_constant_obstruction");
    //第三页：信息
    this->m_button_tab_info->setObjectName("uisettings_button_tab_info");
    this->m_scrollarea_info->setObjectName("uisettings_scrollarea_info");
    this->m_layout_info->setObjectName("uisettings_layout_info");
    this->m_widget_info->setObjectName("uisettings_widget_info");
    this->m_layout_info_desc->setObjectName("uisettings_layout_info_desc");//“介绍”
    this->m_layout_info_desc_label->setObjectName("uisettings_layout_info_desc_label");
    this->m_label_info_desc_title->setObjectName("uisettings_label_info_desc_title");
    this->m_label_info_desc_content->setObjectName("uisettings_label_info_desc_content");
    this->m_button_info_desc->setObjectName("uisettings_button_info_desc");
    this->m_layout_info_version->setObjectName("uisettings_layout_info_version"); //“版本”
    this->m_layout_info_version_label->setObjectName("uisettings_layout_info_version_label");
    this->m_label_info_version_title->setObjectName("uisettings_label_info_version_title");
    this->m_label_info_version_content->setObjectName("uisettings_label_info_version_content");
    this->m_button_info_update->setObjectName("uisettings_button_info_update");
    this->m_layout_info_doc->setObjectName("uisettings_layout_info_doc"); //“文档”
    this->m_layout_info_doc_label->setObjectName("uisettings_layout_info_doc_label");
    this->m_label_info_doc_title->setObjectName("uisettings_label_info_doc_title");
    this->m_label_info_doc_content->setObjectName("uisettings_label_info_doc_content");
    this->m_button_info_doc->setObjectName("uisettings_button_info_doc");
    this->m_layout_info_log->setObjectName("uisettings_layout_info_log"); //“日志”
    this->m_layout_info_log_label->setObjectName("uisettings_layout_info_log_label");
    this->m_label_info_log_title->setObjectName("uisettings_label_info_log_title");
    this->m_label_info_log_content->setObjectName("uisettings_label_info_log_content");
    this->m_button_info_log->setObjectName("uisettings_button_info_log");
    this->m_label_info_copyright->setObjectName("uisettings_label_info_copyright");    //“版权”
}


void UISettings::initConnect(){
    connect(this->m_button_tab_character, SIGNAL(clicked()), this, SLOT(slotChangeTabCharacter()));
    connect(this->m_button_tab_general, SIGNAL(clicked()), this, SLOT(slotChangeTabGeneral()));
    connect(this->m_button_tab_info, SIGNAL(clicked()), this, SLOT(slotChangeTabInfo()));

    connect(this->m_button_character_switch, SIGNAL(clicked()), this, SLOT(slotShowUICharacter()));
    connect(this->m_button_tasks, SIGNAL(clicked()), this, SLOT(slotShowUITasks()));

    connect(this->m_button_info_desc, SIGNAL(clicked()), this, SLOT(slotOpenUrlDesc()));
    connect(this->m_button_info_doc, SIGNAL(clicked()), this, SLOT(slotOpenUrlDoc()));
}

void UISettings::refreshValue()
{
    this->m_lineedit_general_option_character_size->setText(QString::number(this->m_p_config->characterSize));    //“选项-角色大小”
    this->m_lineedit_general_constant_gravitational_acceleration->setText(QString::number(this->m_p_config->gravitationalAcceleration));    //“常量-重力加速度”
    this->m_lineedit_general_constant_obstruction->setText(QString::number(this->m_p_config->obstruction));    //“常量-阻力”
}

void UISettings::saveValue()
{
    this->m_p_config->characterSize = this->m_lineedit_general_option_character_size->text().toInt();    //“选项-角色大小”
    this->m_p_config->gravitationalAcceleration = this->m_lineedit_general_constant_gravitational_acceleration->text().toDouble();    //“常量-重力加速度”
    this->m_p_config->obstruction = this->m_lineedit_general_constant_obstruction->text().toDouble();    //“常量-阻力”

    this->m_p_config->save();
}

QFrame *UISettings::horizontalLine(QString objectName, QWidget *parent)
{
    QFrame *hLine = new QFrame(parent);
    hLine->setObjectName(objectName);
    hLine->setFixedHeight(1);
    return hLine;
}

void UISettings::setBackGround()
{

    QDir rootDir(":\\background");
    QDir userDir("background");
    rootDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    rootDir.setSorting(QDir::Name);
    userDir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    userDir.setSorting(QDir::Name);
    QFileInfoList rootList = rootDir.entryInfoList();
    QFileInfoList userList = userDir.entryInfoList();
    QFileInfoList list;
    for(int i = 0; i < rootList.size(); i++)
    {
        list.push_back(rootList.at(i));
    }
    for(int i = 0; i < userList.size(); i++)
    {
        QFileInfo file(userList.at(i));
        QString suffix = file.suffix().toLower();
        if(suffix == "png" || suffix == "jpg" || suffix == "jpeg" || suffix == "gif")
            list.push_back(userList.at(i));
    }

    //取随机数
    QTime time;
    time= QTime::currentTime();
    qsrand(static_cast <uint>(time.msec()+time.second()*1000));
    int rand = qrand() % list.size();

    //获取图像
    QImage image(list.at(rand).filePath());

    //缩放图像
    int w = image.width();
    int h = image.height();
    int sw = w, sh = h;
    if(w>=h)
    {
        if(h > 800)
        {
            sh = 800;
            sw = w*800/h;
        }
    }else
    {
        if(w > 800)
        {
            sw = 800;
            sh = h*800/w;
        }
    }

    QImage imgScaled(image.scaled(sw, sh,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    //随机位置
    int x = qrand()% (imgScaled.width() - this->width);
    int y = qrand()% (imgScaled.height() - this->width);

    //应用图像
    this->background = imgScaled.copy(x, y, this->width, this->height);
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(QPixmap::fromImage(this->background)));
    this->setPalette(palette);
}

/**********public slots**********/

void UISettings::slotShowUICharacter()
{
    this->m_p_uicharacter = new UICharacter();
    this->m_p_uicharacter->show();
    this->m_p_uicharacter->init(this->m_p_localisation, this->m_p_config, this->m_p_file_character, this->background, this->pos(), QSize(this->width, this->height));
    connect(this->m_p_uicharacter, SIGNAL(signalBack(QString)), this, SLOT(slotUICharacterBack(QString)));
}

void UISettings::slotUICharacterBack(QString name)
{
    this->m_p_uicharacter->hide();
    if(name!=nullptr)
    {
        this->m_p_config->characterName = name;
        this->m_p_config->save();
        this->m_label_character_switch_title->setText(this->m_p_localisation->get(this->m_label_character_switch_title->objectName()) + name);
    }
    disconnect(this->m_p_uicharacter, SIGNAL(signalConfirm(QString)), this, SLOT(slotCharacterChange(QString)));
    delete this->m_p_uicharacter;
    this->m_p_uicharacter = nullptr;
}

void UISettings::slotShowUITasks()
{
    this->m_p_uitasks = new UITasks();
    this->m_p_uitasks->show();
    this->m_p_uitasks->init(this->m_p_localisation, this->m_p_config, this->m_p_file_tasks, this->background, this->pos(), QSize(this->width, this->height));
    connect(this->m_p_uitasks, SIGNAL(signalBack(QString)), this, SLOT(slotUITasksBack(QString)));
}

void UISettings::slotUITasksBack(QString name)
{
    this->m_p_uitasks->hide();
    if(name==nullptr)
    {

    }
    disconnect(this->m_p_uitasks, SIGNAL(signalBack(QString)), this, SLOT(slotUITasksBack(QString)));
    delete this->m_p_uitasks;
    this->m_p_uitasks = nullptr;
}

void UISettings::slotChangeTabCharacter()
{
    this->m_stackedwidget_main->setCurrentIndex(0);
    this->m_button_tab_character->setEnabled(false);
    this->m_button_tab_general->setEnabled(true);
    this->m_button_tab_info->setEnabled(true);

    this->m_button_tab_general->setFocus();
}
void UISettings::slotChangeTabGeneral()
{
    this->m_stackedwidget_main->setCurrentIndex(1);
    this->m_button_tab_character->setEnabled(true);
    this->m_button_tab_general->setEnabled(false);
    this->m_button_tab_info->setEnabled(true);

    this->m_button_tab_info->setFocus();
}
void UISettings::slotChangeTabInfo()
{
    this->m_stackedwidget_main->setCurrentIndex(2);
    this->m_button_tab_character->setEnabled(true);
    this->m_button_tab_general->setEnabled(true);
    this->m_button_tab_info->setEnabled(false);

    this->m_button_tab_character->setFocus();
}

void UISettings::slotOpenUrlDesc()
{
    QDesktopServices::openUrl(QUrl("http://desktoppony.22web.org/"));
}

void UISettings::slotOpenUrlDoc()
{
    QDesktopServices::openUrl(QUrl("http://desktoppony.22web.org/doc/"));
}
