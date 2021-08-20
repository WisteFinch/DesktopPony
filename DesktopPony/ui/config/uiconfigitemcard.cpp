#include "uiconfigitemcard.h"

UiConfigItemCard::UiConfigItemCard(QWidget *parent) : QWidget(parent)
{

}

UiConfigItemCard::~UiConfigItemCard()
{
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
    this->ui_conf_item_card_layout_main = new QHBoxLayout;

    this->ui_conf_item_card_layout_main->setSpacing(0);
    this->ui_conf_item_card_layout_main->setMargin(0);
    this->ui_conf_item_card_layout_text = new QVBoxLayout;
    this->ui_conf_item_card_layout_text->setSpacing(0);
    this->ui_conf_item_card_layout_text->setMargin(0);
    this->ui_conf_item_card_layout_main->addLayout(this->ui_conf_item_card_layout_text);
    this->ui_conf_item_card_caption = new QLabel;
    this->ui_conf_item_card_desc = new QLabel;
    this->ui_conf_item_card_name = new QLabel;
    this->ui_conf_item_card_layout_text->addWidget(this->ui_conf_item_card_caption);
    if(m_ptrfunc_get_conf(QStringLiteral("sys_debug")).toBool()) {
        this->ui_conf_item_card_layout_text->addWidget(this->ui_conf_item_card_desc);
        this->ui_conf_item_card_layout_text->addWidget(this->ui_conf_item_card_name);
    }
    this->ui_conf_item_card_layout_main->addStretch();

    this->setLayout(ui_conf_item_card_layout_main);
}

void UiConfigItemCard::initObjectName()
{
    this->setObjectName(QStringLiteral("ui_conf_item_card"));
    // 部件部分
    this->ui_conf_item_card_caption->setObjectName(QStringLiteral("ui_conf_item_card_caption"));
    this->ui_conf_item_card_desc->setObjectName(QStringLiteral("ui_conf_item_card_desc"));
    this->ui_conf_item_card_name->setObjectName(QStringLiteral("ui_conf_item_card_name"));
    // 布局部分
    this->ui_conf_item_card_layout_main->setObjectName(QStringLiteral("ui_conf_item_card_layout_main"));
    this->ui_conf_item_card_layout_text->setObjectName(QStringLiteral("ui_conf_item_card_layout_text"));
}

void UiConfigItemCard::initProperty()
{
    this->setProperty("category", QStringLiteral("conf_item_card"));
    // 部件部分
    this->ui_conf_item_card_caption->setProperty("category", QStringLiteral("conf_item_card_caption_label"));
    this->ui_conf_item_card_desc->setProperty("category", QStringLiteral("conf_item_card_desc_label"));
    this->ui_conf_item_card_name->setProperty("category", QStringLiteral("conf_item_card_name_label"));
}

void UiConfigItemCard::initContent()
{
    if(this->m_p_item->info->type == PluginElementConfigData::config_type_integer) {
        this->ui_conf_item_card_spin_box = new QSpinBox;
        this->ui_conf_item_card_layout_main->addWidget(this->ui_conf_item_card_spin_box);

        this->ui_conf_item_card_spin_box->setObjectName(QStringLiteral("ui_conf_item_card_spin_box"));
        this->ui_conf_item_card_spin_box->setProperty("category", QStringLiteral("conf_item_card_value"));
        this->ui_conf_item_card_spin_box->setEnabled(!this->m_p_item->info->read_only);

        this->ui_conf_item_card_spin_box->setMinimum(this->m_p_item->info->range_from.toInt());
        this->ui_conf_item_card_spin_box->setMaximum(this->m_p_item->info->range_to.toInt());
        this->ui_conf_item_card_spin_box->setValue(this->m_p_item->v.toInt());

        connect(this->ui_conf_item_card_spin_box, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [ = ] {this->m_p_item->v = this->ui_conf_item_card_spin_box->value(); emit sigValueChanged();});
    } else if(this->m_p_item->info->type == PluginElementConfigData::config_type_real) {
        this->ui_conf_item_card_spin_box_real = new QDoubleSpinBox;
        this->ui_conf_item_card_layout_main->addWidget(this->ui_conf_item_card_spin_box_real);

        this->ui_conf_item_card_spin_box_real->setObjectName(QStringLiteral("ui_conf_item_card_spin_box_real"));
        this->ui_conf_item_card_spin_box_real->setProperty("category", QStringLiteral("conf_item_card_value"));
        this->ui_conf_item_card_spin_box_real->setEnabled(!this->m_p_item->info->read_only);

        this->ui_conf_item_card_spin_box_real->setMinimum(this->m_p_item->info->range_from.toDouble());
        this->ui_conf_item_card_spin_box_real->setMaximum(this->m_p_item->info->range_to.toDouble());
        this->ui_conf_item_card_spin_box_real->setValue(this->m_p_item->v.toDouble());

        connect(this->ui_conf_item_card_spin_box_real, static_cast<void (QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [ = ] {this->m_p_item->v = this->ui_conf_item_card_spin_box_real->value(); emit sigValueChanged();});
    } else if(this->m_p_item->info->type == PluginElementConfigData::config_type_bool) {
        this->ui_conf_item_card_switch = new QCheckBox;
        this->ui_conf_item_card_layout_main->addWidget(this->ui_conf_item_card_switch);

        this->ui_conf_item_card_switch->setObjectName(QStringLiteral("ui_conf_item_card_switch"));
        this->ui_conf_item_card_switch->setProperty("category", QStringLiteral("conf_item_card_value"));
        this->ui_conf_item_card_switch->setEnabled(!this->m_p_item->info->read_only);

        this->ui_conf_item_card_switch->setChecked(this->m_p_item->v.toBool());

        connect(this->ui_conf_item_card_switch, &QCheckBox::stateChanged, this, [ = ] {this->m_p_item->v = this->ui_conf_item_card_switch->checkState() ==  Qt::Unchecked ? false : true; emit sigValueChanged();});
    } else if(this->m_p_item->info->type == PluginElementConfigData::config_type_string) {
        this->ui_conf_item_card_lineedit = new QLineEdit;
        this->ui_conf_item_card_layout_main->addWidget(this->ui_conf_item_card_lineedit);

        this->ui_conf_item_card_lineedit->setObjectName(QStringLiteral("ui_conf_item_card_lineedit"));
        this->ui_conf_item_card_lineedit->setProperty("category", QStringLiteral("conf_item_card_value"));
        this->ui_conf_item_card_lineedit->setEnabled(!this->m_p_item->info->read_only);

        this->ui_conf_item_card_lineedit->setText(this->m_p_item->v.toString());

        connect(this->ui_conf_item_card_lineedit, &QLineEdit::textEdited, this, [ = ] {this->m_p_item->v = this->ui_conf_item_card_lineedit->text(); emit sigValueChanged();});
    } else if(this->m_p_item->info->type == PluginElementConfigData::config_type_select) {
        this->ui_conf_item_card_combobox = new QComboBox;
        this->ui_conf_item_card_combobox->setView(new QListView(this));
        this->ui_conf_item_card_combobox->view()->parentWidget()->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
        this->ui_conf_item_card_combobox->view()->parentWidget()->setAttribute(Qt::WA_TranslucentBackground);
        this->ui_conf_item_card_layout_main->addWidget(this->ui_conf_item_card_combobox);

        this->ui_conf_item_card_combobox->setObjectName(QStringLiteral("ui_conf_item_card_combobox"));
        this->ui_conf_item_card_combobox->setProperty("category", QStringLiteral("conf_item_card_value"));
        this->ui_conf_item_card_combobox->setEnabled(!this->m_p_item->info->read_only);

        for(int i = 0 ; i < this->m_p_item->info->list.count(); i++) {
            this->ui_conf_item_card_combobox->addItem(this->m_p_text->get(this->m_p_item->info->list.at(i).name, this->m_p_item->info->list.at(i).obj_uuid), i);
        }

        for(int i = 0; i < this->m_p_item->info->list.count(); i++) {
            if(this->m_p_item->v.toString() == this->m_p_item->info->list.at(i).id) {
                this->ui_conf_item_card_combobox->setCurrentIndex(i);
            }
        }

        connect(this->ui_conf_item_card_combobox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [ = ](int index) {
            this->m_p_item->v = this->m_p_item->info->list.at(index).id;
            emit sigValueChanged();
        });
    }
    this->ui_conf_item_card_caption->setText(this->m_p_text->get(this->m_p_item->info->caption, this->m_p_item->obj_uuid));
    this->ui_conf_item_card_caption->setToolTip(this->m_p_text->get(this->m_p_item->info->desc, this->m_p_item->obj_uuid));
    this->ui_conf_item_card_desc->setText(this->m_p_text->get(this->m_p_item->info->desc, this->m_p_item->obj_uuid));
    this->ui_conf_item_card_name->setText(this->m_p_item->info->config_name);
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
    if(this->ui_conf_item_card_status == nullptr) {
        this->ui_conf_item_card_status = new QLabel;
        this->ui_conf_item_card_layout_text->addWidget(this->ui_conf_item_card_status);
        this->ui_conf_item_card_status->setObjectName(QStringLiteral("ui_conf_item_card_status"));
        this->ui_conf_item_card_status->setProperty("category", QStringLiteral("conf_item_card_status_label"));
    }
    this->ui_conf_item_card_status->clear();
    QString txt;
    if(this->m_restart_flag) {
        txt.append(this->m_p_text->getLoc(QStringLiteral("ui_conf_page_item_card_status_restart")));
    }
    this->ui_conf_item_card_status->setText(txt);
}

void UiConfigItemCard::slotValueChanged()
{
    if(this->m_p_item->info->restart) {
        QMessageBox::StandardButton b = QMessageBox::warning(this, this->m_p_text->getLoc("ui_conf_page_msgbox_restart_tit"), this->m_p_text->getLoc("ui_conf_page_msgbox_restart_txt"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        if(b == QMessageBox::Yes) {
            emit sigRestart();
        } else if(!this->m_restart_flag) {
            this->m_restart_flag = true;
            refreshStatus();
        }
    }
}
