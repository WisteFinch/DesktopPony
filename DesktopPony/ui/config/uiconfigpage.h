/**
 * @file ui/config/uiconfigpage.h
 * @brief 界面-配置-配置页
 * @author WisteFinch
 * @date 2021.7.29
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

#ifndef UICONFIGPAGE_H
#define UICONFIGPAGE_H

#include <QWidget>

/**
 * @brief 配置页
 * @details 列出程序配置项
 */
namespace Ui
{
class UiConfigPage;
}

class UiConfigPage : public QWidget
{
    Q_OBJECT

public:
    explicit UiConfigPage(QWidget *parent = nullptr);
    ~UiConfigPage();

private:
    Ui::UiConfigPage *ui;
};

#endif // UICONFIGPAGE_H
