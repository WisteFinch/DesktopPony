/**
 * @file scriptinterpreter/core/public/scriptexceptionData.h
 * @brief 脚本-异常处理
 * @author WisteFinch
 * @date 2021.5.3
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

#ifndef SCRIPTEXCEPTION_H
#define SCRIPTEXCEPTION_H

#include "public/scriptexceptiondata.h"
#include "data/localisation.h"
#include <QString>
#include <QVector>
#include <QRegularExpression>

class ScriptException
{
public:
    ScriptException();
    ~ScriptException();
    void init(ScriptExceptionData data, Localisation *l);
    void init(Localisation *l);
    void initInfo();
    QString toStr();
    ScriptExceptionData *m_exception_data = nullptr;
    QString m_exception_info;
private:
    Localisation *m_p_localisation = nullptr;
    QString m_regexp = "(§\\[.*?\\])";
    ScriptExceptionTypeName m_exception_type_name;
    QString getPara(QString str);
    QVector<int> strRegExp(QString &str, QString regExp);
};

#endif // SCRIPTEXCEPTION_H
