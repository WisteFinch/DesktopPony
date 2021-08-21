/**
 * @file ui/config/uiconfigitemcard.h
 * @brief 界面-配置-配置项卡片
 * @author WisteFinch
 * @date 2021.8.9
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

#ifndef UICONFIGITEMCARD_H
#define UICONFIGITEMCARD_H

#include "data/text.h"
#include "data/config.h"
#include <QLabel>
#include <QVector>
#include <QLayout>
#include <QDir>
#include <QMouseEvent>
#include <QCheckBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QListView>

/**
 * @brief 配置项
 * @details 位于配置页，用于显示配置信息
 */

class UiConfigItemCard : public QWidget
{
    Q_OBJECT
public:
    explicit UiConfigItemCard(QWidget *parent = nullptr);
    ~UiConfigItemCard();

    /**
     * @brief 初始化
     * @details 初始化界面内容
     * @param 文本指针
     * @param 配置项指针
     * @param 函数指针：获取配置
     */
    void init(Text *ptrText, Config::Item *ptrItem, Config::PTRFUNC_GET_CONFIG ptrfuncGetConf);

signals:
    void sigValueChanged(); ///< 信号：值改变
    void sigShowRestart();  ///< 信号：显示重启按钮

private:
    Text *m_p_text = nullptr;   ///< 文本
    Config::Item *m_p_item = nullptr;   ///< 配置项指针
    bool m_restart_flag = false;///< 重启
    Config::PTRFUNC_GET_CONFIG m_ptrfunc_get_conf;///< 函数指针：获取配置

    // 部件部分
    QLabel *ui_conf_item_card_caption = nullptr;///< 标题
    QLabel *ui_conf_item_card_desc = nullptr;   ///< 描述
    QLabel *ui_conf_item_card_name = nullptr;   ///< 配置名称
    QLabel *ui_conf_item_card_status = nullptr; ///< 状态

    QCheckBox *ui_conf_item_card_switch = nullptr;  ///< 开关
    QLineEdit *ui_conf_item_card_lineedit = nullptr;///< 文本编辑
    QDoubleSpinBox *ui_conf_item_card_spin_box_real = nullptr;///< 实数数值框
    QSpinBox *ui_conf_item_card_spin_box = nullptr;  ///< 整数数值框
    QComboBox *ui_conf_item_card_combobox = nullptr;///< 组合框

    // 部件部分
    QHBoxLayout *ui_conf_item_card_layout_main = nullptr; ///< 主布局
    QVBoxLayout *ui_conf_item_card_layout_text = nullptr; ///< 文本布局

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
     * @brief 刷新状态
     */
    void refreshStatus();

    /**
     * @brief 槽：值改变
     */
    void slotValueChanged();

};

#endif // UICONFIGITEMCARD_H
