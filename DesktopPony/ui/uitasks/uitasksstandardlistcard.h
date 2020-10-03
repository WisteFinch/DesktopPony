#ifndef UITASKSSTANDARDLISTCARD_H
#define UITASKSSTANDARDLISTCARD_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>

class UITasksStandardListCard : public QWidget
{
    Q_OBJECT
public:
    explicit UITasksStandardListCard(QWidget *parent = nullptr);
    ~UITasksStandardListCard();

    enum Status{NORMAL, HOVER, PRESS, SELECTED};

    void init(QString str);
    void setStatus(Status flag);

    QHBoxLayout *m_p_layout_main;
    QLabel *m_p_label_status;
    QLabel *m_p_label_image;
    QLabel *m_p_label_name;

    QString m_str_name;
    Status m_e_status;
    bool m_b_mouse_press;

signals:
    void clicked(QString);

protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // UITASKSSTANDARDLISTCARD_H
