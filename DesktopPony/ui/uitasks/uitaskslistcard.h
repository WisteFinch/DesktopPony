#ifndef UITASKSLISTCARD_H
#define UITASKSLISTCARD_H

#include "file/filetasks.h"
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>

class UITasksListCard : public QWidget
{
    Q_OBJECT
public:
    explicit UITasksListCard(QWidget *parent = nullptr);
    ~UITasksListCard();

    enum Status{NORMAL, HOVER, PRESS, SELECTED};

    void init(FileTasks::STaskdata *p);
    void setStatus(Status flag);

    QHBoxLayout *m_p_layout_main;
    QLabel *m_p_label_status;
    QLabel *m_p_label_image;
    QLabel *m_p_label_name;

    FileTasks::STaskdata *m_p_taskdata;
    Status m_e_status;
    bool m_b_mouse_press;

signals:
    void clicked(FileTasks::STaskdata*);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // UITASKSLISTCARD_H
