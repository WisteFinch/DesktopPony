#include "uicharacter.h"
#include "ui_uicharacter.h"

UICharacter::UICharacter(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UICharacter)
{
    ui->setupUi(this);

    this->m_layout_main = new QHBoxLayout();

    this->m_label_metadata_name = new QLabel();
    this->m_label_metadata_author = new QLabel();
    this->m_label_metadata_desc = new QLabel();
    this->m_label_metadata_standard = new QLabel();
    this->m_layout_button = new QVBoxLayout();
    this->m_button_addnew = new QPushButton();
    this->m_button_edit = new QPushButton();
    this->m_button_delete = new QPushButton();
    this->m_button_refresh = new QPushButton();
    this->m_button_confirm = new QPushButton();
    this->m_button_cancel = new QPushButton();

    this->m_scrollarea_character = new QScrollArea();
    this->m_widget_character = new QWidget();
    this->m_layout_character = new QVBoxLayout();
    this->m_list_card = new QList<UICharacterListCard *>;

    setAttribute(Qt::WA_ShowModal, true);    //设置模态
}

UICharacter::~UICharacter()
{
    doClearList();
    delete this->m_layout_character;
    delete this->m_widget_character;
    delete this->m_scrollarea_character;
    delete this->m_label_metadata_name;
    delete this->m_label_metadata_author;
    delete this->m_label_metadata_desc;
    delete this->m_label_metadata_standard;
    delete this->m_button_addnew;
    delete this->m_button_edit;
    delete this->m_button_delete;
    delete this->m_button_refresh;
    delete this->m_button_confirm;
    delete this->m_button_cancel;
    delete this->m_layout_button;
    delete this->m_layout_main;
    delete ui;
}

void UICharacter::closeEvent(QCloseEvent *event)
{
    event->ignore();
    emit signalBack(nullptr);
}

void UICharacter::init(Localisation *tmpLocalisation, Config *tmpConfig, FileCharacter *tmpFileCharacter, QImage bg, QPoint pos, QSize size)
{
    this->m_p_localisation = tmpLocalisation;
    this->m_p_config = tmpConfig;
    this->m_p_file_character = tmpFileCharacter;
    tmpLocalisation = nullptr;
    tmpConfig = nullptr;
    tmpFileCharacter = nullptr;
    this->width = size.width();
    this->height = size.height();

    this->background = bg;
    //=========================test============================

    QFile *a = new QFile;
    a->setFileName(":/resources/css/default.css");
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

    this->m_label_metadata_name->setFixedWidth(85);
    this->m_label_metadata_name->setWordWrap(true);
    this->m_layout_button->addWidget(this->m_label_metadata_name);
    this->m_label_metadata_author->setFixedWidth(85);
    this->m_label_metadata_author->setWordWrap(true);
    this->m_layout_button->addWidget(this->m_label_metadata_author);
    this->m_label_metadata_desc->setFixedWidth(85);
    this->m_label_metadata_desc->setWordWrap(true);
    this->m_layout_button->addWidget(this->m_label_metadata_desc);
    this->m_label_metadata_standard->setFixedWidth(85);
    this->m_label_metadata_standard->setWordWrap(true);
    this->m_layout_button->addWidget(this->m_label_metadata_standard);
    this->m_layout_button->addStretch();
    this->m_layout_button->addWidget(this->m_button_addnew);
    this->m_layout_button->addWidget(this->m_button_edit);
    this->m_layout_button->addWidget(this->m_button_delete);
    this->m_layout_button->addWidget(this->m_button_refresh);
    this->m_layout_button->addSpacing(5);
    this->m_layout_button->addWidget(this->m_button_confirm);
    this->m_layout_button->addWidget(this->m_button_cancel);
    this->m_layout_button->setSpacing(0);
    this->m_layout_main->addLayout(this->m_layout_button);

    this->m_layout_character->setSpacing(0);
    this->m_widget_character->setLayout(this->m_layout_character);
    m_layout_character->setMargin(0);
    this->m_scrollarea_character->setWidget(this->m_widget_character);
    this->m_scrollarea_character->setWidgetResizable(true);
    this->m_layout_main->addWidget(this->m_scrollarea_character);

    this->m_layout_main->setSpacing(0);
    this->setLayout(this->m_layout_main);

    initText(); //初始化文本
    initConnect();  //初始化连接
    doLoadList();   //读取列表

    this->move(pos);
}

void UICharacter::initObjectName()
{
    this->setObjectName("uicharacter_widget");

    this->m_label_metadata_name->setObjectName("uicharacter_label_metadata_name");
    this->m_label_metadata_author->setObjectName("uicharacter_label_metadata_author");
    this->m_label_metadata_desc->setObjectName("uicharacter_label_metadata_desc");
    this->m_label_metadata_standard->setObjectName("uicharacter_label_metadata_standard");
    this->m_layout_main->setObjectName("uicharacter_layout_main");
    this->m_layout_button->setObjectName("uicharacter_layout_button");
    this->m_scrollarea_character->setObjectName("uicharacter_scrollarea_character");
    this->m_widget_character->setObjectName("uicharacter_widget_character");
    this->m_layout_character->setObjectName("uicharacter_layout_character");
    this->m_button_addnew->setObjectName("uicharacter_button_addnew");
    this->m_button_edit->setObjectName("uicharacter_button_edit");
    this->m_button_delete->setObjectName("uicharacter_button_delete");
    this->m_button_refresh->setObjectName("uicharacter_button_refresh");
    this->m_button_confirm->setObjectName("uicharacter_button_confirm");
    this->m_button_cancel->setObjectName("uicharacter_button_cancel");
}

void UICharacter::initText()
{
    this->setWindowTitle(this->m_p_localisation->get(this->objectName()));

    this->m_button_addnew->setText(this->m_p_localisation->get(this->m_button_addnew->objectName()));
    this->m_button_edit->setText(this->m_p_localisation->get(this->m_button_edit->objectName()));
    this->m_button_delete->setText(this->m_p_localisation->get(this->m_button_delete->objectName()));
    this->m_button_refresh->setText(this->m_p_localisation->get(this->m_button_refresh->objectName()));
    this->m_button_confirm->setText(this->m_p_localisation->get(this->m_button_confirm->objectName()));
    this->m_button_cancel->setText(this->m_p_localisation->get(this->m_button_cancel->objectName()));

}

void UICharacter::initConnect()
{
    connect(this->m_button_addnew, SIGNAL(clicked()), this, SLOT(slotAddCharacter()));
    connect(this->m_button_delete, SIGNAL(clicked()), this, SLOT(slotDeleteCharacter()));
    connect(this->m_button_refresh, SIGNAL(clicked()), this, SLOT(slotRefreshList()));

    connect(this->m_button_confirm, SIGNAL(clicked()), this, SLOT(slotConfirm()));
    connect(this->m_button_cancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
}

void UICharacter::setBackGround()
{
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Background, QBrush(QPixmap::fromImage(this->background)));
    this->setPalette(palette);
}

void UICharacter::doClearList()
{
    QLayoutItem *c;
    while ((c = this->m_layout_character->takeAt(0)) != nullptr) {
        //setParent为NULL，防止删除之后界面不消失
        if(c->widget()) {
            c->widget()->setParent(nullptr);
        }

        delete c;
    }
    for(int i = 0; i < this->m_list_card->size(); i++) {
        disconnect(this->m_list_card->at(i), SIGNAL(clicked(QString)), this, SLOT(slotListSelected(QString)));
        this->m_list_card->at(i)->setParent(nullptr);
        this->m_layout_character->removeWidget(this->m_list_card->at(i));
        delete this->m_list_card->at(i);
    }
    delete this->m_list_card;
    this->m_list_card = new QList<UICharacterListCard *>;
}

void UICharacter::doLoadList()
{
    doClearList();

    QFile *loadFile = new QFile;
    loadFile->setFileName(STR_CHARACTER_ROOT_PATH + "/index.json");
    if(!loadFile->open(QIODevice::ReadOnly)) {
        return;
    }

    QByteArray allData = loadFile->readAll();
    loadFile->close();
    delete loadFile;
    loadFile = nullptr;

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError) {
        return;
    }

    QJsonObject obj = jsonDoc.object();
    QStringList keys = obj.keys();

    if(obj.contains(this->m_p_config->get("characterName").toString())) {
        UICharacterListCard *card = new UICharacterListCard;
        card->init(this->m_p_config->get("characterName").toString(), obj.value(this->m_p_config->get("characterName").toString()).toString());
        this->m_list_card->push_back(card);
        this->m_layout_character->addWidget(card);
        connect(card, SIGNAL(clicked(QString)), this, SLOT(slotListSelected(QString)));
        slotListSelected(this->m_p_config->get("characterName").toString());
    }
    for(int i = 0; i < keys.size(); i++) {
        if(this->m_p_config->get("characterName").toString() != keys.at(i)) {
            UICharacterListCard *card = new UICharacterListCard;
            card->init(keys.at(i), obj.value(keys.at(i)).toString());
            this->m_list_card->push_back(card);
            this->m_layout_character->addWidget(card);
            connect(card, SIGNAL(clicked(QString)), this, SLOT(slotListSelected(QString)));
        }
    }

    this->m_layout_character->addStretch();
}

QString UICharacter::addSerial(QString str)
{
    int x = str.lastIndexOf('_');
    if(x == -1) {
        return str + "_1";
    }
    QString n = str.right(str.size() - x - 1);

    if(isNum(n)) {
        return str.left(x + 1) + QString::number(n.toInt() + 1);
    } else {
        return str + "_1";
    }
}

bool UICharacter::isNum(QString str)
{
    for(int i = 0; i < str.size(); i++)
        if(!str[i].isNumber()) {
            return false;
        }
    return true;
}

/**********Public slots**********/

void UICharacter::slotAddCharacter()
{
    int flag = QMessageBox::question(this,
                                     this->m_p_localisation->get("uicharacter_msgbox_addnew_title"),
                                     this->m_p_localisation->get("uicharacter_msgbox_addnew_content"),
                                     this->m_p_localisation->get("uicharacter_msgbox_addnew_blank"),
                                     this->m_p_localisation->get("uicharacter_msgbox_addnew_files"),
                                     this->m_p_localisation->get("uicharacter_msgbox_addnew_cancel"),
                                     2,
                                     2);

    if(flag == 0) {

    } else if (flag == 1) {
        QStringList list = QFileDialog::getOpenFileNames(this,
                           this->m_p_localisation->get("uicharacter_msgbox_addnew_files_title"),
                           ".",
                           "JSON(*.json);;ALL(*.*)");

        QJsonObject indexObj = this->m_p_file_character->getCharacterIndexJsonObject();

        for(int i = 0; i < list.size(); i++) {
            QFileInfo copyFileInfo(list.at(i));
            QString copyFileBaseName = copyFileInfo.baseName();
            QString copyFileSuffix = copyFileInfo.suffix();
            while(QFile::exists(STR_CHARACTER_ROOT_PATH + "/" + copyFileBaseName + '.' + copyFileSuffix)) {
                copyFileBaseName = addSerial(copyFileBaseName);
            }
            QString copyFilePath = STR_CHARACTER_ROOT_PATH + "/" + copyFileBaseName + '.' + copyFileSuffix;
            QFile::copy(copyFileInfo.filePath(), copyFilePath);

            QJsonObject copyObj;
            QFile copyFile(copyFilePath);
            if(copyFile.open(QIODevice::ReadOnly)) {
                QByteArray copyData = copyFile.readAll();
                copyFile.close();
                QJsonParseError json_error;
                QJsonDocument jsonDoc(QJsonDocument::fromJson(copyData, &json_error));
                if(json_error.error == QJsonParseError::NoError) {
                    copyObj = jsonDoc.object();
                    QJsonObject metadata = copyObj.value("metadata").toObject();
                    if(!metadata.contains("name")) {
                        copyFile.remove();
                        continue;
                    }
                    while(indexObj.contains(metadata.value("name").toString())) {
                        metadata.insert("name", addSerial(metadata.value("name").toString()));
                        copyObj.insert("metadata", metadata);

                        copyFile.remove();
                        copyFile.open(QFile::ReadWrite);
                        jsonDoc.setObject(copyObj);
                        copyFile.write(jsonDoc.toJson());
                        copyFile.close();
                    }
                }
            }
            this->m_p_file_character->initCharacterIndex();
            indexObj = this->m_p_file_character->getCharacterIndexJsonObject();
        }
    }

    slotRefreshList();
}
void UICharacter::slotEditCharacter()
{

}
void UICharacter::slotDeleteCharacter()
{
    QString name = this->m_str_selected_character;
    int flag = QMessageBox::warning(this,
                                    this->m_p_localisation->get("uicharacter_msgbox_delete_title"),
                                    this->m_p_localisation->get("uicharacter_msgbox_delete_content_1") + name + this->m_p_localisation->get("uicharacter_msgbox_delete_content_2"),
                                    this->m_p_localisation->get("uicharacter_msgbox_delete_confirm"),
                                    this->m_p_localisation->get("uicharacter_msgbox_delete_cancel"),
                                    nullptr,
                                    1,
                                    1);

    if(flag == 0) {
        QFile *loadFile = new QFile;
        loadFile->setFileName(STR_CHARACTER_ROOT_PATH + "/index.json");
        if(!loadFile->open(QIODevice::ReadOnly)) {
            return;
        }

        QByteArray allData = loadFile->readAll();
        loadFile->close();
        delete loadFile;
        loadFile = nullptr;

        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

        if(json_error.error != QJsonParseError::NoError) {
            return;
        }

        QJsonObject obj = jsonDoc.object();
        QString path = obj.value(name).toString();

        QFile::remove(path);
        slotRefreshList();
    }
}
void UICharacter::slotRefreshList()
{
    this->m_p_file_character->initCharacterIndex();
    doLoadList();
}

void UICharacter::slotConfirm()
{
    emit signalBack(this->m_str_selected_character);
}
void UICharacter::slotCancel()
{
    emit signalBack(nullptr);
}

void UICharacter::slotListChange(QString path)
{
    QFile *loadFile = new QFile;
    loadFile->setFileName(path);
    if(!loadFile->open(QIODevice::ReadOnly)) {
        //return;
    }

    QByteArray allData = loadFile->readAll();
    loadFile->close();
    delete loadFile;
    loadFile = nullptr;

    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

    if(json_error.error != QJsonParseError::NoError) {
        //return;
    }

    QJsonObject metadata = jsonDoc.object().value("metadata").toObject();
    this->m_label_metadata_name->setText(this->m_p_localisation->get(this->m_label_metadata_name->objectName()) + metadata.value("name").toString());
    this->m_label_metadata_author->setText(this->m_p_localisation->get(this->m_label_metadata_author->objectName()) + metadata.value("author").toString());
    this->m_label_metadata_desc->setText(this->m_p_localisation->get(this->m_label_metadata_desc->objectName()) + metadata.value("desc").toString());
    this->m_label_metadata_standard->setText(this->m_p_localisation->get(this->m_label_metadata_standard->objectName()) + metadata.value("standard").toString());

}

void UICharacter::slotListSelected(QString name)
{
    this->m_str_selected_character = name;
    for(int i = 0; i < this->m_list_card->size(); i++) {
        if(this->m_list_card->at(i)->m_str_name == name) {
            this->m_list_card->at(i)->setStatus(UICharacterListCard::SELECTED);
        } else {
            if(this->m_list_card->at(i)->m_e_status == UICharacterListCard::SELECTED) {
                this->m_list_card->at(i)->setStatus(UICharacterListCard::NORMAL);
            }
        }
    }

    for(int i = 0; i < this->m_list_card->size(); i++)
        if(this->m_list_card->at(i)->m_str_name == name) {
            slotListChange(this->m_list_card->at(i)->m_str_path);
        }
}
