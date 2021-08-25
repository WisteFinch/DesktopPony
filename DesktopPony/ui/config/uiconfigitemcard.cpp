#include "uiconfigitemcard.h"

UiConfigItemCard::UiConfigItemCard(QWidget *parent) : QWidget(parent)
{

}

UiConfigItemCard::~UiConfigItemCard()
{
    this->m_ptrfunc_get_conf = nullptr;
    this->m_p_item = nullptr;
    this->m_p_text = nullptr;
    clearConnect();
}

void UiConfigItemCard::init(Text *ptrText, Config::Item *ptrItem, Config::PTRFUNC_GET_CONFIG ptrfuncGetConf)
{
    this->m_p_text = ptrText;
    this->m_p_item = ptrItem;
    this->m_ptrfunc_get_conf = ptrfuncGetConf;
    ptrText = nullptr;
    ptrItem = nullptr;

    this->setAttribute(Qt::WA_StyledBackground);
    initWidget();
    initObjectName();
    initProperty();
    initContent();
    initConnect();
}

void UiConfigItemCard::initWidget()
{
    this->ui_layout_main = new QHBoxLayout;

    this->ui_layout_main->setSpacing(0);
    this->ui_layout_main->setMargin(0);
    this->ui_layout_text = new QVBoxLayout;
    this->ui_layout_text->setSpacing(0);
    this->ui_layout_text->setMargin(0);
    this->ui_layout_main->addLayout(this->ui_layout_text);
    this->ui_caption = new QLabel;
    this->ui_desc = new QLabel;
    this->ui_name = new QLabel;
    this->ui_layout_text->addWidget(this->ui_caption);
    if(m_ptrfunc_get_conf(QStringLiteral("sys_debug")).toBool()) {
        this->ui_layout_text->addWidget(this->ui_desc);
        this->ui_layout_text->addWidget(this->ui_name);
    }
    this->ui_layout_main->addStretch();

    this->setLayout(ui_layout_main);
}

void UiConfigItemCard::initObjectName()
{
    this->setObjectName(QStringLiteral("ui_conf_item_card"));
    // 部件部分
    this->ui_caption->setObjectName(QStringLiteral("ui_conf_item_card_caption"));
    this->ui_desc->setObjectName(QStringLiteral("ui_conf_item_card_desc"));
    this->ui_name->setObjectName(QStringLiteral("ui_conf_item_card_name"));
    // 布局部分
    this->ui_layout_main->setObjectName(QStringLiteral("ui_conf_item_card_layout_main"));
    this->ui_layout_text->setObjectName(QStringLiteral("ui_conf_item_card_layout_text"));
}

void UiConfigItemCard::initProperty()
{
    this->setProperty("category", QStringLiteral("conf_item_card"));
    // 部件部分
    this->ui_caption->setProperty("category", QStringLiteral("conf_item_card_caption_label"));
    this->ui_desc->setProperty("category", QStringLiteral("conf_item_card_desc_label"));
    this->ui_name->setProperty("category", QStringLiteral("conf_item_card_name_label"));
}

void UiConfigItemCard::initContent()
{
    if(this->m_p_item->info->type == PluginElementConfigData::config_type_integer) {
        this->ui_spin_box = new QSpinBox;
        this->ui_layout_main->addWidget(this->ui_spin_box);

        this->ui_spin_box->setObjectName(QStringLiteral("ui_conf_item_card_spin_box"));
        this->ui_spin_box->setProperty("category", QStringLiteral("conf_item_card_value"));
        this->ui_spin_box->setEnabled(!this->m_p_item->info->read_only);

        this->ui_spin_box->setMinimum(this->m_p_item->info->range_from.toInt());
        this->ui_spin_box->setMaximum(this->m_p_item->info->range_to.toInt());
        this->ui_spin_box->setValue(this->m_p_item->v.toInt());

        connect(this->ui_spin_box, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [ = ] {this->m_p_item->v = this->ui_spin_box->value(); emit sigValueChanged();});
    } else if(this->m_p_item->info->type == PluginElementConfigData::config_type_real) {
        this->ui_spin_box_real = new QDoubleSpinBox;
        this->ui_layout_main->addWidget(this->ui_spin_box_real);

        this->ui_spin_box_real->setObjectName(QStringLiteral("ui_conf_item_card_spin_box_real"));
        this->ui_spin_box_real->setProperty("category", QStringLiteral("conf_item_card_value"));
        this->ui_spin_box_real->setEnabled(!this->m_p_item->info->read_only);

        this->ui_spin_box_real->setMinimum(this->m_p_item->info->range_from.toDouble());
        this->ui_spin_box_real->setMaximum(this->m_p_item->info->range_to.toDouble());
        this->ui_spin_box_real->setValue(this->m_p_item->v.toDouble());

        connect(this->ui_spin_box_real, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [ = ] {this->m_p_item->v = this->ui_spin_box_real->value(); emit sigValueChanged();});
    } else if(this->m_p_item->info->type == PluginElementConfigData::config_type_bool) {
        this->ui_switch = new QCheckBox;
        this->ui_layout_main->addWidget(this->ui_switch);

        this->ui_switch->setObjectName(QStringLiteral("ui_conf_item_card_switch"));
        this->ui_switch->setProperty("category", QStringLiteral("conf_item_card_value"));
        this->ui_switch->setEnabled(!this->m_p_item->info->read_only);

        this->ui_switch->setChecked(this->m_p_item->v.toBool());

        connect(this->ui_switch, &QCheckBox::stateChanged, this, [ = ] {this->m_p_item->v = this->ui_switch->checkState() ==  Qt::Unchecked ? false : true; emit sigValueChanged();});
    } else if(this->m_p_item->info->type == PluginElementConfigData::config_type_string) {
        this->ui_lineedit = new QLineEdit;
        this->ui_layout_main->addWidget(this->ui_lineedit);

        this->ui_lineedit->setObjectName(QStringLiteral("ui_conf_item_card_lineedit"));
        this->ui_lineedit->setProperty("category", QStringLiteral("conf_item_card_value"));
        this->ui_lineedit->setEnabled(!this->m_p_item->info->read_only);

        this->ui_lineedit->setText(this->m_p_item->v.toString());

        connect(this->ui_lineedit, &QLineEdit::textEdited, this, [ = ] {this->m_p_item->v = this->ui_lineedit->text(); emit sigValueChanged();});
    } else if(this->m_p_item->info->type == PluginElementConfigData::config_type_select) {
        this->ui_combobox = new QComboBox;
        this->ui_combobox->setView(new QListView(this));
        this->ui_combobox->view()->parentWidget()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
        this->ui_combobox->view()->parentWidget()->setAttribute(Qt::WA_TranslucentBackground);
        this->ui_layout_main->addWidget(this->ui_combobox);

        this->ui_combobox->setObjectName(QStringLiteral("ui_conf_item_card_combobox"));
        this->ui_combobox->setProperty("category", QStringLiteral("conf_item_card_value"));
        this->ui_combobox->setEnabled(!this->m_p_item->info->read_only);

        qint32 index = 0;
        for(int i = 0 ; i < this->m_p_item->info->list.count(); i++) {
            this->ui_combobox->addItem(this->m_p_text->get(this->m_p_item->info->list.at(i).name, this->m_p_item->info->list.at(i).obj_uuid));
            if(this->m_p_item->info->list.at(i).id == this->m_p_item->v.toString()) {
                index = i;
            }
        }
        this->ui_combobox->setCurrentIndex(index);

        connect(this->ui_combobox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [ = ](int index) {
            this->m_p_item->v = this->m_p_item->info->list.at(index).id;
            emit sigValueChanged();
        });
    }
    this->ui_caption->setText(this->m_p_text->get(this->m_p_item->info->caption, this->m_p_item->obj_uuid));
    this->ui_caption->setToolTip(this->m_p_text->get(this->m_p_item->info->desc, this->m_p_item->obj_uuid));
    this->ui_desc->setText(this->m_p_text->get(this->m_p_item->info->desc, this->m_p_item->obj_uuid));
    this->ui_name->setText(this->m_p_item->info->config_name);
}

void UiConfigItemCard::initConnect()
{
    connect(this, &UiConfigItemCard::sigValueChanged, this, &UiConfigItemCard::slotValueChanged);
}

void UiConfigItemCard::clearConnect()
{
    disconnect(this, &UiConfigItemCard::sigValueChanged, this, &UiConfigItemCard::slotValueChanged);
}
void UiConfigItemCard::refreshStatus()
{
    if(this->ui_status == nullptr) {
        this->ui_status = new QLabel;
        this->ui_layout_text->addWidget(this->ui_status);
        this->ui_status->setObjectName(QStringLiteral("ui_conf_item_card_status"));
        this->ui_status->setProperty("category", QStringLiteral("conf_item_card_status_label"));
    }
    this->ui_status->clear();
    QString txt;
    if(this->m_restart_flag) {
        txt.append(this->m_p_text->getLoc(QStringLiteral("ui_conf_page_item_card_status_restart")));
    }
    this->ui_status->setText(txt);
}

void UiConfigItemCard::slotValueChanged()
{
    if(this->m_p_item->info->restart) {
        this->m_restart_flag = true;
        refreshStatus();
        emit sigShowRestart();
    }
}
