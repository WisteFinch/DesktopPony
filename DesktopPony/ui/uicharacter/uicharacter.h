#ifndef UICHARACTER_H
#define UICHARACTER_H

#include "file/localisation.h"
#include "imageprocessing/imageprocessing.h"
#include "file/config.h"
#include "file/filecharacter.h"
#include "uicharacterlistcard.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QImage>
#include <QPixmap>
#include <QFile>
#include <QCloseEvent>
#include <QListWidget>
#include <QFileDialog>

namespace Ui {
class UICharacter;
}

class UICharacter : public QWidget
{
    Q_OBJECT

public:
    explicit UICharacter(QWidget *parent = nullptr);
    ~UICharacter();

    int width;
    int height;
    QImage background;

    void closeEvent(QCloseEvent *event);

    /**
     * @brief 初始化
     */
    void init(Localisation *tmpLocalisation, Config *tmpConfig, FileCharacter *tmpFileCharacter, QImage bg, QPoint pos, QSize size);

    /**
     * @brief 初始化文本
     */
    void initText();

    /**
     * @brief 初始化Object名
     */
    void initObjectName();

    /**
     * @brief 初始化连接
     */
    void initConnect();

    /**
     * @brief 设置背景
     */
    void setBackGround();

    /**
     * @brief 读取角色列表
     */
    void doLoadList();

    /**
     * @brief 清空角色列表
     */
    void doClearList();

    /**
     * @brief 添加序号
     * @param 字符串
     * @return 添加序号后的字符串
     */
    QString addSerial(QString str);

    /**
     * @brief 判断是否为数字
     * @param 字符串
     * @return bool
     */
    bool isNum(QString str);

public slots:
    /**
     * @brief 槽：添加角色
     */
    void slotAddCharacter();
    /**
     * @brief 槽：编辑角色
     */
    void slotEditCharacter();
    /**
     * @brief 槽：删除角色
     */
    void slotDeleteCharacter();
    /**
     * @brief 槽：刷新角色列表
     */
    void slotRefreshList();

    /**
     * @brief 槽：确认
     */
    void slotConfirm();
    /**
     * @brief 槽：取消
     */
    void slotCancel();

    /**
     * @brief 槽：列表改变
     */
    void slotListChange(QString path);

    /**
     * @brief 槽：列表选定
     * @param 名称
     */
    void slotListSelected(QString name);

signals:

    void signalBack(QString name);

private:
    Ui::UICharacter *ui;

    QHBoxLayout *m_layout_main;///<主界面布局
    QVBoxLayout *m_layout_button;///<按钮布局
    QScrollArea *m_scrollarea_character;///<角色列表
    QWidget *m_widget_character;///<角色列表-界面
    QVBoxLayout *m_layout_character;///<角色列表-布局
    QList<UICharacterListCard*> *m_list_card;///<角色列表-卡片

    QLabel *m_label_metadata_name;///<“元数据-名称”标签
    QLabel *m_label_metadata_author;///<“元数据-作者”标签
    QLabel *m_label_metadata_desc;///<“元数据-介绍”标签
    QLabel *m_label_metadata_standard;///<“元数据-标准”标签
    QPushButton *m_button_addnew;///<新建按钮
    QPushButton *m_button_edit;///<编辑按钮
    QPushButton *m_button_delete;///<删除按钮
    QPushButton *m_button_refresh;///<刷新按钮
    QPushButton *m_button_confirm;///<确认按钮
    QPushButton *m_button_cancel;///<取消按钮


    QString m_str_selected_character;

    Localisation *m_p_localisation;
    Config *m_p_config;
    FileCharacter *m_p_file_character;

};

#endif // UICHARACTER_H
