#ifndef UITASKSEVENTCARD_H
#define UITASKSEVENTCARD_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextEdit>

class UITasksEventCard
{
public:
    UITasksEventCard();
    ~UITasksEventCard();

    QLabel *m_label_title;
    QTextEdit *m_textedit_text;
    QHBoxLayout *m_layout_main;

    void init(QString title, QString text);
    QHBoxLayout* getLayout();
};

#endif // UITASKSEVENTCARD_H
