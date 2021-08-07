/**
 * @file ui/tools/animationstackedwidget.h
 * @brief 界面-工具-带动画的部件堆
 * @author WisteFinch
 * @date 2021.7.30
 *
 * MIT License
 * Copyright (c) 2019-2021 WisteFinch
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

#ifndef ANIMATIONSTACKEDWIDGET_H
#define ANIMATIONSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QPainter>
#include <QVariant>
#include <QPaintEvent>

/**
 * @brief 带动画的部件堆
 */
class AnimationStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit AnimationStackedWidget(QWidget *parent = nullptr);
    ~AnimationStackedWidget();

    /**
     * @brief 类型枚举
     */
    enum TYPE {
        vertical,   ///< 竖向
        horizational///< 横向
    };

    void paintEvent(QPaintEvent *);
    /**
     * @brief 设置参数
     * @param 时长
     * @param 启用快速模式
     * @param 加速倍率
     * @param 启用加速时距离
     *
     */
    void setParam(quint32 duration = 500, bool quickMode = true, quint32 quickRatio = 2, quint32 quickDistance = 2);

    /**
     * @brief 设置动画方向
     * @param 方向类型
     */
    void setType(TYPE t = vertical);

public slots:
    /**
     * @brief 槽：属性动画值改变
     */
    void slotValueChanged(QVariant );
    /**
     * @brief 槽：动画切换完成
     */
    void slotAnimationFinished();
    /**
     * @brief 槽：切换到下一页
     */
    void slotNext();
    /**
     * @brief 槽：切换到上一页
     */
    void slotForward();

    /**
     * @brief 槽：切换到指定页
     * @return 是否成功
     */
    bool slotSetIndex(qint32 i = 0);

private:

    /**
     * @brief 上一页
     */
    void next();

    /**
     * @brief 下一页
     */
    void forward();

    /**
     * @brief 绘制上一页
     */
    void paintPrevious(QPainter &, int);

    /**
     * @brief 绘制下一页
     */
    void paintNext(QPainter &, int);

private:
    QPropertyAnimation *m_p_animation;  ///< 动画框架
    qreal m_duration = 500;  ///< 动画的持续时间
    bool m_is_animation;    ///< 是否正在动画
    bool m_quick_mode_enable = true;    ///< 是否启用快速模式
    bool m_is_quick_mode;   ///< 是否正在快速模式
    quint32 m_quick_ratio = 2;  ///< 加速倍率
    qint32 m_quick_distance = 2;///< 加速时距离
    QVariant m_current_value;   ///< 被Animation动画改变的值
    qint32 m_widget_count;  ///< 保存当前StackWidget里面的子成员数
    qint32 m_next_index;///< 下一个要切换的索引
    qint32 m_change_direction;  ///< 切换方向
    qint32 m_target_index;  ///< 目标索引
    TYPE m_type = vertical;///< 类型
};

#endif // ANIMATIONSTACKEDWIDGET_H
