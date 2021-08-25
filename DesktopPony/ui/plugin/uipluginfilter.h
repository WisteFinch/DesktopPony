/**
 * @file ui/plugin/uipluginfilter.h
 * @brief 界面-插件-插件筛选器
 * @author WisteFinch
 * @date 2021.8.18
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

#ifndef UIPLUGINFILTER_H
#define UIPLUGINFILTER_H

#include <data/text.h>
#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QTimer>

class UiPluginFilter : public QWidget
{
    Q_OBJECT
public:
    explicit UiPluginFilter(QWidget *parent = nullptr);
    ~UiPluginFilter();

    bool m_filter_loc = true;   ///< 本地化筛选
    bool m_filter_style = true; ///< 样式筛选
    bool m_filter_event = true; ///< 事件筛选
    bool m_filter_action = true;///< 动作筛选
    bool m_filter_acc = true;   ///< 部件筛选
    bool m_filter_model = true; ///< 模型筛选
    bool m_filter_conf = true;  ///< 配置筛选
    bool m_filter_rc = true;///< 资源筛选
    bool m_filter_lib = true;   ///< 库筛选
    bool m_filter_null = true;  ///< 空白筛选


    /**
     * @brief 初始化
     * @details 初始化界面内容
     * @param 文本指针
     * @param 父按钮
     */
    void init(Text *ptrText, QPushButton *ptrParent);

    /**
     * @brief 显示筛选器
     */
    void showFilter();

    /**
     * @brief 重置
     */
    void reset();
signals:
    void sigFilterChanged();   ///< 信号：筛选器改变

private:
    Text *m_p_text = nullptr;   ///< 文本
    QPushButton *m_p_parent_button; ///< 父按钮
    QTimer *m_p_hide_timer; ///< 隐藏筛选器计时器

    bool m_is_mouse_entered;  ///< 鼠标是否移入
    int m_mouse_unentered_time; ///< 鼠标未移入时间

    // 部件部分
    QWidget *ui_widget = nullptr;
    QLabel *ui_localisation = nullptr;  ///< 本地化
    QLabel *ui_style = nullptr; ///< 样式
    QLabel *ui_event = nullptr; ///< 事件
    QLabel *ui_action = nullptr;///< 动作
    QLabel *ui_accessories = nullptr;   ///< 部件
    QLabel *ui_model = nullptr; ///< 模型
    QLabel *ui_config = nullptr;///< 配置
    QLabel *ui_rc = nullptr;///< 资源
    QLabel *ui_lib = nullptr;   ///< 库
    QLabel *ui_null = nullptr;  ///< 空白
    QCheckBox *ui_localisation_switch = nullptr;///< 本地化开关
    QCheckBox *ui_style_switch = nullptr;   ///< 样式开关
    QCheckBox *ui_event_switch = nullptr;   ///< 事件开关
    QCheckBox *ui_action_switch = nullptr;  ///< 动作开关
    QCheckBox *ui_accessories_switch = nullptr; ///< 部件开关
    QCheckBox *ui_model_switch = nullptr;   ///< 模型开关
    QCheckBox *ui_config_switch = nullptr;  ///< 配置开关
    QCheckBox *ui_rc_switch = nullptr;  ///< 资源开关
    QCheckBox *ui_lib_switch = nullptr; ///< 库开关
    QCheckBox *ui_null_switch = nullptr;///< 空白开关
    // 部件部分
    QVBoxLayout *ui_layout_main = nullptr;  ///< 主布局布局
    QVBoxLayout *ui_layout_widget = nullptr;///< 控件布局
    QHBoxLayout *ui_layout_localisation = nullptr;  ///< 本地化布局
    QHBoxLayout *ui_layout_style = nullptr; ///< 样式布局
    QHBoxLayout *ui_layout_event = nullptr; ///< 事件布局
    QHBoxLayout *ui_layout_action = nullptr;///< 动作布局
    QHBoxLayout *ui_layout_accessories = nullptr;   ///< 部件布局
    QHBoxLayout *ui_layout_model = nullptr; ///< 模型布局
    QHBoxLayout *ui_layout_config = nullptr;///< 配置布局
    QHBoxLayout *ui_layout_rc = nullptr;///< 资源布局
    QHBoxLayout *ui_layout_lib = nullptr;   ///< 库布局
    QHBoxLayout *ui_layout_null = nullptr;  ///< 空白布局

    /**
     * @brief 初始化部件
     */
    void initWidget();

    /**
     * @brief 初始化对象名称
     */
    void initObjectName();

    /**
     * @brief 初始化属性
     */
    void initProperty();

    /**
     * @brief 初始化内容
     */
    void initContent();

    /**
     * @brief 初始化信号槽
     */
    void initConnect();

    /**
     * @brief 清理信号槽
     */
    void clearConnect();

    /**
     * @brief 移动自身
     */
    void moveSelf();

    /**
     * @brief 隐藏筛选器计时器超时
     */
    void timerHideTimeOut();
protected:
    void enterEvent(QEvent *ev);

};

#endif // UIPLUGINFILTER_H
