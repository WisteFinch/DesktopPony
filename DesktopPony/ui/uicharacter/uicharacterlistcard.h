/**
 * @file ui/uicharacter/uicharacterlistcard.h
 * @brief 界面-角色-列表卡片
 * @author WisteFinch
 * @date 2020.11.29
 *
 * MIT License
 * Copyright (c) 2019-2020 WisteFinch
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
