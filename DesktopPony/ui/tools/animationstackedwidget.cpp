#include "animationstackedwidget.h"

AnimationStackedWidget::AnimationStackedWidget(QWidget *parent): QStackedWidget(parent)
{
    this->m_is_animation = false;
    this->m_is_quick_mode = false;
    this->m_type = vertical;
    this->m_change_direction = 0;
    //初始化动画框架、并连接信号和槽
    this->m_p_animation = new QPropertyAnimation(this, QByteArray());
    this->m_p_animation->setEasingCurve(QEasingCurve::OutQuint);
    connect(this->m_p_animation, &QPropertyAnimation::valueChanged, this, &AnimationStackedWidget::slotValueChanged);
    connect(this->m_p_animation, &QPropertyAnimation::finished, this, &AnimationStackedWidget::slotAnimationFinished);
}

AnimationStackedWidget::~AnimationStackedWidget()
{

}

void AnimationStackedWidget::setParam(quint32 duration, bool quickMode, quint32 quickRatio, quint32 quickDistance)
{
    this->m_duration = duration;
    this->m_quick_mode_enable = quickMode;
    this->m_quick_ratio = quickRatio;
    this->m_quick_distance = quickDistance;
}

void AnimationStackedWidget::setType(TYPE t)
{
    this->m_type = t;
}

void AnimationStackedWidget::next()
{
    // 如果正在动画则结束
    if(this->m_is_animation) {
    } else {
        this->m_is_animation = true;
        this->m_widget_count = count();
        int c = currentIndex();
        // 计算下一页的索引
        this->m_next_index = (c + 1) % this->m_widget_count;
        // 隐藏当前的widget
        widget(c)->hide();
        // 开始动画并设置间隔和开始、结束值
        QRect g = geometry();
        this->m_p_animation->setStartValue(this->m_type == vertical ? g.height() : g.width());
        this->m_p_animation->setEndValue(0);
        this->m_p_animation->setDuration(this->m_duration);
        this->m_p_animation->start();
    }
}

void AnimationStackedWidget::forward()
{
    // 如果正在动画则结束
    if(this->m_is_animation) {
    } else {
        this->m_is_animation = true;
        this->m_widget_count = count();
        int c = currentIndex();
        // 计算下一页的索引
        this->m_next_index = (c + this->m_widget_count - 1) % this->m_widget_count;
        // 隐藏当前的部件
        widget(c)->hide();
        // 开始动画并设置参数
        QRect g = geometry();
        this->m_p_animation->setStartValue(0);
        this->m_p_animation->setEndValue(this->m_type == vertical ? g.height() : g.width());
        this->m_p_animation->setDuration(this->m_duration);
        this->m_p_animation->start();
    }
}

void AnimationStackedWidget::slotValueChanged(QVariant value)
{
    this->m_current_value = value;
    update();
}

bool AnimationStackedWidget::slotSetIndex(qint32 i)
{
    if(!m_is_animation) {
        this->m_target_index = i;
        // 计算切换方向
        // 计算距离
        qint32 dForward = i > this->currentIndex() ? this->currentIndex() + this->count() - i : this->currentIndex() - i;
        qint32 dBackward = i >= this->currentIndex() ? i - this->currentIndex() : i + this->count() - this->currentIndex();
        qint32 dMin = dForward > dBackward ? dBackward : dForward;
        // 计算是否开启快速模式
        if(dMin >= this->m_quick_distance && m_quick_mode_enable) {
            this->m_is_quick_mode = true;
            this->m_duration /= this->m_quick_ratio;
            this->m_p_animation->setEasingCurve(QEasingCurve::Linear);
        }
        if(dMin == 0) {
            return true;
        }
        if(i >= this->currentIndex()) {
            if(dForward >= dBackward) {
                this->m_change_direction = 1;
                next();
            } else {
                this->m_change_direction = -1;
                forward();
            }
        } else {
            if(dForward > dBackward) {
                this->m_change_direction = 1;
                next();
            } else {
                this->m_change_direction = -1;
                forward();
            }
        }
        return true;
    } else {
        return false;
    }
}

void AnimationStackedWidget::slotNext()
{
    this->m_widget_count = count();
    int c = currentIndex();
    // 计算目标索引
    this->m_target_index = (c + 1) % this->m_widget_count;
    next();
}

void AnimationStackedWidget::slotForward()
{
    this->m_widget_count = count();
    int c = currentIndex();
    // 计算目标索引
    this->m_target_index = (c + this->m_widget_count - 1) % this->m_widget_count;
    forward();
}

void AnimationStackedWidget::paintEvent(QPaintEvent *e)
{
    if(this->m_is_animation) {
        QPainter paint(this);
        // 绘制当前部件
        paintPrevious(paint, currentIndex());
        // 绘制下一个部件
        paintNext(paint, this->m_next_index);
    }
    e->ignore();
}

void AnimationStackedWidget::paintPrevious(QPainter &paint, int currentIndex)
{
    // 获得当前页面的部件
    QWidget *w = widget(currentIndex);
    QPixmap pixmap(w->size());
    pixmap.fill(Qt::transparent);
    // 将部件的内容渲染到QPixmap中
    w->render(&pixmap);
    QRect r = w->geometry();
    // 绘制当前的部件
    double value = this->m_current_value.toDouble();
    if(this->m_change_direction == 1) {
        if(m_type == vertical) {
            QRectF r1(0.0, 0.0, r.width(), value);
            QRectF r2(0.0, r.height() - value, r.width(), value);
            paint.drawPixmap(r1, pixmap, r2);
        } else {
            QRectF r1(0.0, 0.0, value, r.height());
            QRectF r2(r.width() - value, 0, value, r.height());
            paint.drawPixmap(r1, pixmap, r2);
        }
    } else {
        if(m_type == vertical) {
            QRectF r1(0.0, value, r.width(), r.height() - value);
            QRectF r2(0.0, 0.0, r.width(), r.height() - value);
            paint.drawPixmap(r1, pixmap, r2);
        } else {
            QRectF r1(value, 0.0, r.width() - value, r.height());
            QRectF r2(0.0, 0, r.width() - value, r.height());
            paint.drawPixmap(r1, pixmap, r2);
        }
    }
}

void AnimationStackedWidget::paintNext(QPainter &paint, int nextIndex)
{
    // 获得下一页面的部件
    QWidget *nextWidget = widget(nextIndex);
    QRect r = geometry();
    // 为下一child分配大小
    nextWidget->resize(r.width(), r.height());
    // 将部件的内容渲染到QPixmap中
    QPixmap nextPixmap(nextWidget->size());
    nextPixmap.fill(Qt::transparent);
    nextWidget->render(&nextPixmap);
    double value = this->m_current_value.toDouble();
    if(this->m_change_direction == 1) {
        if(this->m_type == vertical) {
            QRectF r1(0.0, value, r.width(), r.height() - value);
            QRectF r2(0.0, 0.0, r.width(), r.height() - value);
            paint.drawPixmap(r1, nextPixmap, r2);
        } else {
            QRectF r1(value, 0.0, r.width() - value, r.height());
            QRectF r2(0.0, 0.0, r.width() - value, r.height());
            paint.drawPixmap(r1, nextPixmap, r2);
        }
    } else {
        if(this->m_type == vertical) {
            QRectF r1(0.0, 0.0, r.width(), value);
            QRectF r2(0.0, r.height() - value, r.width(), value);
            paint.drawPixmap(r1, nextPixmap, r2);
        } else {
            QRectF r1(0.0, 0.0, value, r.height());
            QRectF r2(r.width() - value, 0.0, value, r.height());
            paint.drawPixmap(r1, nextPixmap, r2);
        }
    }
}

void AnimationStackedWidget::slotAnimationFinished()
{
    m_is_animation = false;
    widget(currentIndex())->show();
    setCurrentIndex(this->m_next_index);
    //关闭快速模式
    if(qAbs(this->m_target_index - this->currentIndex()) <= 1 && m_is_quick_mode) {
        this->m_is_quick_mode = false;
        this->m_duration *= this->m_quick_ratio;
        this->m_p_animation->setEasingCurve(QEasingCurve::OutQuint);
    }
    // 如果当前页不在目标页，则继续切换
    if(this->m_target_index != currentIndex()) {
        if(this->m_change_direction == -1) {
            forward();
        }
        if(this->m_change_direction == 1) {
            next();
        }
    } else {
        this->m_change_direction = 0;
    }
}

