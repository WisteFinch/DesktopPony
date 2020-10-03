#ifndef UICHARACTERLISTCARD_H
#define UICHARACTERLISTCARD_H

#include<QWidget>
#include<QLabel>
#include<QHBoxLayout>
#include <QMouseEvent>

class UICharacterListCard : public QWidget
{
    Q_OBJECT
public:
    explicit UICharacterListCard(QWidget *parent = nullptr);
    ~UICharacterListCard();

    enum Status{NORMAL, HOVER, PRESS, SELECTED};

    void init(QString name, QString path);
    void setStatus(Status flag);

    QHBoxLayout *m_p_layout_main;
    QLabel *m_p_label_status;
    QLabel *m_p_label_image;
    QLabel *m_p_label_name;

    QString m_str_name;
    QString m_str_path;
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

#endif // UICHARACTERLISTCARD_H
