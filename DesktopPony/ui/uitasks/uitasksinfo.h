#ifndef UITASKSINFO_H
#define UITASKSINFO_H

#include "file/localisation.h"
#include "file/filetasks.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QScrollArea>

namespace Ui {
class UITasksInfo;
}

class UITasksInfo : public QWidget
{
    Q_OBJECT

public:
    explicit UITasksInfo(QWidget *parent = nullptr);
    ~UITasksInfo();

    int width;
    int height;
    QImage background;

    QVBoxLayout *m_layout_main; ///< 界面布局

    QScrollArea *m_scrollarea_info; ///< 信息滚动区域
    QWidget *m_widget_info; ///< 信息界面
    QVBoxLayout *m_layout_info; ///<信息布局

    QHBoxLayout *m_layout_group_name; ///< “组名称”布局
    QLabel *m_label_group_name; ///< “组名称”标签
    QTextEdit *m_text_group_name;   ///< “组名称”文本框

    QHBoxLayout *m_layout_group_path;   ///< “组路径”布局
    QLabel *m_label_group_path; ///< “组路径”标签
    QTextEdit *m_text_group_path;   ///< “组路径”文本框

    QHBoxLayout *m_layout_author; ///< “作者”布局
    QLabel *m_label_author;   ///< “作者”标签
    QTextEdit *m_text_author; ///< “作者”文本框

    QHBoxLayout *m_layout_desc;   ///< “介绍”布局
    QLabel *m_label_desc; ///< “介绍”标签
    QTextEdit *m_text_desc;   ///< “介绍”文本框

    QHBoxLayout *m_layout_standard;   ///< “标准”布局
    QLabel *m_label_standard; ///< “标准”标签
    QTextEdit *m_text_standard;   ///< “标准”文本框

    QHBoxLayout *m_layout_group_trigger; ///< “组触发”布局
    QLabel *m_label_group_trigger;   ///< “组触发”标签
    QTextEdit *m_text_group_trigger; ///< “组触发”文本框

    QHBoxLayout *m_layout_group_option; ///< “组选项”布局
    QLabel *m_label_group_option;   ///< “组选项”标签
    QTextEdit *m_text_group_option; ///< “组选项”文本框

    QHBoxLayout *m_layout_events; ///< “事件”布局
    QLabel *m_label_events;   ///< “事件”标签
    QTextEdit *m_text_events; ///< “事件”文本框

    /**
     * @brief 初始化
     * @param 任务信息
     * @param 背景
     */
    void init(Localisation *tmpLocalisation, FileTasks::STaskdata *tmpTaskdata, QImage bg, QSize size);

    /**
     * @brief 设置背景
     */
    void setBackGround();

    /**
     * @brief 初始化文本
     */
    void initText();

    /**
     * @brief 初始化Object名
     */
    void initObjectName();
private:
    Ui::UITasksInfo *ui;

    FileTasks::STaskdata *m_p_data;
    Localisation *m_p_localisation;

};

#endif // UITASKSINFO_H
