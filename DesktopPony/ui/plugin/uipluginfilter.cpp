#include "uipluginfilter.h"

UiPluginFilter::UiPluginFilter(QWidget *parent) : QWidget(parent)
{
    this->m_p_hide_timer = new QTimer;
    this->m_p_hide_timer->setInterval(50);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    connect(this->m_p_hide_timer, &QTimer::timeout, this, &UiPluginFilter::timerHideTimeOut);
}

UiPluginFilter::~UiPluginFilter()
{
    clearConnect();
    disconnect(this->m_p_hide_timer, &QTimer::timeout, this, &UiPluginFilter::timerHideTimeOut);
}

void UiPluginFilter::init(Text *ptrText, QPushButton *ptrParent)
{
    this->m_p_text = ptrText;
    this->m_p_parent_button = ptrParent;
    ptrText = nullptr;
    ptrParent = nullptr;

    this->setAttribute(Qt::WA_TranslucentBackground);
    initWidget();
    initObjectName();
    initProperty();
    initContent();
    initConnect();
}

void UiPluginFilter::initWidget()
{
    this->ui_plugin_filter_layout_main = new QVBoxLayout;
    this->ui_plugin_filter_layout_main->setSpacing(0);
    this->ui_plugin_filter_layout_main->setMargin(0);
    this->ui_plugin_filter_widget = new QWidget;
    this->ui_plugin_filter_layout_main->addWidget(this->ui_plugin_filter_widget);
    this->ui_plugin_filter_layout_widget = new QVBoxLayout;
    this->ui_plugin_filter_layout_widget->setMargin(5);
    this->ui_plugin_filter_layout_widget->setSpacing(5);
    this->ui_plugin_filter_widget->setLayout(this->ui_plugin_filter_layout_widget);

    this->ui_plugin_filter_layout_localisation = new QHBoxLayout;
    this->ui_plugin_filter_layout_localisation->setMargin(0);
    this->ui_plugin_filter_layout_localisation->setSpacing(0);
    this->ui_plugin_filter_layout_widget->addLayout(this->ui_plugin_filter_layout_localisation);
    this->ui_plugin_filter_localisation = new QLabel;
    this->ui_plugin_filter_layout_localisation->addWidget(this->ui_plugin_filter_localisation);
    this->ui_plugin_filter_layout_localisation->addStretch();
    this->ui_plugin_filter_localisation_switch = new QCheckBox;
    this->ui_plugin_filter_localisation_switch->setChecked(true);
    this->ui_plugin_filter_layout_localisation->addWidget(this->ui_plugin_filter_localisation_switch);

    this->ui_plugin_filter_layout_style = new QHBoxLayout;
    this->ui_plugin_filter_layout_style->setMargin(0);
    this->ui_plugin_filter_layout_style->setSpacing(0);
    this->ui_plugin_filter_layout_widget->addLayout(this->ui_plugin_filter_layout_style);
    this->ui_plugin_filter_style = new QLabel;
    this->ui_plugin_filter_layout_style->addWidget(this->ui_plugin_filter_style);
    this->ui_plugin_filter_layout_style->addStretch();
    this->ui_plugin_filter_style_switch = new QCheckBox;
    this->ui_plugin_filter_style_switch->setChecked(true);
    this->ui_plugin_filter_layout_style->addWidget(this->ui_plugin_filter_style_switch);

    this->ui_plugin_filter_layout_event = new QHBoxLayout;
    this->ui_plugin_filter_layout_event->setMargin(0);
    this->ui_plugin_filter_layout_event->setSpacing(0);
    this->ui_plugin_filter_layout_widget->addLayout(this->ui_plugin_filter_layout_event);
    this->ui_plugin_filter_event = new QLabel;
    this->ui_plugin_filter_layout_event->addWidget(this->ui_plugin_filter_event);
    this->ui_plugin_filter_layout_event->addStretch();
    this->ui_plugin_filter_event_switch = new QCheckBox;
    this->ui_plugin_filter_event_switch->setChecked(true);
    this->ui_plugin_filter_layout_event->addWidget(this->ui_plugin_filter_event_switch);

    this->ui_plugin_filter_layout_action = new QHBoxLayout;
    this->ui_plugin_filter_layout_action->setMargin(0);
    this->ui_plugin_filter_layout_action->setSpacing(0);
    this->ui_plugin_filter_layout_widget->addLayout(this->ui_plugin_filter_layout_action);
    this->ui_plugin_filter_action = new QLabel;
    this->ui_plugin_filter_layout_action->addWidget(this->ui_plugin_filter_action);
    this->ui_plugin_filter_layout_action->addStretch();
    this->ui_plugin_filter_action_switch = new QCheckBox;
    this->ui_plugin_filter_action_switch->setChecked(true);
    this->ui_plugin_filter_layout_action->addWidget(this->ui_plugin_filter_action_switch);

    this->ui_plugin_filter_layout_accessories = new QHBoxLayout;
    this->ui_plugin_filter_layout_accessories->setMargin(0);
    this->ui_plugin_filter_layout_accessories->setSpacing(0);
    this->ui_plugin_filter_layout_widget->addLayout(this->ui_plugin_filter_layout_accessories);
    this->ui_plugin_filter_accessories = new QLabel;
    this->ui_plugin_filter_layout_accessories->addWidget(this->ui_plugin_filter_accessories);
    this->ui_plugin_filter_layout_accessories->addStretch();
    this->ui_plugin_filter_accessories_switch = new QCheckBox;
    this->ui_plugin_filter_accessories_switch->setChecked(true);
    this->ui_plugin_filter_layout_accessories->addWidget(this->ui_plugin_filter_accessories_switch);

    this->ui_plugin_filter_layout_model = new QHBoxLayout;
    this->ui_plugin_filter_layout_model->setMargin(0);
    this->ui_plugin_filter_layout_model->setSpacing(0);
    this->ui_plugin_filter_layout_widget->addLayout(this->ui_plugin_filter_layout_model);
    this->ui_plugin_filter_model = new QLabel;
    this->ui_plugin_filter_layout_model->addWidget(this->ui_plugin_filter_model);
    this->ui_plugin_filter_layout_model->addStretch();
    this->ui_plugin_filter_model_switch = new QCheckBox;
    this->ui_plugin_filter_model_switch->setChecked(true);
    this->ui_plugin_filter_layout_model->addWidget(this->ui_plugin_filter_model_switch);

    this->ui_plugin_filter_layout_config = new QHBoxLayout;
    this->ui_plugin_filter_layout_config->setMargin(0);
    this->ui_plugin_filter_layout_config->setSpacing(0);
    this->ui_plugin_filter_layout_widget->addLayout(this->ui_plugin_filter_layout_config);
    this->ui_plugin_filter_config = new QLabel;
    this->ui_plugin_filter_layout_config->addWidget(this->ui_plugin_filter_config);
    this->ui_plugin_filter_layout_config->addStretch();
    this->ui_plugin_filter_config_switch = new QCheckBox;
    this->ui_plugin_filter_config_switch->setChecked(true);
    this->ui_plugin_filter_layout_config->addWidget(this->ui_plugin_filter_config_switch);

    this->ui_plugin_filter_layout_null = new QHBoxLayout;
    this->ui_plugin_filter_layout_null->setMargin(0);
    this->ui_plugin_filter_layout_null->setSpacing(0);
    this->ui_plugin_filter_layout_widget->addLayout(this->ui_plugin_filter_layout_null);
    this->ui_plugin_filter_null = new QLabel;
    this->ui_plugin_filter_layout_null->addWidget(this->ui_plugin_filter_null);
    this->ui_plugin_filter_layout_null->addStretch();
    this->ui_plugin_filter_null_switch = new QCheckBox;
    this->ui_plugin_filter_null_switch->setChecked(true);
    this->ui_plugin_filter_layout_null->addWidget(this->ui_plugin_filter_null_switch);

    this->setLayout(this->ui_plugin_filter_layout_main);
}

void UiPluginFilter::initObjectName()
{
    this->setObjectName(QStringLiteral("ui_plugin_filter"));
    // 部件部分
    this->ui_plugin_filter_widget->setObjectName(QStringLiteral("ui_plugin_filter_widget"));
    this->ui_plugin_filter_localisation->setObjectName(QStringLiteral("ui_plugin_filter_localisation"));
    this->ui_plugin_filter_localisation_switch->setObjectName(QStringLiteral("ui_plugin_filter_localisation_switch"));
    this->ui_plugin_filter_style->setObjectName(QStringLiteral("ui_plugin_filter_style"));
    this->ui_plugin_filter_style_switch->setObjectName(QStringLiteral("ui_plugin_filter_style_switch"));
    this->ui_plugin_filter_event->setObjectName(QStringLiteral("ui_plugin_filter_event"));
    this->ui_plugin_filter_event_switch->setObjectName(QStringLiteral("ui_plugin_filter_event_switch"));
    this->ui_plugin_filter_action->setObjectName(QStringLiteral("ui_plugin_filter_action"));
    this->ui_plugin_filter_action_switch->setObjectName(QStringLiteral("ui_plugin_filter_action_switch"));
    this->ui_plugin_filter_accessories->setObjectName(QStringLiteral("ui_plugin_filter_accessories"));
    this->ui_plugin_filter_accessories_switch->setObjectName(QStringLiteral("ui_plugin_filter_accessories_switch"));
    this->ui_plugin_filter_model->setObjectName(QStringLiteral("ui_plugin_filter_model"));
    this->ui_plugin_filter_model_switch->setObjectName(QStringLiteral("ui_plugin_filter_model_switch"));
    this->ui_plugin_filter_config->setObjectName(QStringLiteral("ui_plugin_filter_config"));
    this->ui_plugin_filter_config_switch->setObjectName(QStringLiteral("ui_plugin_filter_config_switch"));
    this->ui_plugin_filter_null->setObjectName(QStringLiteral("ui_plugin_filter_null"));
    this->ui_plugin_filter_null_switch->setObjectName(QStringLiteral("ui_plugin_filter_null_switch"));
    // 布局部分
    this->ui_plugin_filter_layout_widget->setObjectName(QStringLiteral("ui_plugin_filter_layout_widget"));
    this->ui_plugin_filter_layout_localisation->setObjectName(QStringLiteral("ui_plugin_filter_layout_localisation"));
    this->ui_plugin_filter_layout_style->setObjectName(QStringLiteral("ui_plugin_filter_layout_style"));
    this->ui_plugin_filter_layout_event->setObjectName(QStringLiteral("ui_plugin_filter_layout_event"));
    this->ui_plugin_filter_layout_action->setObjectName(QStringLiteral("ui_plugin_filter_layout_action"));
    this->ui_plugin_filter_layout_accessories->setObjectName(QStringLiteral("ui_plugin_filter_layout_accessories"));
    this->ui_plugin_filter_layout_model->setObjectName(QStringLiteral("ui_plugin_filter_layout_model"));
    this->ui_plugin_filter_layout_config->setObjectName(QStringLiteral("ui_plugin_filter_layout_config"));
    this->ui_plugin_filter_layout_null->setObjectName(QStringLiteral("ui_plugin_filter_layout_null"));
}
void UiPluginFilter::initProperty()
{
    this->setProperty("category", QStringLiteral("plugin_filter"));
    // 部件部分
    this->ui_plugin_filter_widget->setProperty("category", QStringLiteral("plugin_filter_widget"));
    this->ui_plugin_filter_localisation->setProperty("category", QStringLiteral("plugin_filter_label"));
    this->ui_plugin_filter_localisation_switch->setProperty("category", QStringLiteral("plugin_filter_switch"));
    this->ui_plugin_filter_style->setProperty("category", QStringLiteral("plugin_filter_label"));
    this->ui_plugin_filter_style_switch->setProperty("category", QStringLiteral("plugin_filter_switch"));
    this->ui_plugin_filter_event->setProperty("category", QStringLiteral("plugin_filter_label"));
    this->ui_plugin_filter_event_switch->setProperty("category", QStringLiteral("plugin_filter_switch"));
    this->ui_plugin_filter_action->setProperty("category", QStringLiteral("plugin_filter_label"));
    this->ui_plugin_filter_action_switch->setProperty("category", QStringLiteral("plugin_filter_switch"));
    this->ui_plugin_filter_accessories->setProperty("category", QStringLiteral("plugin_filter_label"));
    this->ui_plugin_filter_accessories_switch->setProperty("category", QStringLiteral("plugin_filter_switch"));
    this->ui_plugin_filter_model->setProperty("category", QStringLiteral("plugin_filter_label"));
    this->ui_plugin_filter_model_switch->setProperty("category", QStringLiteral("plugin_filter_switch"));
    this->ui_plugin_filter_config->setProperty("category", QStringLiteral("plugin_filter_label"));
    this->ui_plugin_filter_config_switch->setProperty("category", QStringLiteral("plugin_filter_switch"));
    this->ui_plugin_filter_null->setProperty("category", QStringLiteral("plugin_filter_label"));
    this->ui_plugin_filter_null_switch->setProperty("category", QStringLiteral("plugin_filter_switch"));
}

void UiPluginFilter::initContent()
{
    this->ui_plugin_filter_localisation->setText(this->m_p_text->getLoc(QStringLiteral("plugin_localisation")));
    this->ui_plugin_filter_style->setText(this->m_p_text->getLoc(QStringLiteral("plugin_style")));
    this->ui_plugin_filter_event->setText(this->m_p_text->getLoc(QStringLiteral("plugin_event")));
    this->ui_plugin_filter_action->setText(this->m_p_text->getLoc(QStringLiteral("plugin_action")));
    this->ui_plugin_filter_accessories->setText(this->m_p_text->getLoc(QStringLiteral("plugin_accessories")));
    this->ui_plugin_filter_model->setText(this->m_p_text->getLoc(QStringLiteral("plugin_model")));
    this->ui_plugin_filter_config->setText(this->m_p_text->getLoc(QStringLiteral("plugin_config")));
    this->ui_plugin_filter_null->setText(this->m_p_text->getLoc(QStringLiteral("ui_plugin_page_filter_null_txt")));
}

void UiPluginFilter::initConnect()
{
    connect(this->ui_plugin_filter_localisation_switch, &QCheckBox::toggled, this, [ = ](bool flag) {
        this->m_filter_loc = flag;
        emit sigFilterChanged();
    });
    connect(this->ui_plugin_filter_style_switch, &QCheckBox::toggled, this, [ = ](bool flag) {
        this->m_filter_style = flag;
        emit sigFilterChanged();
    });
    connect(this->ui_plugin_filter_event_switch, &QCheckBox::toggled, this, [ = ](bool flag) {
        this->m_filter_event = flag;
        emit sigFilterChanged();
    });
    connect(this->ui_plugin_filter_action_switch, &QCheckBox::toggled, this, [ = ](bool flag) {
        this->m_filter_action = flag;
        emit sigFilterChanged();
    });
    connect(this->ui_plugin_filter_accessories_switch, &QCheckBox::toggled, this, [ = ](bool flag) {
        this->m_filter_acc = flag;
        emit sigFilterChanged();
    });
    connect(this->ui_plugin_filter_model_switch, &QCheckBox::toggled, this, [ = ](bool flag) {
        this->m_filter_model = flag;
        emit sigFilterChanged();
    });
    connect(this->ui_plugin_filter_config_switch, &QCheckBox::toggled, this, [ = ](bool flag) {
        this->m_filter_conf = flag;
        emit sigFilterChanged();
    });
    connect(this->ui_plugin_filter_null_switch, &QCheckBox::toggled, this, [ = ](bool flag) {
        this->m_filter_null = flag;
        emit sigFilterChanged();
    });
}

void UiPluginFilter::clearConnect()
{
    disconnect(this->ui_plugin_filter_localisation_switch, &QCheckBox::toggled, this, nullptr);
    disconnect(this->ui_plugin_filter_style_switch, &QCheckBox::toggled, this, nullptr);
    disconnect(this->ui_plugin_filter_event_switch, &QCheckBox::toggled, this, nullptr);
    disconnect(this->ui_plugin_filter_action_switch, &QCheckBox::toggled, this, nullptr);
    disconnect(this->ui_plugin_filter_accessories_switch, &QCheckBox::toggled, this, nullptr);
    disconnect(this->ui_plugin_filter_model_switch, &QCheckBox::toggled, this, nullptr);
    disconnect(this->ui_plugin_filter_config_switch, &QCheckBox::toggled, this, nullptr);
    disconnect(this->ui_plugin_filter_null_switch, &QCheckBox::toggled, this, nullptr);
}

void UiPluginFilter::showFilter()
{
    this->m_is_mouse_entered = false;
    this->m_mouse_unentered_time = 0;
    this->setMouseTracking(true);
    this->m_p_hide_timer->start();
    show();
    moveSelf();
}

void UiPluginFilter::moveSelf()
{
    QPoint pr = this->m_p_parent_button->mapToGlobal(QPoint(0, 0));
    QSize ps = this->m_p_parent_button->size();
    QSize ws = this->ui_plugin_filter_widget->size();
    move(pr.x() + ps.width() - ws.width(), pr.y() + ps.height() + 5);
}

void UiPluginFilter::timerHideTimeOut()
{
    if(this->isVisible()) {
        moveSelf();
        if(this->m_is_mouse_entered) {
            QPoint cp = QCursor::pos();
            QPoint sp = this->mapFromGlobal(cp);
            QRectF rect = this->rect();
            if (!rect.contains(sp)) {
                this->setMouseTracking(false);
                this->m_p_hide_timer->stop();
                this->hide();
            }
        } else {
            if(++this->m_mouse_unentered_time > 50) {
                this->setMouseTracking(false);
                this->m_p_hide_timer->stop();
                this->hide();
            }
        }
    } else {
        this->setMouseTracking(false);
        this->m_p_hide_timer->stop();
        this->hide();
    }
}

void UiPluginFilter::enterEvent(QEvent *ev)
{
    ev->ignore();
    this->m_is_mouse_entered = true;
}

void UiPluginFilter::reset()
{
    this->m_filter_loc = true;
    this->m_filter_style = true;
    this->m_filter_event = true;
    this->m_filter_action = true;
    this->m_filter_acc = true;
    this->m_filter_model = true;
    this->m_filter_conf = true;
    this->m_filter_null = true;

    this->ui_plugin_filter_localisation_switch->setChecked(true);
    this->ui_plugin_filter_style_switch->setChecked(true);
    this->ui_plugin_filter_event_switch->setChecked(true);
    this->ui_plugin_filter_action_switch->setChecked(true);
    this->ui_plugin_filter_accessories_switch->setChecked(true);
    this->ui_plugin_filter_model_switch->setChecked(true);
    this->ui_plugin_filter_config_switch->setChecked(true);
    this->ui_plugin_filter_null_switch->setChecked(true);
}
